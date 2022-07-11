/* ============================================================================================================================ *//**
 * @file       object.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 1:37:45 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the @ref Object class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_OBJECT_OBJECT_H__
#define __ETHERCAT_COMMON_OBJECT_OBJECT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects/value_buffer_reference.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ===================================================== Public ctors & dtors ===================================================== */

Object::Object(Type type, ranges::span<uint8_t> data) :
    serializer{ data },
    buffer{ Value::from_type(type) }
{ }


/* ===================================================== Public common methods ==================================================== */

Type Object::get_type() const {
    return buffer.get_type();
}

/* ================================================== Public data access methods ================================================== */

void Object::serialize() {

    // Get current value
    auto value = buffer.get();
    // Serialize the value
    serializer.write(value);

}


void Object::deserialize() {

    Value value{ buffer.get_type() };

    // Deserialize the value
    serializer.read(value);
    // Get current value
    buffer.set(value);
}


template<Type type>
void Object::write_as(ValueType<type> value) {

    // Store value in the buffer
    buffer.set_as<type>(value);
    // Serialize the value
    serializer.write_as(value);

}


void Object::write(Value value) {

    // Store value in the buffer
    buffer.set(value);
    // Serialize the value
    serializer.write(value);

}


template<Type type>
ValueType<type> Object::read_as() {

    // Deserialize the value
    ValueType<type> value = serializer.read_as<type>();
    // Store value in the buffer
    buffer.set_as<type>(value);

    return value;
}


Value Object::read() {

    Value value{ buffer.get_type() };

    // Deserialize the value
    serializer.read(value);
    // Get current value
    buffer.set(value);

    return value;
}

/* ================================================= Public vuffer access methods ================================================= */

template<Type type>
ValueType<type> Object::get_as() const {
    return buffer.get_as<type>();
}


const Value Object::get() const {
    return buffer.get();
}


template<Type type>
void Object::set_as(ValueType<type> value) {
    buffer.set_as<type>(type);
}


void Object::set(Value value) {
    buffer.set(value);
}


ValueBuffer &Object::get_reference() {
    return buffer;    
}


const ValueBuffer &Object::get_reference() const {
    return buffer;    
}


template<Type type>
ValueBufferReference<type> Object::get_reference() {
    return ValueBufferReference<type>{ buffer };
}


template<Type type>
const ValueBufferReference<type> Object::get_reference() const {

    /**
     * @note const_cast is required to implement reference-wrapper semantic. Logical constness
     *    is enforced by public API of the @ref ValueBufferReference
     */

    return ValueBufferReference<type>{ const_cast<ValueBuffer&>(buffer) };
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

#endif
