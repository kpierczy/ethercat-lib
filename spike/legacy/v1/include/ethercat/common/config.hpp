/* ============================================================================================================================ *//**
 * @file       config.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:49:54 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of some compile-time configurations of the library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_CONFIG_H__
#define __ETHERCAT_COMMON_CONFIG_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/locks.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::config {

/* ========================================================= Configuration ======================================================== */

/**
 * @brief Standard synchronisation primitive used by most of synchronsiation-requiring
 *    routines in the library
 */
using Lock = std::mutex;

/**
 * @brief Light-weight synchronisation primitive used by the library in short-lock-time,
 *    low-collision-chance locking scenarios
 */
using QuickLock = locks::SpinLock;

/* ================================================================================================================================ */

} // End namespace ethercat::config

#endif
