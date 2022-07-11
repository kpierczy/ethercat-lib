/* ============================================================================================================================ *//**
 * @file       type_error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 2:40:48 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of TypeError exception type
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef ___ETHERCAT_TYPES_TYPE_TYPE_H__
#define ___ETHERCAT_TYPES_TYPE_TYPE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/types/type.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ===================================================== Public static methods ==================================================== */

Type Type::parse(std::string_view type_string, std::size_t bitsize) {
    auto builtin = BuiltinType::parse(type_string);
    return builtin.has_value() ? 
        Type{ *builtin } : 
        Type { StructuralType{ { .name = std::string{ type_string }, .bitsize = bitsize } } };
}

/* ================================================================================================================================ */

} // End namespace ethercat::types

#endif