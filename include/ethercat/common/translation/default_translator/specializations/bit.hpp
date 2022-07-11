/* ============================================================================================================================ *//**
 * @file       default_translator.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Specialization of the DefaultTranslator class for bit and bitarray
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_BIT_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_BIT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/default_translator/common.hpp"
#include "ethercat/common/translation/default_translator/types/bit.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ============================================ DefaultTranslator - bit specialization ============================================ */

template<>
class DefaultTranslator<ethercat::types::bit> :
    public details::DefaultTranslatorBase<
        DefaultTranslator<ethercat::types::bit>,
        bool
    >
{
public: /* ----------------------------------------------- Input translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj) {
        return translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() > 0); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() == 1); },
            // Translate
            [&](){ obj = bool(data[0] & 0x1U); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        return translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() * BITS_IN_BYTE > bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() * BITS_IN_BYTE == round_up_to_multiple_of<BITS_IN_BYTE>(bitoffset + 1)); },
            // Translate
            [&](){ obj = bool(data[bitoffset / BITS_IN_BYTE] & (0x1U << (bitoffset % BITS_IN_BYTE))); }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj) {
        return translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() > 0); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() == 1); },
            // Translate
            [&](){ obj ? set_bit(data[0], 0) : clear_bit(data[0], 0); }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0) {
        return translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() * BITS_IN_BYTE > bitoffset); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() * BITS_IN_BYTE == round_up_to_multiple_of<BITS_IN_BYTE>(bitoffset + 1)); },
            // Translate
            [&](){ obj ? 
                set_bit  (data[bitoffset / BITS_IN_BYTE], bitoffset % BITS_IN_BYTE): 
                clear_bit(data[bitoffset / BITS_IN_BYTE], bitoffset % BITS_IN_BYTE); 
            }
        );
    }

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    static constexpr inline auto make_buffer() {
        return std::array<uint8_t, sizeof(Type)>{};
    }
    
};

/* =================================== DefaultTranslator - bit array/vector specialization base =================================== */

namespace details {

template<typename TranslatorT, typename T>
class DefaultTranslatorBitArray :
    public details::DefaultTranslatorBase<TranslatorT, T>
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Type of the base
    using BaseType = details::DefaultTranslatorBase<TranslatorT, T>;
    /// Direction enumeration used to call methods of the base class
    using Direction = typename BaseType::Direction;

    /// Number of bits in a byte
    static constexpr inline std::size_t BITS_IN_BYTE = BaseType::BITS_IN_BYTE;

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
            [&](){ return (data.size() * BITS_IN_BYTE > obj.size()); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() * BITS_IN_BYTE == BaseType::template round_up_to_multiple_of<BITS_IN_BYTE>(obj.size())); },
            // Translate
            [&]()
            { 
                for(std::size_t i = 0; i < obj.size(); ++i) {
                    obj[i] = BaseType::get_bit(data[i / BITS_IN_BYTE], i % BITS_IN_BYTE);
                }
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0) {
        return BaseType::template translate<Direction::Input>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() * BITS_IN_BYTE >= bitoffset + obj.size()); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() * BITS_IN_BYTE == BaseType::template round_up_to_multiple_of<BITS_IN_BYTE>(bitoffset + obj.size())); },
            // Translate
            [&]()
            { 
                for(std::size_t i = bitoffset; i < bitoffset + obj.size(); ++i) {
                    obj[i - bitoffset] = BaseType::get_bit(data[i / BITS_IN_BYTE], i % BITS_IN_BYTE);
                }
            }
        );
    }

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, const Type &obj) {
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() * BITS_IN_BYTE > obj.size()); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() * BITS_IN_BYTE == BaseType::template round_up_to_multiple_of<BITS_IN_BYTE>(obj.size())); },
            // Translate
            [&]()
            { 
                for(std::size_t i = 0; i < obj.size(); ++i) {
                    obj[i] ? 
                        BaseType::set_bit  (data[i / BITS_IN_BYTE], i % BITS_IN_BYTE):
                        BaseType::clear_bit(data[i / BITS_IN_BYTE], i % BITS_IN_BYTE);
                }
            }
        );
    }


    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, const Type &obj, std::size_t bitoffset = 0) {
        return BaseType::template translate<Direction::Output>(
            // Check whether buffer is enough to fit data
            [&](){ return (data.size() * BITS_IN_BYTE >= bitoffset + obj.size()); },
            // Check whether buffer is of exact size to fit data
            [&](){ return (data.size() * BITS_IN_BYTE == BaseType::template round_up_to_multiple_of<BITS_IN_BYTE>(bitoffset + obj.size())); },
            // Translate
            [&]()
            { 
                for(std::size_t i = bitoffset; i < bitoffset + obj.size(); ++i) {
                    obj[i - bitoffset] ?
                        BaseType::set_bit  (data[i / BITS_IN_BYTE], i % BITS_IN_BYTE):
                        BaseType::clear_bit(data[i / BITS_IN_BYTE], i % BITS_IN_BYTE);
                }
            }
        );
    }
    
};

}

/* ====================================== DefaultTranslator - bit array/vector specialization ===================================== */

template<std::size_t N>
class DefaultTranslator<std::array<ethercat::types::bit, N>> :
    public details::DefaultTranslatorBitArray<
        DefaultTranslator<std::array<ethercat::types::bit, N>>,
        std::array<bool, N>
    >
{
public:
    static constexpr inline auto make_buffer() { 
        constexpr std::size_t BITS_IN_BYTE = 8;
        return std::array<uint8_t, (N == 0) ? 0 : (1 + N / BITS_IN_BYTE)>{}; 
    }
};

template<>
class DefaultTranslator<std::vector<ethercat::types::bit>> :
    public details::DefaultTranslatorBitArray<
        DefaultTranslator<std::vector<ethercat::types::bit>>, 
        std::vector<bool>
    >
{
public:
    static inline auto make_buffer(const std::vector<bool> &obj) { 
        constexpr std::size_t BITS_IN_BYTE = 8;
        return std::vector<uint8_t>( (obj.size() == 0) ? 0 : (1 + obj.size() / BITS_IN_BYTE) ); 
    }
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
