/* ============================================================================================================================ *//**
 * @file       sizing_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 7th June 2022 7:19:58 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the TranslationTraits traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_TRANSLATOR_TRAITS_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_TRANSLATOR_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>
// Private includes
#include "ethercat/common/translation/sizing_translator_traits/static.hpp"
#include "ethercat/common/translation/sizing_translator_traits/dynamic.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {
    
/* =============================================== TranslationTraits Implementation =============================================== */

/**
 * @brief Traits structure providing compile-time informations about sizing capabilities of the 
 *    requested translator type
 * 
 * @tparam TranslatorT 
 *    translator type to be examined
 * @tparam T
 *    target type to be translated with the given @p TranslatorT
 */
template<typename TranslatorT, typename T>
struct SizingTraits 
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Static-sizing method's traits
    using StaticSizingTraits = details::StaticSizingTraits<TranslatorT, T>;
    /// Dynamic-sizing method's traits
    using DynamicSizingTraits = details::DynamicSizingTraits<TranslatorT, T>;

public: /* --------------------------------------------------- Public constants --------------------------------------------------- */

    /// @c true if @p TranslatorT provides a stateless static-sizing @c make_buffer() method
    static constexpr bool is_static_stateless { StaticSizingTraits::is_stateless };
    /// @c true if @p TranslatorT provides a statefull static-sizing @c make_buffer() method
    static constexpr bool is_static_statefull { StaticSizingTraits::is_statefull };
    /// @c true if @p TranslatorT provides a specialized static-sizing @c make_buffer() method
    static constexpr bool is_static_specialized { StaticSizingTraits::is_specialized };
    /// @c true if @p TranslatorT provides a generic static-sizing @c make_buffer() method
    static constexpr bool is_static_generic { StaticSizingTraits::is_generic };

    /// @c true if @p TranslatorT provides a stateless dynamic-sizing @c make_buffer() method
    static constexpr bool is_dynamic_stateless { DynamicSizingTraits::is_stateless };
    /// @c true if @p TranslatorT provides a statefull dynamic-sizing @c make_buffer() method
    static constexpr bool is_dynamic_statefull { DynamicSizingTraits::is_statefull };
    /// @c true if @p TranslatorT provides a specialized dynamic-sizing @c make_buffer() method
    static constexpr bool is_dynamic_specialized { DynamicSizingTraits::is_specialized };
    /// @c true if @p TranslatorT provides a generic dynamic-sizing @c make_buffer() method
    static constexpr bool is_dynamic_generic { DynamicSizingTraits::is_generic };

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
