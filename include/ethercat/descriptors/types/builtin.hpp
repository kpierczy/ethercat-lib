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

#ifndef __ETHERCAT_DESCRIPTORS_TYPES_BUILTIN_H__
#define __ETHERCAT_DESCRIPTORS_TYPES_BUILTIN_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string_view>
// Private includes
#include "ethercat/common/types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::descriptors::types {

/* ========================================================= Numeric type ========================================================= */

/**
 * @brief Class providing compile-time description of builtin numeric type of EtherCAT (CoE) objects
 * 
 * @see <a href="https://stackoverflow.com/questions/70356275/working-around-limitation-of-constexpr-static-data-member-with-same-type-as-en">
 *     constexpr-static-data-member-with-same<a/>
 * @see <a href="https://eel.is/c++draft/cmp.partialord#lib:partial_ordering">partial_ordering<a/>
 */
struct NumericType : 
    public common::types::NumericTypeBase
{     
    static const NumericType Bit;               /**< 1-bit boolean              [BIT]    */
    static const NumericType Bool;              /**< 1-bit boolean              [BOOL]   */
    static const NumericType Bool8;             /**< 8-bit boolean              [BOOL8]  */
    static const NumericType Byte;              /**< 8-bit bitset               [BYTE]   */
    static const NumericType Word;              /**< 16-bit bitset              [WORD]   */
    static const NumericType DoubleWord;        /**< 32-bit bitset              [DWORD]  */
    static const NumericType ShortInt;          /**< 8-bit signed int           [SINT]   */
    static const NumericType UnsignedShortInt;  /**< 8-bit unsigned short int   [USINT]  */
    static const NumericType Int;               /**< 16-bit signed int          [INT]    */
    static const NumericType UnsignedInt;       /**< 16-bit unsigned int        [UINT]   */
    static const NumericType DoubleInt;         /**< 32-bit signed int          [DINT]   */
    static const NumericType UnsignedDoubleInt;  /**< 32-bit unsigned double int [UDINT] */
    static const NumericType LongInt;           /**< 64-bit signed int          [LINT]   */
    static const NumericType UnsignedLongInt;   /**< 64-bit unsigned long int   [ULINT]  */
    static const NumericType Real;              /**< 32-bit floating point      [REAL]   */
    static const NumericType LongReal;          /**< 64-bit floating point      [LREAL]  */

    static const NumericType BIT;               /**< 1-bit boolean              [BIT]   */
    static const NumericType BOOL;              /**< 1-bit boolean              [BOOL]  */
    static const NumericType BYTE;              /**< 8-bit bitset               [BYTE]  */
    static const NumericType WORD;              /**< 16-bit bitset              [WORD]  */
    static const NumericType DWORD;             /**< 32-bit bitset              [DWORD] */
    static const NumericType SINT;              /**< 8-bit signed int           [SINT]  */
    static const NumericType USINT;             /**< 8-bit unsigned short int   [USINT] */
    static const NumericType INT;               /**< 16-bit signed int          [INT]   */
    static const NumericType UINT;              /**< 16-bit unsigned int        [UINT]  */
    static const NumericType DINT;              /**< 32-bit signed int          [DINT]  */
    static const NumericType UDINT;             /**< 32-bit unsigned double int [UDINT] */
    static const NumericType LINT;              /**< 64-bit signed int          [LINT]  */
    static const NumericType ULINT;             /**< 64-bit unsigned long int   [ULINT] */
    static const NumericType REAL;              /**< 32-bit floating point      [REAL]  */
    static const NumericType LREAL;             /**< 64-bit floating point      [LREAL] */

    static const NumericType BIT1;              /**< 1-bit boolean              [BIT]   */
    static const NumericType BOOL1;             /**< 1-bit boolean              [BOOL]  */
    static const NumericType BOOL8;             /**< 8-bit boolean              [BOOL]  */
    static const NumericType BYTE8;             /**< 8-bit bitset               [BYTE]  */
    static const NumericType WORD16;            /**< 16-bit bitset              [WORD]  */
    static const NumericType DWORD32;           /**< 32-bit bitset              [DWORD] */
    static const NumericType SINT8;             /**< 8-bit signed int           [SINT]  */
    static const NumericType USINT8;            /**< 8-bit unsigned short int   [USINT] */
    static const NumericType INT16;             /**< 16-bit signed int          [INT]   */
    static const NumericType UINT16;            /**< 16-bit unsigned int        [UINT]  */
    static const NumericType DINT32;            /**< 32-bit signed int          [DINT]  */
    static const NumericType UDINT32;           /**< 32-bit unsigned double int [UDINT] */
    static const NumericType LINT64;            /**< 64-bit signed int          [LINT]  */
    static const NumericType ULINT64;           /**< 64-bit unsigned long int   [ULINT] */
    static const NumericType REAL32;            /**< 32-bit floating point      [REAL]  */
    static const NumericType LREAL64;           /**< 64-bit floating point      [LREAL] */
};

/* ======================================================== StringTypeBase ======================================================== */

/**
 * @brief Class providing compile-time description of builtin string type of EtherCAT (CoE) objects
 */
struct StringType : 
    public common::types::StringTypeBase
{
    template<std::size_t N> static constexpr StringType String  { N }; /**< (n) character string [STRING(n)] */
    template<std::size_t N> static constexpr StringType STRING  { N }; /**< (n) character string [STRING(n)] */
    template<std::size_t N> static constexpr StringType STRINGn { N }; /**< (n) character string [STRING(n)] */
};

/* ======================================================== BuiltinTypeBase ======================================================= */

/**
 * @brief Class providing compile-time description of builtin type of EtherCAT (CoE) objects
 */
struct BuiltinType : 
    public common::types::BuiltinTypeBase<std::string_view, NumericType, StringType>
{
    using common::types::BuiltinTypeBase<std::string_view, NumericType, StringType>::BuiltinTypeBase;

    /* --------------------------------- Scalar constants --------------------------------- */
    
    static const BuiltinType Bit;               /**< 1-bit boolean              [BIT]    */
    static const BuiltinType Bool;              /**< 1-bit boolean              [BOOL]   */
    static const BuiltinType Bool8;             /**< 8-bit boolean              [BOOL8]  */
    static const BuiltinType Byte;              /**< 8-bit bitset               [BYTE]   */
    static const BuiltinType Word;              /**< 16-bit bitset              [WORD]   */
    static const BuiltinType DoubleWord;        /**< 32-bit bitset              [DWORD]  */
    static const BuiltinType ShortInt;          /**< 8-bit signed int           [SINT]   */
    static const BuiltinType UnsignedShortInt;  /**< 8-bit unsigned short int   [USINT]  */
    static const BuiltinType Int;               /**< 16-bit signed int          [INT]    */
    static const BuiltinType UnsignedInt;       /**< 16-bit unsigned int        [UINT]   */
    static const BuiltinType DoubleInt;         /**< 32-bit signed int          [DINT]   */
    static const BuiltinType UnsignedDoubleInt;  /**< 32-bit unsigned double int [UDINT] */
    static const BuiltinType LongInt;           /**< 64-bit signed int          [LINT]   */
    static const BuiltinType UnsignedLongInt;   /**< 64-bit unsigned long int   [ULINT]  */
    static const BuiltinType Real;              /**< 32-bit floating point      [REAL]   */
    static const BuiltinType LongReal;          /**< 64-bit floating point      [LREAL]  */
    
    template<std::size_t N> 
    static constexpr BuiltinType String  
    { StringType{ N } };                        /**< (n) character string [STRING(n)]    */

    static const BuiltinType BIT;               /**< 1-bit boolean              [BIT]   */
    static const BuiltinType BOOL;              /**< 1-bit boolean              [BOOL]  */
    static const BuiltinType BYTE;              /**< 8-bit bitset               [BYTE]  */
    static const BuiltinType WORD;              /**< 16-bit bitset              [WORD]  */
    static const BuiltinType DWORD;             /**< 32-bit bitset              [DWORD] */
    static const BuiltinType SINT;              /**< 8-bit signed int           [SINT]  */
    static const BuiltinType USINT;             /**< 8-bit unsigned short int   [USINT] */
    static const BuiltinType INT;               /**< 16-bit signed int          [INT]   */
    static const BuiltinType UINT;              /**< 16-bit unsigned int        [UINT]  */
    static const BuiltinType DINT;              /**< 32-bit signed int          [DINT]  */
    static const BuiltinType UDINT;             /**< 32-bit unsigned double int [UDINT] */
    static const BuiltinType LINT;              /**< 64-bit signed int          [LINT]  */
    static const BuiltinType ULINT;             /**< 64-bit unsigned long int   [ULINT] */
    static const BuiltinType REAL;              /**< 32-bit floating point      [REAL]  */
    static const BuiltinType LREAL;             /**< 64-bit floating point      [LREAL] */
    
    template<std::size_t N> 
    static constexpr BuiltinType STRING  
    { StringType{ N } };                        /**< (n) character string [STRING(n)]   */
    
    static const BuiltinType BIT1;              /**< 1-bit boolean              [BIT]   */
    static const BuiltinType BOOL1;             /**< 1-bit boolean              [BOOL]  */
    static const BuiltinType BOOL8;             /**< 8-bit boolean              [BOOL]  */
    static const BuiltinType BYTE8;             /**< 8-bit bitset               [BYTE]  */
    static const BuiltinType WORD16;            /**< 16-bit bitset              [WORD]  */
    static const BuiltinType DWORD32;           /**< 32-bit bitset              [DWORD] */
    static const BuiltinType SINT8;             /**< 8-bit signed int           [SINT]  */
    static const BuiltinType USINT8;            /**< 8-bit unsigned short int   [USINT] */
    static const BuiltinType INT16;             /**< 16-bit signed int          [INT]   */
    static const BuiltinType UINT16;            /**< 16-bit unsigned int        [UINT]  */
    static const BuiltinType DINT32;            /**< 32-bit signed int          [DINT]  */
    static const BuiltinType UDINT32;           /**< 32-bit unsigned double int [UDINT] */
    static const BuiltinType LINT64;            /**< 64-bit signed int          [LINT]  */
    static const BuiltinType ULINT64;           /**< 64-bit unsigned long int   [ULINT] */
    static const BuiltinType REAL32;            /**< 32-bit floating point      [REAL]  */
    static const BuiltinType LREAL64;           /**< 64-bit floating point      [LREAL] */
    
    template<std::size_t N> 
    static constexpr BuiltinType STRINGn 
    { StringType{ N } };                        /**< (n) character string [STRING(n)]   */

    /* ---------------------------------- Array constants --------------------------------- */

    template<std::size_t arity> static constexpr BuiltinType BitArray               { BuiltinType{ NumericType::Bit,               arity } }; /**< array of 1-bit booleans              [BIT_ARRAY]    */
    template<std::size_t arity> static constexpr BuiltinType BoolArray              { BuiltinType{ NumericType::Bool,              arity } }; /**< array of 1-bit booleans              [BOOL_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType Bool8Array             { BuiltinType{ NumericType::Bool8,             arity } }; /**< array of 8-bit booleans              [BOOL_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType ByteArray              { BuiltinType{ NumericType::Byte,              arity } }; /**< array of 8-bit bitsets               [BYTE_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType WordArray              { BuiltinType{ NumericType::Word,              arity } }; /**< array of 16-bit bitsets              [WORD_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType DoubleWordArray        { BuiltinType{ NumericType::DoubleWord,        arity } }; /**< array of 32-bit bitsets              [DWORD_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType ShortIntArray          { BuiltinType{ NumericType::ShortInt,          arity } }; /**< array of 8-bit signed ints           [SINT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType UnsignedShortIntArray  { BuiltinType{ NumericType::UnsignedShortInt,  arity } }; /**< array of 8-bit unsigned short ints   [USINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType IntArray               { BuiltinType{ NumericType::Int,               arity } }; /**< array of 16-bit signed ints          [INT_ARRAY]    */
    template<std::size_t arity> static constexpr BuiltinType UnsignedIntArray       { BuiltinType{ NumericType::UnsignedInt,       arity } }; /**< array of 16-bit unsigned ints        [UINT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType DoubleIntArray         { BuiltinType{ NumericType::DoubleInt,         arity } }; /**< array of 32-bit signed ints          [DINT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType UnsignedDoubleIntArray { BuiltinType{ NumericType::UnsignedDoubleInt, arity } }; /**< array of 32-bit unsigned double ints [UDINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType LongIntArray           { BuiltinType{ NumericType::LongInt,           arity } }; /**< array of 64-bit signed ints          [LINT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType UnsignedLongIntArray   { BuiltinType{ NumericType::UnsignedLongInt,   arity } }; /**< array of 64-bit unsigned long ints   [ULINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType RealArray              { BuiltinType{ NumericType::Real,              arity } }; /**< array of 32-bit floating points      [REAL_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType LongRealArray          { BuiltinType{ NumericType::LongReal,          arity } }; /**< array of 64-bit floating points      [LREAL_ARRAY]  */
    
    template<std::size_t N, std::size_t arity> 
    static constexpr BuiltinType StringArray  
    { StringType{ N }, arity }; /**< array of (n) character strings [STRING(n)] */

    template<std::size_t arity> static constexpr BuiltinType BIT_ARRAY   { BitArray<arity>              }; /**< array of 1-bit booleans              [BIT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType BOOL_ARRAY  { BoolArray<arity>             }; /**< array of 1-bit booleans              [BOOL_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType BYTE_ARRAY  { ByteArray<arity>             }; /**< array of 8-bit bitsets               [BYTE_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType WORD_ARRAY  { WordArray<arity>             }; /**< array of 16-bit bitsets              [WORD_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType DWORD_ARRAY { DoubleWordArray<arity>       }; /**< array of 32-bit bitsets              [DWORD_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType SINT_ARRAY  { ShortIntArray<arity>         }; /**< array of 8-bit signed ints           [SINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType USINT_ARRAY { UnsignedShortIntArray<arity> }; /**< array of 8-bit unsigned short ints   [USINT_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType INT_ARRAY   { IntArray<arity>              }; /**< array of 16-bit signed ints          [INT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType UINT_ARRAY  { UnsignedIntArray<arity>      }; /**< array of 16-bit unsigned ints        [UINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType DINT_ARRAY  { DoubleIntArray<arity>        }; /**< array of 32-bit signed ints          [DINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType UDINT_ARRAY { UnsignedDoubleIntArray<arity>}; /**< array of 32-bit unsigned double ints [UDINT_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType LINT_ARRAY  { LongIntArray<arity>          }; /**< array of 64-bit signed ints          [LINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType ULINT_ARRAY { UnsignedLongIntArray<arity>  }; /**< array of 64-bit unsigned long ints   [ULINT_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType REAL_ARRAY  { RealArray<arity>             }; /**< array of 32-bit floating points      [REAL_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType LREAL_ARRAY { LongRealArray<arity>         }; /**< array of 64-bit floating points      [LREAL_ARRAY] */
    
    template<std::size_t N, std::size_t arity> 
    static constexpr BuiltinType STRING_ARRAY  
    { StringType{ N }, arity }; /**< array of (n) character strings [STRING(n)] */
    
    template<std::size_t arity> static constexpr BuiltinType BIT1_ARRAY    { BitArray<arity>               }; /**< array of 1-bit booleans              [BIT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType BOOL1_ARRAY   { BoolArray<arity>              }; /**< array of 1-bit booleans              [BOOL_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType BOOL8_ARRAY   { Bool8Array<arity>             }; /**< array of 8-bit booleans              [BOOL_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType BYTE8_ARRAY   { ByteArray<arity>              }; /**< array of 8-bit bitsets               [BYTE_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType WORD16_ARRAY  { WordArray<arity>              }; /**< array of 16-bit bitsets              [WORD_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType DWORD32_ARRAY { DoubleWordArray<arity>        }; /**< array of 32-bit bitsets              [DWORD_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType SINT8_ARRAY   { ShortIntArray<arity>          }; /**< array of 8-bit signed ints           [SINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType USINT8_ARRAY  { UnsignedShortIntArray<arity>  }; /**< array of 8-bit unsigned short ints   [USINT_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType INT16_ARRAY   { IntArray<arity>               }; /**< array of 16-bit signed ints          [INT_ARRAY]   */
    template<std::size_t arity> static constexpr BuiltinType UINT16_ARRAY  { UnsignedIntArray<arity>       }; /**< array of 16-bit unsigned ints        [UINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType DINT32_ARRAY  { DoubleIntArray<arity>         }; /**< array of 32-bit signed ints          [DINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType UDINT32_ARRAY { UnsignedDoubleIntArray<arity> }; /**< array of 32-bit unsigned double ints [UDINT_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType LINT64_ARRAY  { LongIntArray<arity>           }; /**< array of 64-bit signed ints          [LINT_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType ULINT64_ARRAY { UnsignedLongIntArray<arity>   }; /**< array of 64-bit unsigned long ints   [ULINT_ARRAY] */
    template<std::size_t arity> static constexpr BuiltinType REAL32_ARRAY  { RealArray<arity>              }; /**< array of 32-bit floating points      [REAL_ARRAY]  */
    template<std::size_t arity> static constexpr BuiltinType LREAL64_ARRAY { LongRealArray<arity>          }; /**< array of 64-bit floating points      [LREAL_ARRAY] */
    
    template<std::size_t N, std::size_t arity> 
    static constexpr BuiltinType STRINGn_ARRAY 
    { StringType{ N }, arity }; /**< array of (n) character strings [STRING(n)] */
};

/* ================================================================================================================================ */

} // End namespace ethercat::descriptors::types

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/descriptors/types/builtin/builtin.hpp"

/* ================================================================================================================================ */

#endif