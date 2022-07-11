/* ============================================================================================================================ *//**
 * @file       ethercat_type_hierarchy.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 1st June 2022 7:14:09 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Program testing possibility of implementing common class hierarchy for runtime and compile time type descriptors
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

// Boost includes
#include <boost/mp11.hpp>
#include <iostream>
#include <cstdint>
#include <regex>
#include <bitset>
#include <variant>
#include <type_traits>
#include <range/v3/span.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/property_tree/ptree.hpp>
#include <experimental/type_traits>

/* ============================================================= Base ============================================================= */

struct NumericTypeBase {
    enum ID : std::size_t { DINT };
    ID id;
};

struct StringTypeBase {
    std::size_t size;
};

template<typename StringT, typename NumericTypeT, typename StringTypeT>
struct BuiltinTypeBase {
    std::optional<StringT> name;
    std::variant<NumericTypeT, StringTypeT> base;
    std::size_t arity;
};

template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeT>
struct StructuralTypeBase {

    struct Subitem {
        StringT name;
        BuiltinTypeT type;
        std::size_t bitoffset;
    };

    StringT name;
    std::size_t bitsize;
    SubitemsCollectionT<Subitem> subitems;
};

template<typename StructuralTypeT, typename BuiltinTypeT>
struct TypeBase {
    std::variant<StructuralTypeT, BuiltinTypeT> type;
};

/* ============================================================= Type ============================================================= */

struct NumericType    : public NumericTypeBase { } ;
struct StringType     : public StringTypeBase { } ;
struct BuiltinType    : public BuiltinTypeBase<std::string, NumericType, StringType> { } ;
struct StructuralType : public StructuralTypeBase<std::vector, std::string, BuiltinType> { } ;
struct Type           : public TypeBase<BuiltinType, StructuralType> { } ;

/* ========================================================== Descriptor ========================================================== */

namespace details { 
    template<std::size_t SubitemsN> 
    struct array_n {
        template<typename T>
        using type = std::array<T, SubitemsN>;
    };
}

                                struct NumericTypeDescriptor    : public NumericTypeBase { } ;
                                struct StringTypeDescriptor     : public StringTypeBase { } ;
                                struct BuiltinTypeDescriptor    : public BuiltinTypeBase<std::string_view, NumericTypeDescriptor, StringTypeDescriptor> { } ;
template<std::size_t SubitemsN> struct StructuralTypeDescriptor : public StructuralTypeBase<details::array_n<SubitemsN>::template type, std::string_view, BuiltinTypeDescriptor> { } ;
template<std::size_t SubitemsN> struct TypeDescriptor           : public TypeBase<BuiltinTypeDescriptor, StructuralTypeDescriptor<SubitemsN>> { } ;

/* ============================================================= Main ============================================================= */

int main(int argc, char const *argv[])
{
    
}
