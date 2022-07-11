/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:02:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Type type describing type of the CoE object at runtime
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <sstream>
// Private includes
#include "ethercat/types/type.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {
    
/* ============================================================= Type ============================================================= */

std::string Type::get_name() const {

    // If base type
    if(is_builtin()) {

        auto &type = get_builtin();

        // If custom type given
        if(type.name.has_value())
            return std::string{ *(type.name) };
        
        // Else, deduce name from type ID
        auto name = common::types::traits::name(type.get_id());

        // If zero arity is given, return
        if(type.arity == 0)
            return std::string{ name };

        std::stringstream ss;

        // Else, append array specifier
        ss << "ARRAY [0.." << type.arity << "] of " << name;

        return ss.str();

    // If structural type, return it's name
    } else {
        return std::string{ get_structural().name };
    }
}

/* ================================================================================================================================ */

} // End namespace ethercat::types
