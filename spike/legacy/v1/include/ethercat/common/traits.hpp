/* ============================================================================================================================ *//**
 * @file       traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:42:48 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of common type-traits for the EtherCAT library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRAITS_H__
#define __ETHERCAT_COMMON_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <functional>
#include <type_traits>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::traits {

/* ===================================================== Generic types traits ===================================================== */

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

/* ===================================================== Callable-types traits ==================================================== */

namespace details {

    /// Helper structure defining dependent-name dispatching to @b void
    struct void_t { using type = void; };

    template<typename T> 
    struct function_traits;  

    /// Specialization of the trait for std::function
    template<typename R, typename ...Args> 
    struct function_traits<std::function<R(Args...)>> {

        /// Number of arguments
        static const size_t nargs = sizeof...(Args);

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

} // End namespace details

/**
 * @brief Helper traits used to obtain types related to the callable objects
 */
template<typename T> 
struct function_traits : public details::function_traits<T> { };

/// Alias for argument type of callable
template<typename T, std::size_t i> 
using arg_t = typename function_traits<T>::template arg<i>::type;

/// Alias for argument type of callable
template<typename T> 
using result_t = typename function_traits<T>::result_type;

/* ======================================================= std::tuple traits ====================================================== */

namespace details {

    /**
     * @brief Helper compile-time function calculating index corresponding to the given type
     *    held by std::tuple
     * 
     * @tparam N 
     *    index of the first type to be checked 
     * @tparam T 
     *    requested type
     * @tparam Tuple 
     *    inspected tuple
     * @returns 
     *    index of the variant storing type @p T
     */
    template< std::size_t N, typename T, typename Tuple>
    constexpr std::size_t get_index_of() {

        static_assert(N < std::tuple_size<Tuple>::value, "[ethercat::traits::details::get_index_of] The element is not in the tuple");

        if constexpr(std::is_same<T, typename std::tuple_element<N, Tuple>::type>::value )
            return N;
        else
            return get_index_of<N + 1, T, Tuple>();
    }

    /// Trait wrapping @ref get_index_of() function
    template<typename T, typename Tuple>
    struct index_of {
        static constexpr size_t value = get_index_of<0, T, Tuple>();
    };

}

/**
 * @brief Helper trait defining @a value field storing index of the given @p T type
 *   in the list of types of the @p Tuple specialization of std::tuple
 * 
 * @tparam T 
 *    type to be found
 * @tparam Tuple 
 *    tuple defining list of types to search
 */
template<typename T, typename Tuple>
struct index_of : 
    public details::index_of<T, Tuple> 
{ };

/// Alias for index_of<T, Tuple>::value
template<typename T, typename Tuple>
constexpr std::size_t index_of_v = index_of<T, Tuple>::value;

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

} // End namespace ethercat::traits

#endif
