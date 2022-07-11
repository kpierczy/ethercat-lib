/* ============================================================================================================================ *//**
 * @file       value.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:14:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the methods of the value class prividing type-safe access interface to the @ref ValueStorage tuple type
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_VALUE_VALUE_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_VALUE_VALUE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ===================================================== Public static methods ==================================================== */

Value Value::from_type(Type type) {
    return make_value_storage(type);
}

/* ===================================================== Public ctors & dtors ===================================================== */

constexpr Value::Value(const ValueStorage &value_storage) :
    value{ value_storage }
{ }


constexpr Value::Value(Type type) :
    value{ make_value_storage(type) }
{ }


template<Type type, typename T>
constexpr Value::Value(InPlaceTypeTag<type> tag, T value) :
    value{ InPlaceValueIndex<type>, value }
{ }

/* ======================================================= Public operators ======================================================= */

template<typename T>
inline constexpr Value &Value::operator=(T value) {
    this->value = value;
    return *this;
}

/* ===================================================== Public common methods ==================================================== */

constexpr Type Value::get_type() const {
    return abstract::objects::get_type(value);
}

/* ===================================================== Public access methods ==================================================== */

template<Type type>
constexpr ValueType<type> &Value::as() {
    return abstract::objects::get<type>(this->value);
}


template<Type type>
constexpr const ValueType<type> &Value::as() const {
    return abstract::objects::get<type>(this->value);
}


constexpr const ValueStorage &Value::get() {
    return value;
}


constexpr const ValueStorage &Value::get() const {
    return value;
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

#endif
