/* ============================================================================================================================ *//**
 * @file       translator_wrapper.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 7th June 2022 7:19:58 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the TranslatorWrapper traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_WRAPPER_TRANSLATOR_WRAPPER_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATOR_WRAPPER_TRANSLATOR_WRAPPER_H__

/* =========================================================== Includes =========================================================== */

// Boost includes
#include <boost/preprocessor.hpp>
// Private includes
#include "ethercat/common/translation/translator_wrapper.hpp"
#include "ethercat/common/translation/translation_error.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ================================================ TranslatorWrapperStatefullBase ================================================ */

namespace details {

template<typename TranslatorT, typename T>
template<typename... ArgsT>
constexpr TranslatorWrapperStatefullBase<TranslatorT, T>::TranslatorWrapperStatefullBase(ArgsT&&... args) :
    translator{ std::forward<ArgsT>(args)... }
{ }

}

/* ========================================================= Public ctors ========================================================= */

template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename... ArgsT, typename TraitsT,
    std::enable_if_t<TraitsT::template is_statefull_in_dir<dir>, bool>>
constexpr TranslatorWrapper<dir, TranslatorT, T>::TranslatorWrapper(ArgsT&&... args) :
    Base{ std::forward<ArgsT>(args)... }
{ }

/* ===================================================== Common implementation ==================================================== */

namespace details {

    template<bool flag = false>
    inline constexpr void return_type_error() {
        static_assert(flag, 
            "[ethercat::common::translation::TranslatorWrapper] Translator is required to "
            "either return value convertible to bool (if boolean return value support is "
            "enabled) or return void!"
        ); 
    }


    template<bool flag = false>
    inline constexpr void call_format_not_match() {
        static_assert(flag, "[ethercat::common::translation::TranslatorWrapper][BUG] Translator is identified as neither specialized or generic"); 
    }


    /**
     * @brief Auxiliary wrapper dispatching call to the target translation method based on the 
     *    libraries' configuration conserning supported return type of translators
     */
    template<typename WrapperT> 
    __attribute__((always_inline)) 
    constexpr auto dispatch_translation(WrapperT &&method) {

        // Check if non-void return type supported
        if constexpr(config::translation::SupportBooleanReturn) {
            
            // Check if return type is convertible to bool; if so, return result of the call
            if constexpr(std::is_convertible_v<decltype(method()), bool>) {
                return method();
            // Otherwise make sure that the function does no return anything
            } else if constexpr (std::is_same_v<decltype(method()), void>){
                method();
                return true;
            // Otherwise, report error
            } else {
                details::return_type_error();
            }

        // Otherwise make sure that the function does no return anything
        } else if constexpr (std::is_same_v<decltype(method()), void>){
            method();
            return true;
        // Otherwise, report error
        } else {
            details::return_type_error();
        }
    }

    /**
     * @brief Auxiliary wrapper calling given translation @p method based on the traits resolution
     *    for the given @p dir direction
     */
    template<typename TranslatorT, typename T,
        TranslationError::Direction potential_error_direction,
        bool handles_bitoffset,
        typename WrapperT,
        typename DataT,
        typename ObjectT
    > __attribute__((always_inline))
    constexpr auto wrap_translation_core(
        WrapperT &&method,
        DataT &&data,
        ObjectT &&obj,
        std::size_t bitoffset
    ) {
        // If translator handles bitoffset, simply wrap
        if constexpr(handles_bitoffset) {

            // Wrap method call
            return dispatch_translation([&](){ return method(data, obj, bitoffset); });

        // Otherwise, wrap with bitoffset parameter unused
        } else {

            // If configuration switch enabled, check if translation actually needs unaligned-bitoffset handling
            if constexpr(config::translation::RequireBitAlignmentHandling) {
                if(bitoffset != 0) {
                    throw TranslationError::make<
                        potential_error_direction, TranslatorT, T>(
                            "bit-aligned data not handled by the translator",
                            "ethercat::common::translation"
                        );
                }
            }

            // Wrap method call
            return dispatch_translation([&](){ return method(data, obj); });
        }                                                                            
    }

    /**
     * @brief Auxiliary wrapper calling given translation @p method based on the traits resolution
     *    for the given @p dir direction. Adds Exceptions-wrapping if related configuraion switch 
     *    is set and passes wrapped method call to further library-config-dependent dispatch
     */
    template<typename TranslatorT, typename T,
        TranslationError::Direction potential_error_direction,
        bool handles_bitoffset,
        typename WrapperT,
        typename DataT,
        typename ObjectT
    > __attribute__((always_inline)) 
    constexpr auto wrap_translation(
        WrapperT &&method,
        DataT &&data,
        ObjectT &&obj,
        std::size_t bitoffset
    ) {
        /**
         * @note According to C++17 try { } block cannot appear in constexpr function. This changed 
         *    in C++20 but as the library is compilled with C+17 GCC will warn about non-standard usage
         */

        // If configuration switch enabled, wrap translation core with try-catch block
        if constexpr (config::translation::WrapExceptions) {
            try { 
                return wrap_translation_core<TranslatorT, T, potential_error_direction, handles_bitoffset>(
                    std::forward<WrapperT>(method),
                    std::forward<DataT>(data),
                    std::forward<ObjectT>(obj),
                    bitoffset
                );
            }
            catch(std::exception &ex) {
                throw TranslationError::make<
                    potential_error_direction, TranslatorT, T>(
                        ex.what(), "ethercat::common::translation"
                    );
            }

        // Otherwise, directly wrap translation code
        } else {
            return wrap_translation_core<TranslatorT, T, potential_error_direction, handles_bitoffset>(
                std::forward<WrapperT>(method),
                std::forward<DataT>(data),
                std::forward<ObjectT>(obj),
                bitoffset
            );
        }
    }

}

/* ======================================================== Public methods ======================================================== */

template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<is_at_least_input_dir_v<dir> and TraitsT::is_input_statefull, bool>>
constexpr auto TranslatorWrapper<dir, TranslatorT, T>::translate_to(
    config::types::Span<const uint8_t> data,
    Type &obj,
    std::size_t bitoffset
) {
    return details::wrap_translation<TranslatorT, T, 
        TranslationError::Direction::Input,
        Traits::handles_input_bitoffset
    >(
        [&](auto&&... args) __attribute__((always_inline)) {
            return this->translator.translate_to(std::forward<decltype(args)>(args)...);
        }, data, obj, bitoffset
    );
}


template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<is_at_least_output_dir_v<dir> and TraitsT::is_output_statefull, bool>>
constexpr auto TranslatorWrapper<dir, TranslatorT, T>::translate_from(
    config::types::Span<uint8_t> data,
    ArgType obj,
    std::size_t bitoffset
) {
    return details::wrap_translation<TranslatorT, T, 
        TranslationError::Direction::Output,
        Traits::handles_output_bitoffset
    >(
        [&](auto&&... args) __attribute__((always_inline)) {
            return this->translator.translate_from(std::forward<decltype(args)>(args)...);
        }, data, obj, bitoffset
    );
}

/* ===================================================== Public static methods ==================================================== */

template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<is_at_least_input_dir_v<dir> and TraitsT::is_input_stateless, bool>>
constexpr auto TranslatorWrapper<dir, TranslatorT, T>::translate_to(
    config::types::Span<const uint8_t> data,
    Type &obj,
    std::size_t bitoffset
) {
    return details::wrap_translation<TranslatorT, T, 
        TranslationError::Direction::Input,
        Traits::handles_input_bitoffset
    >(
        [&](auto&&... args) __attribute__((always_inline)) {
            return TranslatorT::translate_to(std::forward<decltype(args)>(args)...);
        }, data, obj, bitoffset
    );
}


template<TranslationDirection dir, typename TranslatorT, typename T>
template<typename TraitsT,
    std::enable_if_t<is_at_least_output_dir_v<dir> and TraitsT::is_output_stateless, bool>>
constexpr auto TranslatorWrapper<dir, TranslatorT, T>::translate_from(
    config::types::Span<uint8_t> data,
    ArgType obj,
    std::size_t bitoffset
) {
    return details::wrap_translation<TranslatorT, T, 
        TranslationError::Direction::Output,
        Traits::handles_output_bitoffset
    >(
        [&](auto&&... args) __attribute__((always_inline)) {
            return TranslatorT::translate_from(std::forward<decltype(args)>(args)...);
        }, data, obj, bitoffset
    );
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
