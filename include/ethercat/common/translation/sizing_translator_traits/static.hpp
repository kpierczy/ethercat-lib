/* ============================================================================================================================ *//**
 * @file       static.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 5:20:06 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the StaticSizingTraits traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_STATIC_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_STATIC_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/traits.hpp"
#include "ethercat/common/translation/sizing_translator_traits/base.hpp"
#include "ethercat/common/translation/sizing_translator_traits/sizing_method_traits/static.hpp"
#include "ethercat/common/translation/sizing_translator_traits/sizing_method_traits/static_template.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ======================================================== Used namespaces ======================================================= */

using namespace utilities::traits;

/* ====================================================== StaticSizingTraits ====================================================== */

/**
 * @brief Auxiliary class of the *SizingTraits hierarchy describing traits of the translator with 
 *    respect to the static-sizing @c make_buffer() method
 */
template<typename TranslatorT, typename T>
struct StaticSizingTraits :
    public 
        std::conditional_t< /* Non-static, non-template sizing method */
            is_non_static_static_sizing_method_callable_with_v<TranslatorT>,
                wrap_type_t<StatefullSpecializedSizingTraitsBase>,
        std::conditional_t< /* Static, non-template sizing method */
            is_static_static_sizing_method_callable_with_v<TranslatorT>,
                wrap_type_t<StatelessSpecializedSizingTraitsBase>,
        std::conditional_t< /* Non-static, template sizing method */
            is_non_static_static_sizing_method_template_callable_with_v<TranslatorT, T>,
                wrap_type_t<StatefullGenericSizingTraitsBase>,
        std::conditional_t< /* Static, template sizing method */
            is_static_static_sizing_method_template_callable_with_v<TranslatorT, T>,
                wrap_type_t<StatelessGenericSizingTraitsBase>,
        wrap_type_t<NoSizingTraitsBase>>>>>
{ };

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
