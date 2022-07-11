/* ============================================================================================================================ *//**
 * @file       serializer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 10:54:00 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the helper proxy class providing automated serialization/deserialization of the CoE objects
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_OBJECT_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_OBJECT_H__

/* =========================================================== Includes =========================================================== */

// External includes
#include "range/v3/span.hpp"
// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Class representing CoE object in the OD (Object Dictionary) of the EtherCAT slave.
 *    Provides mechanisms for bufferred, thread-safe access to the stored value as well
 *    as it's serializing/deserializing to the byte image
 */
class Object {

public: /* ------------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /**
     * @brief Constructs CoE Object of the given @p type with the given serialization 
     *    @p data buffer
     * 
     * @param type 
     *    type of the object
     * @param data 
     *    reference to the buffer used to serialize/deserialize the object
     * 
     * @throws std::range_error 
     *    if size of the referenced byte buffer is too small to contain CoE object
     *    of type @p type
     */
    inline Object(Type type, ranges::span<uint8_t> data);

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    // Disable copying
    Object(const Object& robject) = delete;
    Object &operator=(const Object& robject) = delete;
    // Enable moving to allow emplacing back Objects in the std::vector
    Object(Object&& robject) = default;
    Object &operator=(Object&& robject) = default;

public: /* ------------------------------------------------- Public common methods ------------------------------------------------ */

    /**
     * @returns 
     *    type of the object
     */
    inline Type get_type() const;

public: /* ---------------------------------------------- Public data access methods ---------------------------------------------- */

    /**
     * @brief Serializes current value of the object to the associated buffr
     */
    inline void serialize();
    
    /**
     * @brief Deserializes current value of the object from the associated buffr
     */
    inline void deserialize();

    /**
     * @brief Sets current value of the object to the @p value and serializes object
     *     to the associated buffer
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline void write_as(ValueType<type> value);
    
    /**
     * @brief Sets current value of the object to the @p value and serializes object
     *     to the associated buffer
     */
    inline void write(Value value);

    /**
     * @brief Deserializes current value of the object from the associated buffr and returns it
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline ValueType<type> read_as();
    
    /**
     * @brief Deserializes current value of the object from the associated buffr and returns it
     inline */
    inline Value read();
    
public: /* --------------------------------------------- Public buffer access methods --------------------------------------------- */

    /**
     * @brief Reads current value of the object
     * 
     * @returns 
     *    current value of the object
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline ValueType<type> get_as() const;

    /**
     * @brief Reads current value of the object
     * 
     * @returns 
     *    current value of the object
     */
    inline const Value get() const;

    /**
     * @brief Sets current value of the object to @p value
     * 
     * @tparam type 
     *    object type corresponding to the @p value type
     * @param value 
     *    value to be set
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline void set_as(ValueType<type> value);

    /**
     * @brief Sets current value of the object to @p value
     * 
     * @param value 
     *    value to be set
     */
    inline void set(Value value);

    /**
     * @returns 
     *    Returns reference to the buffer
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    inline ValueBuffer &get_reference();

    /**
     * @returns 
     *    Returns reference to the buffer
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    inline const ValueBuffer &get_reference() const;

    /**
     * @tparam type
     *    target CoE type for the reference
     * @returns 
     *    reference to the buffer
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline ValueBufferReference<type> get_reference();

    /**
     * @tparam type
     *    target CoE type for the reference
     * @returns 
     *    const reference to the buffer
     * 
     * @throws TypeError 
     *    if @p type does noe match actual type
     */
    template<Type type>
    inline const ValueBufferReference<type> get_reference() const;
    
protected: /* ---------------------------------------------- Protected data members ----------------------------------------------- */
    
    /// Raw-data serializer/deserializer
    Serializer serializer;
    /// Synchronized value buffer
    ValueBuffer buffer;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/objects/object/object.hpp"

/* ================================================================================================================================ */

#endif
