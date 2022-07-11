/* ============================================================================================================================ *//**
 * @file       synchronised_reference.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 7th June 2022 6:49:43 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_SYNCHRONISATION_SYNCHRONISED_REFERENCE_SYNCHRONISED_REFERENCE_H__
#define __ETHERCAT_COMMON_SYNCHRONISATION_SYNCHRONISED_REFERENCE_SYNCHRONISED_REFERENCE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <assert.h>
// Private includes
#include "ethercat/common/synchronisation/synchronised_reference.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::synchronisation {

/* ================================================ Public ctors, dtors & operators =============================================== */

template<typename T>
constexpr SynchronisedReference<T>::SynchronisedReference(T &obj) :
    obj{ &obj }
{
    this->obj->lock();
}


template<typename T>
SynchronisedReference<T>::~SynchronisedReference() {
    if(not moved)
        obj->unlock();
}


template<typename T>
constexpr SynchronisedReference<T>::SynchronisedReference(SynchronisedReference &&rref) :
    obj{ rref.obj }
{
    rref.moved = true;
    rref.obj   = nullptr;
}


template<typename T>
constexpr SynchronisedReference<T> &SynchronisedReference<T>::operator=(SynchronisedReference &&rref) {
    obj        = rref.obj;
    rref.moved = true;
    rref.obj   = nullptr;
}

/* ======================================================== Public methods ======================================================== */

template<typename T>
constexpr T &SynchronisedReference<T>::get() {
    assert(obj != nullptr);
    return *obj;
}


template<typename T>
constexpr const T &SynchronisedReference<T>::get() const {
    return const_cast<SynchronisedReference*>(this)->get();
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::synchronisation

/* ================================================================================================================================ */

#endif
