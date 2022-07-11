/* ============================================================================================================================ *//**
 * @file       dynamic.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 5:50:59 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Auxiliary template-metaprogramming utilities describing translator type with respect to the dynamic sizing
 *             method
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_SIZING_METHOD_TRAITS_DYNAMIC_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_SIZING_METHOD_TRAITS_DYNAMIC_H__

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

/* =============================================== Input translation methods traits =============================================== */

/// Helper meta-functions for std::experimental::is_detected checking whether @p TranslatorT defines static dynamic-sizing method 
template<typename TranslatorT>
using has_static_dynamic_sizing_method = decltype(TranslatorT::make_buffer);

template <typename TranslatorT>
constexpr bool has_static_dynamic_sizing_method_v = 
    std::experimental::is_detected_v<has_static_dynamic_sizing_method, TranslatorT>;

/// Helper meta-functions for std::experimental::is_detected checking whether @p TranslatorT defines dynamic-sizing method 
template<typename TranslatorT>
using has_dynamic_sizing_method = decltype(&TranslatorT::make_buffer);

template <typename TranslatorT>
constexpr bool has_dynamic_sizing_method_v = 
    std::experimental::is_detected_v<has_dynamic_sizing_method, TranslatorT>;

template <typename TranslatorT>
constexpr bool is_dynamic_sizing_method_decltypeable_v = 
    std::experimental::is_detected_v<has_dynamic_sizing_method, TranslatorT>;

/**
 * @brief Helper trait checking whether dynamic-sizing method is static (false if dynamic-sizing method
 *    does not exist at all)
 */
template <typename TranslatorT>
constexpr bool is_dynamic_sizing_method_static_v = 
    has_static_dynamic_sizing_method_v<TranslatorT>;

/**
 * @brief Helper trait checking whether dynamic-sizing method is non-static (false if dynamic-sizing method
 *    does not exist at all)
 */
template <typename TranslatorT>
constexpr bool is_dynamic_sizing_method_non_static_v = 
    has_dynamic_sizing_method_v<TranslatorT> and 
    not has_static_dynamic_sizing_method_v<TranslatorT>;

/**
 * @brief Helper meta-functions for std::experimental::is_detected checking whether static dynamic-sizing
 *    method of the @p TranslatorT can be called to obtain buffer object
 */
template<typename T>
struct is_static_dynamic_sizing_method_callable_with {
    
    template <typename TranslatorT>
    using check = decltype(
        Span<uint8_t>(
            std::declval<
                remove_cvref_t<decltype(TranslatorT::make_buffer(std::declval<ArgType<T>>()))>
            >()
        )
    );
};

template<typename TranslatorT, typename T>
constexpr bool is_static_dynamic_sizing_method_callable_with_v =
    std::experimental::is_detected_v<is_static_dynamic_sizing_method_callable_with<T>::template check, TranslatorT>;

/**
 * @brief Helper meta-functions for std::experimental::is_detected checking whether non-static dynamic-sizing
 *    method of the @p TranslatorT can be called to obtain buffer object
 */
template<typename T>
struct is_non_static_dynamic_sizing_method_callable_with {
    
    template <typename TranslatorT>
    using check = decltype(
        Span<uint8_t>(
            std::declval<
                remove_cvref_t<decltype(std::declval<TranslatorT>().make_buffer(std::declval<ArgType<T>>()))>
            >()
        )
    );
};

template<typename TranslatorT, typename T>
constexpr bool is_non_static_dynamic_sizing_method_callable_with_v =
    std::experimental::is_detected_v<is_non_static_dynamic_sizing_method_callable_with<T>::template check, TranslatorT>;
    
/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
