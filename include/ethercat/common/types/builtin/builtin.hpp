/* ============================================================================================================================ *//**
 * @file       builtin.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of common base types describing builtin EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_BUILTIN_BUILTIN_H__
#define __ETHERCAT_COMMON_TYPES_BUILTIN_BUILTIN_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/types/builtin.hpp"
#include "ethercat/common/types/type.hpp"
#include "ethercat/common/types/traits.hpp"

/**
 * @note `types/type.hpp` is included here to provide proper inclusion order 
 *    with respecte to `types/traits.hpp`
 */

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ========================================================= Numeric type ========================================================= */

constexpr std::string_view NumericTypeBase::get_name() const {
    return traits::name(id);
}


constexpr std::string_view NumericTypeBase::get_coe_name() const {
    return traits::coe_name(id);
}


constexpr std::size_t NumericTypeBase::get_bitsize() const {
    return traits::bitsize(id);
}


constexpr std::size_t NumericTypeBase::get_bytesize() const {
    constexpr std::size_t BITS_IN_BYTE = 8;
    return get_bitsize() / BITS_IN_BYTE;
}

/* ======================================================== StringTypeBase ======================================================== */

constexpr std::size_t StringTypeBase::get_bitsize() const {
    return (traits::TypeTraits<ID::String>::char_bitsize * size);
}


constexpr std::size_t StringTypeBase::get_bytesize() const {
    constexpr std::size_t BITS_IN_BYTE = 8;
    return get_bitsize() / BITS_IN_BYTE;
}

/* ======================================================== BuiltinTypeBase ======================================================= */

template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::BuiltinTypeBase(
    NumericTypeBaseT numeric,
    std::size_t arity
) :
    arity{ arity },
    base{ numeric }
{ }


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::BuiltinTypeBase(
    std::string_view name,
    NumericTypeBaseT numeric,
    std::size_t arity
) :
    name{ name },
    arity{ arity },
    base{ numeric }
{ }


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::BuiltinTypeBase(
    StringTypeBaseT str,
    std::size_t arity
) :
    arity{ arity },
    base{ str }
{ }


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::BuiltinTypeBase(
    std::string_view name,
    StringTypeBaseT str,
    std::size_t arity
) :
    name{ name },
    arity{ arity },
    base{ str }
{ }


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
bool BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::operator==(const BuiltinTypeBase &rtype) const {
    return
        (arity    == rtype.arity   ) and
        (get_id() == rtype.get_id());
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
bool BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::operator!=(const BuiltinTypeBase &rtype) const {
    return not((*this) == rtype);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr bool BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::is_numeric() const {
    return std::holds_alternative<NumericTypeBaseT>(base);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr bool BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::is_string() const {
    return std::holds_alternative<StringTypeBaseT>(base);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr bool BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::is_scalar() const {
    return (arity == 0);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr bool BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::is_array() const {
    return (not is_scalar());
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr typename BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::ID 
BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_id() const {
    return is_numeric() ?
        ID( get_numeric().id ) :
        ID::String;
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr NumericTypeBaseT& BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_numeric() {
    return std::get<NumericTypeBaseT>(base);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr const NumericTypeBaseT& BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_numeric() const {
    return std::get<NumericTypeBaseT>(base);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr StringTypeBaseT& BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_string() {
    return std::get<StringTypeBaseT>(base);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr const StringTypeBaseT& BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_string() const {
    return std::get<StringTypeBaseT>(base);
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr std::string_view BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_name() const {
    return is_numeric() ?
        get_numeric().get_name() :
        traits::names<ID::String>[0];
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr std::string_view BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_coe_name() const {
    return is_numeric() ?
        get_numeric().get_coe_name() :
        traits::coe_names<ID::String>[0];
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr std::size_t BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_bitsize() const {
    return is_numeric() ?
        (get_numeric().get_bitsize() * std::max(arity, std::size_t(1))) :
        (get_string().get_bitsize()  * std::max(arity, std::size_t(1))) ;
}


template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
constexpr std::size_t BuiltinTypeBase<StringT, NumericTypeBaseT, StringTypeBaseT>::get_bytesize() const {
    constexpr std::size_t BITS_IN_BYTE = 8;
    return get_bitsize() / BITS_IN_BYTE;
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
