/* ============================================================================================================================ *//**
 * @file       supported_types.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of auxiliary traits detcing types supported by the default translator
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SUPPORTED_TYPES_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_SUPPORTED_TYPES_H__

/* =========================================================== Includes =========================================================== */

// System includes
#include <cstdint>
// Private includes
#include "ethercat/common/utilities/traits.hpp"
#include "ethercat/common/translation/default_translator/types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::default_translator {

/* ======================================================== Supported types ======================================================= */

/// Auxiliary trait indicating whether given T type is a builtin type supported by the default translator
template<typename T>
constexpr bool is_supported_builtin_v { 
    std::is_enum_v<T>           ||
    std::is_same_v<T, bool>     ||
    std::is_same_v<T, uint8_t>  ||
    std::is_same_v<T, uint8_t>  ||
    std::is_same_v<T, int8_t>   ||
    std::is_same_v<T, uint16_t> ||
    std::is_same_v<T, int16_t>  ||
    std::is_same_v<T, uint32_t> ||
    std::is_same_v<T, int32_t>  ||
    std::is_same_v<T, uint64_t> ||
    std::is_same_v<T, int64_t>  ||
    std::is_same_v<T, float>    ||
    std::is_same_v<T, double>
};

/// Auxiliary trait indicating whether given T array type of builtin objects is supported by the default translator
template<typename T, typename Enabler = void>
struct is_supported_builtin_array : 
    public std::false_type
{ };

template<typename T>
struct is_supported_builtin_array<T, std::enable_if_t<utilities::traits::is_array_v<T> && is_supported_builtin_v<typename T::value_type>>> : 
    public std::true_type
{ };

template<typename T>
constexpr bool is_supported_builtin_array_v = is_supported_builtin_array<T>::value;

/// Auxiliary trait indicating whether given T vector type of builtin objects is supported by the default translator
template<typename T, typename Enabler = void>
struct is_supported_builtin_vector : 
    public std::false_type
{ };

template<typename T>
struct is_supported_builtin_vector<T, std::enable_if_t<utilities::traits::is_vector_v<T> && is_supported_builtin_v<typename T::value_type>>> : 
    public std::true_type
{ };

template<typename T>
constexpr bool is_supported_builtin_vector_v = is_supported_builtin_vector<T>::value;

/// Auxiliary trait indicating whether given T type is supported bit type
template<typename T>
constexpr bool is_supported_bit_type_v { 
    std::is_same_v<T,              ethercat::types::bit> ||
    utilities::traits::is_array_v <ethercat::types::bit> ||
    utilities::traits::is_vector_v<ethercat::types::bit>
};

/// Auxiliary trait indicating whether given T type is string bit type
template<typename T>
constexpr bool is_supported_string_type_v { 
    std::is_same_v<T,              std::string> ||
    utilities::traits::is_array_v <std::string> ||
    utilities::traits::is_vector_v<std::string>
};

/// Auxiliary trait indicating whether given T type is static_string bit type
template<typename T>
constexpr bool is_supported_static_string_type_v { 
    ethercat::types::traits::is_static_string_v       <T> ||
    ethercat::types::traits::is_static_string_array_v <T> ||
    ethercat::types::traits::is_static_string_vector_v<T>
};

/**
 * @brief Auxiliary trait indicating whether given T type is supported by the 
 *    default translator
 * 
 * @tparam T 
 *    type to be examined
 */
template<typename T>
constexpr bool is_supported_v { 
    is_supported_builtin_v<T>            ||
    is_supported_builtin_array_v<T>      ||
    is_supported_builtin_vector_v<T>     ||
    is_supported_bit_type_v<T>           ||
    is_supported_string_type_v<T>        ||
    is_supported_static_string_type_v<T>
};

/**
 * @brief Auxiliary base class resolving to false static_assertion when 
 *    @ref DefaultTranslator is specialized for unsupported type
 */
template<typename T>
struct DefaultTranslatorInvalidSpecialization {
    static_assert(utilities::traits::always_false_v<T>,
        "[ethercat::common::translation] DefaultTranslator has been specialized for unsupported type");
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::default_translator

#endif
