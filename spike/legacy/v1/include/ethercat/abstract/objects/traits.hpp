/* ============================================================================================================================ *//**
 * @file       traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 12:50:30 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Helper type traits related to module-defined types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_TRAITS_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects::traits {

/* ============================================================ Traits ============================================================ */

namespace details {

    template<typename T>
    struct ValueTypeTraits : 
        public std::false_type 
    { };

    template<> struct ValueTypeTraits<bool> : 
        public std::true_type 
    { 
        static constexpr auto name = "bool";
    };
    
    template<> struct ValueTypeTraits<uint8_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "uint8_t";
    };
    
    template<> struct ValueTypeTraits<uint16_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "uint16_t";
    };
    
    template<> struct ValueTypeTraits<uint32_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "uint32_t";
    };
    
    template<> struct ValueTypeTraits<uint64_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "uint64_t";
    };
    
    template<> struct ValueTypeTraits<int8_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "int8_t";
    };
    
    template<> struct ValueTypeTraits<int16_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "int16_t";
    };
    
    template<> struct ValueTypeTraits<int32_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "int32_t";
    };
    
    template<> struct ValueTypeTraits<int64_t> : 
        public std::true_type 
    { 
        static constexpr auto name = "int64_t";
    };
    
    template<> struct ValueTypeTraits<float> : 
        public std::true_type 
    { 
        static constexpr auto name = "float";
    };
    
    template<> struct ValueTypeTraits<double> : 
        public std::true_type 
    { 
        static constexpr auto name = "double";
    };
    

}

/**
 * @brief Helper type traits describing C++ types used to store values of CoE objects
 * 
 * @tparam T 
 *    type to be described
 */
template<typename T>
struct ValueTypeTraits : 
    public details::ValueTypeTraits<T> 
{ };

namespace details {

    template<Type type>
    struct TypeTraits : 
        public std::false_type 
    { };

    template<> struct TypeTraits<Type::Bool> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::Bool>>::name;
        static constexpr auto name      = "Bool";
        static constexpr auto coe_name  = "BOOL";
    };

    template<> struct TypeTraits<Type::Byte> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::Byte>>::name;
        static constexpr auto name      = "Byte";
        static constexpr auto coe_name  = "BYTE";
    };

    template<> struct TypeTraits<Type::Word> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::Word>>::name;
        static constexpr auto name      = "Word";
        static constexpr auto coe_name  = "WORD";
    };

    template<> struct TypeTraits<Type::DoubleWord> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::DoubleWord>>::name;
        static constexpr auto name      = "DoubleWord";
        static constexpr auto coe_name  = "DWORD";
    };

    template<> struct TypeTraits<Type::ShortInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::ShortInt>>::name;
        static constexpr auto name      = "ShortInt";
        static constexpr auto coe_name  = "SINT";
    };

    template<> struct TypeTraits<Type::UnsignedShortInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::UnsignedShortInt>>::name;
        static constexpr auto name      = "UnsignedShortInt";
        static constexpr auto coe_name  = "USINT";
    };

    template<> struct TypeTraits<Type::Int> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::Int>>::name;
        static constexpr auto name      = "Int";
        static constexpr auto coe_name  = "INT";
    };

    template<> struct TypeTraits<Type::UnsignedInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::UnsignedInt>>::name;
        static constexpr auto name      = "UnsignedInt";
        static constexpr auto coe_name  = "UINT";
    };

    template<> struct TypeTraits<Type::DoubleInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::DoubleInt>>::name;
        static constexpr auto name      = "DoubleInt";
        static constexpr auto coe_name  = "DINT";
    };

    template<> struct TypeTraits<Type::UnsignedDoubleInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::UnsignedDoubleInt>>::name;
        static constexpr auto name      = "UnsignedDoubleInt";
        static constexpr auto coe_name  = "UDINT";
    };

    template<> struct TypeTraits<Type::LongInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::LongInt>>::name;
        static constexpr auto name      = "LongInt";
        static constexpr auto coe_name  = "LINT";
    };

    template<> struct TypeTraits<Type::UnsignedLongInt> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::UnsignedLongInt>>::name;
        static constexpr auto name      = "UnsignedLongInt";
        static constexpr auto coe_name  = "ULINT";
    };

    template<> struct TypeTraits<Type::Real> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::Real>>::name;
        static constexpr auto name      = "Real";
        static constexpr auto coe_name  = "REAL";
    };

    template<> struct TypeTraits<Type::LongReal> : 
        public std::true_type 
    { 
        static constexpr auto type_name = ValueTypeTraits<ValueType<Type::LongReal>>::name;
        static constexpr auto name      = "LongReal";
        static constexpr auto coe_name  = "LREAL";
    };


}

/**
 * @brief Helper type traits describing supported CoE types
 * 
 * @tparam type
 *    type to be described
 */
template<Type type>
struct TypeTraits : 
    public details::TypeTraits<type> 
{ };

/* ============================================================ Helpers =========================================================== */

template<typename T>
inline constexpr std::string_view type_name() {
    return ValueTypeTraits<T>::name;
}

inline constexpr std::string_view type_name(Type type) {
    switch(type) { 
        case Type::Bool:              return TypeTraits<Type::Bool>::type_name;
        case Type::Byte:              return TypeTraits<Type::Byte>::type_name;
        case Type::Word:              return TypeTraits<Type::Word>::type_name;
        case Type::DoubleWord:        return TypeTraits<Type::DoubleWord>::type_name;
        case Type::ShortInt:          return TypeTraits<Type::ShortInt>::type_name;
        case Type::UnsignedShortInt:  return TypeTraits<Type::UnsignedShortInt>::type_name;
        case Type::Int:               return TypeTraits<Type::Int>::type_name;
        case Type::UnsignedInt:       return TypeTraits<Type::UnsignedInt>::type_name;
        case Type::DoubleInt:         return TypeTraits<Type::DoubleInt>::type_name;
        case Type::UnsignedDoubleInt: return TypeTraits<Type::UnsignedDoubleInt>::type_name;
        case Type::LongInt:           return TypeTraits<Type::LongInt>::type_name;
        case Type::UnsignedLongInt:   return TypeTraits<Type::UnsignedLongInt>::type_name;
        case Type::Real:              return TypeTraits<Type::Real>::type_name;
        case Type::LongReal:          return TypeTraits<Type::LongReal>::type_name;
    }
}

inline constexpr std::string_view name(Type type) {
    switch(type) { 
        case Type::Bool:              return TypeTraits<Type::Bool>::name;
        case Type::Byte:              return TypeTraits<Type::Byte>::name;
        case Type::Word:              return TypeTraits<Type::Word>::name;
        case Type::DoubleWord:        return TypeTraits<Type::DoubleWord>::name;
        case Type::ShortInt:          return TypeTraits<Type::ShortInt>::name;
        case Type::UnsignedShortInt:  return TypeTraits<Type::UnsignedShortInt>::name;
        case Type::Int:               return TypeTraits<Type::Int>::name;
        case Type::UnsignedInt:       return TypeTraits<Type::UnsignedInt>::name;
        case Type::DoubleInt:         return TypeTraits<Type::DoubleInt>::name;
        case Type::UnsignedDoubleInt: return TypeTraits<Type::UnsignedDoubleInt>::name;
        case Type::LongInt:           return TypeTraits<Type::LongInt>::name;
        case Type::UnsignedLongInt:   return TypeTraits<Type::UnsignedLongInt>::name;
        case Type::Real:              return TypeTraits<Type::Real>::name;
        case Type::LongReal:          return TypeTraits<Type::LongReal>::name;
    }
}

inline constexpr std::string_view coe_name(Type type) {
    switch(type) { 
        case Type::Bool:              return TypeTraits<Type::Bool>::coe_name;
        case Type::Byte:              return TypeTraits<Type::Byte>::coe_name;
        case Type::Word:              return TypeTraits<Type::Word>::coe_name;
        case Type::DoubleWord:        return TypeTraits<Type::DoubleWord>::coe_name;
        case Type::ShortInt:          return TypeTraits<Type::ShortInt>::coe_name;
        case Type::UnsignedShortInt:  return TypeTraits<Type::UnsignedShortInt>::coe_name;
        case Type::Int:               return TypeTraits<Type::Int>::coe_name;
        case Type::UnsignedInt:       return TypeTraits<Type::UnsignedInt>::coe_name;
        case Type::DoubleInt:         return TypeTraits<Type::DoubleInt>::coe_name;
        case Type::UnsignedDoubleInt: return TypeTraits<Type::UnsignedDoubleInt>::coe_name;
        case Type::LongInt:           return TypeTraits<Type::LongInt>::coe_name;
        case Type::UnsignedLongInt:   return TypeTraits<Type::UnsignedLongInt>::coe_name;
        case Type::Real:              return TypeTraits<Type::Real>::coe_name;
        case Type::LongReal:          return TypeTraits<Type::LongReal>::coe_name;
    }
}

/* ================================================================================================================================ */

} // End namespace namespace ethercat::abstract::objects::traits

#endif
