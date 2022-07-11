/* ============================================================================================================================ *//**
 * @file       general_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:42:48 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of general type-traits for the EtherCAT library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_TRAITS_GENERAL_TRAITS_H__
#define __ETHERCAT_COMMON_UTILITIES_TRAITS_GENERAL_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities::traits {

/* ===================================================== Generic types traits ===================================================== */

/**
 * @brief Auxiliary empty class for base-class conditional resolution
 */
struct empty { };

/**
 * @brief Auxiliary alias for choosing between target and empty type based on the 
 *    given conditon
 */
template<bool condition, typename T>
using type_or_empty = std::conditional_t<condition, T, empty>;

/**
 * @brief Auxiliary alias for choosing between target and empty type based on the 
 *    given conditon
 */
template<bool condition, typename T>
using empty_or_type = std::conditional_t<condition, empty, T>;

/**
 * @brief Helper trait wrapping type @p T into a type-dependant context
 * 
 * @tparam T 
 *    wrapped type
 * 
 * @note This trait can be used to prevent specialization of the type T in an unwanted
 *    context (e.g. in argument of std::conditional_t)
 */
template<typename T>
struct wrap_type {
    using type = T;
};

template<typename T>
using wrap_type_t = typename wrap_type<T>::type;

/**
 * @brief Helper integral-constant trait providing false value in a type-dependent context
 * 
 * @tparam T 
 *    dependent type
 * 
 * @note This trait may be used e.g. to trigger static assert only if a class template is 
 *    specialized
 */
template<typename T>
struct always_false : public std::false_type { };

template<typename T>
constexpr bool always_false_v = always_false<T>::value;

/**
 * @brief Helper integral-constant trait providing true value in a type-dependent context
 * 
 * @tparam T 
 *    dependent type
 */
template<typename T>
struct always_true : public std::true_type { };

template<typename T>
constexpr bool always_true_v = always_false<T>::value;

namespace details {

    template<class T>
    struct remove_vref {
        using type = std::remove_volatile_t<std::remove_reference_t<T>>;
    };

} // End namespace details

/**
 * @brief Helper trait removing volatile and reference attribtues from the type @p T
 * 
 * @tparam T 
 *   type to be modified
 */
template<class T>
struct remove_vref : public details::remove_vref<T> { };

/// Helper alias for remove_vref<T>::type
template<class T>
using remove_vref_t = typename remove_vref<T>::type;

namespace details {

    template<class T>
    struct remove_cvref {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };

} // End namespace details

/**
 * @brief Helper trait removing cv and reference attribtues from the type @p T
 * 
 * @tparam T 
 *   type to be modified
 * 
 * @note It's defined as standard trait since C++20
 */
template<class T>
struct remove_cvref : public details::remove_cvref<T> { };

/// Helper alias for remove_cvref<T>::type
template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;

namespace details {

    template<class T, class D>
    struct cvref_derived_no_ref {
        static constexpr bool value = 
            std::is_same_v<
                remove_cvref_t<D>, T
            > && not std::is_same_v<
                D, T&
            >;
    };

} // End namespace details

/**
 * @brief Helper trait checking whether @p D is cv-ref-derived from @p T but is not
 *    @p T&
 * 
 * @tparam T 
 *   base type
 * @tparam D 
 *   derived type
 */
template<class T, class D>
struct cvref_derived_no_ref : public details::cvref_derived_no_ref<T, D> { };

/// Helper alias for remove_vref<T>::type
template<class T, class D>
constexpr bool cvref_derived_no_ref_v = cvref_derived_no_ref<T, D>::value;

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities::traits

#endif
