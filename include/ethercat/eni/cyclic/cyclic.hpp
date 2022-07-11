/* ============================================================================================================================ *//**
 * @file       cyclic.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Cyclic class implementing parsing interface of the <Cyclic> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_CYCLIC_CYCLIC_H__
#define __ETHERCAT_COMMON_CYCLIC_CYCLIC_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/cyclic.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::chrono::microseconds Cyclic::get_cycle_time() const {
    return std::chrono::microseconds{ get_child_value<int64_t>("CycleTime") };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
