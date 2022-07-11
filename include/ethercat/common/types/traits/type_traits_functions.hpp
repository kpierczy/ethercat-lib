/* ============================================================================================================================ *//**
 * @file       type_traits_functions.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Friday, 3rd June 2022 2:31:16 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of auxiliary functions related to TypeTraits traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_TRAITS_TYPE_TRAITS_FUNCTIONS_H__
#define __ETHERCAT_COMMON_TYPES_TRAITS_TYPE_TRAITS_FUNCTIONS_H__

/* =========================================================== Includes =========================================================== */

// Boost includes
#include <boost/mp11.hpp>
// Private includes
#include "ethercat/common/utilities.hpp"
#include "ethercat/common/types/traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types::traits {

/* ================================================== Type traits functions ================================================ */

template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr std::string_view value_type_name(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::value_type_name;
    });
}


template<std::size_t N, typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr std::string_view name(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::names[N];
    });
}


template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr auto names(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return ranges::span( 
            TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::names.data(),
            TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::names.size()
        );
    });
}


template<std::size_t N, typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr std::string_view coe_name(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::coe_names[N];
    });
}


template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr auto coe_names(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return ranges::span( 
            TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::coe_names.data(),
            TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::coe_names.size()
        );
    });
}


template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr std::size_t bytesize(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::bytesize;
    });
}


template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr std::size_t bitsize(TypeIdEnum type_id) {
    return boost::mp11::mp_with_index<types::TYPES_NUM>( utilities::to_underlying(type_id), [&](auto I){
        return TypeTraits<utilities::to_enum<TypeIdEnum>(I)>::bitsize;
    });
}


template<typename TypeIdEnum,
    std::enable_if_t<std::is_enum_v<TypeIdEnum>, bool> = true>
constexpr std::size_t type_index(TypeIdEnum type_id) {
    return utilities::to_underlying(type_id);
}

/* ================================================================================================================================ */

} // End namespace namespace ethercat::common::types::traits

#endif
