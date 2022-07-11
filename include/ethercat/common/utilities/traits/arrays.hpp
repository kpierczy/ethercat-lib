/* ============================================================================================================================ *//**
 * @file       trarraysits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of auxiliary traits used to examine array-like types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_TRAITS_ARRAYS_H__
#define __ETHERCAT_COMMON_UTILITIES_TRAITS_ARRAYS_H__

/* =========================================================== Includes =========================================================== */

// System includes
#include <type_traits>
#include <array>
#include <vector>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities::traits {

/* =========================================================== is_array =========================================================== */

// Implementation of is_array
template<typename T>
struct is_array_impl : 
    std::false_type
{ };

// Specialization for arrays
template<typename T, std::size_t N>
struct is_array_impl<std::array<T, N>> : 
    std::true_type
{ };

/**
 * @brief Auxiliary trait checking whether @p T is an std::array
 * 
 * @tparam T 
 *    type to be examined
 */
template<typename T>
using is_array = is_array_impl<T>;

// Alias for is_array<...>::value
template<typename T>
constexpr bool is_array_v = is_array<T>::value;

/* =========================================================== is_vector ========================================================== */

// Implementation of is_vector
template<typename T>
struct is_vector_impl : 
    std::false_type
{ };

// Specialization for arrays
template<typename T>
struct is_vector_impl<std::vector<T>> : 
    std::true_type
{ };

/**
 * @brief Auxiliary trait checking whether @p T is an std::vector
 * 
 * @tparam T 
 *    type to be examined
 */
template<typename T>
using is_vector = is_vector_impl<T>;

// Alias for is_vector<...>::value
template<typename T>
constexpr bool is_vector_v = is_vector<T>::value;

/* ======================================================= is_plain_or_array ====================================================== */

// Implementation of is_plain_or_array
template<typename T, typename Target>
struct is_plain_or_array_impl : 
    std::false_type
{ };

// Specialization for identity
template<typename Target>
struct is_plain_or_array_impl<Target, Target> : 
    std::true_type
{ };

// Specialization for array
template<typename Target, std::size_t N>
struct is_plain_or_array_impl<std::array<Target, N>, Target> : 
    std::true_type
{ };

// Specialization for vector
template<typename Target>
struct is_plain_or_array_impl<std::vector<Target>, Target> : 
    std::true_type
{ };

/**
 * @brief Auxiliary trait checking whether @p T is of type @p Target or an 
 *   std::vector/std::array derived from @p Target
 * 
 * @tparam T 
 *    type to be examined
 * @tparam Target 
 *    target type
 */
template<typename T, typename Target>
using is_plain_or_array = is_plain_or_array_impl<T, Target>;

// Alias for is_plain_or_array<...>::value
template<typename T, typename Target>
constexpr bool is_plain_or_array_v = is_plain_or_array<T, Target>::value;

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities::traits

#endif
