/* ============================================================================================================================ *//**
 * @file       base.hpp
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

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_BASE_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_TRAITS_BASE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ======================================================== Used namespaces ======================================================= */

using namespace ethercat::common::utilities::traits;

/* =========================================================== Constants ========================================================== */

/// Auxiliary enumeration identifying translation direction
enum class TranslationDirection { Input, Output };

/* ======================================================= Common utilities ======================================================= */

/**
 * @brief Auxiliary compile-time function computing whether the given type fits 
 *    into machine regster
 * 
 * @tparam T 
 *    type to be examined
 * @retval true 
 *    if type fits into the machine's register
 * @retval false 
 *    otherwise
 * 
 * @warning This check relies on how usually pointer types are implemented by C++ compilers
 *    (it holds for most popular ones on x86/x64 platforms like MSVC, GCC, clang). It is not
 *    guaranted to return true answer.
 */
template<typename T>
constexpr bool fits_in_register() { return (sizeof(T) <= sizeof(void*)); }

/**
 * @brief Auxiliary type alias template deciding whether obejct should be passed by value
 *    or by const reference based on it's size
 * 
 * @tparam T 
 *    type to be examined
 */
template<typename T>
using ArgType = std::conditional_t<
    std::is_same_v<T, void> || fits_in_register<utilities::traits::remove_cvref_t<T>>(),
        utilities::traits::remove_cvref_t<T>,
        const utilities::traits::remove_cvref_t<T>&
    >;

/* ===================================================== TranslatorTraitsBase ===================================================== */

/**
 * @brief Base class of the *TranslatorTraits hierarchy providing type aliases for the given 
 *    @p TranslatorT translator type and @p T target type deduced by the lower traits in the
 *    hierarchy
 */
template<typename TranslatorT, typename T>
struct TranslatorTraitsBase {

    /// Translator type
    using TranslatorType = TranslatorT;
    /// Translated type
    using Type = utilities::traits::remove_cvref_t<T>;
    /// Auxiliary alias providing type derived from @p Type that can be effectively passed to functions
    using ArgType = details::ArgType<T>;
    
};

/**
 * @brief Indirect base class of the *TranslatorTraits hierarchy verifying whether translation routine 
 *    handle bit-aligned objects
 */
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct TranslatorTraitsBitoffsetHandling :
    public TranslatorTraitsBase<TranslatorT, T>
{
    /// Compile-time flag stating whether translation routine handle bit-aligned objects
    static constexpr bool handles_bitoffset = handles_bitoffset_v;
};

/* ====================================================== NoTranslatorTraits ====================================================== */

/**
 * @brief Indirect base class of the *TranslatorTraits hierarchy describing type that is not a translator
 */
template<typename TranslatorT, typename T = void>
struct NoTranslatorTraits :
    public TranslatorTraitsBitoffsetHandling<TranslatorT, T, false>
{ 
    static constexpr bool is_statefull   = false;
    static constexpr bool is_stateless   = false;
    static constexpr bool is_specialized = false;
    static constexpr bool is_generic     = false;
};

/* =================================================== StatefullTranslatorTraits ================================================== */

/**
 * @brief Indirect base class of the *TranslatorTraits hierarchy describing statefull translator type
 */
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct StatefullTranslatorTraits :
    public TranslatorTraitsBitoffsetHandling<TranslatorT, T, handles_bitoffset_v>
{ 
    static constexpr bool is_statefull = true;
    static constexpr bool is_stateless = not is_statefull;
};

/// Specialization of @ref TranslatorTraitsBase for specialized translators
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct StatefullSpecializedTranslatorTraits :
    public StatefullTranslatorTraits<TranslatorT, T, handles_bitoffset_v>
{ 
    static constexpr bool is_specialized = true;
    static constexpr bool is_generic     = not is_specialized;
};

/// Specialization of @ref TranslatorTraitsBase for generic translators
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct StatefullGenericTranslatorTraits :
    public StatefullTranslatorTraits<TranslatorT, T, handles_bitoffset_v>
{ 
    static constexpr bool is_specialized = false;
    static constexpr bool is_generic     = not is_specialized;
};

/* =================================================== StatelessTranslatorTraits ================================================== */

/**
 * @brief Indirect base class of the *TranslatorTraits hierarchy describing stateless translator type
 */
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct StatelessTranslatorTraits :
    public TranslatorTraitsBitoffsetHandling<TranslatorT, T, handles_bitoffset_v>
{ 
    static constexpr bool is_statefull = false;
    static constexpr bool is_stateless = not is_statefull;
};

/// Specialization of @ref StatelessTranslatorTraits for specialized translators
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct StatelessSpecializedTranslatorTraits :
    public StatelessTranslatorTraits<TranslatorT, T, handles_bitoffset_v>
{ 
    static constexpr bool is_specialized = true;
    static constexpr bool is_generic     = not is_specialized;
};

/// Specialization of @ref StatelessTranslatorTraits for generic translators
template<typename TranslatorT, typename T, bool handles_bitoffset_v>
struct StatelessGenericTranslatorTraits :
    public StatelessTranslatorTraits<TranslatorT, T, handles_bitoffset_v>
{ 
    static constexpr bool is_specialized = false;
    static constexpr bool is_generic     = not is_specialized;
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
