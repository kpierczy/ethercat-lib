/* ============================================================================================================================ *//**
 * @file       input.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 7th June 2022 7:19:58 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Auxiliary template-metaprogramming utilities describing translator type with respect to the input translation
 *             method
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_TRANSLATION_METHOD_TRAITS_INPUT_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_TRANSLATION_METHOD_TRAITS_INPUT_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>
#include <experimental/type_traits>
// Private includes
#include "ethercat/common/utilities/traits.hpp"
#include "ethercat/config.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ======================================================== Used namespaces ======================================================= */

using namespace utilities::traits;
using namespace config::types;

/* =============================================== Input translation methods traits =============================================== */

/// Helper meta-functions for std::experimental::is_detected checking whether @p TranslatorT defines static input translation method
template<typename TranslatorT>
using has_input_static_translation_method = decltype(TranslatorT::translate_to);

template <typename TranslatorT>
constexpr bool has_input_static_translation_method_v = 
    std::experimental::is_detected_v<has_input_static_translation_method, TranslatorT>;

/// Helper meta-functions for std::experimental::is_detected checking whether @p TranslatorT defines input translation method
template<typename TranslatorT>
using has_input_translation_method = decltype(&TranslatorT::translate_to);

template <typename TranslatorT>
constexpr bool has_input_translation_method_v = 
    std::experimental::is_detected_v<has_input_translation_method, TranslatorT>;

template <typename TranslatorT>
constexpr bool is_input_translation_method_decltypeable_v = 
    std::experimental::is_detected_v<has_input_translation_method, TranslatorT>;

/**
 * @brief Helper trait checking whether input translation method is static (false if input translation method
 *    does not exist at all)
 */
template <typename TranslatorT>
constexpr bool is_input_translation_method_static_v = 
    has_input_static_translation_method_v<TranslatorT>;

/**
 * @brief Helper trait checking whether input translation method is non-static (false if input translation method
 *    does not exist at all)
 */
template <typename TranslatorT>
constexpr bool is_input_translation_method_non_static_v = 
    has_input_translation_method_v<TranslatorT> and 
    not has_input_static_translation_method_v<TranslatorT>;

/**
 * @brief Helper meta-functions for std::experimental::is_detected checking whether input static translation
 *    method of the @p TranslatorT can be called with object of type T
 */
template<typename T>
struct is_input_static_translation_method_callable_with {
    
    template <typename TranslatorT>
    using check_without_bitshift = decltype(
        TranslatorT::translate_to(std::declval<Span<const uint8_t>>(), std::declval<T&>()));
    
    template <typename TranslatorT>
    using check_with_bitshift = decltype(
        TranslatorT::translate_to(std::declval<Span<const uint8_t>>(), std::declval<T&>(), std::declval<std::size_t>()));
};

template<typename TranslatorT, typename T>
constexpr bool is_input_static_translation_method_callable_with_without_bitshift_v =
    std::experimental::is_detected_v<is_input_static_translation_method_callable_with<T>::template check_without_bitshift, TranslatorT>;

template<typename TranslatorT, typename T>
constexpr bool is_input_static_translation_method_callable_with_with_bitshift_v =
    std::experimental::is_detected_v<is_input_static_translation_method_callable_with<T>::template check_with_bitshift, TranslatorT>;

template<typename TranslatorT, typename T>
constexpr bool is_input_static_translation_method_callable_with_v =
    std::experimental::is_detected_v<is_input_static_translation_method_callable_with<T>::template check_without_bitshift, TranslatorT> or
    std::experimental::is_detected_v<is_input_static_translation_method_callable_with<T>::template check_with_bitshift,    TranslatorT>;

/**
 * @brief Helper meta-functions for std::experimental::is_detected checking whether input non-static translation
 *    method of the @p TranslatorT can be called with object of type T
 */
template<typename T>
struct is_input_non_static_translation_method_callable_with {
    
    template <typename TranslatorT>
    using check_without_bitshift = decltype(
        std::declval<TranslatorT>().translate_to(std::declval<Span<const uint8_t>>(), std::declval<T&>()));
    
    template <typename TranslatorT>
    using check_with_bitshift = decltype(
        std::declval<TranslatorT>().translate_to(std::declval<Span<const uint8_t>>(), std::declval<T&>(), std::declval<std::size_t>()));
};

template<typename TranslatorT, typename T>
constexpr bool is_input_non_static_translation_method_callable_with_without_bitshift_v =
    not is_input_static_translation_method_callable_with_without_bitshift_v<TranslatorT, T> and
        std::experimental::is_detected_v<is_input_non_static_translation_method_callable_with<T>::template check_without_bitshift, TranslatorT>;

template<typename TranslatorT, typename T>
constexpr bool is_input_non_static_translation_method_callable_with_with_bitshift_v =
    not is_input_static_translation_method_callable_with_with_bitshift_v<TranslatorT, T> and
        std::experimental::is_detected_v<is_input_non_static_translation_method_callable_with<T>::template check_with_bitshift, TranslatorT>;

template<typename TranslatorT, typename T>
constexpr bool is_input_non_static_translation_method_callable_with_v = 
    not is_input_static_translation_method_callable_with_v<TranslatorT, T> and (
        std::experimental::is_detected_v<is_input_non_static_translation_method_callable_with<T>::template check_without_bitshift, TranslatorT> or
        std::experimental::is_detected_v<is_input_non_static_translation_method_callable_with<T>::template check_with_bitshift,    TranslatorT>
    );
    
/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
