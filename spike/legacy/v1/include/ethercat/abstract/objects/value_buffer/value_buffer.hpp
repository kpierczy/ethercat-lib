/* ============================================================================================================================ *//**
 * @file       value_buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 10:48:13 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the @ref ValueBuffer class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_VALUE_BUFFER_VALUE_BUFFER_H__
#define __ETHERCAT_COMMON_VALUE_BUFFER_VALUE_BUFFER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects/value_buffer.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ===================================================== Public ctors & dtors ===================================================== */

ValueBuffer::ValueBuffer(Value value) :
    value{ value }
{  }

template<Type type, typename T>
ValueBuffer::ValueBuffer(InPlaceTypeTag<type> tag, T value) :
    value{ tag, value }
{ }


/* ===================================================== Public common methods ==================================================== */

constexpr Type ValueBuffer::get_type() const {
    return value.get_type();
}

/* =================================================== Public getters & setters =================================================== */

template<Type type>
ValueType<type> ValueBuffer::get_as() const {
    std::lock_guard<LockType> guard{ lock };
    return value.as<type>();
}


const Value ValueBuffer::get() const {
    std::lock_guard<LockType> guard{ lock };
    return value;
}


template<Type type>
void ValueBuffer::set_as(ValueType<type> value) {
    std::lock_guard<LockType> guard{ lock };
    this->value.as<type> = value;
}


void ValueBuffer::set(Value value) {
    std::lock_guard<LockType> guard{ lock };
    this->value = value;
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

#endif
