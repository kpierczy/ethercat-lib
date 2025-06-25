/* ============================================================================================================================ *//**
 * @file       traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 12:50:30 pm
 * @modified   Wednesday, 25th June 2025 3:14:46 pm
 * @project    ethercat-lib
 * @brief      Helper type traits related to module-defined types
 *
 *
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_TRAITS_H__
#define __ETHERCAT_COMMON_TYPES_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/types/type.hpp"
#include "ethercat/common/types/traits/type_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types::traits {

/* ===================================================== Type::ID traits =================================================== */

/**
 * @brief Helper type traits describing supported CoE types
 * @details Each specialization of traits struucture contains:
 *
 *    - type            : type for which traits has been specialized
 *    - value_type      : C++ type used to represent the type (except for Structural type)
 *    - value_type_name : name of the C++ type (except for Structural type)
 *    - bytesize        : size of the EtherCAT type in bytes (except for Structural type)
 *    - bitsize         : size of the EtherCAT type in bits (except for Structural type)
 *    - names           : list of human-readable names of the EtherCAT type
 *    - coe_names       : list of CoE-specific names of the EtherCAT type (except for Structural type)
 *
 * @tparam type_id_id
 *    ID of the type to be described
 */
template<auto type_id>
struct TypeTraits :
    public details::TypeTraits<type_id>
{ };

/**
 * @brief Type alias template providing default C++ that can be used to represent EtherCAT
 *    builtin type of the given @p type_id and given @p arity
 *
 * @tparam type_id
 *    ID of the target EtherCAT builtin type
 * @tparam arity
 *   arity of the target type ( @c 0 for scalar type )
 */
template<auto type_id, std::size_t arity = 0>
using TypeRepresentation = details::TypeRepresentation<type_id, arity>;

/**
 * @brief Helper meta-function converting @ref Type::ID enumeration into index of the corresponding
 *    @ref Value alternative
 */
template<auto type_id>
constexpr std::size_t TypeIndex = utilities::to_underlying(type_id);

/* ========================================= Type::ID traits functions (dynamic dispatch) ========================================= */

/**
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 *
 * @param type_id
 *    CoE type ID
 * @returns
 *    human-readable name of the C++ type used to represent EtherCAT type with the fiven @p type_id
 */
template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr std::string_view value_type_name(TypeIdEnum type_id);

/**
 * @tparam N
 *    index of the name variant to be returns (for types with multiple names)
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 *
 * @param type_id
 *    CoE type ID
 * @returns
 *    human-readable name of the EtherCAT type with the fiven @p type_id
 */
template<std::size_t N = 0, typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr std::string_view name(TypeIdEnum type_id);

/**
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 * @param type_id
 *    CoE type ID
 *
 * @returns
 *    array of human-readable names of the EtherCAT type with the fiven @p type_id
 */
template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr auto names(TypeIdEnum type_id);

/**
 * @tparam N
 *    index of the name variant to be returns (for types with multiple names)
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 * @param type_id
 *    CoE type ID
 *
 * @returns
 *    human-readable CoE name of the EtherCAT type with the fiven @p type_id
 */
template<std::size_t N = 0, typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr std::string_view coe_name(TypeIdEnum type_id);

/**
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 * @param type_id
 *    CoE type ID
 *
 * @returns
 *    array of human-readable CoE names of the EtherCAT type with the fiven @p type_id
 */
template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr auto coe_names(TypeIdEnum type_id);

/**
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 * @param type_id
 *    CoE type ID
 *
 * @returns
 *    byte size of the EtherCAT type with the fiven @p type_id
 */
template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr std::size_t bytesize(TypeIdEnum type_id);

/**
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 * @param type_id
 *    CoE type ID
 *
 * @returns
 *    bit size of the EtherCAT type with the fiven @p type_id
 */
template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr std::size_t bitsize(TypeIdEnum type_id);

/**
 * @brief Converts runtime object @p type to it's corresponding index
 *
 * @tparam TypeIdEnum
 *    type of the type ID enumeration
 */
template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
inline constexpr std::size_t type_index(TypeIdEnum type_id);

/* ================================================================================================================================ */

} // End namespace namespace ethercat::common::types::traits

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/types/traits/type_traits_functions.hpp"

/* ================================================================================================================================ */

#endif
