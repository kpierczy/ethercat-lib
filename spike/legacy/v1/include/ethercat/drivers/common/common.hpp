/* ============================================================================================================================ *//**
 * @file       common.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 30th May 2022 6:09:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of common data types utilized by both Slave and Master drivers APIs
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DRIVERS_COMMON_COMMON_H__
#define __ETHERCAT_DRIVERS_COMMON_COMMON_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/drivers/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ========================================================== Data types ========================================================== */

void EventHandler::operator()() const {
    std::lock_guard guard{ lock };
    if(handler)
        handler();
}

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/drivers/common/common.hpp"

/* ================================================================================================================================ */

#endif
