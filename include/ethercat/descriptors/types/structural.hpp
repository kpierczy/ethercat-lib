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

#ifndef __ETHERCAT_DESCRIPTORS_TYPES_STRUCTURAL_H__
#define __ETHERCAT_DESCRIPTORS_TYPES_STRUCTURAL_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string_view>
// Private includes
#include "ethercat/common/types.hpp"
#include "ethercat/descriptors/types/builtin.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::descriptors::types {

/* ============================================================ Helpers =========================================================== */

namespace details { 
    template<std::size_t SubitemsN> 
    struct array_n {
        template<typename T>
        using type = std::array<T, SubitemsN>;
    };
}

/* ======================================================== Structural type ======================================================= */

/**
 * @brief Class providing compile-time description of custom structural type of EtherCAT (CoE) objects
 * 
 * @tparam SubitemsN
 *    number of subitems
 * 
 * @note StructuralType does not support nested structural types (however I'm not sure 
 *    whether CoE support such types at all)
 */
template<std::size_t SubitemsN = 0>
struct StructuralType : 
    public common::types::StructuralTypeBase<details::array_n<SubitemsN>::template type, std::string_view, BuiltinType>
{ };

/* ================================================================================================================================ */

} // End namespace ethercat::descriptors::types

#endif