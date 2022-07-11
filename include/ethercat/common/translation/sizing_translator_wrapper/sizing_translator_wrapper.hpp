/* ============================================================================================================================ *//**
 * @file       sizing_translator_wrapper.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 6:19:16 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the SizingTranslatorWrapper traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_WRAPPER_SIZING_TRANSLATOR_WRAPPER_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_WRAPPER_SIZING_TRANSLATOR_WRAPPER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/sizing_translator_wrapper.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ================================================ TranslatorWrapperStatefullBase ================================================ */

namespace details {

template<translation::TranslationDirection dir, typename TranslatorT, typename T>
template<typename... ArgsT>
constexpr SizingTranslatorWrapperStatefullBase<dir, TranslatorT, T>::SizingTranslatorWrapperStatefullBase(ArgsT&&... args) :
    translator{ std::forward<ArgsT>(args)... }
{ }

}

/* ========================================================= Public ctors ========================================================= */

template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename... ArgsT, typename TraitsT,
    std::enable_if_t<TraitsT::sizing::template is_statefull_in_dir<dir>, bool>>
constexpr SizingTranslatorWrapper<dir, TranslatorT, T>::SizingTranslatorWrapper(ArgsT&&... args) :
    Base{ std::forward<ArgsT>(args)... }
{ }

/* ===================================================== Common implementation ==================================================== */

namespace details {

    template<bool flag = false>
    inline constexpr void invalid_sizing_translator_traits() {
        static_assert(flag, 
            "[ethercat::common::translation::SizingTranslatorWrapper][BUG] make_buffer() method has been "
            "instantiated although underlying translator defines neither specified nor generic version of "
            "the method"
        ); 
    }

}

/* ======================================================== Public methods ======================================================== */

template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<TraitsT::static_sizing::is_statefull, bool>>
constexpr auto SizingTranslatorWrapper<dir, TranslatorT, T>::make_buffer(){
    if constexpr (TraitsT::static_sizing::is_specialized)
        return this->translator.make_buffer();
    else if constexpr (TraitsT::static_sizing::is_generic)
        return this->translator.template make_buffer<T>();
    else
        details::invalid_sizing_translator_traits();
}


template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<TraitsT::dynamic_sizing::is_statefull, bool>>
constexpr auto SizingTranslatorWrapper<dir, TranslatorT, T>::make_buffer(typename Base::ArgType obj){
    if constexpr (TraitsT::static_sizing::is_specialized)
        return this->translator.make_buffer(obj);
    else if constexpr (TraitsT::static_sizing::is_generic)
        return this->translator.template make_buffer<T>(obj);
    else
        details::invalid_sizing_translator_traits();
}

/* ===================================================== Public static methods ==================================================== */

template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<TraitsT::static_sizing::is_stateless, bool>>
constexpr auto SizingTranslatorWrapper<dir, TranslatorT, T>::make_buffer(){
    if constexpr (TraitsT::static_sizing::is_specialized)
        return TranslatorT::make_buffer();
    else if constexpr (TraitsT::static_sizing::is_generic)
        return TranslatorT::template make_buffer<T>();
    else
        details::invalid_sizing_translator_traits();
}


template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<TraitsT::dynamic_sizing::is_stateless, bool>>
constexpr auto SizingTranslatorWrapper<dir, TranslatorT, T>::make_buffer(typename Base::ArgType obj){
    if constexpr (TraitsT::static_sizing::is_specialized)
        return TranslatorT::make_buffer(obj);
    else if constexpr (TraitsT::static_sizing::is_generic)
        return TranslatorT::template make_buffer<T>(obj);
    else
        details::invalid_sizing_translator_traits();
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
