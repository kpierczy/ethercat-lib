/* ============================================================================================================================ *//**
 * @file       value_buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:02:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the ValueBuffer class prividing thread-safe access to the @ref Value object
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_VALUE_BUFFER_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_VALUE_BUFFER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/config.hpp"
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Class prividing thread-safe access to the @ref Value object
 */
class ValueBuffer {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Lock type used to synchronsie the buffer
    using LockType = config::QuickLock;

public: /* ------------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /// Default constructor
    inline ValueBuffer() = default;

    /**
     * @brief Default value constructor (by variant storage)
     * 
     * @param value 
     *    initial value to be set
     */
    inline ValueBuffer(Value value = Value{ });

    /**
     * @brief Default value constructor (by data type)
     * 
     * @tparam type 
     *    type of the CoE data stored by the object
     * @param value 
     *    initial value to be set
     */
    template<Type type, typename T>
    inline ValueBuffer(InPlaceTypeTag<type> tag, T value = T{});

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /// Disable copying
    ValueBuffer(const ValueBuffer& robject) = delete;
    ValueBuffer &operator=(const ValueBuffer& robject) = delete;
    /// Enable moving to allow emplacing back ValueBuffer in the std::vector
    ValueBuffer(ValueBuffer&& robject) = default;
    ValueBuffer &operator=(ValueBuffer&& robject) = default;

public: /* ------------------------------------------------ Public common methods ------------------------------------------------- */

    /**
     * @returns 
     *    type of the current CoE data stored in the underlying variant
     */
    inline constexpr Type get_type() const;

public: /* ----------------------------------------------- Public getters & setters ----------------------------------------------- */

    /**
     * @brief Reads buffer content as a value of type associated with @p type
     * 
     * @tparam type 
     *    type of the CoE data stored by the object
     * @returns 
     *    copy of the current value of the variant
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline ValueType<type> get_as() const;

    /**
     * @brief Reads buffer content 
     * @returns 
     *    copy of the current value
     */
    inline const Value get() const;

    /**
     * @brief Writes buffer content to a @p value of type associated with @p type
     * 
     * @tparam type 
     *    type of the CoE data stored by the object
     * @param value 
     *    value to be set
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline void set_as(ValueType<type> value);
    
    /**
     * @brief Writes buffer content
     * 
     * @param value 
     *    value to be written
     */
    inline void set(Value value);
    
protected: /* ---------------------------------------------- Protected data members ----------------------------------------------- */

    /// Buffer for object accesses
    Value value;
    /// Synchronisation lock for the buffer
    mutable LockType lock;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/objects/value_buffer/value_buffer.hpp"

/* ================================================================================================================================ */

#endif
