/* ============================================================================================================================ *//**
 * @file       value_buffer_reference.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 1:31:02 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the @ref ValueBufferRefference class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_VALUE_BUFFER_REFERENCE_VALUE_BUFFER_REFERENCE_H__
#define __ETHERCAT_COMMON_VALUE_BUFFER_REFERENCE_VALUE_BUFFER_REFERENCE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects/value_buffer_reference.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ===================================================== Public ctors & dtors ===================================================== */

template<Type type>
ValueBufferReference<type>::ValueBufferReference(ValueBuffer &buffer) :
    buffer{ buffer }
{ }

/* ======================================================= Public operators ======================================================= */

template<Type type>
const ValueType<type> ValueBufferReference<type>::get() const {
    return buffer.get_as<type>();
}


template<Type type>
void ValueBufferReference<type>::set(ValueType<type> value) {
    buffer.set_as<type>(value);
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

#endif
