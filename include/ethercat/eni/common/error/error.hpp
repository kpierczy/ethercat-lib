/* ============================================================================================================================ *//**
 * @file       error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of inline methods the Element class constituting base for parsing interface classes of the ENI 
*              (EtherCAT Network Informations) files
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ERROR_ERROR_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ERROR_ERROR_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/error.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ===================================================== Public ctors & dtors ===================================================== */

Error::Error(const std::string &msg) :
    std::runtime_error{ msg }
{ }

/* ======================================================== Free functions ======================================================== */

template <typename ActionT, typename ContextStrT>
auto wrap_error(ActionT &&action, ContextStrT context) {
    
    // Try to call the action
    try { return action(); }
    // On error, rethrow
    catch (std::exception &ex) {

        // Convert context into string
        std::string_view context_s = context;
        // Rethrow wrapped error
        if (context_s.size() == 0)
            throw Error{ std::string{ ex.what() } };
        else {
            std::stringstream ss;
            ss << "[" << context_s.data() << "] " << ex.what();
            throw Error{ ss.str() };
        }
            
    }
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
