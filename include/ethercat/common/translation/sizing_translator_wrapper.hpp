/* ============================================================================================================================ *//**
 * @file       sizing_translator_wrapper.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 6:02:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the TranslatorWrapper traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_WRAPPER_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_WRAPPER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/sizing_translator_traits.hpp"
#include "ethercat/common/translation/translator_wrapper.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ============================================= SizingTranslatorWrapperStatefullBase ============================================= */

namespace details {

/**
 * @brief Base of the @ref SizingTranslatorWrapper for translators with stateless translating methods
 *   but statefull sizing methods
 */
template<translation::TranslationDirection dir, typename TranslatorT, typename T>
class SizingTranslatorWrapperStatefullBase :
    public TranslatorWrapper<dir, TranslatorT, T>
{
public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Construct a new SizingTranslatorWrapperStatefullBase passing given arguments
     *    to the wrapped translator's constructor
     * 
     * @tparam ArgsT 
     *    types of @p args
     * @param args 
     *    arguments to be passed to the wrapped translator's constructor
     */
    template<typename... ArgsT>
    constexpr SizingTranslatorWrapperStatefullBase(ArgsT&&... args);

    // Enable default constructor (if available)
    SizingTranslatorWrapperStatefullBase() = default;

    /// Default copy-construction
    SizingTranslatorWrapperStatefullBase(const SizingTranslatorWrapperStatefullBase &rwrapper) = default;
    /// Default copy-asignment
    SizingTranslatorWrapperStatefullBase &operator=(const SizingTranslatorWrapperStatefullBase &rwrapper) = default;
    /// Default move-construction
    SizingTranslatorWrapperStatefullBase(SizingTranslatorWrapperStatefullBase &&rwrapper) = default;
    /// Default move-asignment
    SizingTranslatorWrapperStatefullBase &operator=(SizingTranslatorWrapperStatefullBase &&rwrapper) = default;

protected: /* ------------------------------------------------- protected members ------------------------------------------------- */

    /// Wrapped statefull translator
    TranslatorT translator;

};

}

/* ==================================================== SizingTranslatorWrapper =================================================== */

/**
 * @brief Class template wrapping an arbitrary translator type with a unified translating interface
 * @details This class wraps @ref TranslatorWrapper interface providing unified interface for sizing
 *    methods of the translator
 * 
 * @tparam dir 
 *    translation direction
 * @tparam TranslatorT 
 *    type of the wrapped translator
 * @tparam T (optional, default: void)
 *    target type; if void wrapped tries to deduce it automatically
 */
template<TranslationDirection dir, typename TranslatorT, typename T = void>
class SizingTranslatorWrapper : 

    /// Statefull base
    public std::conditional_t<
        (
            TranslatorTraits<TranslatorT, T>::template is_stateless_in_dir<dir> and 
            SizingTranslatorTraits<TranslatorT, T>::sizing::template is_statefull_in_dir<dir>
        ),
            details::SizingTranslatorWrapperStatefullBase<dir, TranslatorT, T>,
            TranslatorWrapper<dir, TranslatorT, T>>
{

private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Base type
    using Base = std::conditional_t<
        (
            TranslatorTraits<TranslatorT, T>::template is_stateless_in_dir<dir> and 
            SizingTranslatorTraits<TranslatorT, T>::sizing::template is_statefull_in_dir<dir>
        ),
            details::SizingTranslatorWrapperStatefullBase<dir, TranslatorT, T>,
            TranslatorWrapper<dir, TranslatorT, T>>;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Traits of the translator type
    using SizingTraits = SizingTranslatorTraits<TranslatorT, T>;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Construct a new SizingTranslatorWrapper passing given arguments
     *    to the wrapped translator's constructor
     * 
     * @tparam ArgsT 
     *    types of @p args
     * @param args 
     *    arguments to be passed to the wrapped translator's constructor
     * 
     * @note Constructor is enabled only if wrapped translator is statefull
     */
    template<typename... ArgsT, typename TraitsT = SizingTraits,
        std::enable_if_t<TraitsT::sizing::template is_statefull_in_dir<dir>, bool> = true>
    constexpr SizingTranslatorWrapper(ArgsT&&... args);

    // Enable default constructor (if available)
    SizingTranslatorWrapper() = default;

    /// Default copy-construction
    SizingTranslatorWrapper(const SizingTranslatorWrapper &rwrapper) = default;
    /// Default copy-asignment
    SizingTranslatorWrapper &operator=(const SizingTranslatorWrapper &rwrapper) = default;
    /// Default move-construction
    SizingTranslatorWrapper(SizingTranslatorWrapper &&rwrapper) = default;
    /// Default move-asignment
    SizingTranslatorWrapper &operator=(SizingTranslatorWrapper &&rwrapper) = default;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @brief Creates a statically-sized buffer for storing binary-image of the translator's target type
     */
    template<typename TraitsT = SizingTraits,
        std::enable_if_t<TraitsT::static_sizing::is_statefull, bool> = true>
    constexpr auto make_buffer();

    /**
     * @brief Creates a dynamically-sized buffer for storing binary-image of the translator's target type
     * 
     * @param obj
     *    object to deduce size of the required buffer from
     */
    template<typename TraitsT = SizingTraits,
        std::enable_if_t<TraitsT::dynamic_sizing::is_statefull, bool> = true>
    constexpr auto make_buffer(typename Base::ArgType obj);

public: /* ------------------------------------------------- Public static methods ------------------------------------------------ */

    /**
     * @brief Creates a statically-sized buffer for storing binary-image of the translator's target type
     */
    template<typename TraitsT = SizingTraits,
        std::enable_if_t<TraitsT::static_sizing::is_stateless, bool> = true>
    static constexpr auto make_buffer();

    /**
     * @brief Creates a dynamically-sized buffer for storing binary-image of the translator's target type
     * 
     * @param obj
     *    object to deduce size of the required buffer from
     */
    template<typename TraitsT = SizingTraits,
        std::enable_if_t<TraitsT::dynamic_sizing::is_stateless, bool> = true>
    static constexpr auto make_buffer(typename Base::ArgType obj);
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/translation/sizing_translator_wrapper/sizing_translator_wrapper.hpp"

/* ================================================================================================================================ */

#endif
