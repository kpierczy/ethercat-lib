/* ============================================================================================================================ *//**
 * @file       translator_wrapper.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 7th June 2022 7:19:58 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the TranslatorWrapper traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_WRAPPER_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_WRAPPER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/traits.hpp"
#include "ethercat/common/translation/translator_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ================================================ TranslatorWrapperStatefullBase ================================================ */

namespace details {

/**
 * @brief Base of the @ref TranslatorWrapper for statefull translators
 */
template<typename TranslatorT, typename T>
class TranslatorWrapperStatefullBase {

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Construct a new TranslatorWrapperStatefullBase passing given arguments
     *    to the wrapped translator's constructor
     * 
     * @tparam ArgsT 
     *    types of @p args
     * @param args 
     *    arguments to be passed to the wrapped translator's constructor
     */
    template<typename... ArgsT>
    constexpr TranslatorWrapperStatefullBase(ArgsT&&... args);

    // Enable default constructor (if available)
    TranslatorWrapperStatefullBase() = default;

    /// Default copy-construction
    TranslatorWrapperStatefullBase(const TranslatorWrapperStatefullBase &rwrapper) = default;
    /// Default copy-asignment
    TranslatorWrapperStatefullBase &operator=(const TranslatorWrapperStatefullBase &rwrapper) = default;
    /// Default move-construction
    TranslatorWrapperStatefullBase(TranslatorWrapperStatefullBase &&rwrapper) = default;
    /// Default move-asignment
    TranslatorWrapperStatefullBase &operator=(TranslatorWrapperStatefullBase &&rwrapper) = default;

public: /* ---------------------------------------------------- Public members ---------------------------------------------------- */

    /// Wrapped statefull translator
    TranslatorT translator;

};

}

/* ======================================================= TranslatorWrapper ====================================================== */

/**
 * @brief Class template wrapping an arbitrary translator type with a unified translating interface
 * 
 * @tparam dir 
 *    translation direction
 * @tparam TranslatorT 
 *    type of the wrapped translator
 * @tparam T (optional, default: void)
 *    target type; if void wrapped tries to deduce it automatically
 * 
 * @performance TranslatorWrapper provides only wrappers of translation methods that take a bitoffset
 *    as an argument. This could potentially force parsing an unused parameter if the wrapped translator
 *    does not handle it. However, body of each wrapper function is very small and the call should in practice
 *    be inlined and so unused parameter would not be passed. If it turns out to be false assumption, change
 *    implementation in the future
 */
template<TranslationDirection dir, typename TranslatorT, typename T = void>
class TranslatorWrapper : 

    /// Statefull base
    private utilities::traits::type_or_empty<
        TranslatorTraits<TranslatorT, T>::template is_statefull_in_dir<dir>,
            details::TranslatorWrapperStatefullBase<TranslatorT, T>>
{

private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Base type
    using Base = utilities::traits::type_or_empty<
        TranslatorTraits<TranslatorT, T>::template is_statefull_in_dir<dir>,
            details::TranslatorWrapperStatefullBase<TranslatorT, T>>;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Traits of the translator type
    using Traits = TranslatorTraits<TranslatorT, T>;

    /// Translator type
    using TranslatorType = typename Traits::TranslatorType;
    /// Target type
    using Type = typename Traits::Type;
    /// Argument type ( @ref TranslatorTraits )
    using ArgType = typename Traits::ArgType;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Construct a new TranslatorWrapper passing given arguments
     *    to the wrapped translator's constructor
     * 
     * @tparam ArgsT 
     *    types of @p args
     * @param args 
     *    arguments to be passed to the wrapped translator's constructor
     * 
     * @note Constructor is enabled only if wrapped translator is statefull
     */
    template<typename... ArgsT, typename TraitsT = Traits,
        std::enable_if_t<TraitsT::template is_statefull_in_dir<dir>, bool> = true>
    constexpr TranslatorWrapper(ArgsT&&... args);

    // Enable default constructor (if available)
    TranslatorWrapper() = default;

    /// Default copy-construction
    TranslatorWrapper(const TranslatorWrapper &rwrapper) = default;
    /// Default copy-asignment
    TranslatorWrapper &operator=(const TranslatorWrapper &rwrapper) = default;
    /// Default move-construction
    TranslatorWrapper(TranslatorWrapper &&rwrapper) = default;
    /// Default move-asignment
    TranslatorWrapper &operator=(TranslatorWrapper &&rwrapper) = default;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @brief Translated buffer of raw bytes into the target object with bit-aligned layout
     * 
     * @tparam TraitsT 
     *    (unused) type used to provide enabling condition
     * @param data 
     *    source data buffer
     * @param[out] obj 
     *    target object
     * @param bitoffset 
     *    bitoffset of data in @p data buffer
     */
    template<typename TraitsT = Traits,
        std::enable_if_t<is_at_least_input_dir_v<dir> and TraitsT::is_input_statefull, bool> = true>
    constexpr auto translate_to(
        config::types::Span<const uint8_t> data,
        Type &obj,
        std::size_t bitoffset = 0
    );

    /**
     * @brief Translated the target object into buffer of raw bytes with bit-aligned layout
     * 
     * @tparam TraitsT 
     *    (unused) type used to provide enabling condition
     * @param[out] data 
     *    source data buffer
     * @param obj 
     *    target object
     * @param bitoffset 
     *    bitoffset of data in @p data buffer
     */
    template<typename TraitsT = Traits,
        std::enable_if_t<is_at_least_output_dir_v<dir> and TraitsT::is_output_statefull, bool> = true>
    constexpr auto translate_from(
        config::types::Span<uint8_t> data,
        ArgType obj,
        std::size_t bitoffset = 0
    );

public: /* ------------------------------------------------- Public static methods ------------------------------------------------ */

    /**
     * @brief Translated buffer of raw bytes into the target object with bit-aligned layout
     * 
     * @tparam TraitsT 
     *    (unused) type used to provide enabling condition
     * @param data 
     *    source data buffer
     * @param[out] obj 
     *    target object
     * @param bitoffset 
     *    bitoffset of data in @p data buffer
     */
    template<typename TraitsT = Traits,
        std::enable_if_t<is_at_least_input_dir_v<dir> and TraitsT::is_input_stateless, bool> = true>
    static constexpr auto translate_to(
        config::types::Span<const uint8_t> data,
        Type &obj,
        std::size_t bitoffset = 0
    );

    /**
     * @brief Translated the target object into buffer of raw bytes with bit-aligned layout
     * 
     * @tparam TraitsT 
     *    (unused) type used to provide enabling condition
     * @param[out] data 
     *    source data buffer
     * @param obj 
     *    target object
     * @param bitoffset 
     *    bitoffset of data in @p data buffer
     */
    template<typename TraitsT = Traits,
        std::enable_if_t<is_at_least_output_dir_v<dir> and TraitsT::is_output_stateless, bool> = true>
    static constexpr auto translate_from(
        config::types::Span<uint8_t> data,
        ArgType obj,
        std::size_t bitoffset = 0
    );
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/translation/translator_wrapper/translator_wrapper.hpp"

/* ================================================================================================================================ */

#endif
