/* ============================================================================================================================ *//**
 * @file       templates_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:42:48 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of templates-related type-traits for the EtherCAT library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_TRAITS_TEMPLATES_TRAITS_H__
#define __ETHERCAT_COMMON_UTILITIES_TRAITS_TEMPLATES_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities::traits {

/* =========================================================== Templates ========================================================== */

namespace details {
    
    template <typename T, template <typename...> typename Template>
    struct is_specialization_of : std::false_type {};
    
    template <template <typename...> typename Template, typename... Args>
    struct is_specialization_of<Template<Args...>, Template> : std::true_type {};
}

/**
 * @brief Trait checking whether the type is a specialization a class template
 * @tparam T 
 *    class to be verified
 * @tparam Template 
 *    template to be compared against
 */
template <typename T, template <typename...> typename Template>
struct is_specialization_of : 
    details::is_specialization_of<remove_cvref_t<T>, Template> {};

/// @brief Alias for @ref is_specialization_of<...>::value
template <typename T, template <typename...> typename Template>
inline constexpr bool is_specialization_of_v = 
    is_specialization_of<T, Template>::value;

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities::traits

#endif
