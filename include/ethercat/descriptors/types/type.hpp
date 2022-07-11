/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of types describing EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DESCTIPTORS_TYPES_TYPE_H__
#define __ETHERCAT_DESCTIPTORS_TYPES_TYPE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/types.hpp"
#include "ethercat/descriptors/types/builtin.hpp"
#include "ethercat/descriptors/types/structural.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::descriptors::types {

/* ======================================================== Structural type ======================================================= */

/**
 * @brief Class providing compile-time description of arbitrary type of EtherCAT (CoE) objects
 */
template<std::size_t SubitemsN = 0>
struct Type : 
    public common::types::TypeBase<BuiltinType, StructuralType<SubitemsN>>
{
    using common::types::TypeBase<BuiltinType, StructuralType<SubitemsN>>::TypeBase;
    
    /* --------------------------------- Scalar constants --------------------------------- */

    static const Type Bit;               /**< 1-bit boolean              [BIT]    */
    static const Type Bool;              /**< 1-bit boolean              [BOOL]   */
    static const Type Bool8;             /**< 8-bit boolean              [BOOL8]  */
    static const Type Byte;              /**< 8-bit bitset               [BYTE]   */
    static const Type Word;              /**< 16-bit bitset              [WORD]   */
    static const Type DoubleWord;        /**< 32-bit bitset              [DWORD]  */
    static const Type ShortInt;          /**< 8-bit signed int           [SINT]   */
    static const Type UnsignedShortInt;  /**< 8-bit unsigned short int   [USINT]  */
    static const Type Int;               /**< 16-bit signed int          [INT]    */
    static const Type UnsignedInt;       /**< 16-bit unsigned int        [UINT]   */
    static const Type DoubleInt;         /**< 32-bit signed int          [DINT]   */
    static const Type UnsignedDoubleInt;  /**< 32-bit unsigned double int [UDINT] */
    static const Type LongInt;           /**< 64-bit signed int          [LINT]   */
    static const Type UnsignedLongInt;   /**< 64-bit unsigned long int   [ULINT]  */
    static const Type Real;              /**< 32-bit floating point      [REAL]   */
    static const Type LongReal;          /**< 64-bit floating point      [LREAL]  */
    
    template<std::size_t N> 
    static constexpr Type String  
    { BuiltinType{ StringType{ N } } };  /**< (n) character string [STRING(n)]    */

    static const Type BIT;               /**< 1-bit boolean              [BIT]   */
    static const Type BOOL;              /**< 1-bit boolean              [BOOL]  */
    static const Type BYTE;              /**< 8-bit bitset               [BYTE]  */
    static const Type WORD;              /**< 16-bit bitset              [WORD]  */
    static const Type DWORD;             /**< 32-bit bitset              [DWORD] */
    static const Type SINT;              /**< 8-bit signed int           [SINT]  */
    static const Type USINT;             /**< 8-bit unsigned short int   [USINT] */
    static const Type INT;               /**< 16-bit signed int          [INT]   */
    static const Type UINT;              /**< 16-bit unsigned int        [UINT]  */
    static const Type DINT;              /**< 32-bit signed int          [DINT]  */
    static const Type UDINT;             /**< 32-bit unsigned double int [UDINT] */
    static const Type LINT;              /**< 64-bit signed int          [LINT]  */
    static const Type ULINT;             /**< 64-bit unsigned long int   [ULINT] */
    static const Type REAL;              /**< 32-bit floating point      [REAL]  */
    static const Type LREAL;             /**< 64-bit floating point      [LREAL] */
    
    template<std::size_t N> 
    static constexpr Type STRING  
    { BuiltinType{ StringType{ N } } };  /**< (n) character string [STRING(n)]   */
    
    static const Type BIT1;              /**< 1-bit boolean              [BIT]   */
    static const Type BOOL1;             /**< 1-bit boolean              [BOOL]  */
    static const Type BOOL8;             /**< 8-bit boolean              [BOOL]  */
    static const Type BYTE8;             /**< 8-bit bitset               [BYTE]  */
    static const Type WORD16;            /**< 16-bit bitset              [WORD]  */
    static const Type DWORD32;           /**< 32-bit bitset              [DWORD] */
    static const Type SINT8;             /**< 8-bit signed int           [SINT]  */
    static const Type USINT8;            /**< 8-bit unsigned short int   [USINT] */
    static const Type INT16;             /**< 16-bit signed int          [INT]   */
    static const Type UINT16;            /**< 16-bit unsigned int        [UINT]  */
    static const Type DINT32;            /**< 32-bit signed int          [DINT]  */
    static const Type UDINT32;           /**< 32-bit unsigned double int [UDINT] */
    static const Type LINT64;            /**< 64-bit signed int          [LINT]  */
    static const Type ULINT64;           /**< 64-bit unsigned long int   [ULINT] */
    static const Type REAL32;            /**< 32-bit floating point      [REAL]  */
    static const Type LREAL64;           /**< 64-bit floating point      [LREAL] */
    
    template<std::size_t N> 
    static constexpr Type STRINGn 
    { BuiltinType{ StringType{ N } } };  /**< (n) character string [STRING(n)]   */

    /* ---------------------------------- Array constants --------------------------------- */

    template<std::size_t arity> static constexpr Type BitArray               { BuiltinType{ NumericType::Bit,               arity } }; /**< array of 1-bit booleans              [BIT_ARRAY]    */
    template<std::size_t arity> static constexpr Type BoolArray              { BuiltinType{ NumericType::Bool,              arity } }; /**< array of 1-bit booleans              [BOOL_ARRAY]   */
    template<std::size_t arity> static constexpr Type Bool8Array             { BuiltinType{ NumericType::Bool8,             arity } }; /**< array of 8-bit booleans              [BOOL_ARRAY]   */
    template<std::size_t arity> static constexpr Type ByteArray              { BuiltinType{ NumericType::Byte,              arity } }; /**< array of 8-bit bitsets               [BYTE_ARRAY]   */
    template<std::size_t arity> static constexpr Type WordArray              { BuiltinType{ NumericType::Word,              arity } }; /**< array of 16-bit bitsets              [WORD_ARRAY]   */
    template<std::size_t arity> static constexpr Type DoubleWordArray        { BuiltinType{ NumericType::DoubleWord,        arity } }; /**< array of 32-bit bitsets              [DWORD_ARRAY]  */
    template<std::size_t arity> static constexpr Type ShortIntArray          { BuiltinType{ NumericType::ShortInt,          arity } }; /**< array of 8-bit signed ints           [SINT_ARRAY]   */
    template<std::size_t arity> static constexpr Type UnsignedShortIntArray  { BuiltinType{ NumericType::UnsignedShortInt,  arity } }; /**< array of 8-bit unsigned short ints   [USINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type IntArray               { BuiltinType{ NumericType::Int,               arity } }; /**< array of 16-bit signed ints          [INT_ARRAY]    */
    template<std::size_t arity> static constexpr Type UnsignedIntArray       { BuiltinType{ NumericType::UnsignedInt,       arity } }; /**< array of 16-bit unsigned ints        [UINT_ARRAY]   */
    template<std::size_t arity> static constexpr Type DoubleIntArray         { BuiltinType{ NumericType::DoubleInt,         arity } }; /**< array of 32-bit signed ints          [DINT_ARRAY]   */
    template<std::size_t arity> static constexpr Type UnsignedDoubleIntArray { BuiltinType{ NumericType::UnsignedDoubleInt, arity } }; /**< array of 32-bit unsigned double ints [UDINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type LongIntArray           { BuiltinType{ NumericType::LongInt,           arity } }; /**< array of 64-bit signed ints          [LINT_ARRAY]   */
    template<std::size_t arity> static constexpr Type UnsignedLongIntArray   { BuiltinType{ NumericType::UnsignedLongInt,   arity } }; /**< array of 64-bit unsigned long ints   [ULINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type RealArray              { BuiltinType{ NumericType::Real,              arity } }; /**< array of 32-bit floating points      [REAL_ARRAY]   */
    template<std::size_t arity> static constexpr Type LongRealArray          { BuiltinType{ NumericType::LongReal,          arity } }; /**< array of 64-bit floating points      [LREAL_ARRAY]  */
    
    template<std::size_t N, std::size_t arity> 
    static constexpr Type StringArray  
    { BuiltinType{ StringType{ N }, arity } }; /**< array of (n) character strings [STRING(n)] */

    template<std::size_t arity> static constexpr Type BIT_ARRAY   { BitArray<arity>              }; /**< array of 1-bit booleans              [BIT_ARRAY]   */
    template<std::size_t arity> static constexpr Type BOOL_ARRAY  { BoolArray<arity>             }; /**< array of 1-bit booleans              [BOOL_ARRAY]  */
    template<std::size_t arity> static constexpr Type BYTE_ARRAY  { ByteArray<arity>             }; /**< array of 8-bit bitsets               [BYTE_ARRAY]  */
    template<std::size_t arity> static constexpr Type WORD_ARRAY  { WordArray<arity>             }; /**< array of 16-bit bitsets              [WORD_ARRAY]  */
    template<std::size_t arity> static constexpr Type DWORD_ARRAY { DoubleWordArray<arity>       }; /**< array of 32-bit bitsets              [DWORD_ARRAY] */
    template<std::size_t arity> static constexpr Type SINT_ARRAY  { ShortIntArray<arity>         }; /**< array of 8-bit signed ints           [SINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type USINT_ARRAY { UnsignedShortIntArray<arity> }; /**< array of 8-bit unsigned short ints   [USINT_ARRAY] */
    template<std::size_t arity> static constexpr Type INT_ARRAY   { IntArray<arity>              }; /**< array of 16-bit signed ints          [INT_ARRAY]   */
    template<std::size_t arity> static constexpr Type UINT_ARRAY  { UnsignedIntArray<arity>      }; /**< array of 16-bit unsigned ints        [UINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type DINT_ARRAY  { DoubleIntArray<arity>        }; /**< array of 32-bit signed ints          [DINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type UDINT_ARRAY { UnsignedDoubleIntArray<arity>}; /**< array of 32-bit unsigned double ints [UDINT_ARRAY] */
    template<std::size_t arity> static constexpr Type LINT_ARRAY  { LongIntArray<arity>          }; /**< array of 64-bit signed ints          [LINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type ULINT_ARRAY { UnsignedLongIntArray<arity>  }; /**< array of 64-bit unsigned long ints   [ULINT_ARRAY] */
    template<std::size_t arity> static constexpr Type REAL_ARRAY  { RealArray<arity>             }; /**< array of 32-bit floating points      [REAL_ARRAY]  */
    template<std::size_t arity> static constexpr Type LREAL_ARRAY { LongRealArray<arity>         }; /**< array of 64-bit floating points      [LREAL_ARRAY] */
    
    template<std::size_t N, std::size_t arity> 
    static constexpr StringType STRING_ARRAY  
    { BuiltinType{ StringType{ N }, arity } }; /**< array of (n) character strings [STRING(n)] */
    
    template<std::size_t arity> static constexpr Type BIT1_ARRAY    { BitArray<arity>               }; /**< array of 1-bit booleans              [BIT_ARRAY]   */
    template<std::size_t arity> static constexpr Type BOOL1_ARRAY   { BoolArray<arity>              }; /**< array of 1-bit booleans              [BOOL_ARRAY]  */
    template<std::size_t arity> static constexpr Type BOOL8_ARRAY   { Bool8Array<arity>             }; /**< array of 8-bit booleans              [BOOL_ARRAY]  */
    template<std::size_t arity> static constexpr Type BYTE8_ARRAY   { ByteArray<arity>              }; /**< array of 8-bit bitsets               [BYTE_ARRAY]  */
    template<std::size_t arity> static constexpr Type WORD16_ARRAY  { WordArray<arity>              }; /**< array of 16-bit bitsets              [WORD_ARRAY]  */
    template<std::size_t arity> static constexpr Type DWORD32_ARRAY { DoubleWordArray<arity>        }; /**< array of 32-bit bitsets              [DWORD_ARRAY] */
    template<std::size_t arity> static constexpr Type SINT8_ARRAY   { ShortIntArray<arity>          }; /**< array of 8-bit signed ints           [SINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type USINT8_ARRAY  { UnsignedShortIntArray<arity>  }; /**< array of 8-bit unsigned short ints   [USINT_ARRAY] */
    template<std::size_t arity> static constexpr Type INT16_ARRAY   { IntArray<arity>               }; /**< array of 16-bit signed ints          [INT_ARRAY]   */
    template<std::size_t arity> static constexpr Type UINT16_ARRAY  { UnsignedIntArray<arity>       }; /**< array of 16-bit unsigned ints        [UINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type DINT32_ARRAY  { DoubleIntArray<arity>         }; /**< array of 32-bit signed ints          [DINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type UDINT32_ARRAY { UnsignedDoubleIntArray<arity> }; /**< array of 32-bit unsigned double ints [UDINT_ARRAY] */
    template<std::size_t arity> static constexpr Type LINT64_ARRAY  { LongIntArray<arity>           }; /**< array of 64-bit signed ints          [LINT_ARRAY]  */
    template<std::size_t arity> static constexpr Type ULINT64_ARRAY { UnsignedLongIntArray<arity>   }; /**< array of 64-bit unsigned long ints   [ULINT_ARRAY] */
    template<std::size_t arity> static constexpr Type REAL32_ARRAY  { RealArray<arity>              }; /**< array of 32-bit floating points      [REAL_ARRAY]  */
    template<std::size_t arity> static constexpr Type LREAL64_ARRAY { LongRealArray<arity>          }; /**< array of 64-bit floating points      [LREAL_ARRAY] */
    
    template<std::size_t N, std::size_t arity> 
    static constexpr StringType STRINGn_ARRAY 
    { BuiltinType{ StringType{ N }, arity } }; /**< array of (n) character strings [STRING(n)] */
};

/* ================================================================================================================================ */

} // End namespace ethercat::descriptors::types

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/descriptors/types/type/type.hpp"

/* ================================================================================================================================ */

#endif