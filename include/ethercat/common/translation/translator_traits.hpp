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

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <optional>
// Private includes
#include "ethercat/common/translation/translator_traits/translator_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* =========================================================== Constants ========================================================== */

/// Auxiliary enumeration identifying translation direction
enum class TranslationDirection { Input, Output, Bidirectional };

/* ======================================================= Direction traits ======================================================= */

/**
 * @brief Boolean constant checking whether given @p dir is input direction
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 */
template<auto dir>
constexpr bool is_input_dir_v =
    (static_cast<TranslationDirection>(dir) == TranslationDirection::Input);

/**
 * @brief Boolean constant checking whether given @p dir is output direction
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 */
template<auto dir>
constexpr bool is_output_dir_v =
    (static_cast<TranslationDirection>(dir) == TranslationDirection::Output);

/**
 * @brief Boolean constant checking whether given @p dir covers both direction
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 */
template<auto dir>
constexpr bool is_bidirectional_dir_v = 
    (static_cast<TranslationDirection>(dir) == TranslationDirection::Bidirectional);

/**
 * @brief Boolean constant checking whether given @p dir covers input direction
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 */
template<auto dir>
constexpr bool is_at_least_input_dir_v = (is_input_dir_v<dir> or
    (static_cast<TranslationDirection>(dir) == TranslationDirection::Bidirectional));

/**
 * @brief Boolean constant checking whether given @p dir covers output direction
 * 
 * @tparam dir 
 *    direction of translation; any enumeration value-compatible with @ref Direction
 */
template<auto dir>
constexpr bool is_at_least_output_dir_v = (is_output_dir_v<dir> or
    (static_cast<TranslationDirection>(dir) == TranslationDirection::Bidirectional));

/* ======================================================= Auxiliary traits ======================================================= */

/**
 * @brief Auxiliary type alias template deciding whether obejct should be passed by value
 *    or by const reference based on it's size
 * 
 * @tparam T 
 *    type to be examined
 */
template<typename T>
using ArgType = details::ArgType<T>;

/* ======================================================= TranslationTraits ====================================================== */

/**
 * @brief Traits structure providing compile-time informations about the @p TranslatorT
 * 
 * @tparam TranslatorT 
 *    translator to be examined
 * @tparam T (optional, default: void)
 *    target type; if @c void traits try to deduce it automatically from thefinition of the
 *    @p TranslatorT type
 */
template<typename TranslatorT, typename T = void>
struct TranslatorTraits
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Alias for Traits base class
    using Traits = details::TranslatorTraits<
        common::utilities::traits::remove_cvref_t<TranslatorT>,
        common::utilities::traits::remove_cvref_t<T>>;
    
public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Translator type
    using TranslatorType = typename Traits::TranslatorType;
    /// Translated type
    using Type = typename Traits::Type;
    /// Auxiliary alias providing type derived from @p Type that can be effectively passed to functions
    using ArgType = typename Traits::ArgType;

public: /* --------------------------------------------------- Public constants --------------------------------------------------- */

    /// @c true if @p TranslatorT is stateless for the target @a Type in the input direction
    static constexpr bool is_input_stateless { Traits::is_input_stateless };
    /// @c true if @p TranslatorT is stateless for the target @a Type in the output direction
    static constexpr bool is_output_stateless { Traits::is_output_stateless };
    /// @c true if @p TranslatorT is stateless for the target @a Type in at least one direction
    static constexpr bool is_stateless { is_input_stateless or is_output_stateless };
    /// @c true if @p TranslatorT is stateless for the target @a Type in both directions
    static constexpr bool is_bidirectional_stateless { is_input_stateless and is_output_stateless };
    /// @c true if @p TranslatorT is stateless for the target @a Type in exactly one direction
    static constexpr bool is_unidirectional_stateless { is_stateless and (not is_bidirectional_stateless) };
    /// @c true if @p TranslatorT is stateless for the target @a Type in the given direction
    template<TranslationDirection dir> static constexpr bool is_stateless_in_dir {
        (dir == TranslationDirection::Input ) ? is_input_stateless         :(
        (dir == TranslationDirection::Output) ? is_output_stateless        :(
                                                is_bidirectional_stateless ))};
         
    /// @c true if @p TranslatorT is statefull for the target @a Type in the input direction
    static constexpr bool is_input_statefull { Traits::is_input_statefull };
    /// @c true if @p TranslatorT is statefull for the target @a Type in the output direction
    static constexpr bool is_output_statefull { Traits::is_output_statefull };
    /// @c true if @p TranslatorT is statefull for the target @a Type in at least one direction
    static constexpr bool is_statefull { is_input_statefull or is_output_statefull };
    /// @c true if @p TranslatorT is statefull for the target @a Type in both directions
    static constexpr bool is_bidirectional_statefull { is_input_statefull and is_output_statefull };
    /// @c true if @p TranslatorT is statefull for the target @a Type in exactly one direction
    static constexpr bool is_unidirectional_statefull { is_statefull and (not is_bidirectional_statefull) };
    /// @c true if @p TranslatorT is statefull for the target @a Type in the given direction (note weak requirement for being statefull in bidirectional direction)
    template<TranslationDirection dir> static constexpr bool is_statefull_in_dir {
        (dir == TranslationDirection::Input ) ? is_input_statefull         :(
        (dir == TranslationDirection::Output) ? is_output_statefull        :(
                                                is_statefull               ))};

    /// @c true if @p TranslatorT is specialized for the target @a Type in the input direction
    static constexpr bool is_input_specialized { Traits::is_input_specialized };
    /// @c true if @p TranslatorT is specialized for the target @a Type in the output direction
    static constexpr bool is_output_specialized { Traits::is_output_specialized };
    /// @c true if @p TranslatorT is specialized for the target @a Type in at least one direction
    static constexpr bool is_specialized { is_input_specialized or is_output_specialized };
    /// @c true if @p TranslatorT is specialized for the target @a Type in both directions
    static constexpr bool is_bidirectional_specialized { is_input_specialized and is_output_specialized };
    /// @c true if @p TranslatorT is specialized for the target @a Type in exactly one direction
    static constexpr bool is_unidirectional_specialized { is_specialized and (not is_bidirectional_specialized) };
    /// @c true if @p TranslatorT is specialized for the target @a Type in the given direction
    template<TranslationDirection dir> static constexpr bool is_specialized_in_dir {
        (dir == TranslationDirection::Input ) ? is_input_specialized         :(
        (dir == TranslationDirection::Output) ? is_output_specialized        :(
                                                is_bidirectional_specialized ))};

    /// @c true if @p TranslatorT is specialized for the target @a Type in the input direction
    static constexpr bool is_input_generic { Traits::is_input_generic };
    /// @c true if @p TranslatorT is generic for the target @a Type in the output direction
    static constexpr bool is_output_generic { Traits::is_output_generic };
    /// @c true if @p TranslatorT is generic for the target @a Type in at least one direction
    static constexpr bool is_generic { is_input_generic or is_output_generic };
    /// @c true if @p TranslatorT is generic for the target @a Type in both directions
    static constexpr bool is_bidirectional_generic { is_input_generic and is_output_generic };
    /// @c true if @p TranslatorT is generic for the target @a Type in exactly one direction
    static constexpr bool is_unidirectional_generic { is_generic and (not is_bidirectional_generic) };
    /// @c true if @p TranslatorT is generic for the target @a Type in the given direction
    template<TranslationDirection dir> static constexpr bool is_generic_in_dir {
        (dir == TranslationDirection::Input ) ? is_input_generic         :(
        (dir == TranslationDirection::Output) ? is_output_generic        :(
                                                is_bidirectional_generic ))};

    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in the input direction
    static constexpr bool handles_input_bitoffset { Traits::handles_input_bitoffset };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in the output direction
    static constexpr bool handles_output_bitoffset { Traits::handles_output_bitoffset };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in at least one direction
    static constexpr bool handles_bitoffset { handles_input_bitoffset or handles_output_bitoffset };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in both directions
    static constexpr bool handles_bidirectional_bitoffset { handles_input_bitoffset and handles_output_bitoffset };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in exactly one direction
    static constexpr bool handles_unidirectional_bitoffset { handles_bitoffset and (not handles_bidirectional_bitoffset) };
    /// @c true if @p TranslatorT handles bit-aligned ofset for the target @a Type in the given direction
    template<TranslationDirection dir> static constexpr bool handles_bitoffset_in_dir {
        (dir == TranslationDirection::Input ) ? handles_input_bitoffset         :(
        (dir == TranslationDirection::Output) ? handles_output_bitoffset        :(
                                                handles_bidirectional_bitoffset ))};

    /// @c true if @p TranslatorT is translator for the target @a Type in the input direction
    static constexpr bool is_input_translator { is_input_stateless or is_input_statefull };
    /// @c true if @p TranslatorT is translator for the target @a Type in the output direction
    static constexpr bool is_output_translator { is_output_stateless or is_output_statefull };
    /// @c true if @p TranslatorT is translator for the target @a Type in at least one direction
    static constexpr bool is_translator { is_input_translator or is_output_translator };
    /// @c true if @p TranslatorT is translator for the target @a Type in both directions
    static constexpr bool is_bidirectional_translator { is_input_translator and is_output_translator };
    /// @c true if @p TranslatorT is translator for the target @a Type in exactly one direction
    static constexpr bool is_unidirectional_translator { is_translator and (not is_bidirectional_translator) };
    /// @c true if @p TranslatorT is translator for the target @a Type in the given direction
    template<TranslationDirection dir> static constexpr bool is_translator_in_dir {
        (dir == TranslationDirection::Input ) ? is_input_translator         :(
        (dir == TranslationDirection::Output) ? is_output_translator        :(
                                                is_bidirectional_translator ))};

    /// Direction of supported translation
    static constexpr std::optional<TranslationDirection> direction = 
        (is_input_translator && is_output_translator) ? TranslationDirection::Bidirectional  :(
        (is_input_translator                        ) ? TranslationDirection::Input          :(
        (is_output_translator                       ) ? TranslationDirection::Output         : 
                                                        std::optional<TranslationDirection>{} ));
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
