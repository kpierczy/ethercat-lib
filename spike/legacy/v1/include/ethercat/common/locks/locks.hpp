/* ============================================================================================================================ *//**
 * @file       locks.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 1:24:47 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Implementations of helper synchronisation primitives
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_LOCKS_LOCKS_H__
#define __ETHERCAT_COMMON_LOCKS_LOCKS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/locks.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::locks {

/* ===================================================== SpinLock definitions ===================================================== */

SpinLock::SpinLock() : 
    state(State::Unlocked) 
{ }


SpinLock::SpinLock([[maybe_unused]] SpinLock &&rlock) : 
    state(State::Unlocked) 
{ }


void SpinLock::lock() { 
    while (state.exchange(State::Locked, std::memory_order_acquire) == State::Locked) { }
}


void SpinLock::unlock() { 
    state.store(State::Unlocked, std::memory_order_release); 
}

/* ================================================================================================================================ */

} // End namespace ethercat::locks

#endif
