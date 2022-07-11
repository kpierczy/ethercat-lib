/* ============================================================================================================================ *//**
 * @file       builtin_type_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 12:50:30 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of TypeTraits traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_TRAITS_TYPE_TRAITS_H__
#define __ETHERCAT_COMMON_TYPES_TRAITS_TYPE_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/traits.hpp"
#include "ethercat/common/types/builtin.hpp"
#include "ethercat/common/types/structural.hpp"
#include "ethercat/common/types/type.hpp"
#include "ethercat/common/types/traits.hpp"
#include "ethercat/common/utilities/traits/type_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types::traits {

/* ========================================================== Type traits ========================================================= */

namespace details {

    /**
     * @details Bye the design, @ref TypeTraits should be specializable with an arbitrary enum type 
     *    (also enum class type) although values for which given traits are defined are immutable.
     *    To achieve this, thanks to C++17, one can define traits hierarchy in scope of the class 
     *    template that will point target type for traits. EnumDispatch class provides such a 
     *    type-dependant scope
     */
    template<typename TypeIdEnum>
    struct EnumDispatch {

        /* ------------------------------- Base -------------------------------- */
        
        /**
         * @brief Base specialization of traits
         * @note Full specializations are not allowed in non-namespace scope and so a dummy
         *    template parameter is required
         */
        template<TypeIdEnum type_id, bool dummy = true>
        struct TypeTraits { 
            static_assert(utilities::traits::always_false_v<type_id>,
                "[ethercat::common::types::traits::TypeTraits] Traits specialized for not-supported CoE type"
            );
        };

        /// Base type gathering common definions of traits specialization
        template<auto type_id, typename ValueT> 
        struct TypeTraitsBase
        { 
            /// Specialization's value
            static constexpr auto type = static_cast<TypeIdEnum>(type_id);
            /// Types
            using value_type = ValueT;
            using value_type_traits = utilities::traits::TypeTraits<value_type>;
            // Array types
            template<std::size_t N> 
            using array_type = typename value_type_traits::template array_type<N>;
            using dynami_array_type = typename value_type_traits::dynami_array_type;
            /// Constants
            static constexpr auto        value_type_name = value_type_traits::name;
            static constexpr std::size_t bytesize        = value_type_traits::bytesize;
            static constexpr std::size_t bitsize         = value_type_traits::bitsize;
        };

        /* -------------------------- Specializations -------------------------- */
        
        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Bit), dummy> :
            public TypeTraitsBase<types::ID::Bit, bool>
        { 
            /// Constants
            static constexpr std::size_t bitsize = 1;
            /// Names
            static constexpr std::array names     { "Bit", "Bool" };
            static constexpr std::array coe_names { "BIT", "BOOL" };
        };
        
        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Bool8), dummy> :
            public TypeTraitsBase<types::ID::Bool8, bool>
        {
            /// Names
            static constexpr std::array names     { "Bool8" };
            static constexpr std::array coe_names { "BOOL8" };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Byte), dummy> :
            public TypeTraitsBase<types::ID::Byte, uint8_t>
        {
            /// Names
            static constexpr std::array names     { "Byte" };
            static constexpr std::array coe_names { "BYTE" };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Word), dummy> :
            public TypeTraitsBase<types::ID::Word, uint16_t>
        {
            /// Names
            static constexpr std::array names     { "Word" };
            static constexpr std::array coe_names { "WORD" };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::DoubleWord), dummy> :
            public TypeTraitsBase<types::ID::DoubleWord, uint32_t>
        {
            /// Names
            static constexpr std::array names     { "DoubleWord" };
            static constexpr std::array coe_names { "DWORD"      };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::ShortInt), dummy> :
            public TypeTraitsBase<types::ID::ShortInt, int8_t>
        {
            /// Names
            static constexpr std::array names     { "ShortInt" };
            static constexpr std::array coe_names { "SINT"     };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::UnsignedShortInt), dummy> :
            public TypeTraitsBase<types::ID::UnsignedShortInt, uint8_t>
        {
            /// Names
            static constexpr std::array names     { "UnsignedShortInt" };
            static constexpr std::array coe_names { "USINT"            };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Int), dummy> :
            public TypeTraitsBase<types::ID::Int, int16_t>
        {
            /// Names
            static constexpr std::array names     { "Int" };
            static constexpr std::array coe_names { "INT" };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::UnsignedInt), dummy> :
            public TypeTraitsBase<types::ID::UnsignedInt, uint16_t>
        {
            /// Names
            static constexpr std::array names     { "UnsignedInt" };
            static constexpr std::array coe_names { "UINT"        };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::DoubleInt), dummy> :
            public TypeTraitsBase<types::ID::DoubleInt, int32_t>
        {
            /// Names
            static constexpr std::array names     { "DoubleInt" };
            static constexpr std::array coe_names { "DINT"      };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::UnsignedDoubleInt), dummy> :
            public TypeTraitsBase<types::ID::UnsignedDoubleInt, uint32_t>
        {
            /// Names
            static constexpr std::array names     { "UnsignedDoubleInt" };
            static constexpr std::array coe_names { "UDINT"             };
        };    

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::LongInt), dummy> :
            public TypeTraitsBase<types::ID::LongInt, int64_t>
        {
            /// Names
            static constexpr std::array names     { "LongInt" };
            static constexpr std::array coe_names { "LINT"    };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::UnsignedLongInt), dummy> :
            public TypeTraitsBase<types::ID::UnsignedLongInt, uint64_t>
        {
            /// Names
            static constexpr std::array names     { "UnsignedLongInt" };
            static constexpr std::array coe_names { "ULINT"           };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Real), dummy> :
            public TypeTraitsBase<types::ID::Real, float>
        {
            /// Names
            static constexpr std::array names     { "Real" };
            static constexpr std::array coe_names { "REAL" };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::LongReal), dummy> :
            public TypeTraitsBase<types::ID::LongReal, double>
        {
            /// Names
            static constexpr std::array names     { "LongReal" };
            static constexpr std::array coe_names { "LREAL"    };
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::String), dummy> :
            public TypeTraitsBase<types::ID::String, std::string>
        {
            /// Names
            static constexpr std::array names     { "String" };
            static constexpr std::array coe_names { "STRING" };

            /// Types
            using char_value_type = char;
            using char_value_type_traits = utilities::traits::TypeTraits<char_value_type>;
            // Array types
            template<std::size_t N> 
            using char_array_type = typename char_value_type_traits::template array_type<N>;
            using char_dynami_array_type = typename char_value_type_traits::dynami_array_type;
            /// Constants (Force 0 size of the string EtherCAT type's representation just in case to avoid potential buffer overflows by bugs)
            static constexpr std::size_t bytesize             = 0;
            static constexpr std::size_t bitsize              = 0;
            static constexpr auto        char_value_type_name = char_value_type_traits::name;
            static constexpr std::size_t char_bytesize        = char_value_type_traits::bytesize;
            static constexpr std::size_t char_bitsize         = char_value_type_traits::bitsize;
        };

        template<bool dummy> struct TypeTraits<static_cast<TypeIdEnum>(types::ID::Structural), dummy>
        { 
            /// Specialization's value
            static constexpr auto type = static_cast<TypeIdEnum>(types::ID::Structural);
            /// Types
            using value_type = void;
            using value_type_traits = void;
            // Array types
            template<std::size_t N> 
            using array_type = void;
            using dynami_array_type = void;
            /// Constants
            static constexpr auto        value_type_name = "void";
            static constexpr std::size_t bytesize        = 0;
            static constexpr std::size_t bitsize         = 0;
            /// Names
            static constexpr std::array names     { "Structural" };
            static constexpr std::array coe_names { "STRUCT"     };
        };

        /* --------------------------------------------------------------------- */

        // Helper alias for default representation-type guidance
        template<TypeIdEnum type_id, std::size_t arity>
        using TypeRepresentation = std::conditional_t<arity == 0,
            /* Scalar representation */ typename TypeTraits<type_id>::value_type,
            /* Array representation  */ typename TypeTraits<type_id>::template array_type<arity>>;

    };

    // Dispatch TypeTraits hierarchy for an arbitrary enum value
    template<auto TypeId, 
        std::enable_if_t<std::is_enum_v<decltype(TypeId)>, bool> = true>
    struct TypeTraits  : 
        public EnumDispatch<decltype(TypeId)>::template TypeTraits<TypeId>
    { };

    // Dispatch TypeRepresentation for an arbitrary enum value
    template<auto TypeId, std::size_t arity, 
        std::enable_if_t<std::is_enum_v<decltype(TypeId)>, bool> = true>
    using TypeRepresentation 
        = typename EnumDispatch<decltype(TypeId)>::template TypeRepresentation<TypeId, arity>;

}

/* ================================================================================================================================ */

} // End namespace namespace ethercat::common::types::traits

#endif
