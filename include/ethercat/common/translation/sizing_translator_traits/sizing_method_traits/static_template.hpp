/* ============================================================================================================================ *//**
 * @file       static_template.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 5:46:05 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Auxiliary template-metaprogramming utilities describing translator type with respect to the static sizing
 *             method template
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_SIZING_METHOD_TRAITS_STATIC_TEMPLATE_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_SIZING_METHOD_TRAITS_STATIC_TEMPLATE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>
#include <experimental/type_traits>
// Private includes
#include "ethercat/config.hpp"
#include "ethercat/common/utilities/traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ======================================================== Used namespaces ======================================================= */

using namespace utilities::traits;
using namespace config::types;

/* ============================================ Static-sizing methods templates traits ============================================ */

/// Helper meta-functions for std::experimental::is_detected checking whether @p TranslatorT defines static static-sizing method template
template<typename T>
struct has_static_static_sizing_method_template {
    template<typename TranslatorT>
    using check = decltype(TranslatorT::template make_buffer<T>);
};

template <typename TranslatorT, typename T>
constexpr bool has_static_static_sizing_method_template_v = 
    std::experimental::is_detected_v<has_static_static_sizing_method_template<T>::check, TranslatorT>;

/// Helper meta-functions for std::experimental::is_detected checking whether @p TranslatorT defines static-sizing method template
template<typename T>
struct has_static_sizing_method_template {
    template<typename TranslatorT>
    using check = decltype(&TranslatorT::template make_buffer<T>);
};

template <typename TranslatorT, typename T>
constexpr bool has_static_sizing_method_template_v = 
    std::experimental::is_detected_v<has_static_sizing_method_template<T>::check, TranslatorT>;

template <typename TranslatorT, typename T>
constexpr bool is_static_sizing_method_template_decltypeable_v = 
    std::experimental::is_detected_v<has_static_sizing_method_template<T>::check, TranslatorT>;

/**
 * @brief Helper trait checking whether static-sizing method template is static (false if static-sizing method
 *    template does not exist at all)
 */
template <typename TranslatorT, typename T>
constexpr bool is_static_sizing_method_template_static_v = 
    has_static_static_sizing_method_template_v<TranslatorT, T>;

/**
 * @brief Helper trait checking whether static-sizing method template is non-static (false if static-sizing method
 *    template does not exist at all)
 */
template <typename TranslatorT, typename T>
constexpr bool is_static_sizing_method_template_non_static_v = 
    has_static_sizing_method_template_v<TranslatorT, T> and
    not has_static_static_sizing_method_template_v<TranslatorT, T>;

/**
 * @brief Helper meta-functions for std::experimental::is_detected checking whether static static-sizing
 *    method template of the @p TranslatorT can be called to obtain buffer object
 */
template<typename T>
struct is_static_static_sizing_method_template_callable_with {
    
    template <typename TranslatorT>
    using check = decltype(
        Span<uint8_t>(
            std::declval<
                remove_cvref_t<decltype(TranslatorT::template make_buffer<T>())>
            >()
        )
    );
};

template<typename TranslatorT, typename T>
constexpr bool is_static_static_sizing_method_template_callable_with_v =
    std::experimental::is_detected_v<is_static_static_sizing_method_template_callable_with<T>::template check, TranslatorT>;

/**
 * @brief Helper meta-functions for std::experimental::is_detected checking whether non-static static-sizing
 *    method template of the @p TranslatorT can be called to obtain buffer object
 */
template<typename T>
struct is_non_static_static_sizing_method_template_callable_with {
    
    template <typename TranslatorT>
    using check = decltype(
        Span<uint8_t>(
            std::declval<
                remove_cvref_t<decltype(std::declval<TranslatorT>().template make_buffer<T>())>
            >()
        )
    );
};

template<typename TranslatorT, typename T>
constexpr bool is_non_static_static_sizing_method_template_callable_with_v =
    not is_static_static_sizing_method_template_callable_with_v<TranslatorT, T> and
        std::experimental::is_detected_v<is_non_static_static_sizing_method_template_callable_with<T>::template check, TranslatorT>;

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
