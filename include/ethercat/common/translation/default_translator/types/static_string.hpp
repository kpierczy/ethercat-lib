/* ============================================================================================================================ *//**
 * @file       static_string.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of an auxiliary type providing statically-allocated string interface
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_TYPES_STATIC_STRING_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_TYPES_STATIC_STRING_H__

/* =========================================================== Includes =========================================================== */

// System includes
#include <type_traits>
// Private includes
#include "ethercat/config.hpp"
#include "ethercat/common/utilities/traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ============================================================= Class ============================================================ */

/**
 * @brief Storage class wrapping a staticaly allocated buffer of characters with a mutable
 *   interface of the std::string_view
 * 
 * @tparam N 
 *    number of characters in the string (excluding trailing \0 byte)
 */
template<std::size_t N>
struct static_string : 
    public config::types::StaticString<N>
{ };

/* ============================================================ Traits ============================================================ */

namespace traits {

/**
 * @brief Auxiliary trait indicating whether given type is a static_string
 * 
 * @tparam T 
 *    type to be examined
 */
template<typename T>
struct is_static_string :
    public std::false_type
{ };

template<std::size_t N>
struct is_static_string<static_string<N>> :
    public std::true_type
{ };

template<typename T>
constexpr bool is_static_string_v = is_static_string<T>::value;

/// Auxiliary trait indicating whether given T array type is supported by the default translator
template<typename T, typename Enabler = void>
struct is_static_string_array : 
    public std::false_type
{ };

template<typename T>
struct is_static_string_array<T, 
    std::enable_if_t<common::utilities::traits::is_array_v<T> && is_static_string_v<typename T::value_type>>
> : 
    public std::true_type
{ };

template<typename T>
constexpr bool is_static_string_array_v = is_static_string_array<T>::value;

/// Auxiliary trait indicating whether given T vector type is supported by the default translator
template<typename T, typename Enabler = void>
struct is_static_string_vector : 
    public std::false_type
{ };

template<typename T>
struct is_static_string_vector<T, 
    std::enable_if_t<common::utilities::traits::is_vector_v<T> && is_static_string_v<typename T::value_type>>
> : 
    public std::true_type
{ };

template<typename T>
constexpr bool is_static_string_vector_v = is_static_string_vector<T>::value;

}

/* ================================================================================================================================ */

} // End namespace ethercat::types

#endif
