/* ============================================================================================================================ *//**
 * @file       implicit.hpp
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

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_IMPLICIT_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_IMPLICIT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/translator_traits/base.hpp"
#include "ethercat/common/translation/translator_traits/translation_method_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ======================================================== Used namespaces ======================================================= */

using namespace utilities::traits;

/* ============================================================ Helpers =========================================================== */

/**
 * @brief Helper type alias template providing deduction of the target type based on the signature
 *    of the translating routine
 */
template<typename TranslationMethodT>
using signature_deduced_target_t = 
    utilities::traits::remove_cvref_t<utilities::traits::arg_t<TranslationMethodT, 1>>;

/* =============================================== SignatureDeducedTranslatorTraits =============================================== */

template<typename TranslatorT>
struct SignatureDeduced_StatefullInputTranslatorTraits : 
    public StatefullSpecializedTranslatorTraits<TranslatorT, 
        signature_deduced_target_t<decltype(&TranslatorT::translate_to)>,
        is_input_non_static_translation_method_callable_with_with_bitshift_v<TranslatorT,
            signature_deduced_target_t<decltype(&TranslatorT::translate_to)>
        >
    >
{ };

template<typename TranslatorT>
struct SignatureDeduced_StatefullOutputTranslatorTraits : 
    public StatefullSpecializedTranslatorTraits<TranslatorT, 
        signature_deduced_target_t<decltype(&TranslatorT::translate_from)>,
        is_output_non_static_translation_method_callable_with_with_bitshift_v<TranslatorT,
            signature_deduced_target_t<decltype(&TranslatorT::translate_from)>
        >
    >
{ };

template<typename TranslatorT>
struct SignatureDeduced_StatelessInputTranslatorTraits : 
    public StatelessSpecializedTranslatorTraits<TranslatorT, 
        signature_deduced_target_t<decltype(&TranslatorT::translate_to)>,
        is_input_static_translation_method_callable_with_with_bitshift_v<TranslatorT,
            signature_deduced_target_t<decltype(&TranslatorT::translate_to)>
        >
    >
{ };

template<typename TranslatorT>
struct SignatureDeduced_StatelessOutputTranslatorTraits : 
    public StatelessSpecializedTranslatorTraits<TranslatorT, 
        signature_deduced_target_t<decltype(&TranslatorT::translate_from)>,
        is_output_static_translation_method_callable_with_with_bitshift_v<TranslatorT,
            signature_deduced_target_t<decltype(&TranslatorT::translate_from)>
        >
    >
{ };

/**
 * @brief Traits base type deducing target type of the translator from signature of 
 *    translating routines
 */
template<TranslationDirection dir, typename TranslatorT>
struct SignatureDeducedTranslatorTraits :
    public std::conditional_t<dir == TranslationDirection::Input,
        // Input translator
        std::conditional_t< /* Type deduction from non-static method */
            is_input_translation_method_non_static_v<TranslatorT>, 
                wrap_type_t<SignatureDeduced_StatefullInputTranslatorTraits<TranslatorT>>,
        std::conditional_t< /* Type deduction from static method */
            is_input_translation_method_static_v<TranslatorT> and
            is_input_translation_method_decltypeable_v<TranslatorT>,
                wrap_type_t<SignatureDeduced_StatelessInputTranslatorTraits<TranslatorT>>,
        wrap_type_t<NoTranslatorTraits<TranslatorT>>>>,
        // Output translator
        std::conditional_t< /* Type deduction from non-static method */
            is_output_translation_method_non_static_v<TranslatorT>, 
                wrap_type_t<SignatureDeduced_StatefullOutputTranslatorTraits<TranslatorT>>,
        std::conditional_t< /* Type deduction from static method */
            is_output_translation_method_static_v<TranslatorT> and
            is_output_translation_method_decltypeable_v<TranslatorT>,
                wrap_type_t<SignatureDeduced_StatelessOutputTranslatorTraits<TranslatorT>>,
        wrap_type_t<NoTranslatorTraits<TranslatorT>>>>
    >
{ };

/* =================================================== ImplicitTranslatorTraits =================================================== */

/**
 * @brief Helper meta-function for std::experimental::is_detected checking whether 
 *    @p TranslatorT defines target type explicitly
 */
template <typename TranslatorT>
using defines_explicit_target_type = typename TranslatorT::Type;

template <typename TranslatorT>
constexpr bool defines_explicit_target_type_v = std::experimental::is_detected_v<defines_explicit_target_type, TranslatorT>;

/**
 * @brief Helper trait defining nested type @a type as void if TranslatorT::Type
 *    is not detected or as TranslatorT::Type otherwise
 */
template<typename TranslatorT, bool = defines_explicit_target_type_v<TranslatorT>>
struct detected_type_or_void {
    using type = void;
};

template<typename TranslatorT>
struct detected_type_or_void<TranslatorT, true> {
    using type = typename TranslatorT::Type;
};

template<typename TranslatorT>
using detected_type_or_void_t = typename detected_type_or_void<TranslatorT>::type;

/**
 * @brief Traits base type computing whether target type is given explicitly by the @p TranslatorT 
 *    or whether it has to be deduced from translation-routines' signature
 */
template<TranslationDirection dir, typename TranslatorT>
struct ImplicitTranslatorTraits :
    public std::conditional_t<defines_explicit_target_type_v<TranslatorT>, 
        wrap_type_t<ExplicitTranslatorTraits<dir, TranslatorT, detected_type_or_void_t<TranslatorT>>>,
        wrap_type_t<SignatureDeducedTranslatorTraits<dir, TranslatorT>>
    >
{ };

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
