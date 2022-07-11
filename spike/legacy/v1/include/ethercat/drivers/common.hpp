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

#ifndef __ETHERCAT_DRIVERS_COMMON_H__
#define __ETHERCAT_DRIVERS_COMMON_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <functional>
// Private includes
#include "ethercat/abstract/pdo.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ========================================================== Data types ========================================================== */

/**
 * @brief Auxiliary pair-like structure storing event handler functor along with it's
 *    synchronising long
 */
struct EventHandler {

    /**
     * @brief Synchronising lock
     * @performance Think about non-blocking synchronisation
     */
    mutable ::ethercat::config::Lock lock;
    
    /**
     * @brief Handler
     * @performance Handlers utilize std::function<> to implement callbacks used in the critical
     *    path of the bus I/O loop. This utilizes dynamic memory and dynamic dispatch that can
     *    corrupt performance (std::function<> is not inlined very well on the GCC). Think about
     *    moving to other implementation of function<> template
     */
    std::function<void(void)> handler;

    /**
     * @brief Calls the handler if it is registered
     * @synchronised
     */
    inline void operator()() const;

};

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/drivers/common/common.hpp"

/* ================================================================================================================================ */

#endif
