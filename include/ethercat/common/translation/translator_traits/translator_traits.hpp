/* ============================================================================================================================ *//**
 * @file       translator_traits.hpp
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

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_TRANSLATOR_TRAITS_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_TRANSLATOR_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>
// Private includes
#include "ethercat/common/translation/translator_traits/explicit.hpp"
#include "ethercat/common/translation/translator_traits/implicit.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {
    
/* =============================================== TranslationTraits Implementation =============================================== */

/// TranslationTraits type dispatching manual/auto-deduction of the target type
template<TranslationDirection dir, typename TranslatorT, typename T>
struct TranslatorTraitsDispatch :
    public std::conditional_t<std::is_same_v<T, void>, 
        utilities::traits::wrap_type_t<ImplicitTranslatorTraits<dir, TranslatorT>>,
        utilities::traits::wrap_type_t<ExplicitTranslatorTraits<dir, TranslatorT, T>>
    >
{ };

/**
 * @brief Traits structure providing compile-time informations about requested translator type
 * 
 * @tparam TranslatorT 
 *    translator type to be examined
 * @tparam T (optional, default: void)
 *    target type to be translated with the given @p TranslatorT
 */
template<typename TranslatorT, typename T = void>
struct TranslatorTraits :
    private TranslatorTraitsDispatch<TranslationDirection::Input, TranslatorT, T>,
    private TranslatorTraitsDispatch<TranslationDirection::Output, TranslatorT, T>
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Alias for InputTranslatorTraits base class
    using InputTranslatorTraits = TranslatorTraitsDispatch<TranslationDirection::Input, TranslatorT, T>;
    /// Alias for OutputTranslatorTraits base class
    using OutputTranslatorTraits = TranslatorTraitsDispatch<TranslationDirection::Output, TranslatorT, T>;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Sanity-check whether translator types deduced by both bases match
    static_assert(std::is_same_v<
        typename InputTranslatorTraits::TranslatorType,
        typename OutputTranslatorTraits::TranslatorType
    >, "[ethercat::common::translation::BidirectionalTranslatorTraits][BUG] TranslatorType deduced by base classes are not identical!");

    /// Sanity-check whether translated types deduced by both bases match
    static_assert(std::is_same_v<
        typename InputTranslatorTraits::Type,
        typename OutputTranslatorTraits::Type
    >, "[ethercat::common::translation::BidirectionalTranslatorTraits][BUG] Type deduced by base classes are not identical!");

    /// Sanity-check whether argument types deduced by both bases match
    static_assert(std::is_same_v<
        typename InputTranslatorTraits::ArgType,
        typename OutputTranslatorTraits::ArgType
    >, "[ethercat::common::translation::BidirectionalTranslatorTraits][BUG] ArgType deduced by base classes are not identical!");

    /// Translator type
    using TranslatorType = typename InputTranslatorTraits::TranslatorType;
    /// Translated type
    using Type = typename InputTranslatorTraits::Type;
    /// Auxiliary alias providing type derived from @p Type that can be effectively passed to functions
    using ArgType = typename InputTranslatorTraits::ArgType;

public: /* --------------------------------------------------- Public constants --------------------------------------------------- */

    /// @c true if @p TranslatorT is stateless for the target @a Type in the input direction
    static constexpr bool is_input_stateless { InputTranslatorTraits::is_stateless };
    /// @c true if @p TranslatorT is statefull for the target @a Type in the input direction
    static constexpr bool is_input_statefull { InputTranslatorTraits::is_statefull };
    /// @c true if @p TranslatorT is specialized for the target @a Type in the input direction
    static constexpr bool is_input_specialized { InputTranslatorTraits::is_specialized }; 
    /// @c true if @p TranslatorT is generic for the target @a Type in the input direction
    static constexpr bool is_input_generic { InputTranslatorTraits::is_generic };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in the input direction
    static constexpr bool handles_input_bitoffset { InputTranslatorTraits::handles_bitoffset };

    /// @c true if @p TranslatorT is stateless for the target @a Type in the output direction
    static constexpr bool is_output_stateless { OutputTranslatorTraits::is_stateless };
    /// @c true if @p TranslatorT is statefull for the target @a Type in the output direction
    static constexpr bool is_output_statefull { OutputTranslatorTraits::is_statefull };
    /// @c true if @p TranslatorT is specialized for the target @a Type in the output direction
    static constexpr bool is_output_specialized { OutputTranslatorTraits::is_specialized }; 
    /// @c true if @p TranslatorT is generic for the target @a Type in the output direction
    static constexpr bool is_output_generic { OutputTranslatorTraits::is_generic };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in the output direction
    static constexpr bool handles_output_bitoffset { OutputTranslatorTraits::handles_bitoffset };

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
