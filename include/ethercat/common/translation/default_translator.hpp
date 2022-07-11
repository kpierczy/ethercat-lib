/* ============================================================================================================================ *//**
 * @file       default_translator.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the DefaultTranslator class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string>
#include <vector>
#include <type_traits>
// Private includes
#include "ethercat/config.hpp"
#include "ethercat/common/translation/translator_traits.hpp"
#include "ethercat/common/translation/default_translator/supported_types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ======================================================= TranslationError ======================================================= */

/**
 * @brief Default <i>translator</i> type used by the library to translate EtherCAT objects
 *    between binary image and app-domain entities
 * @details In terms of nomenclature of the @ref translation module, DefaultTranslator is
 *    a <i>specialized generic</i> translator. Currently supported target types are:
 * 
 *        - bit/bool (as 1-bit entity) [1]
 *        - bool (as 8-bit entity) [1]
 *        - uint8_t / int8_t
 *        - uint16_t / int16_t
 *        - uint32_t / int32_t
 *        - uint64_t / int64_t
 *        - float / double
 *        - ethercat::types::static_string
 *        - std::string [2]
 *        - enumerations
 * 
 *    and specializations of @ref std::array @ref std::vector ([3]) of above. Library support for 
 *    custom (e.g. structural) types can be added following instruction in [4]
 * 
 * @tparam T 
 *    target type of the translator
 * 
 * @note [1] In app domain 1-bit booleans are represented in the same way as 8-bit booleanes 
 *   (i.e. as bool/std::array<bool, N>/std::vector<bool>). Difference between two cases lies 
 *   in a way in which they are translated to/from binary image. To differntiate both types
 *   special <i>tag</i> types has been introduced. 8-bit booleans are refered by passing 
 *   @p T as actual representation (i.e. as bool/std::array<bool, N>/std::vector<bool>), while
 *   1-bit variant can be refered by ::types::bit (or it's std::array/std::vector derivation)
 *   although actual representation type will be bool-based.
 * 
 * @note std::string is a dynamically allocated type. For this reason in some cases it's target
 *   size cannot be unambigously by the translator. In particular:
 * 
 *      - <T = std::string> specialization is fully functional and workd as expected.
 *       In both translation directions translator serializes / deserializes all 
 *       available data
 * 
 *      - <T = std::array<std::string, N>> specialization is somehow arbitrary. EtherCAT 
 *        does not support concept of arrays of variable-size strings. For this reason
 *        way in which this specialization behaves depends on the user configuration 
 *        ( @ref ethercat::config::translation::default_translator::string )
 * 
 *      - <T = std::vector<std::string>> specialization is analogour to static-arry case
 *        ( @ref ethercat::config::translation::default_translator::string )
 * 
 *    In all cases number of characters in the string is always taken as the current
 *    size of the vector and is not a subject of change by the DefaultTranslator 
 * 
 * @note For std::vector specializations number of elements is always taken as the current
 *    size of the vector and is not a subject of change by the DefaultTranslator
 * 
 * @see [4] See description of the translation namespace
 * 
 * @warning DefaultTranslator is designed specifically for EtherCAT protocol and x86 platform 
 *    that both utilize <b>Little Endian</b> format of data. Thanks to this assumtpion translator
 *    can safely copy byte-by-byte betwen binary image and target POD data. If your target platform
 *    uses <b>Big Endian</b> format, the translator needs to be modified
 * 
 * @todo Add support for Big Endian platforms
 * @todo Add (optional) support for 1-bti and 8-bit boolean being represented by std::bitset / 
 *    boost::dynamic_bitset
 */
template<typename T, typename Enabler = void>
class DefaultTranslator 
{
public: /* ------------------------------------------------------ Public types ---------------------------------------------------- */

    /// Target type
    using Type = T;
    /// Derivation of Type that is efficiently passed as function argument
    using ArgType = translation::ArgType<T>;

public: /* ----------------------------------------------- Input translation methods ---------------------------------------------- */

    /**
     * @brief Translates an arbitrary object of supported type from the binary image into the 
     *    app-domain entity (overload not-supporting bit-aligned binary image)
     * 
     * @tparam T 
     *    target translation type
     * @param data 
     *    const reference to the source binary image
     * @param obj 
     *    reference to the destination object
     */
    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj);

    /**
     * @brief Translates an arbitrary object of supported type from the binary image into the 
     *    app-domain entity (overload not-supporting bit-aligned binary image)
     * 
     * @tparam T 
     *    target translation type
     * @param data 
     *    const reference to the source binary image
     * @param obj 
     *    reference to the destination object
     * @param bitoffset 
     *    bitoffset of the binary image
     */
    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_to(config::types::Span<const uint8_t> data, Type &obj, std::size_t bitoffset = 0);

public: /* ---------------------------------------------- Output translation methods ---------------------------------------------- */

    /**
     * @brief Translates an arbitrary object of supported type from app-domain entity into the 
     *    the binary image (overload not-supporting bit-aligned binary image)
     * 
     * @tparam T 
     *    target translation type
     * @param data 
     *    reference to the destination binary image
     * @param obj 
     *    source object
     */
    template<bool is_enabled = not config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj);
    
    /**
     * @brief Translates an arbitrary object of supported type from app-domain entity into the 
     *    the binary image (overload not-supporting bit-aligned binary image)
     * 
     * @tparam T 
     *    target translation type
     * @param data 
     *    reference to the destination binary image
     * @param obj 
     *    source object
     * @param bitoffset 
     *    bitoffset of the binary image
     */
    template<bool is_enabled = config::translation::default_translators::BitAlignedSupport,
        std::enable_if_t<is_enabled, bool> = true>
    static inline auto translate_from(config::types::Span<uint8_t> data, ArgType obj, std::size_t bitoffset = 0);

public: /* ---------------------------------------------------- Sizing methods ---------------------------------------------------- */

    /**
     * @brief Creates binary-image buffer for object of type @p T
     */
    static inline auto make_buffer();

    /**
     * @brief Creates binary-image buffer for @p obj object of type @p T
     * @note This overload is enabled only for specializations for std::vector<...> types
     */
    static inline auto make_buffer(ArgType obj);
    
};

/* ===================================== DefaultTranslator - unsupported types specialization ===================================== */

template<typename T>
class DefaultTranslator<T, 
    std::enable_if_t<not default_translator::is_supported_v<T>>> 
: 
    default_translator::DefaultTranslatorInvalidSpecialization<T>
{ };

/* ============================================================ Helpers =========================================================== */

/**
 * @brief Auxilar type alias removing cv-ref qualifiers from the type @p T before
 *    specializing the DefaultTranslator template
 */
template<typename T>
using DefaultTranslatorOfQualified = DefaultTranslator<utilities::traits::remove_cvref_t<T>>;

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/translation/default_translator/specializations.hpp"

/* ================================================================================================================================ */

#endif
