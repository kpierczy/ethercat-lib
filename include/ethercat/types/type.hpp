/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of types describing EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_TYPES_TYPE_H__
#define __ETHERCAT_TYPES_TYPE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/types.hpp"
#include "ethercat/types/builtin.hpp"
#include "ethercat/types/structural.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ======================================================== Structural type ======================================================= */

/**
 * @brief Class providing run-time description of arbitrary type of EtherCAT (CoE) objects
 */
struct Type : 
    public common::types::TypeBase<BuiltinType, StructuralType>
{ 
    using common::types::TypeBase<BuiltinType, StructuralType>::TypeBase;

public: /* ----------------------------------------------------- Public methods --------------------------------------------------- */

    /**
     * @returns 
     *    name of the type
     * 
     * @todo When C++20 is available implement fixed_string class that woulde enable forming
     *    names of array-typed types at compile time and making this function constexpr
     */
    std::string get_name() const;

public: /* ------------------------------------------------- Public static methods ------------------------------------------------ */

    /**
     * @brief Parses @p type_string string to corresponding data type
     * 
     * @param type_string 
     *    string to be converted
     * @param bitsize 
     *    bitsize of the type (taken into account if @p type_string is a name of non-builtin type; 
     *    otheriwse size is deduced automaitcally)
     * 
     * @returns
     *    type descriptor
     */
    static inline Type parse(std::string_view type_string, std::size_t bitsize = 0);
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::types

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/types/type/type.hpp"

/* ================================================================================================================================ */

#endif