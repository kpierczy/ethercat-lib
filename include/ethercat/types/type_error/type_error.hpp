/* ============================================================================================================================ *//**
 * @file       type_error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 2:40:48 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of TypeError exception type
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_TYPES_TYPE_ERROR_TYPE_ERROR_H__
#define __ETHERCAT_TYPES_TYPE_ERROR_TYPE_ERROR_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <sstream>
// Private includes
#include "ethercat/types/type_error.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ============================================================ Helpers =========================================================== */

namespace details {

    static inline std::string make_type_error_message(
        const Type &requested,
        std::string_view context,
        std::string_view msg
    ) {
        std::stringstream ss;

        // Construct context string
        if(context.size() != 0)
            ss << "[" << context << "] ";
        // Construct the message
        ss << "Invalid CoE object type: " << requested.get_name();
        
        // Construct additional message string
        if(msg.size() != 0)
            ss << " (" << msg << ")";

        return ss.str();
    }

    static inline std::string make_type_error_message(
        const Type &requested,
        const Type &actual,
        std::string_view context,
        std::string_view msg
    ) {
        std::stringstream ss;

        // Construct context string
        if(context.size() != 0)
            ss << "[" << context << "] ";
        // Construct the message
        ss << "CoE object types mismatch, requested: " << requested.get_name() << ", actual: " << actual.get_name();
        // Construct additional message string
        if(msg.size() != 0)
            ss << " (" << msg << ")";

        return ss.str();
    }

}

/* =========================================================== TypeError ========================================================== */

TypeError::TypeError(
    const Type &requested,
    std::string_view context,
    std::string_view msg
) :
    std::runtime_error{ details::make_type_error_message(requested, context, msg) },
    requested{ requested }
{ }


TypeError::TypeError(
    const Type &requested,
    const Type &actual,
    std::string_view context,
    std::string_view msg
) :
    std::runtime_error{ details::make_type_error_message(requested, actual, context, msg) },
    requested{ requested },
    actual{ actual }
{ }

Type TypeError::get_requested() const {
    return requested;
}


std::optional<Type> TypeError::get_actual() const {
    return actual;
}

/* ================================================================================================================================ */

} // End namespace ethercat::types

#endif