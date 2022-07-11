/* ============================================================================================================================ *//**
 * @file       type_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Friday, 3rd June 2022 2:31:16 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the TypeTraits trait
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_TYPE_TRAITS_H__
#define __ETHERCAT_COMMON_UTILITIES_TYPE_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <cstdint>
// Private includes
#include "ethercat/common/utilities/traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities::traits {

/* ========================================================== TypeTraits ========================================================== */

namespace details {

    /* ----------------------------- Utilities ----------------------------- */
    
    /// Number of bits in abyte
    constexpr std::size_t BITS_IN_BYTE = 8;

    /// Auxiliary assertion macro
    #define ASSERT_TYPE_SIZE(name, size) \
        static_assert(sizeof(type) == bytesize, \
            "[ethercat::utilities::traits::TypeTraits<" name ">] Size of '" name "' is expected to be " #size)

    /* ------------------------------- Base -------------------------------- */
    
    template<typename T>
    struct TypeTraits { 
        static_assert(utilities::traits::always_false<T>::value,
            "[ethercat::utilities::traits::TypeTraits] Specialized for not-supported CoE type"
        );
    };

    template<typename T> 
    struct TypeTraitsBase
    {   
        // Types
        using type = T;
        // Array types
        template<std::size_t N> 
        using array_type = std::array<type, N>;
        using dynami_array_type = std::vector<type>;
        // Constants
        static constexpr std::size_t bytesize = sizeof(T);
        static constexpr std::size_t bitsize  = bytesize * BITS_IN_BYTE;
    };

    /* -------------------------- Specializations -------------------------- */
    
    template<> struct TypeTraits<bool> :
        public TypeTraitsBase<bool>
    {   
        // Constants
        static constexpr auto name = "bool";
        // Conditions
        ASSERT_TYPE_SIZE("bool", 1);
    };
    
    template<> struct TypeTraits<uint8_t> :
        public TypeTraitsBase<uint8_t>
    {   
        // Constants
        static constexpr auto name = "uint8_t";
        // Conditions
        ASSERT_TYPE_SIZE("uint8_t", 1);
    };
    
    template<> struct TypeTraits<uint16_t> :
        public TypeTraitsBase<uint16_t>
    {   
        // Constants
        static constexpr auto name = "uint16_t";
        // Conditions
        ASSERT_TYPE_SIZE("uint16_t", 2);
    };
    
    template<> struct TypeTraits<uint32_t> :
        public TypeTraitsBase<uint32_t>
    {   
        // Constants
        static constexpr auto name = "uint32_t";
        // Conditions
        ASSERT_TYPE_SIZE("uint32_t", 4);
    };
    
    template<> struct TypeTraits<uint64_t> :
        public TypeTraitsBase<uint64_t>
    {   
        // Constants
        static constexpr auto name = "uint64_t";
        // Conditions
        ASSERT_TYPE_SIZE("uint64_t", 8);
    };
    
    template<> struct TypeTraits<int8_t> :
        public TypeTraitsBase<int8_t>
    {   
        // Constants
        static constexpr auto name = "int8_t";
        // Conditions
        ASSERT_TYPE_SIZE("int8_t", 1);
    };
    
    template<> struct TypeTraits<int16_t> :
        public TypeTraitsBase<int16_t>
    {   
        // Constants
        static constexpr auto name = "int16_t";
        // Conditions
        ASSERT_TYPE_SIZE("int16_t", 2);
    };
    
    template<> struct TypeTraits<int32_t> :
        public TypeTraitsBase<int32_t>
    {   
        // Constants
        static constexpr auto name = "int32_t";
        // Conditions
        ASSERT_TYPE_SIZE("int32_t", 4);
    };
    
    template<> struct TypeTraits<int64_t> :
        public TypeTraitsBase<int64_t>
    {   
        // Constants
        static constexpr auto name = "int64_t";
        // Conditions
        ASSERT_TYPE_SIZE("int64_t", 8);
    };
    
    template<> struct TypeTraits<float> :
        public TypeTraitsBase<float>
    {   
        // Constants
        static constexpr auto name = "float";
        // Conditions
        ASSERT_TYPE_SIZE("float", 4);
    };
    
    template<> struct TypeTraits<double> :
        public TypeTraitsBase<double>
    {   
        // Constants
        static constexpr auto name = "double";
        // Conditions
        ASSERT_TYPE_SIZE("double", 8);
    };
    
    template<> struct TypeTraits<char> :
        public TypeTraitsBase<char>
    {   
        // Constants
        static constexpr auto name = "char";
        // Conditions
        ASSERT_TYPE_SIZE("char", 1);
    };
    
    template<> struct TypeTraits<std::string> :
        public TypeTraitsBase<std::string>
    {   
        // Constants
        static constexpr auto        name     = "std::string";
        static constexpr std::size_t bytesize = 0;
        static constexpr std::size_t bitsize  = bytesize * BITS_IN_BYTE;
    };
    
    /* --------------------------------------------------------------------- */
    
    #undef ASSERT_TYPE_SIZE
    
}

/**
 * @brief Traits structure providing informations about basic C++ types
 */
template<typename T>
struct TypeTraits :
    public details::TypeTraits<T>
{ };

/* ================================================================================================================================ */

} // End namespace namespace ethercat::common::utilities::traits

#endif
