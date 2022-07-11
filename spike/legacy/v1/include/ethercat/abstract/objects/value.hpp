/* ============================================================================================================================ *//**
 * @file       value.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:02:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Value class prividing type-safe access interface to the @ref ValueStorage tuple type
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_VALUE_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_VALUE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Class prividing type-safe access interface to the @ref ValueStorage tuple type
 */
class Value {

public: /* ---------------------------------------------------- Public types ------------------------------------------------------ */

    /// Storage type used by the class
    using Storage = ValueStorage;

public: /* ------------------------------------------------ Public static methods ------------------------------------------------- */

    /**
     * @brief Constructs default-valued @ref Value storing type corresponding to the given @p type
     * 
     * @param type 
     *    target type
     * @returns 
     *    default-valued @ref Value storing type corresponding to the given @p type
     */
    static inline Value from_type(Type type);

public: /* ------------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /// Default constructor
    inline constexpr Value() = default;
    
    /// Default copy-asignment constructor
    inline constexpr Value(const Value &rvalue) = default;
    
    /**
     * @brief Copy-asignment constructor from storage
     */
    inline constexpr Value(const ValueStorage &value_storage);

    /**
     * @brief Creates default-initialized Value with the active variant being @p type
     * 
     * @tparam type 
     *    type of the data stored by the object
     */
    inline constexpr Value(Type type);

    /**
     * @brief Creates Value object representing CoE object of type @tparam type with the given 
     *     initial @p value 
     * 
     * @tparam type 
     *    type of the data stored by the object
     * @tparam T 
     *    type of initializing data
     * @param tag 
     *    tag used for deduction of the @p type
     * @param value 
     *    initial value to be set
     */
    template<Type type, typename T>
    inline constexpr Value(InPlaceTypeTag<type> tag, T value = T{});

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /// Default copy-asignment operator
    inline constexpr Value &operator=(const Value &rvalue) = default;

    /**
     * @brief Assigns new C++ @p value to the object
     * 
     * @tparam T 
     *    type of the value to be assigned
     * @param value 
     *    value to be set
     * @returns 
     *    reference to *this
     * 
     * @note Operator follows the same definition guidline as std::variant::operator=(). 
     *    If T does not resolve to an unambiguous index of the variant's alternative,
     *    the operator is not defined
     */
    template<typename T>
    inline constexpr Value &operator=(T value);

public: /* ------------------------------------------------ Public common methods ------------------------------------------------- */

    /**
     * @returns 
     *    type of the current CoE data stored in the underlying variant
     */
    inline constexpr Type get_type() const;

public: /* ------------------------------------------------ Public access methods ------------------------------------------------- */

    /**
     * @brief Accesses storage variant's member of type associated with @p type CoE type
     * 
     * @tparam type 
     *    target CoE type of the storeed data
     * @returns 
     *    reference to the underlying @ref std::variant's member associated with the
     *    @p type
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline constexpr ValueType<type> &as();
    
    /**
     * @brief Accesses storage variant's member of type associated with @p type CoE type (const)
     * 
     * @tparam type 
     *    target CoE type of the storeed data
     * @returns 
     *    const reference to the underlying @ref std::variant's member associated with the
     *    @p type
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline constexpr const ValueType<type> &as() const;

    /**
     * @returns 
     *    reference to the underlying @ref std::variant
     */
    inline constexpr const ValueStorage &get();

    /**
     * @returns 
     *    const reference to the underlying @ref std::variant
     */
    inline constexpr const ValueStorage &get() const;

protected: /* ---------------------------------------------- Protected data members ----------------------------------------------- */

    /// Underlying storage variant
    ValueStorage value;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/objects/value/value.hpp"

/* ================================================================================================================================ */

#endif
