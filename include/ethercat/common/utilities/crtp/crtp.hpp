/* ============================================================================================================================ *//**
 * @file       crtp.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 2:57:32 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Implementation of inline methods of the CRTP template class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_CRTP_CRTP_H__
#define __ETHERCAT_COMMON_UTILITIES_CRTP_CRTP_H__

/* =========================================================== Includes =========================================================== */

#include "ethercat/common/utilities/crtp.hpp"

/* =========================================================== Namespace ========================================================== */

namespace ethercat::common::utilities {

/* ========================================================== Namespaces ========================================================== */

template<typename T>
constexpr T &CRTP<T>::impl() {
    return *static_cast<T*> (this);
}


template<typename T>
constexpr const T &CRTP<T>::impl() const { 
    return *static_cast<const T*>(this); 
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities

#endif
