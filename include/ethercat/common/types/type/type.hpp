/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of common base types describing arbitrary EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_TYPE_TYPE_H__
#define __ETHERCAT_COMMON_TYPES_TYPE_TYPE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/types/type.hpp"
#include "ethercat/common/types/traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ========================================================= Public ctors ========================================================= */

template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::TypeBase(const BuiltinTypeBaseT &builtin) :
    type{ builtin }
{ }


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::TypeBase(const StructuralTypeBaseT &structural) :
    type{ structural }
{ }

/* ======================================================= Public operators ======================================================= */

template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
bool TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::operator==(const TypeBase &rtype) const {

    // Check if IDs of types match
    if(get_id() != rtype.get_id())
        return false;

    // If both types are structural, compare types
    if(get_id() == ID::Structural)
        return get_structural() == rtype.get_structural();
    else
        return get_builtin() == rtype.get_builtin();
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
bool TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::operator!=(const TypeBase &rtype) const {
    return not((*this) == rtype);
}

/* ==================================================== Public traits methodsx ==================================================== */

template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr bool TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::is_builtin() const {
    return std::holds_alternative<BuiltinTypeBaseT>(type);
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr bool TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::is_structural() const {
    return std::holds_alternative<StructuralTypeBaseT>(type);
}

/* ======================================================== Public getters ======================================================== */

template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr typename TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::ID 
TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_id() const {
    return is_builtin() ?
        ID( get_builtin().get_id() ) :
        ID::Structural;
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr BuiltinTypeBaseT& TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_builtin() {
    return std::get<BuiltinTypeBaseT>(type);
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr const BuiltinTypeBaseT& TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_builtin() const {
    return std::get<BuiltinTypeBaseT>(type);
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr StructuralTypeBaseT& TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_structural() {
    return std::get<StructuralTypeBaseT>(type);
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr const StructuralTypeBaseT& TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_structural() const {
    return std::get<StructuralTypeBaseT>(type);
}

/* ===================================================== Public helper methods ==================================================== */

template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr std::string_view TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_name() const {
    return is_builtin() ?
        get_builtin().get_name() :
        traits::TypeTraits<ID::Structural>::names[0];
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr std::string_view TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_coe_name() const {
    return is_builtin() ?
        get_builtin().get_coe_name() :
        traits::TypeTraits<ID::Structural>::coe_names[0];
}


template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr std::size_t TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_bitsize() const {
    return is_builtin() ?
        ( get_builtin().get_bitsize()    ) :
        ( get_structural().get_bitsize() ) ;
}

template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
constexpr std::size_t TypeBase<BuiltinTypeBaseT, StructuralTypeBaseT>::get_bytesize() const {
    constexpr std::size_t BITS_IN_BYTE = 8;
    return get_bitsize() / BITS_IN_BYTE;
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
