/* ============================================================================================================================ *//**
 * @file       builtin.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Specialization of the DefaultTranslator class for builtin types and static arrays
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SPECIALIZATIONS_BUILTIN_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SPECIALIZATIONS_BUILTIN_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/default_translator/specializations/bit.hpp"
#include "ethercat/common/translation/default_translator/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {  

/* ================================= DefaultTranslator - base types & builtin arrays specialization =============================== */

template<typename T>
class DefaultTranslator<T, 
    std::enable_if_t<
        default_translator::is_supported_builtin_v<T> ||
        default_translator::is_supported_builtin_array_v<T>
    >
> : public details::DefaultTranslatorBase<DefaultTranslator<T>, T>
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Type of the base
    using BaseType = details::DefaultTranslatorBase<DefaultTranslator<T>, T>;
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
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(Type)); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(Type)); },
            // Translate
            [&](){ BaseType::copy_bytes(&data[0], reinterpret_cast<uint8_t*>(&obj), sizeof(Type)); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(Type), bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(Type), bitoffset); },
            // Translate
            [&](){ BaseType::copy_bytes_from_bitshifted(&data[0], reinterpret_cast<uint8_t*>(&obj), sizeof(Type), bitoffset); }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj) {
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(Type)); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(Type)); },
            // Translate
            [&](){ BaseType::copy_bytes(reinterpret_cast<const uint8_t*>(&obj), &data[0], sizeof(Type)); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0) {
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, sizeof(Type), bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, sizeof(Type), bitoffset); },
            // Translate
            [&](){ BaseType::copy_bytes_to_bitshifted(reinterpret_cast<const uint8_t*>(&obj), &data[0], sizeof(Type), bitoffset); }
        );
    }

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    static constexpr inline auto make_buffer() {
        return std::array<uint8_t, sizeof(Type)>{};
    }
    
};

/* ====================================== DefaultTranslator - builtin vectors specialization ====================================== */

template<typename T>
class DefaultTranslator<T, 
    std::enable_if_t<default_translator::is_supported_builtin_vector_v<T> and not std::is_same_v<T, bool>>
> : public details::DefaultTranslatorBase<DefaultTranslator<T>, T>
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Type of the base
    using BaseType = details::DefaultTranslatorBase<DefaultTranslator<T>, T>;
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
        
        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();
        
        // Dispatch translation
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){ BaseType::copy_bytes(&data[0], reinterpret_cast<uint8_t*>(&obj[0]), bytes_num); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        
        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){ BaseType::copy_bytes_from_bitshifted(&data[0], reinterpret_cast<uint8_t*>(&obj.at(0)), bytes_num, bitoffset); }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, const Type &obj) {

        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){ BaseType::copy_bytes(reinterpret_cast<const uint8_t*>(&obj), &data[0], bytes_num); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, const Type &obj, std::size_t bitoffset = 0) {

        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();

        // Dispatch translation
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return BaseType::is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return BaseType::is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){ BaseType::copy_bytes_to_bitshifted(reinterpret_cast<const uint8_t*>(&obj), &data[0], bytes_num, bitoffset); }
        );
    }

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    static inline auto make_buffer(const Type &obj) { 
        return std::vector<uint8_t>(sizeof(typename Type::value_type) * obj.size()); 
    }
    
};

/* ====================================== DefaultTranslator - builtin vectors specialization ====================================== */

/// Make std::vecto<bool> specialization because of a stupid stupid standard .-.
template<>
class DefaultTranslator<std::vector<bool>> : 
    public details::DefaultTranslatorBase<DefaultTranslator<std::vector<bool>>, std::vector<bool>>
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
            [&](){ 
                for(std::size_t i = 0; i < obj.size(); ++i)
                    obj[i] = static_cast<bool>(data[i]);
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        
        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();

        // Dispatch translation
        return translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){ 
        
                // Calculate bitoffset from the byte boundry
                std::size_t bitoffset_remainder = bitoffset % BITS_IN_BYTE;
                // Calculate number of full bytes in the offset
                std::size_t bytes_offset = bitoffset / BITS_IN_BYTE;

                // Take pointer to the beggining of the source buffer (skip bytes offset in the source)
                auto src = &data[0] + bytes_offset;

                // If bitoffset does not cross byte boundary, copy raw bytes
                if(bitoffset_remainder == 0) {
                    for(std::size_t i = 0; i < obj.size(); ++i)
                        obj[i] = static_cast<bool>(src[i]);
                    return;
                }

                // Compute distance from the LSBit (of the source byte) to the bitoffset boundary
                std::size_t lsb_bitoffset = bitoffset_remainder;
                // Compute distance from the MSBit (of the source byte) to the bitoffset boundary
                std::size_t msb_bitoffset = (BITS_IN_BYTE - bitoffset_remainder);

                // Copy data to the output buffer
                for(std::size_t i = 0; i < obj.size(); ++i)
                    obj[i] = static_cast<uint8_t>((src[i + 1] << msb_bitoffset) | (src[i] >> lsb_bitoffset)); 
            }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, const Type &obj) {

        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();

        // Dispatch translation
        return translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, bytes_num); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, bytes_num); },
            // Translate
            [&](){ 
                for(std::size_t i = 0; i < obj.size(); ++i)
                    data[i] = static_cast<uint8_t>(bool(obj[i]));
            }  
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, const Type &obj, std::size_t bitoffset = 0) {

        // Calculate number of bytes in the vector
        std::size_t bytes_num = sizeof(typename Type::value_type) * obj.size();

        // Dispatch translation
        return translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return is_enough_size_to_fit(data, bytes_num, bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return is_minimal_size_to_fit(data, bytes_num, bitoffset); },
            // Translate
            [&](){ 

                // Calculate bitoffset from the byte boundry
                std::size_t bitoffset_remainder = bitoffset % BITS_IN_BYTE;
                // Calculate number of full bytes in the offset
                std::size_t bytes_offset = bitoffset / BITS_IN_BYTE;

                // Take pointer to the beggining of the destination buffer (skip bytes offset in the source)
                auto dst = &data[0] + bytes_offset;

                // Compute number of booleans to be translated
                std::size_t bools_num = obj.size();

                // Auxiliary lambda translating std::vector<bool>'s reference proxy to uint8_t
                constexpr auto bit_to_byte = [](auto &&bit_proxy) { 
                    return static_cast<uint8_t>(bool(bit_proxy)); 
                };

                // If bitoffset does not cross byte boundary, copy raw bytes
                if(bitoffset_remainder == 0) {
                    for(std::size_t i = 0; i < bools_num; ++i)
                        dst[i] = bit_to_byte(obj[i]);
                }

                // Compute distance from the LSBit (of the destination byte) to the bitoffset boundary
                std::size_t lsb_bitoffset = bitoffset_remainder;
                // Compute distance from the MSBit (of the destination byte) to the bitoffset boundary
                std::size_t msb_bitoffset = (BITS_IN_BYTE - bitoffset_remainder);


                // Handle the first element of the output manually preserving [bitoffset_remainder] LSBits
                dst[0] = ((bit_to_byte(obj[0]) << lsb_bitoffset) | (dst[0] & (0xFFU >> msb_bitoffset)));

                // Copy data to all (bools_num-1) 'full' bytes of the destination (i.e. bytes that will hold 8 bits from the source buffer each)
                for(std::size_t i = 0; i < bools_num - 1; ++i)
                    dst[i + 1] = (bit_to_byte(obj[i + 1]) << lsb_bitoffset) | (bit_to_byte(obj[i]) >> msb_bitoffset);

                // Handle the last element of the output manually preserving [8 - bitoffset_remainder] MSBits
                dst[bools_num - 1] = ((dst[bools_num - 1] & (0xFFU << lsb_bitoffset)) | (bit_to_byte(obj[bools_num - 1]) >> msb_bitoffset));
            }
        );
    }

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    static inline auto make_buffer(const Type &obj) { 
        constexpr std::size_t BITS_IN_BYTE = 8;
        return std::vector<uint8_t>( (obj.size() == 0) ? 0 : (1 + obj.size() / BITS_IN_BYTE) ); 
    }
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
