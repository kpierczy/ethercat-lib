/* ============================================================================================================================ *//**
 * @file       objects.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:53:35 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of primitives and routines representing notion of the CoE (CANopen over EtherCAT) 'object' entity 
 *             from the Dictionary Object (DO) of the EtherCAT slave device.
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <cstdint>
#include <variant>
#include <optional>
// Private includes
#include "ethercat/config.hpp"
#include "ethercat/common/traits.hpp"
#include "ethercat/common/utilities.hpp"

/* ========================================================== Namespaces ========================================================== */

/**
 * @brief Definitions of types and routines modelling and managing abstract Objects used
 *    to controll slave devices on the EtherCAT bus
 */
namespace ethercat::abstract::objects {

/* ========================================================= Enumerations ========================================================= */

/**
 * @brief Enumeration of supported objects data types
 */
enum class Type : std::size_t {
    Bool              = 0,  /**< 8-bit boolean              [BOOL]  */
    Byte              = 1,  /**< 8-bit bitset               [BYTE]  */
    Word              = 2,  /**< 16-bit bitset              [WORD]  */
    DoubleWord        = 3,  /**< 32-bit bitset              [DWORD] */
    ShortInt          = 4,  /**< 8-bit signed int           [SINT]  */
    UnsignedShortInt  = 5,  /**< 8-bit unsigned short int   [USINT] */
    Int               = 6,  /**< 16-bit signed int          [INT]   */
    UnsignedInt       = 7,  /**< 16-bit unsigned int        [UINT]  */
    DoubleInt         = 8,  /**< 32-bit signed int          [DINT]  */
    UnsignedDoubleInt = 9,  /**< 32-bit unsigned double int [UDINT] */
    LongInt           = 10, /**< 64-bit signed int          [LINT]  */
    UnsignedLongInt   = 11, /**< 64-bit unsigned long int   [ULINT] */
    Real              = 12, /**< 32-bit floating point      [REAL]  */
    LongReal          = 13  /**< 64-bit floating point      [LREAL] */
};

/* ========================================================== Exceptions ========================================================== */

/**
 * @brief Module-specific exception type indicating mismatch of requested and actual
 *    object type
 */
class TypeError : public std::runtime_error {

public: /* -------------------------------------------------- Public constructors ------------------------------------------------- */

    /**
     * @brief Constructs TypeError object describing wrong usage of @p requested . 
     *    Prepends resulting message with the given @p context .
     * 
     * @param requested 
     *    requested object type
     * @param context 
     *    message context
     * @param msg
     *    additional message
     */
    inline TypeError(
        Type requested,
        std::string_view context = "",
        std::string_view msg = ""
    );

    /**
     * @brief Constructs TypeError object describing mismatch of @p requested and @p actual 
     *    type. Prepends resulting message with the given @p context .
     * 
     * @param requested 
     *    requested object type
     * @param actual 
     *    actual object type
     * @param context 
     *    message context
     * @param msg
     *    additional message
     */
    inline TypeError(
        Type requested,
        Type actual,
        std::string_view context = "",
        std::string_view msg = ""
    );
    
public: /* ----------------------------------------------------- Public getters --------------------------------------------------- */

    
    /**
     * @returns 
     *    requested type associated with the error
     */
    inline Type get_requested() const;
    
    /**
     * @returns 
     *    actual type associated with the error
     */
    inline std::optional<Type> get_actual() const;

private: /* ----------------------------------------------- Private member variables ---------------------------------------------- */

    /// Erronous requested type
    Type requested;
    /// Actual type
    std::optional<Type> actual;

};

/* ============================================================ Storage =========================================================== */

/**
 * @brief Union type used to store object's value
 * 
 * @note At the moment only basic CoE types are supported. Neither bit-size nor array-like types are
 *    supported. This might be easily implemented in the future by providing C++ types for representing 
 *    those as well as specific implementations of PDI (Process Data Image) raw-bytes-parsing routines
 *    for these typs
 */
using ValueStorage = std::variant<
    bool,     /**< 8-bit boolean              [BOOL]  */
    uint8_t,  /**< 8-bit bitset               [BYTE]  */
    uint16_t, /**< 16-bit bitset              [WORD]  */
    uint32_t, /**< 32-bit bitset              [DWORD] */
    int8_t,   /**< 8-bit signed int           [SINT]  */
    uint8_t,  /**< 8-bit unsigned short int   [USINT] */
    int16_t,  /**< 16-bit signed int          [INT]   */
    uint16_t, /**< 16-bit unsigned int        [UINT]  */
    int32_t,  /**< 32-bit signed int          [DINT]  */
    uint32_t, /**< 32-bit unsigned double int [UDINT] */
    int64_t,  /**< 64-bit signed int          [LINT]  */
    uint64_t, /**< 64-bit unsigned long int   [ULINT] */
    float,    /**< 32-bit floating point      [REAL]  */
    double    /**< 64-bit floating point      [LREAL] */
>;

/**
 * @brief Disambiguation tags that can be passed to the constructors of classes wrapping 
 *    @ref ValueStorage to indicate that the contained object should be constructed in-place
 * 
 * @note Tage is type-compaining counterpart to the std::in_place_type_t
 */
template <Type type> struct InPlaceTypeTag { explicit InPlaceTypeTag() = default; };

/**
 * @brief Disambiguation tags that can be passed to the constructors of classes wrapping 
 *    @ref ValueStorage to indicate that the contained object should be constructed in-place
 * 
 * @note Tage is type-compaining counterpart to the std::in_place_type
 */
template <Type type> inline constexpr InPlaceTypeTag<type> InPlaceType{ };

/**
 * @brief Helper meta-function converting @ref Type enumeration into corresponding data type
 */
template<Type type>
using ValueType = std::variant_alternative_t<utilities::to_underlying(type), ValueStorage>;

/**
 * @brief Helper meta-function converting @ref Type enumeration into index of the corresponding 
 *    @ref Value alternative
 */
template<Type type>
constexpr std::size_t TypeIndex = utilities::to_underlying(type);

/**
 * @brief Helper meta-function converting @tparam I index to correspodning @ref Type enumeration
 */
template<std::size_t I>
constexpr Type IndexType = utilities::to_enum<Type>(I);

/**
 * @brief Helper meta-function converting @ref Type enumeration into corresponding data type
 */
template<Type type>
constexpr auto InPlaceValueIndex = std::in_place_index<TypeIndex<type>>;

/* ======================================================= Getters & setters ====================================================== */

/**
 * @brief Converts runtime object @p type to it's corresponding index
 */
inline constexpr std::size_t type_index(Type type);

/**
 * @brief Converts runtime index to it's corresponding object @p type
 */
inline constexpr Type index_type(std::size_t type);

/**
 * @brief Reads type of the CoE object stored by the @p value_storage
 * 
 * @param value_storage 
 *    reference to the storage to be inspected
 * @returns 
 *    type of the CoE object stored by the @p value_storage
 */
inline constexpr Type get_type(const ValueStorage &value_storage);

/**
 * @brief Helper wrapper around std::get for referencing members of @ref ValueStorage
 *    by CoE object type enumeration
 * 
 * @tparam type 
 *    requested type
 * @param value_storage 
 *    reference to the std::tuple
 * @returns 
 *    reference to the members of @ref ValueStorage by CoE object type enumeration
 * 
 * @throws std::bad_variant_access 
 *    if @p type is not an actual type of the object
 */
template<Type type>
inline constexpr ValueType<type> &get(ValueStorage &value_storage);

/**
 * @brief Helper wrapper around std::get for referencing members of @ref ValueStorage
 *    by CoE object type enumeration (const)
 * 
 * @tparam type 
 *    requested type
 * @param value_storage 
 *    reference to the std::tuple
 * @returns 
 *    reference to the const members of @ref ValueStorage by CoE object type enumeration
 * 
 * @throws std::bad_variant_access 
 *    if @p type is not an actual type of the object
 */
template<Type type>
inline constexpr const ValueType<type> &get(const ValueStorage &value_storage);

/**
 * @brief Enumeration of supported objects data types
 */
inline constexpr ValueStorage make_value_storage(Type type);

/* ============================================================ Helpers =========================================================== */

/**
 * @brief Parses @p type_string string to corresponding data type
 * 
 * @param type_string 
 *    string to be converted
 * 
 * @throws std::runtime_error 
 *    if @p type_string does not represent valid data type
 */
inline Type parse_type(std::string_view type_string);

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

/* ====================================================== Submodules includes ===================================================== */

#include "ethercat/abstract/objects/traits.hpp"
#include "ethercat/abstract/objects/objects.hpp"
#include "ethercat/abstract/objects/value.hpp"
#include "ethercat/abstract/objects/value_buffer.hpp"
#include "ethercat/abstract/objects/value_buffer_reference.hpp"
#include "ethercat/abstract/objects/serializer.hpp"
#include "ethercat/abstract/objects/object.hpp"
#include "ethercat/abstract/objects/register_descriptor.hpp"

/* ================================================================================================================================ */

#endif
