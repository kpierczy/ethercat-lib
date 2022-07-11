/* ============================================================================================================================ *//**
 * @file       static_string.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Specialization of the DefaultTranslator class for static strings and it's derivations
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SPECIALIZATIONS_STATIC_STRING_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SPECIALIZATIONS_STATIC_STRING_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/default_translator/common.hpp"
#include "ethercat/common/translation/default_translator/types/static_string.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {  

/* ======================================= DefaultTranslator - static_string specialization ======================================= */

template<std::size_t N>
class DefaultTranslator<ethercat::types::static_string<N>> :
    public details::DefaultTranslatorBase<
        DefaultTranslator<ethercat::types::static_string<N>>,
        ethercat::types::static_string<N>
    >
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Type of the base
    using BaseType = details::DefaultTranslatorBase<DefaultTranslator<ethercat::types::static_string<N>>, ethercat::types::static_string<N>>;
    /// Direction enumeration used to call methods of the base class
    using Direction = typename BaseType::Direction;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Self type
    using SelfType = typename BaseType::SelfType;
    /// Target type
    using Type = typename BaseType::Type;
    /// Argument-efficient type
    using ArgType = typename BaseType::ArgType;

public: /* ----------------------------------------------- Input translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj) {
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(char) * N); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(char) * N); },
            // Translate
            [&](){ BaseType::copy_bytes(&data[0], reinterpret_cast<uint8_t*>(&obj[0]), sizeof(char) * N); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(char) * N, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(char) * N, bitoffset); },
            // Translate
            [&](){ BaseType::copy_bytes_from_bitshifted(&data[0], reinterpret_cast<uint8_t*>(&obj[0]), sizeof(char) * N, bitoffset); }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj) {
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(char) * N); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(char) * N); },
            // Translate
            [&](){ BaseType::copy_bytes(reinterpret_cast<const uint8_t*>(&obj[0]), &data[0], sizeof(char) * N); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0) {
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(char) * N, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(char) * N, bitoffset); },
            // Translate
            [&](){ BaseType::copy_bytes_to_bitshifted(reinterpret_cast<const uint8_t*>(&obj[0]), &data[0], sizeof(char) * N, bitoffset); }
        );
    }

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    static constexpr inline auto make_buffer() {
        return std::array<uint8_t, sizeof(char) * N>{};
    }
    
};

/* ============================== DefaultTranslator - static_string array/vector specialization base ============================== */

namespace details {

template<typename TranslatorT, typename T>
class DefaultTranslatorStaticStringArray :
    public details::DefaultTranslatorBase<TranslatorT,T>
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Type of the base
    using BaseType = details::DefaultTranslatorBase<TranslatorT,T>;
    /// Direction enumeration used to call methods of the base class
    using Direction = typename BaseType::Direction;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Self type
    using SelfType = typename BaseType::SelfType;
    /// Target type
    using Type = typename BaseType::Type;
    /// Argument-efficient type
    using ArgType = typename BaseType::ArgType;

public: /* ----------------------------------------------- Input translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj) {

        constexpr std::size_t N = Type::value_type::max_size_n;
        
        // Calculate number of bytes in the array
        std::size_t bytes_num = sizeof(char) * N * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){
                for(std::size_t i = 0; i < obj.size(); ++i)
                    BaseType::copy_bytes(&data[0] + i * N, reinterpret_cast<uint8_t*>(&(obj[i][0])), N);
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {

        constexpr std::size_t N = Type::value_type::max_size_n;
        
        // Calculate number of bytes in the array
        std::size_t bytes_num = sizeof(char) * N * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){
                for(std::size_t i = 0; i < obj.size(); ++i)
                    BaseType::copy_bytes_from_bitshifted(&data[0] + i * N, reinterpret_cast<uint8_t*>(&(obj[i][0])), bitoffset); 
            }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj) {

        constexpr std::size_t N = Type::value_type::max_size_n;
        
        // Calculate number of bytes in the array
        std::size_t bytes_num = sizeof(char) * N * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){
                for(std::size_t i = 0; i < obj.size(); ++i)
                    BaseType::copy_bytes(reinterpret_cast<const uint8_t*>(&(obj[i][0])), &data[0] + i * N, N);
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0) {

        constexpr std::size_t N = Type::value_type::max_size_n;
        
        // Calculate number of bytes in the array
        std::size_t bytes_num = sizeof(char) * N * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){
                for(std::size_t i = 0; i < obj.size(); ++i)
                    BaseType::copy_bytes_to_bitshifted(reinterpret_cast<const uint8_t*>(&(obj[i][0])), &data[0] + i * N, bitoffset); 
            }
        );
    }
};

}

/* ================================= DefaultTranslator - static_string array/vector specialization ================================ */

template<std::size_t N, std::size_t ElementsN>
class DefaultTranslator<std::array<ethercat::types::static_string<N>, ElementsN>> :
    public details::DefaultTranslatorStaticStringArray<
        DefaultTranslator<std::array<ethercat::types::static_string<N>, ElementsN>>,
        std::array<ethercat::types::static_string<N>, ElementsN>
    >
{
public:
    static constexpr inline auto make_buffer() { 
        return std::array<uint8_t, sizeof(char) * N * ElementsN>{}; 
    }
};


template<std::size_t N>
class DefaultTranslator<std::vector<ethercat::types::static_string<N>>> :
    public details::DefaultTranslatorStaticStringArray<
        DefaultTranslator<std::vector<ethercat::types::static_string<N>>>,
        std::vector<ethercat::types::static_string<N>>
    >
{
public:
    static inline auto make_buffer(const std::vector<ethercat::types::static_string<N>> &obj) { 
        return std::vector<uint8_t>( sizeof(char) * N * obj.size() ); 
    }
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
