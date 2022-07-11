/* ============================================================================================================================ *//**
 * @file       structural.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of types describing structural EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __TYPES_STRUCTURAL_H__
#define __TYPES_STRUCTURAL_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <vector>
#include <string_view>
// Private includes
#include "ethercat/common/types.hpp"
#include "ethercat/types/builtin.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ======================================================== Structural type ======================================================= */

/**
 * @brief Class providing run-time description of custom structural type of EtherCAT (CoE) objects
 * 
 * @tparam SubitemsN
 *    number of subitems
 * 
 * @note StructuralType does not support nested structural types (however I'm not sure 
 *    whether CoE support such types at all)
 */
struct StructuralType : 
    public common::types::StructuralTypeBase<std::vector, std::string, BuiltinType>
{ };

/* ================================================================================================================================ */

} // End namespace ethercat::types

#endif