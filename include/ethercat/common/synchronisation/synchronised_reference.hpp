/* ============================================================================================================================ *//**
 * @file       synchronised_reference.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Friday, 22nd April 2022 1:09:36 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of the SynchronisedReference class template
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_SYNCHRONISATION_SYNCHRONISED_REFERENCE_H__
#define __ETHERCAT_COMMON_SYNCHRONISATION_SYNCHRONISED_REFERENCE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <mutex>
#include <atomic>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::synchronisation {

/* ===================================================== SynchronisedReference ==================================================== */

/**
 * @brief Auxiliary RAII class providing synchronised reference to an arbitrary object
 * @details Class wraps handle to an arbitrary object calling lock() and unlock() methods
 *    on it at it's construction and destruction respectively. Reference is safely movable
 *    but non-copyable
 * 
 * @tparam T 
 *    type of the referenced object
 */
template<typename T>
class SynchronisedReference {
    
public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Synchronised type
    using Type = T;

public: /* ------------------------------------------- Public ctors, dtors & operators -------------------------------------------- */

    /**
     * @brief Constructs reference to the given object. Locks the object
     */
    constexpr SynchronisedReference(T &obj);

    /**
     * @brief Destroys the reference unlocking the referenced object (if reference
     *    has not been moved)
     * @todo Make it constexpr while C++20 is available
     */
    ~SynchronisedReference();

    /// No copy-constructible
    SynchronisedReference(const SynchronisedReference &rref) = delete;
    /// No copy-asignable
    SynchronisedReference &operator=(const SynchronisedReference &rref) = delete;
    /// Move-constructible
    constexpr SynchronisedReference(SynchronisedReference &&rref);
    /// Move-asignable
    constexpr SynchronisedReference &operator=(SynchronisedReference &&rref);

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /// @returns reference to the object
    constexpr T &get();

    /// @returns const reference to the object
    constexpr const T &get() const;

private: /* ---------------------------------------------------- Private data ----------------------------------------------------- */

    /// Flag stating whether object still holds the reference ( @c false when object has been moved )
    bool moved { false };
    /// Reference to the synchronised obejct
    T *obj;

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::synchronisation

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/synchronisation/synchronised_reference/synchronised_reference.hpp"

/* ================================================================================================================================ */

#endif
