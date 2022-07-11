/* ============================================================================================================================ *//**
 * @file       explicit.hpp
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

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_EXPLICIT_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_EXPLICIT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/translator_traits/base.hpp"
#include "ethercat/common/translation/translator_traits/translation_method_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ======================================================== Used namespaces ======================================================= */

using namespace utilities::traits;

/* =================================================== ExplicitTranslatorTraits =================================================== */

template<typename TranslatorT, typename T>
struct Explicit_StatefullSpecializedInputTranslatorTraits :
    StatefullSpecializedTranslatorTraits<TranslatorT, T, 
        is_input_non_static_translation_method_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatelessSpecializedInputTranslatorTraits :
    StatelessSpecializedTranslatorTraits<TranslatorT, T, 
        is_input_static_translation_method_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatefullGenericInputTranslatorTraits :
    StatefullGenericTranslatorTraits<TranslatorT, T, 
        is_input_non_static_translation_method_template_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatelessGenericInputTranslatorTraits :
    StatelessGenericTranslatorTraits<TranslatorT, T, 
        is_input_static_translation_method_template_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatefullSpecializedOutputTranslatorTraits :
    StatefullSpecializedTranslatorTraits<TranslatorT, T, 
        is_output_non_static_translation_method_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatelessSpecializedOutputTranslatorTraits :
    StatelessSpecializedTranslatorTraits<TranslatorT, T, 
        is_output_static_translation_method_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatefullGenericOutputTranslatorTraits :
    StatefullGenericTranslatorTraits<TranslatorT, T, 
        is_output_non_static_translation_method_template_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

template<typename TranslatorT, typename T>
struct Explicit_StatelessGenericOutputTranslatorTraits :
    StatelessGenericTranslatorTraits<TranslatorT, T, 
        is_output_static_translation_method_template_callable_with_with_bitshift_v<TranslatorT, T>
    >
{ };

/**
 * @brief Direct base class of the *TranslatorTraits hierarchy describing translator with 
 *    explicitly defined target type
 */
template<TranslationDirection dir, typename TranslatorT, typename T>
struct ExplicitTranslatorTraits :
    public std::conditional_t<dir == TranslationDirection::Input,
        // Input translator
        std::conditional_t< /* Non-static, non-template translation method */
            is_input_non_static_translation_method_callable_with_v<TranslatorT, T>, 
                wrap_type_t<Explicit_StatefullSpecializedInputTranslatorTraits<TranslatorT, T>>,
        std::conditional_t< /* Static, non-template translation method */
            is_input_static_translation_method_callable_with_v<TranslatorT, T>,
                wrap_type_t<Explicit_StatelessSpecializedInputTranslatorTraits<TranslatorT, T>>,
        std::conditional_t< /* Non-static, template translation method */
            is_input_non_static_translation_method_template_callable_with_v<TranslatorT, T>, 
                wrap_type_t<Explicit_StatefullGenericInputTranslatorTraits<TranslatorT, T>>,
        std::conditional_t< /* Static, template translation method */
            is_input_static_translation_method_template_callable_with_v<TranslatorT, T>,
                wrap_type_t<Explicit_StatelessGenericInputTranslatorTraits<TranslatorT, T>>,
        wrap_type_t<NoTranslatorTraits<TranslatorT, T>>>>>>,
        // Output translator
        std::conditional_t< /* Non-static, non-template translation method */
            is_output_non_static_translation_method_callable_with_v<TranslatorT, T>, 
                wrap_type_t<Explicit_StatefullSpecializedOutputTranslatorTraits<TranslatorT, T>>,
        std::conditional_t< /* Static, non-template translation method */
            is_output_static_translation_method_callable_with_v<TranslatorT, T>,
                wrap_type_t<Explicit_StatelessSpecializedOutputTranslatorTraits<TranslatorT, T>>,
        std::conditional_t< /* Non-static, template translation method */
            is_output_non_static_translation_method_template_callable_with_v<TranslatorT, T>, 
                wrap_type_t<Explicit_StatefullGenericOutputTranslatorTraits<TranslatorT, T>>,
        std::conditional_t< /* Static, template translation method */
            is_output_static_translation_method_template_callable_with_v<TranslatorT, T>,
                wrap_type_t<Explicit_StatelessGenericOutputTranslatorTraits<TranslatorT, T>>,
        wrap_type_t<NoTranslatorTraits<TranslatorT, T>>>>>>
    >
{ };

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
