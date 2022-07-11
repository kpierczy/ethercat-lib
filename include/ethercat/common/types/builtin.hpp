/* ============================================================================================================================ *//**
 * @file       builtin.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of common base types describing builtin EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_BUILTIN_H__
#define __ETHERCAT_COMMON_TYPES_BUILTIN_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <cstdint>
#include <variant>
#include <optional>
#include <string_view>
// Private includes
#include "ethercat/common/utilities/enum.hpp"
#include "ethercat/common/types/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ========================================================= Numeric type ========================================================= */

/**
 * @brief Base type for types describing builtin numeric type of EtherCAT (CoE) objects
 */
struct NumericTypeBase {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Enumeration of basic EtherCAT numeric builtin data types
     */
    enum class ID : IdType {

        Bit               = 0,  /**< 1-bit boolean              [BIT]    */
        Bool              = 0,  /**< 1-bit boolean              [BOOL]   */
        Bool8             = 1,  /**< 8-bit boolean              [BOOL8]  */
        Byte              = 2,  /**< 8-bit bitset               [BYTE]   */
        Word              = 3,  /**< 16-bit bitset              [WORD]   */
        DoubleWord        = 4,  /**< 32-bit bitset              [DWORD]  */
        ShortInt          = 5,  /**< 8-bit signed int           [SINT]   */
        UnsignedShortInt  = 6,  /**< 8-bit unsigned short int   [USINT]  */
        Int               = 7,  /**< 16-bit signed int          [INT]    */
        UnsignedInt       = 8,  /**< 16-bit unsigned int        [UINT]   */
        DoubleInt         = 9,  /**< 32-bit signed int          [DINT]   */
        UnsignedDoubleInt = 10,  /**< 32-bit unsigned double int [UDINT] */
        LongInt           = 11, /**< 64-bit signed int          [LINT]   */
        UnsignedLongInt   = 12, /**< 64-bit unsigned long int   [ULINT]  */
        Real              = 13, /**< 32-bit floating point      [REAL]   */
        LongReal          = 14, /**< 64-bit floating point      [LREAL]  */

        BIT    = Bit,               /**< 1-bit boolean              [BIT]   */
        BOOL   = Bool,              /**< 1-bit boolean              [BOOL]  */
        BYTE   = Byte,              /**< 8-bit bitset               [BYTE]  */
        WORD   = Word,              /**< 16-bit bitset              [WORD]  */
        DWORD  = DoubleWord,        /**< 32-bit bitset              [DWORD] */
        SINT   = ShortInt,          /**< 8-bit signed int           [SINT]  */
        USINT  = UnsignedShortInt,  /**< 8-bit unsigned short int   [USINT] */
        INT    = Int,               /**< 16-bit signed int          [INT]   */
        UINT   = UnsignedInt,       /**< 16-bit unsigned int        [UINT]  */
        DINT   = DoubleInt,         /**< 32-bit signed int          [DINT]  */
        UDINT  = UnsignedDoubleInt, /**< 32-bit unsigned double int [UDINT] */
        LINT   = LongInt,           /**< 64-bit signed int          [LINT]  */
        ULINT  = UnsignedLongInt,   /**< 64-bit unsigned long int   [ULINT] */
        REAL   = Real,              /**< 32-bit floating point      [REAL]  */
        LREAL  = LongReal,          /**< 64-bit floating point      [LREAL] */

        BIT1    = BIT,    /**< 1-bit boolean              [BIT]   */
        BOOL1   = BOOL,   /**< 1-bit boolean              [BOOL]  */
        BOOL8   = Bool8,  /**< 8-bit boolean              [BOOL]  */
        BYTE8   = BYTE,   /**< 8-bit bitset               [BYTE]  */
        WORD16  = WORD,   /**< 16-bit bitset              [WORD]  */
        DWORD32 = DWORD,  /**< 32-bit bitset              [DWORD] */
        SINT8   = SINT,   /**< 8-bit signed int           [SINT]  */
        USINT8  = USINT,  /**< 8-bit unsigned short int   [USINT] */
        INT16   = INT,    /**< 16-bit signed int          [INT]   */
        UINT16  = UINT,   /**< 16-bit unsigned int        [UINT]  */
        DINT32  = DINT,   /**< 32-bit signed int          [DINT]  */
        UDINT32 = UDINT,  /**< 32-bit unsigned double int [UDINT] */
        LINT64  = LINT,   /**< 64-bit signed int          [LINT]  */
        ULINT64 = ULINT,  /**< 64-bit unsigned long int   [ULINT] */
        REAL32  = REAL,   /**< 32-bit floating point      [REAL]  */
        LREAL64 = LREAL,  /**< 64-bit floating point      [LREAL] */

    };

public: /* ---------------------------------------------------- Public constants -------------------------------------------------- */

    /// Number of supported basic data types
    static constexpr std::size_t TYPES_NUM = 15;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */
    
    /**
     * @returns 
     *    name of the described type
     */
    constexpr std::string_view get_name() const;
    
    /**
     * @returns 
     *    CoE name of the described type
     */
    constexpr std::string_view get_coe_name() const;
    
    /**
     * @returns 
     *    bitesize of the described numeric type
     */
    constexpr std::size_t get_bitsize() const;
    
    /**
     * @returns 
     *    bytesize (rounded down) of the described numeric type
     */
    constexpr std::size_t get_bytesize() const;

public: /* ----------------------------------------------------- Public members --------------------------------------------------- */

    /// ID of the type
    ID id;
    
};

/* ======================================================== StringTypeBase ======================================================== */

/**
 * @brief Base type for types describing builtin string type of EtherCAT (CoE) objects
 */
struct StringTypeBase {

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */
    
    /**
     * @returns 
     *    bitesize of the described numeric type
     */
    constexpr std::size_t get_bitsize() const;
    
    /**
     * @returns 
     *    bytesize (rounded down) of the described numeric type
     */
    constexpr std::size_t get_bytesize() const;

public: /* ----------------------------------------------------- Public members --------------------------------------------------- */

    /// Size of the string (number of characters)
    std::size_t size;
    
};

/* ======================================================== BuiltinTypeBase ======================================================= */

/**
 * @brief Base type for types describing builtin string type of EtherCAT (CoE) objects
 */
template<typename StringT, typename NumericTypeBaseT, typename StringTypeBaseT>
class BuiltinTypeBase {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Enumeration of basic EtherCAT builtin data types
     */
    enum class ID : IdType {

        Bit               = utilities::to_underlying(NumericTypeBaseT::ID::Bit),               /**< 1-bit boolean              [BIT]       */
        Bool              = utilities::to_underlying(NumericTypeBaseT::ID::Bool),              /**< 1-bit boolean              [BOOL]      */
        Bool8             = utilities::to_underlying(NumericTypeBaseT::ID::Bool8),             /**< 8-bit boolean              [BOOL8]     */
        Byte              = utilities::to_underlying(NumericTypeBaseT::ID::Byte),              /**< 8-bit bitset               [BYTE]      */
        Word              = utilities::to_underlying(NumericTypeBaseT::ID::Word),              /**< 16-bit bitset              [WORD]      */
        DoubleWord        = utilities::to_underlying(NumericTypeBaseT::ID::DoubleWord),        /**< 32-bit bitset              [DWORD]     */
        ShortInt          = utilities::to_underlying(NumericTypeBaseT::ID::ShortInt),          /**< 8-bit signed int           [SINT]      */
        UnsignedShortInt  = utilities::to_underlying(NumericTypeBaseT::ID::UnsignedShortInt),  /**< 8-bit unsigned short int   [USINT]     */
        Int               = utilities::to_underlying(NumericTypeBaseT::ID::Int),               /**< 16-bit signed int          [INT]       */
        UnsignedInt       = utilities::to_underlying(NumericTypeBaseT::ID::UnsignedInt),       /**< 16-bit unsigned int        [UINT]      */
        DoubleInt         = utilities::to_underlying(NumericTypeBaseT::ID::DoubleInt),         /**< 32-bit signed int          [DINT]      */
        UnsignedDoubleInt = utilities::to_underlying(NumericTypeBaseT::ID::UnsignedDoubleInt), /**< 32-bit unsigned double int [UDINT]     */
        LongInt           = utilities::to_underlying(NumericTypeBaseT::ID::LongInt),           /**< 64-bit signed int          [LINT]      */
        UnsignedLongInt   = utilities::to_underlying(NumericTypeBaseT::ID::UnsignedLongInt),   /**< 64-bit unsigned long int   [ULINT]     */
        Real              = utilities::to_underlying(NumericTypeBaseT::ID::Real),              /**< 32-bit floating point      [REAL]      */
        LongReal          = utilities::to_underlying(NumericTypeBaseT::ID::LongReal),          /**< 64-bit floating point      [LREAL]     */
        String            = utilities::to_underlying(NumericTypeBaseT::ID::LongReal) + 1,      /**< (n) character string       [STRING(n)] */

        BIT    = Bit,               /**< 1-bit boolean              [BIT]       */
        BOOL   = Bool,              /**< 1-bit boolean              [BOOL]      */
        BYTE   = Byte,              /**< 8-bit bitset               [BYTE]      */
        WORD   = Word,              /**< 16-bit bitset              [WORD]      */
        DWORD  = DoubleWord,        /**< 32-bit bitset              [DWORD]     */
        SINT   = ShortInt,          /**< 8-bit signed int           [SINT]      */
        USINT  = UnsignedShortInt,  /**< 8-bit unsigned short int   [USINT]     */
        INT    = Int,               /**< 16-bit signed int          [INT]       */
        UINT   = UnsignedInt,       /**< 16-bit unsigned int        [UINT]      */
        DINT   = DoubleInt,         /**< 32-bit signed int          [DINT]      */
        UDINT  = UnsignedDoubleInt, /**< 32-bit unsigned double int [UDINT]     */
        LINT   = LongInt,           /**< 64-bit signed int          [LINT]      */
        ULINT  = UnsignedLongInt,   /**< 64-bit unsigned long int   [ULINT]     */
        REAL   = Real,              /**< 32-bit floating point      [REAL]      */
        LREAL  = LongReal,          /**< 64-bit floating point      [LREAL]     */
        STRING = String,            /**< (n) character string       [STRING(n)] */

        BIT1    = BIT,    /**< 1-bit boolean              [BIT]       */
        BOOL1   = BOOL,   /**< 1-bit boolean              [BOOL]      */
        BOOL8   = Bool8,  /**< 8-bit boolean              [BOOL]      */
        BYTE8   = BYTE,   /**< 8-bit bitset               [BYTE]      */
        WORD16  = WORD,   /**< 16-bit bitset              [WORD]      */
        DWORD32 = DWORD,  /**< 32-bit bitset              [DWORD]     */
        SINT8   = SINT,   /**< 8-bit signed int           [SINT]      */
        USINT8  = USINT,  /**< 8-bit unsigned short int   [USINT]     */
        INT16   = INT,    /**< 16-bit signed int          [INT]       */
        UINT16  = UINT,   /**< 16-bit unsigned int        [UINT]      */
        DINT32  = DINT,   /**< 32-bit signed int          [DINT]      */
        UDINT32 = UDINT,  /**< 32-bit unsigned double int [UDINT]     */
        LINT64  = LINT,   /**< 64-bit signed int          [LINT]      */
        ULINT64 = ULINT,  /**< 64-bit unsigned long int   [ULINT]     */
        REAL32  = REAL,   /**< 32-bit floating point      [REAL]      */
        LREAL64 = LREAL,  /**< 64-bit floating point      [LREAL]     */
        STRINGn = String, /**< (n) character string       [STRING(n)] */

    };

public: /* ---------------------------------------------------- Public constants -------------------------------------------------- */

    /// Number of supported basic data types
    static constexpr std::size_t TYPES_NUM = NumericTypeBaseT::TYPES_NUM + 1;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Constructs BuiltinTypeBase describing (potentially array) numeric type
     */
    constexpr BuiltinTypeBase(NumericTypeBaseT numeric, std::size_t arity = 0);
    
    /**
     * @brief Constructs BuiltinTypeBase describing (potentially array) string type with 
     *    custom name
     */
    constexpr BuiltinTypeBase(std::string_view name, NumericTypeBaseT numeric, std::size_t arity = 0);
    
    /**
     * @brief Constructs BuiltinTypeBase describing (potentially array) numeric type
     */
    constexpr BuiltinTypeBase(StringTypeBaseT str, std::size_t arity = 0);
    
    /**
     * @brief Constructs BuiltinTypeBase describing (potentially array) string type with 
     *    custom name
     */
    constexpr BuiltinTypeBase(std::string_view name, StringTypeBaseT str, std::size_t arity = 0);

public: /* ---------------------------------------------------- Public operators -------------------------------------------------- */

    /**
     * @retval true 
     *    if both objects represent the same type
     * @retval false 
     *    otherwise
     */
    inline bool operator==(const BuiltinTypeBase &rtype) const;

    /**
     * @retval true 
     *    if objects represent different types
     * @retval false 
     *    otherwise
     */
    inline bool operator!=(const BuiltinTypeBase &rtype) const;

public: /* ------------------------------------------------ Public traits methods ------------------------------------------------- */
    
    /**
     * @retval true
     *    if type is numeric
     * @retval false
     *    otherwise
     */
    constexpr bool is_numeric() const;
    
    /**
     * @retval true
     *    if type is string
     * @retval false
     *    otherwise
     */
    constexpr bool is_string() const;
    
    /**
     * @retval true
     *    if type is scalar
     * @retval false
     *    otherwise
     */
    constexpr bool is_scalar() const;
    
    /**
     * @retval true
     *    if type is array
     * @retval false
     *    otherwise
     */
    constexpr bool is_array() const;

public: /* ---------------------------------------------------- Public getters ---------------------------------------------------- */
    
    /**
     * @returns 
     *    ID of the described type
     */
    constexpr ID get_id() const;
    
    /**
     * @returns 
     *    reference of the description of the numeric base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe numeric type
     */
    constexpr NumericTypeBaseT& get_numeric();
    
    /**
     * @returns 
     *    const reference of the description of the numeric base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe numeric type
     */
    constexpr const NumericTypeBaseT& get_numeric() const;
    
    /**
     * @returns 
     *    reference of the description of the string base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe string type
     */
    constexpr StringTypeBaseT& get_string();
    
    /**
     * @returns 
     *    const reference of the description of the string base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe string type
     */
    constexpr const StringTypeBaseT& get_string() const;

public: /* ------------------------------------------------ Public helper methods ------------------------------------------------- */
    
    /**
     * @returns 
     *    name of the described type
     */
    constexpr std::string_view get_name() const;
    
    /**
     * @returns 
     *    CoE name of the described type
     */
    constexpr std::string_view get_coe_name() const;
    
    /**
     * @returns 
     *    bitesize of the described builtin type
     */
    constexpr std::size_t get_bitsize() const;
    
    /**
     * @returns 
     *    bytesize (rounded down) of the described builtin type
     */
    constexpr std::size_t get_bytesize() const;

public: /* ------------------------------------------------------ Public data ----------------------------------------------------- */

    /// (Optional) Custom name of the type
    std::optional<StringT> name;
    /// Arity of the type (0 for scalar types)
    std::size_t arity { 0 };

private: /* ---------------------------------------------------- Private data ----------------------------------------------------- */

    /// Actual base type
    std::variant<NumericTypeBaseT, StringTypeBaseT> base;

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
