/* ============================================================================================================================ *//**
 * @file       objects.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:02:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of routines associated with the representation of CoE (CANopen over EtherCAT) 'object' entities from the 
 *             Dictionary Object (DO) of the EtherCAT slave device.
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_OBJECTS_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_OBJECTS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <sstream>
// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ========================================================== Exceptions ========================================================== */

namespace details {

    static inline std::string make_type_error_message(
        Type requested,
        std::string_view context,
        std::string_view msg
    ) {
        std::stringstream ss;

        // Construct context string
        if(context.size() != 0)
            ss << "[" << context << "] ";
        // Construct the message
        ss << "Invalid CoE object type: " << traits::name(requested);
        // Construct additional message string
        if(msg.size() != 0)
            ss << " (" << msg << ")";

        return ss.str();
    }

    static inline std::string make_type_error_message(
        Type requested,
        Type actual,
        std::string_view context,
        std::string_view msg
    ) {
        std::stringstream ss;

        // Construct context string
        if(context.size() != 0)
            ss << "[" << context << "] ";
        // Construct the message
        ss << "CoE object types mismatch, requested: " << traits::name(requested) << ", actual: " << traits::name(actual);
        // Construct additional message string
        if(msg.size() != 0)
            ss << " (" << msg << ")";

        return ss.str();
    }

}

TypeError::TypeError(
    Type requested,
    std::string_view context,
    std::string_view msg
) :
    std::runtime_error{ details::make_type_error_message(requested, context, msg) },
    requested{ requested }
{ }


TypeError::TypeError(
    Type requested,
    Type actual,
    std::string_view context,
    std::string_view msg
) :
    std::runtime_error{ details::make_type_error_message(requested, actual, context, msg) },
    requested{ requested },
    actual{ actual }
{ }

Type TypeError::get_requested() const {
    return requested;
}


std::optional<Type> TypeError::get_actual() const {
    return actual;
}

/* ======================================================= Getters & setters ====================================================== */

constexpr std::size_t type_index(Type type) {
    return utilities::to_underlying(type);
}


constexpr Type index_type(std::size_t type) {
    return utilities::to_enum<Type>(type);
}


constexpr Type get_type(const ValueStorage &value_storage) {
    return utilities::to_enum<Type>(value_storage.index());
}


template<Type type>
constexpr ValueType<type> &get(ValueStorage &value_storage) {
    if(auto value_p = std::get_if<TypeIndex<type>>(&value_storage); value_p != nullptr)
        return *value_p;
    else
        throw TypeError{ type, get_type(value_storage), "ethercat::abstract::objects::get" };
}


template<Type type>
constexpr const ValueType<type> &get(const ValueStorage &value_storage) {
    if(auto value_p = std::get_if<TypeIndex<type>>(&value_storage); value_p != nullptr)
        return *value_p;
    else
        throw TypeError{ type, get_type(value_storage), "ethercat::abstract::objects::get" };
}


constexpr ValueStorage make_value_storage(Type type) {
    return utilities::make_variant<ValueStorage>(utilities::to_underlying(type));
}

/* ============================================================ Helpers =========================================================== */

Type parse_type(std::string_view type_string) {
    
    // Parse data type
    if(type_string == "BOOL"  ) return Type::Bool;
    if(type_string == "BYTE"  ) return Type::Byte;
    if(type_string == "WORD"  ) return Type::Word;
    if(type_string == "DWORD" ) return Type::DoubleWord;
    if(type_string == "SINT"  ) return Type::ShortInt;
    if(type_string == "USINT" ) return Type::UnsignedShortInt;
    if(type_string == "INT"   ) return Type::Int;
    if(type_string == "UINT"  ) return Type::UnsignedInt;
    if(type_string == "DINT"  ) return Type::DoubleInt;
    if(type_string == "UDINT" ) return Type::UnsignedDoubleInt;
    if(type_string == "LINT"  ) return Type::LongInt;
    if(type_string == "ULINT" ) return Type::UnsignedLongInt;
    if(type_string == "REAL"  ) return Type::Real;
    if(type_string == "LREAL" ) return Type::LongReal;

    // If non-supported data type parse, throw error
    throw std::runtime_error{ "Not supported data type (" + std::string{ type_string } + ")" };
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

#endif
