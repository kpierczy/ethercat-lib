/* ============================================================================================================================ *//**
 * @file       master.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Master class implementing parsing interface of the <Master> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_MASTER_MASTER_H__
#define __ETHERCAT_COMMON_MASTER_MASTER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::string Master::get_name() const {
    return get_child_value<std::string>("Info.Name");
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
