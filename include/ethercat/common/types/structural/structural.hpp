/* ============================================================================================================================ *//**
 * @file       structural.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of common base types describing structural EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_STRUCTURAL_STRUCTURAL_H__
#define __ETHERCAT_COMMON_TYPES_STRUCTURAL_STRUCTURAL_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// Private includes
#include "ethercat/common/types/structural.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ======================================================== Structural type ======================================================= */

template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
bool StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::Subitem::operator==(const Subitem &ritem) const {
    return
        (subindex  == ritem.subindex ) and
        (name      == ritem.name     ) and
        (type      == ritem.type     ) and
        (bitoffset == ritem.bitoffset);
}


template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
bool StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::Subitem::operator!=(const Subitem &ritem) const {
    return not((*this) == ritem);
}


template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
bool StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::operator==(const StructuralTypeBase &rtype) const {
    return
        (name     == rtype.name    ) and
        (subitems == rtype.subitems) and
        (bitsize  == rtype.bitsize );
}


template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
bool StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::operator!=(const StructuralTypeBase &rtype) const {
    return not((*this) == rtype);
}


template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
constexpr std::size_t StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::get_bitsize() const {
    return bitsize;
}


template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
constexpr std::size_t StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::get_bytesize() const {
    constexpr std::size_t BITS_IN_BYTE = 8;
    return get_bitsize() / BITS_IN_BYTE;
}


template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
constexpr std::size_t StructuralTypeBase<SubitemsCollectionT, StringT, BuiltinTypeBase>::calculate_bitsize(config::types::Span<const Subitem> items) {
    
    std::size_t highest_bit = 0;
    
    // Iterate over items to find the highest bit occupied in the object's footprint (unluckily no std::max_transformed in C++17)
    for(const auto &item : items) {
        auto high_bit = item.bitoffset + item.type.get_bitsize();
        if(high_bit > highest_bit)
            highest_bit = high_bit;
    }

    return highest_bit;
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
