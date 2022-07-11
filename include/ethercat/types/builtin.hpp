/* ============================================================================================================================ *//**
 * @file       builtin.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of types describing builtin EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_TYPES_BUILTIN_H__
#define __ETHERCAT_TYPES_BUILTIN_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <optional>
// Private includes
#include "ethercat/common/types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ========================================================= Numeric type ========================================================= */

/**
 * @brief Class providing run-time description of builtin numeric type of EtherCAT (CoE) objects
 */
struct NumericType : 
    public common::types::NumericTypeBase
{ };

/* ======================================================== StringTypeBase ======================================================== */

/**
 * @brief Class providing run-time description of builtin string type of EtherCAT (CoE) objects
 */
struct StringType : 
    public common::types::StringTypeBase
{ };

/* ======================================================== BuiltinTypeBase ======================================================= */

/**
 * @brief Class providing run-time description of builtin type of EtherCAT (CoE) objects
 */
struct BuiltinType : 
    public common::types::BuiltinTypeBase<std::string, NumericType, StringType>
{
    using common::types::BuiltinTypeBase<std::string, NumericType, StringType>::BuiltinTypeBase;

public: /* ------------------------------------------------- Public static methods ------------------------------------------------ */

    /**
     * @brief Parses @p type_string string to corresponding data type
     * 
     * @param type_string 
     *    string to be converted
     * 
     * @retval specifier 
     *    of the base type on success
     * @retval empty 
     *    optional if string does not describe base type 
     */
    static std::optional<BuiltinType> parse(std::string_view type_string);

};

/* ================================================================================================================================ */

} // End namespace ethercat::types

#endif