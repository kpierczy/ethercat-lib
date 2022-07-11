/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of common base types describing arbitrary EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* ================================================================================================================================ */

#ifndef __ETHERCAT_COMMON_TYPES_TYPE_H__
#define __ETHERCAT_COMMON_TYPES_TYPE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/config.hpp"
#include "ethercat/common/utilities/enum.hpp"
#include "ethercat/common/types/builtin.hpp"
#include "ethercat/common/types/common.hpp"
#include "ethercat/common/types/structural.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ======================================================== Structural type ======================================================= */

/**
 * @brief Base type for types describing arbitrary type of EtherCAT (CoE) objects
 */
template<typename BuiltinTypeBaseT, typename StructuralTypeBaseT>
class TypeBase {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Enumeration of basic EtherCAT builtin data types
     */
    enum class ID : IdType {

        Bit               = utilities::to_underlying(BuiltinTypeBaseT::ID::Bit),               /**< 1-bit boolean              [BIT]       */
        Bool              = utilities::to_underlying(BuiltinTypeBaseT::ID::Bool),              /**< 1-bit boolean              [BOOL]      */
        Bool8             = utilities::to_underlying(BuiltinTypeBaseT::ID::Bool8),             /**< 8-bit boolean              [BOOL8]     */
        Byte              = utilities::to_underlying(BuiltinTypeBaseT::ID::Byte),              /**< 8-bit bitset               [BYTE]      */
        Word              = utilities::to_underlying(BuiltinTypeBaseT::ID::Word),              /**< 16-bit bitset              [WORD]      */
        DoubleWord        = utilities::to_underlying(BuiltinTypeBaseT::ID::DoubleWord),        /**< 32-bit bitset              [DWORD]     */
        ShortInt          = utilities::to_underlying(BuiltinTypeBaseT::ID::ShortInt),          /**< 8-bit signed int           [SINT]      */
        UnsignedShortInt  = utilities::to_underlying(BuiltinTypeBaseT::ID::UnsignedShortInt),  /**< 8-bit unsigned short int   [USINT]     */
        Int               = utilities::to_underlying(BuiltinTypeBaseT::ID::Int),               /**< 16-bit signed int          [INT]       */
        UnsignedInt       = utilities::to_underlying(BuiltinTypeBaseT::ID::UnsignedInt),       /**< 16-bit unsigned int        [UINT]      */
        DoubleInt         = utilities::to_underlying(BuiltinTypeBaseT::ID::DoubleInt),         /**< 32-bit signed int          [DINT]      */
        UnsignedDoubleInt = utilities::to_underlying(BuiltinTypeBaseT::ID::UnsignedDoubleInt), /**< 32-bit unsigned double int [UDINT]     */
        LongInt           = utilities::to_underlying(BuiltinTypeBaseT::ID::LongInt),           /**< 64-bit signed int          [LINT]      */
        UnsignedLongInt   = utilities::to_underlying(BuiltinTypeBaseT::ID::UnsignedLongInt),   /**< 64-bit unsigned long int   [ULINT]     */
        Real              = utilities::to_underlying(BuiltinTypeBaseT::ID::Real),              /**< 32-bit floating point      [REAL]      */
        LongReal          = utilities::to_underlying(BuiltinTypeBaseT::ID::LongReal),          /**< 64-bit floating point      [LREAL]     */
        String            = utilities::to_underlying(BuiltinTypeBaseT::ID::String),            /**< (n) character string       [STRING(n)] */
        Structural        = utilities::to_underlying(BuiltinTypeBaseT::ID::String) + 1,        /**< Structural type                        */

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
        STRUCT = Structural,        /**< Structural type                        */

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
    static constexpr std::size_t TYPES_NUM = BuiltinTypeBaseT::TYPES_NUM + 1;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Constructs TypeBase describing a builtin type
     */
    constexpr TypeBase(const BuiltinTypeBaseT &builtin);
    
    /**
     * @brief Constructs TypeBase describing a structural type
     */
    constexpr TypeBase(const StructuralTypeBaseT &structural);

public: /* ---------------------------------------------------- Public operators -------------------------------------------------- */

    /**
     * @retval true 
     *    if both objects represent the same type
     * @retval false 
     *    otherwise
     */
    inline bool operator==(const TypeBase &rtype) const;

    /**
     * @retval true 
     *    if objects represent different types
     * @retval false 
     *    otherwise
     */
    inline bool operator!=(const TypeBase &rtype) const;
    
public: /* ------------------------------------------------ Public traits methods ------------------------------------------------- */

    /**
     * @retval true
     *    if objects described builtin type
     * @retval false
     *    otherwise
     */
    constexpr bool is_builtin() const;

    /**
     * @retval true
     *    if objects described structural type
     * @retval false
     *    otherwise
     */
    constexpr bool is_structural() const;

public: /* ---------------------------------------------------- Public getters ---------------------------------------------------- */
    
    /**
     * @returns 
     *    ID of the described type
     */
    constexpr ID get_id() const;
    
    /**
     * @returns 
     *    reference of the description of the builtin base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe builtin type
     */
    constexpr BuiltinTypeBaseT& get_builtin();
    
    /**
     * @returns 
     *    const reference of the description of the builtin base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe builtin type
     */
    constexpr const BuiltinTypeBaseT& get_builtin() const;
    
    /**
     * @returns 
     *    reference of the description of the structural base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe structural type
     */
    constexpr StructuralTypeBaseT& get_structural();
    
    /**
     * @returns 
     *    const reference of the description of the structural base of the type
     * 
     * @throws std::bad_variant_access 
     *    if object does not describe structural type
     */
    constexpr const StructuralTypeBaseT& get_structural() const;

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
     *    bitesize of the described numeric type
     */
    constexpr std::size_t get_bitsize() const;
    
    /**
     * @returns 
     *    bytesize (rounded down) of the described numeric type
     */
    constexpr std::size_t get_bytesize() const;

private: /* ---------------------------------------------------- Private data ----------------------------------------------------- */

    /// Actual type descriptor
    std::variant<BuiltinTypeBaseT, StructuralTypeBaseT> type;

};

/* ============================================================ Helpers =========================================================== */

/**
 * @brief Basic enumeration of CoE types IDs
 */
using ID = typename TypeBase<
    BuiltinTypeBase<std::string, NumericTypeBase, StringTypeBase>,
    StructuralTypeBase<std::vector, std::string, BuiltinTypeBase<std::string, NumericTypeBase, StringTypeBase>>
>::ID;

/**
 * @brief Enumber of basic enumerations of CoE types IDs
 */
constexpr auto TYPES_NUM = TypeBase<
    BuiltinTypeBase<std::string, NumericTypeBase, StringTypeBase>,
    StructuralTypeBase<std::vector, std::string, BuiltinTypeBase<std::string, NumericTypeBase, StringTypeBase>>
>::TYPES_NUM;

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
