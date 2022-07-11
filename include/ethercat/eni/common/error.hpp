/* ============================================================================================================================ *//**
 * @file       error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:12:57 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Error exception class related to ENI-specific errors
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ERROR_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ERROR_H__

/* =========================================================== Includes =========================================================== */

// System includes
#include <stdexcept>
#include <string>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================= Class ============================================================ */

/**
 * @brief Helper exception class providing description of the runtime error related to the
 *    parsed ENI file 
 */
class Error : public std::runtime_error {

public: /* -------------------------------------------------- Public constructors ------------------------------------------------- */

    /**
     * @brief Constructs a new Error with the given message
     * 
     * @param msg 
     *    message associated with error
     */
    inline Error(const std::string &msg);

};

/* ======================================================== Free functions ======================================================== */

/**
 * @brief Helper wrapper around an arbitrary @p action that handles possible exceptions
 *    by wrapping them into the module-specifc exception class
 * 
 * @tparam ActionT 
 *    type of the aciton callable
 * @tparam ContextStrT 
 *    type of the context string
 * @param action 
 *    action to be wrapped
 * @param context
 *    context string 
 * @returns 
 *    whatever @p action returns;
 * 
 * @throws eni::Error 
 *    if any exception has been thrown by the @p action
 */
template <typename ActionT, typename ContextStrT>
inline auto wrap_error(ActionT &&action, ContextStrT context = "");

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/common/error/error.hpp"

/* ================================================================================================================================ */

#endif
