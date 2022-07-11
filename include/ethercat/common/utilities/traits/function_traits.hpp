/* ============================================================================================================================ *//**
 * @file       function_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:42:48 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of callables-related type-traits for the EtherCAT library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_TRAITS_FUNCTION_TRAITS_H__
#define __ETHERCAT_COMMON_UTILITIES_TRAITS_FUNCTION_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>
#include <functional>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities::traits {

/* ===================================================== Callable-types traits ==================================================== */

namespace details {

    /// Helper structure defining dependent-name dispatching to @b void
    struct void_t { using type = void; };

    template<typename T> 
    struct function_traits; 

    /**
     * @brief Base type for function traits
     * 
     * @tparam R 
     *    result type
     * @tparam Args 
     *    argument types
     */
    template<typename R, typename ...Args> 
    struct function_traits_base {

        /// Number of arguments
        static constexpr size_t nargs = sizeof...(Args);

        /// Resulting type
        using result_type = R;

        /// Arguments' types
        template <size_t i>
        struct arg {
            using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
        };

        /// Helper meta-function returning type of the n'th argument or void if index invalid
        template <size_t i>
        struct arg_or_void {
            using type = typename std::conditional<
                i < sizeof...(Args),
                std::tuple_element<i, std::tuple<Args...>>,
                void_t
            >::type::type;
        };

    }; 

    /// Specialization of the trait for simple functions
    template<typename R, typename ...Args> 
    struct function_traits<R (*)(Args...)> :
        public function_traits_base<R, Args...>
    { };

    /// Specialization of the trait for simple functions with variadic template list
    template<typename R, typename ...Args> 
    struct function_traits<R (*)(Args..., ...)> :
        public function_traits_base<R, Args...>
    { };

    /// Specialization of the trait for methods
    template <typename R, typename T, typename ... Args>
    struct function_traits<R (T::*)(Args...)> :
        public function_traits_base<R, Args...>
    { };

    /// Specialization of the trait for methods (const)
    template <typename R, typename T, typename ... Args>
    struct function_traits<R (T::*)(Args...) const> :
        public function_traits_base<R, Args...>
    { };

    /// Specialization of the trait for methods with variadic template list
    template <typename R, typename T, typename ... Args>
    struct function_traits<R (T::*)(Args..., ...)> :
        public function_traits_base<R, Args...>
    { };

    /// Specialization of the trait for methods with variadic template list (const)
    template <typename R, typename T, typename ... Args>
    struct function_traits<R (T::*)(Args..., ...) const> :
        public function_traits_base<R, Args...>
    { };

    /// Specialization of the trait for functors
    template<typename Functor> 
    struct function_traits :
        public function_traits<decltype(&std::remove_reference<Functor>::type::operator())>
    { };

    /// Specialization of the trait for std::function
    template<typename R, typename ...Args> 
    struct function_traits<std::function<R(Args...)>> :
        public function_traits_base<R, Args...>
    { };

} // End namespace details

/**
 * @brief Helper traits used to obtain types related to the callable objects
 */
template<typename T> 
struct function_traits : public details::function_traits<T> { };

/// Alias for number of arguments of callable
template<typename T> 
constexpr size_t nargs = function_traits<T>::nargs;

/// Alias for argument type of callable
template<typename T> 
using result_t = typename function_traits<T>::result_type;

/// Alias for argument type of callable
template<typename T, std::size_t i> 
using arg_t = typename function_traits<T>::template arg<i>::type;

/// Alias for 'argument type or void' helper function of callable
template<typename T, std::size_t i> 
using arg_or_void_t = typename function_traits<T>::template arg_or_void<i>::type;

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities::traits

#endif
