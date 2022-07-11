/* ============================================================================================================================ *//**
 * @file       type_error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 2:39:47 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of TypeError exception type
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __TYPES_TYPE_ERROR_H__
#define __TYPES_TYPE_ERROR_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <stdexcept>
// Private includes
#include "ethercat/types/type.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* =========================================================== TypeError ========================================================== */

/**
 * @brief Module-specific exception type indicating mismatch of requested and actual
 *    object type
 */
class TypeError : public std::runtime_error {

public: /* -------------------------------------------------- Public constructors ------------------------------------------------- */

    /**
     * @brief Constructs TypeError object describing wrong usage of @p requested . 
     *    Prepends resulting message with the given @p context .
     * 
     * @param requested 
     *    requested object type
     * @param context 
     *    message context
     * @param msg
     *    additional message
     */
    inline TypeError(
        const Type &requested,
        std::string_view context = "",
        std::string_view msg = ""
    );

    /**
     * @brief Constructs TypeError object describing mismatch of @p requested and @p actual 
     *    type. Prepends resulting message with the given @p context .
     * 
     * @param requested 
     *    requested object type
     * @param actual 
     *    actual object type
     * @param context 
     *    message context
     * @param msg
     *    additional message
     */
    inline TypeError(
        const Type &requested,
        const Type &actual,
        std::string_view context = "",
        std::string_view msg = ""
    );
    
public: /* ----------------------------------------------------- Public getters --------------------------------------------------- */

    
    /**
     * @returns 
     *    requested type associated with the error
     */
    inline Type get_requested() const;
    
    /**
     * @returns 
     *    actual type associated with the error
     */
    inline std::optional<Type> get_actual() const;

private: /* ----------------------------------------------- Private member variables ---------------------------------------------- */

    /// Erronous requested type
    Type requested;
    /// Actual type
    std::optional<Type> actual;

};

/* ================================================================================================================================ */

} // End namespace ethercat::types

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/types/type_error/type_error.hpp"

/* ================================================================================================================================ */

#endif