/* ============================================================================================================================ *//**
 * @file       string.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Specialization of the DefaultTranslator class for strings and it's derivations
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SPECIALIZATIONS_STRING_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SPECIALIZATIONS_STRING_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/default_translator/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {  

/* =========================================== DefaultTranslator - string specialization ========================================== */

template<>
class DefaultTranslator<std::string> :
    public details::DefaultTranslatorBase<
        DefaultTranslator<std::string>,
        std::string
    >
{
public: /* ----------------------------------------------- Input translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj) {
        return translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, obj.size()); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, obj.size()); },
            // Translate
            [&](){ copy_bytes(&data[0], reinterpret_cast<uint8_t*>(&obj[0]), obj.size()); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        return translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, obj.size(), bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, obj.size(), bitoffset); },
            // Translate
            [&](){ copy_bytes_from_bitshifted(&data[0], reinterpret_cast<uint8_t*>(&obj[0]), obj.size(), bitoffset); }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj) {
        return translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, obj.size()); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, obj.size()); },
            // Translate
            [&](){ copy_bytes(reinterpret_cast<const uint8_t*>(&obj[0]), &data[0], obj.size()); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0) {
        return translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, obj.size(), bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, obj.size(), bitoffset); },
            // Translate
            [&](){ copy_bytes_to_bitshifted(reinterpret_cast<const uint8_t*>(&obj[0]), &data[0], obj.size(), bitoffset); }
        );
    }

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    static inline auto make_buffer(ArgType obj) {
        return std::vector<uint8_t>(sizeof(char) * obj.size());
    }
    
};

/* ================================== DefaultTranslator - string array/vector specialization base ================================= */

namespace details {

template<typename TranslatorT, typename T>
class DefaultTranslatorStringArray :
    public details::DefaultTranslatorBase<TranslatorT, T>
{
protected: /* ------------------------------------------------ Protected types ---------------------------------------------------- */

    /// Type of the base
    using BaseType = details::DefaultTranslatorBase<TranslatorT, T>;
    /// Direction enumeration used to call methods of the base class
    using Direction = typename BaseType::Direction;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Self type
    using SelfType = typename BaseType::SelfType;
    /// Target type
    using Type = typename BaseType::Type;
    /// Argument-efficient type
    using ArgType = typename BaseType::ArgType;

private: /* ---------------------------------------------------- Private helpers -------------------------------------------------- */

    /**
     * @brief Calculates number of bytes in the array that should be subject to translation
     */
    static inline std::size_t calculate_bytes_num(Type &obj) {

        using namespace config::translation::default_translators::string;

        std::size_t bytes_num = 0;

        // Calculate number of bytes to be translated
        if constexpr(ArrayParsingMode == ArrayMode::AllowVariableSize) {
            for(const auto &elem : obj)
                bytes_num += elem.size();
        } else {
            if(auto elements = obj.size(); elements > 0)
                bytes_num = elements * obj[0].size();
        }

        return bytes_num;
    }

    /**
     * @brief Auxiliary method wrapping common part of all translation routine regarding string arrays 
     * 
     * @param obj 
     *    translated object
     * @param translate 
     *    direct translation call to  wrapped with void(std::size_t data_offset, std::size_t size)
     */
    template<typename ObjectT, typename TranslateT>
    static inline void wrap_translation(
        ObjectT &&obj,
        TranslateT &&translate
    ) {

        using namespace config::translation::default_translators::string;

        // If variable size is allowed, translate each string individually
        if constexpr(ArrayParsingMode == ArrayMode::AllowVariableSize) {

            std::size_t bytes_translated = 0;

            // Translate strings
            for(std::size_t i = 0; i < obj.size(); ++i) {

                // Calculate size of the current object
                std::size_t size = obj[i].size();
                // Translate string
                translate(bytes_translated, size);
                // Update number of translated bytes
                bytes_translated += size;
            }

        // Otherwise copy data to all strings assuming equal size
        } else {

            // Calculate size of each element
            std::size_t size = (obj.size() > 0) ? obj[0].size() : 0;
            // Translate strings
            for(std::size_t i = 0; i < obj.size(); ++i)
                translate(i, i * size, size);
        }
    }

public: /* ----------------------------------------------- Input translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj) {

        using namespace config::translation::default_translators::string;

        // Validate strings, if required
        if constexpr (ArrayParsingMode == ArrayMode::RequireEqualSize) {
            for(const auto &elem : obj)
                if(elem.size() != obj[0].size())
                    return BaseType::report_error("Not all string in the array have the same size");
        }

        // Calculate number of bytes to be translated
        std::size_t bytes_num = calculate_bytes_num(obj);

        // Dispatch translation
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){ wrap_translation(obj, 
                [&](std::size_t object_offset, std::size_t offset, std::size_t size) { 
                    BaseType::copy_bytes(
                        &data[0] + offset,
                        reinterpret_cast<uint8_t*>(&obj[object_offset][0]),
                        size);
                });
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {

        using namespace config::translation::default_translators::string;

        // Validate strings, if required
        if constexpr (ArrayParsingMode == ArrayMode::RequireEqualSize) {
            for(const auto &elem : obj)
                if(elem.size() != obj[0].size())
                    return BaseType::report_error("Not all string in the array have the same size");
        }

        // Calculate number of bytes to be translated
        std::size_t bytes_num = calculate_bytes_num(obj);
        
        // Dispatch translation
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){ wrap_translation(obj, 
                [&](std::size_t object_offset, std::size_t offset, std::size_t size) { 
                    BaseType::copy_bytes_to_bitshifted(
                        &data[0] + offset,
                        reinterpret_cast<uint8_t*>(&obj[object_offset][0]),
                        size,
                        bitoffset);
                });
            }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj) {

        using namespace config::translation::default_translators::string;

        // Validate strings, if required
        if constexpr (ArrayParsingMode == ArrayMode::RequireEqualSize) {
            for(const auto &elem : obj)
                if(elem.size() != obj[0].size())
                    return BaseType::report_error("Not all string in the array have the same size");
        }

        // Calculate number of bytes to be translated
        std::size_t bytes_num = calculate_bytes_num(obj);
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){ wrap_translation(obj, 
                [&](std::size_t object_offset, std::size_t offset, std::size_t size) { 
                    BaseType::copy_bytes(
                        reinterpret_cast<const uint8_t*>(&obj[object_offset][0]),
                        &data[0] + offset,
                        size);
                });
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0) {

        using namespace config::translation::default_translators::string;

        // Validate strings, if required
        if constexpr (ArrayParsingMode == ArrayMode::RequireEqualSize) {
            for(const auto &elem : obj)
                if(elem.size() != obj[0].size())
                    return BaseType::report_error("Not all string in the array have the same size");
        }

        // Calculate number of bytes to be translated
        std::size_t bytes_num = calculate_bytes_num(obj);
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){ wrap_translation(obj, 
                [&](std::size_t object_offset, std::size_t offset, std::size_t size) { 
                    BaseType::copy_bytes_to_bitshifted(
                        reinterpret_cast<const uint8_t*>(&obj[object_offset][0]),
                        &data[0] + offset,
                        size,
                        bitoffset);
                });
            }
        );
    }
};

}

/* ==================================== DefaultTranslator - string array/vector specialization ==================================== */

template<std::size_t N>
class DefaultTranslator<std::array<std::string, N>> :
    public details::DefaultTranslatorStringArray<
        DefaultTranslator<std::array<std::string, N>>,
        std::array<std::string, N>
    >
{
public:
    static constexpr inline auto make_buffer(const std::string &obj) { 
        return std::vector<uint8_t>(sizeof(char) * obj.size()); 
    }
};

template<>
class DefaultTranslator<std::vector<std::string>> :
    public details::DefaultTranslatorStringArray<
        DefaultTranslator<std::vector<std::string>>,
        std::vector<std::string>
    >
{
public:
    static inline auto make_buffer(const std::vector<std::string> &obj) { 

        using namespace config::translation::default_translators::string;

        // Use alias for base type
        using BaseType = typename details::DefaultTranslatorStringArray<
            DefaultTranslator<std::vector<std::string>>,
            std::vector<std::string>
        >::BaseType;
        
        // Is all string of equal size, return vector of size resulting from simple multiplication
        if constexpr (ArrayParsingMode == ArrayMode::AssumeEqualSize) {
            
            return (obj.size() == 0)   ? 
                std::vector<uint8_t>{} :
                std::vector<uint8_t>( sizeof(char) * obj.size() * obj[0].size() ); 

        // Is all string required to have the same size, verify it
        } else if constexpr (ArrayParsingMode == ArrayMode::RequireEqualSize) {
            
            // Validate object
            for(const auto &elem : obj) {
                if(elem.size() != obj[0].size()) {
                    BaseType::report_error<BaseType::Direction::None>("Not all string in the array have the same size");
                    return std::vector<uint8_t>{};
                }
            }
            
            // Create buffer
            return (obj.size() == 0)   ? 
                std::vector<uint8_t>{} :
                std::vector<uint8_t>( sizeof(char) * obj.size() * obj[0].size() ); 

        // Is all string of variable size, return vector of size resulting from summing sizes
        } else if constexpr (ArrayParsingMode == ArrayMode::AllowVariableSize) {
            
            std::size_t bytes_num = 0;
            for(const auto &elem : obj)
                bytes_num += (sizeof(char) * elem.size());
            return std::vector<uint8_t>(bytes_num);
            
        }
    }
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
