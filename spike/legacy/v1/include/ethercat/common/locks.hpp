/* ============================================================================================================================ *//**
 * @file       locks.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Friday, 22nd April 2022 1:09:36 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of helper synchronisation primitives
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_LOCKS_H__
#define __ETHERCAT_COMMON_LOCKS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <mutex>
#include <atomic>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::locks {

/* ========================================================== Empty lock ========================================================== */

/**
 * @brief Auxiliary class implementing stub for locking object
 */
struct EmptyLock {

    /// Stub for lock() method
    inline void lock() { }

    /// Stub for unlock() method
    inline void unlock() { }

};

/* =========================================================== Spinlock =========================================================== */

/**
 * @brief Auxiliary class implementing atomic-based spinlock
 */
class SpinLock {
    
public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// State of the lock
    enum class State : bool { 
        Locked,
        Unlocked
    };

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @brief Constructs the spinlock
     */
    inline SpinLock();

    /**
     * @brief Move-constructs the spinlock
     * @warning Usually synchronisation primitives should not be moved as it implies need
     *    of some communication protocol between synchronised threads to ensure that all
     *    of them refer to the same primitve. The move-constructor for the SpinLock class 
     *    has been introduced @b only to allow storing such locks in dynamically-allocated
     *    containers that can be reallocated during it's lifetime. SpinLock should not be
     *    moved it any other context
     */
    inline SpinLock(SpinLock &&rlock);

    /**
     * @brief Locks the spinlock
     */
    inline void lock();
    
    /**
     * @brief Unlocks the spinlock
     */
    inline void unlock();

private: /* ---------------------------------------------------- Private data ----------------------------------------------------- */

    /// State of the lock
    std::atomic<State> state;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::locks

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/locks/locks.hpp"

/* ================================================================================================================================ */

#endif
