/* ============================================================================================================================ *//**
 * @file       translation_error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the TranslationError exception traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATION_ERROR_TRANSLATION_ERROR_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATION_ERROR_TRANSLATION_ERROR_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <sstream>
// Private includes
#include "ethercat/common/utilities/type_name.hpp"
#include "ethercat/common/translation/translation_error.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ======================================================= TranslationError ======================================================= */

template<
    typename TranslatorT,
    typename T
> TranslationError TranslationError::make(std::string_view what, std::string_view context)
{
    std::stringstream msg;

    /// (optional) Add context string to the message
    if(not context.empty())
        msg << "[" << context << "] ";

    /// Add message core
    msg << "Error occurred when translating between "
        << "object of type " << utilities::type_name<T>() << " "
        << "and binary buffer ";
    /// Add translator info
    msg << "with translator of type " << utilities::type_name<TranslatorT>();
    
    /// (optional) Add what string
    if(not what.empty())
        msg << " (" << what << ")";

    return TranslationError{ msg.str() };
}


template<
    TranslationError::Direction dir,
    typename TranslatorT,
    typename T
> TranslationError TranslationError::make(std::string_view what, std::string_view context)
{
    std::stringstream msg;

    /// (optional) Add context string to the message
    if(not context.empty())
        msg << "[" << context << "] ";

    /// Add message header
    msg << "Error occurred when translating ";
    /// Add source
    if(dir == TranslationError::Direction::Input)
        msg << "binary buffer ";
    else
        msg << "object of type " << utilities::type_name<T>() << " ";
    /// Add joint
    msg << "into ";
    /// Add destination
    if(dir == TranslationError::Direction::Input)
        msg << "object of type " << utilities::type_name<T>() << " ";
    else
        msg << "binary buffer ";
    /// Add translator info
    msg << "with translator of type " << utilities::type_name<TranslatorT>();
    
    /// (optional) Add what string
    if(not what.empty())
        msg << " (" << what << ")";

    return TranslationError{ msg.str() };
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
