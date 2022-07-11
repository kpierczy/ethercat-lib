/* ============================================================================================================================ *//**
 * @file       translators_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 4:53:39 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Set of auxiliary types traits wrapping std::enable_if type template with conditions specific for evaluating
 *             translator types used by methods of the Slave class template
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_TRAITS_H__
#define __ETHERCAT_SLAVE_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard incudes
#include <type_traits>
// Private incudes
#include "ethercat/common/utilities/enum.hpp"
#include "ethercat/common/translation.hpp"
#include "ethercat/types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::slave::traits {

/* ======================================================== Used namespaces ======================================================= */

using namespace common::utilities;
using namespace common::translation;

/* ============================================================= Types ============================================================ */

/**
 * @brief Auxiliary enumeration describing direction of data flow between application and slave
 * @details This enumeration is used to unify underlying values of @ref Slave::SdoDirection / 
 *    @ref Slave::PdoDirection enumeration with the values passed to underlying traits
 */
enum Direction {
    Input         = traits::to_underlying(TranslationDirection::Input),
    Output        = traits::to_underlying(TranslationDirection::Output),
    Bidirectional = traits::to_underlying(TranslationDirection::Bidirectional)
};

/**
 * @brief Auxiliary meta-function converting TranslationDirection-compatible @p dir into 
 *    TranslationDirection
 */
template<auto dir>
constexpr TranslationDirection to_translation_dir = static_cast<TranslationDirection>(dir);

/* ========================================= SDO-specific traits (for sizing translators) ========================================= */

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    type is a sizing translator for the given direction @p dir and target type @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_sizing_translator :
    public std::enable_if<
        SizingTranslatorTraits<TranslatorT, T>::sizing::
            template is_translator_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };

/// Alias for enable_if_sizing_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_sizing_translator_t = 
    typename enable_if_sizing_translator<dir, TranslatorT, T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    type is a stateless sizing translator for the given direction @p dir and target 
 *    type @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_stateless_sizing_translator :
    public std::enable_if<
        SizingTranslatorTraits<TranslatorT, T>::sizing::
            template is_stateless_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };

/// Alias for enable_if_stateless_sizing_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_stateless_sizing_translator_t = 
    typename enable_if_stateless_sizing_translator<dir, TranslatorT, T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    type is a statefull sizing translator for the given direction @p dir and target 
 *    type @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_statefull_sizing_translator :
    public std::enable_if<
        SizingTranslatorTraits<TranslatorT, T>::sizing::
            template is_statefull_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };

/// Alias for enable_if_statefull_sizing_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_statefull_sizing_translator_t = 
    typename enable_if_statefull_sizing_translator<dir, TranslatorT, T>::type;

/* ======================================= PDO-specific traits (for non-sizing translators) ======================================= */

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    type is a translator given direction @p dir and target type @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_translator :
    public std::enable_if<
        TranslatorTraits<TranslatorT, T>::
            template is_translator_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };


/// Alias for enable_if_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_translator_t = 
    typename enable_if_translator<dir, TranslatorT, T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    type is NOT a translator given direction @p dir and target type @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_not_translator :
    public std::enable_if<
        not TranslatorTraits<TranslatorT, T>::
            template is_translator_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };

/// Alias for enable_if_not_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_not_translator_t = 
    typename enable_if_not_translator<dir, TranslatorT, T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    Translator is stateless for the given direction @p dir and target type 
 *    @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 * 
 * @note If @p dir is bidirectional translator is required to be stateless in @b both
 *    directions. This is to properly resolve overloads of templated of the @ref Slave 
 *    class template and it's nested classes.
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_stateless_translator :
    public std::enable_if<
        TranslatorTraits<TranslatorT, T>::
            template is_stateless_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };

/// Alias for enable_if_stateless_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_stateless_translator_t = 
    typename enable_if_stateless_translator<dir, TranslatorT, T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    Translator is statefull for the given direction @p dir and target type 
 *    @p T 
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 * @tparam TranslatorT 
 *    target translator type
 * @tparam T 
 *    target translation type; if @c void type will be deduced automatically from
 *    definition of @p TranslatorT
 * 
 * @note If @p dir is bidirectional translator is required to be statefull in <b>at least</b>
 *    one of directions. This is to properly resolve overloads of templated of the @ref Slave 
 *    class template and it's nested classes.
 */
template<auto dir, typename TranslatorT, typename T = void>
struct enable_if_statefull_translator :
    public std::enable_if<
        TranslatorTraits<TranslatorT, T>::
            template is_statefull_in_dir<static_cast<TranslationDirection>(dir)>
    , bool>
{ };

/* =============================================== DefaultTranslator-related traits =============================================== */

/// Alias for enable_if_statefull_translator<...>::type
template<auto dir, typename TranslatorT, typename T = void>
using enable_if_statefull_translator_t = 
    typename enable_if_statefull_translator<dir, TranslatorT, T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    library can use default translator to translate @p T type
 * 
 * @tparam T 
 *    target translation type
 */
template<typename T>
struct enable_if_default_translatable :
    std::enable_if<default_translator::is_supported_v<T>, bool>
{ };

/// Alias for enable_if_default_translatable<...>::type
template<typename T>
using enable_if_default_translatable_t = 
    typename enable_if_default_translatable<T>::type;

/**
 * @brief Auxiliary wrapper around std::enable_if enabling template when given
 *    library can use default translator to translate representation type of
 *    the EtherCAt builtin type with the given @p type_id and @p arity (should
 *    be possible for all builtin types, but check it just in case)
 * 
 * 
 * @tparam type_id 
 *    ID of the target EtherCAT builtin type
 * @tparam arity  
 *    arity of the target type ( @c 0 for scalars )
 */
template<types::BuiltinType::ID type_id, std::size_t arity = 0>
struct enable_if_builtin_default_translatable :
    public enable_if_default_translatable<common::types::traits::TypeRepresentation<type_id, arity>>
{ };

/// Alias for enable_if_default_translatable<...>::type
template<types::BuiltinType::ID type_id, std::size_t arity = 0>
using enable_if_builtin_default_translatable_t = 
    typename enable_if_builtin_default_translatable<type_id, arity>::type;

/* ================================================================================================================================ */

} // End namespace ethercat::slave::traits

/* ================================================================================================================================ */

#endif
