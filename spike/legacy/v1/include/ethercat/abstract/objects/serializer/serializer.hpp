/* ============================================================================================================================ *//**
 * @file       serializer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 12:34:38 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of inline methods of the @ref ethercat::abstract::objects::Serializer class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_SERIALIZER_SERIALIZER_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_SERIALIZER_SERIALIZER_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <stdexcept>
#include <cstring>
// Boost includes
#include <boost/mp11.hpp>
// Private includes
#include "ethercat/abstract/objects/serializer.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ===================================================== Public ctors & dtors ===================================================== */

constexpr Serializer::Serializer(ranges::span<uint8_t> data) :
    data{ data }
{ }

/* ======================================================== Public methods ======================================================== */

void Serializer::read(Value &value) const {

    using namespace std::literals::string_literals;
    
    // Read containe dvalue
    return boost::mp11::mp_with_index<std::variant_size_v<Value::Storage>>( value.get().index(), [&](auto I){

            // Get reference to data
            auto &v = value.as<IndexType<I>>();

            // Check if referenced buffer can contain requested value
            if(sizeof(v) > static_cast<std::size_t>(data.size())) {

                // Get object type name
                constexpr auto obj_name = traits::TypeTraits<IndexType<I>>::coe_name;
                
                // Throw exception
                throw std::range_error { 
                    "[ethercat::abstract::objects::Serializer::read] Too small buffer to contain alue of type "s + obj_name
                };
            }

            // Copy raw bytes
            std::memcpy(
                static_cast<void*>(&v),
                static_cast<void*>(data.data()),
                sizeof(v)
            );

    });
}


template<Type type>
ValueType<type> Serializer::read_as() const {

    using namespace std::literals::string_literals;
    
    // Check if referenced buffer can contain requested value
    if(sizeof(ValueType<type>) > data.size()) {

        // Get object type name
        constexpr auto obj_name = traits::TypeTraits<type>::coe_name;
        
        // Throw exception
        throw std::range_error { 
            "[ethercat::abstract::objects::Serializer::read] Too small buffer to contain alue of type "s + obj_name
        };
    }

    ValueType<type> ret;

    // Copy raw bytes
    std::memcpy(
        static_cast<void*>(&ret),
        static_cast<void*>(data.data()),
        sizeof(ValueType<type>)
    );

    return ret;
}


void Serializer::write(Value value) {

    using namespace std::literals::string_literals;
    
    // Read containe dvalue
    return boost::mp11::mp_with_index<std::variant_size_v<Value::Storage>>( value.get().index(), [&](auto I){

            // Get reference to data
            auto &v = value.as<IndexType<I>>();

            // Check if referenced buffer can contain requested value
            if(sizeof(v) > static_cast<std::size_t>(data.size())) {

                // Get object type name
                constexpr auto obj_name = traits::TypeTraits<IndexType<I>>::coe_name;
                
                // Throw exception
                throw std::range_error { 
                    "[ethercat::abstract::objects::Serializer::write] Too small buffer to contain alue of type "s + obj_name
                };
            }

            // Copy raw bytes
            std::memcpy(
                static_cast<void*>(data.data()),
                static_cast<void*>(&v),
                sizeof(v)
            );

    });
}


template<Type type>
void Serializer::write_as(ValueType<type> value) {

    using namespace std::literals::string_literals;
    
    // Check if referenced buffer can contain requested value
    if(sizeof(value) > data.size()) {

        // Get object type name
        constexpr auto obj_name = traits::TypeTraits<type>::coe_name;
        
        // Throw exception
        throw std::range_error { 
            "[ethercat::abstract::objects::Serializer::write] Too small buffer to contain alue of type "s + obj_name
        };
    }

    // Copy raw bytes
    std::memcpy(
        static_cast<void*>(data.data()),
        static_cast<void*>(&value),
        sizeof(value)
    );

}

/* ================================================================================================================================ */

} // End namespace

#endif
