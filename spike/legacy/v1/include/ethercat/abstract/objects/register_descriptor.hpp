/* ============================================================================================================================ *//**
 * @file       register_descriptor.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 30th May 2022 6:09:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of auxiliary RegisterDescriptor class providing compile-time description of the CoE register
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_REGISTER_DESCRIPTOR_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_REGISTER_DESCRIPTOR_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <cstdint>
#include <string_view>
// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ============================================================= Class ============================================================ */

/**
 * @brief Auxiliary structure providing way to define compile-time constants describing CoE 
 *    registers
 * 
 * @tparam data_type 
 *    type of data held by the register
 * @tparam index_value 
 *    CoE index of the object
 */
struct RegisterDescriptor {

    /**
     * @brief Constructs the register descriptor
     * 
     * @param name 
     *    name of the register
     * @param type 
     *    type of the register
     * @param index 
     *    index of the register
     */
    constexpr RegisterDescriptor(
        std::string_view name,
        abstract::objects::Type type,
        uint16_t index
    ) :
        name { name },
        type { type },
        index { index }
    { }

    /// Name of the register
    std::string_view name;
    /// Type of the register
    abstract::objects::Type type;
    /// Index of the register
    uint16_t index;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

#endif
