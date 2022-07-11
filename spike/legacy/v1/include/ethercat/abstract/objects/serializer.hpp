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

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_SERIALIZER_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_SERIALIZER_H__

/* =========================================================== Includes =========================================================== */

// External includes
#include "range/v3/span.hpp"
// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Helper proxy class providing automated serialization/deserialization of the CoE objects
 */
class Serializer {

public: /* ------------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /**
     * @brief Constructs Serializer referencing given byte image @p data 
     * 
     * @param data 
     *    data buffer managed by the proxy
     */
    inline constexpr Serializer(ranges::span<uint8_t> data);

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /// Remove copy-asignment operator to enforce explicit management of byte buffer
    Serializer(const Serializer& rserializer) = delete;
    Serializer &operator=(const Serializer& rserializer) = delete;
    /// Enable moving to allow emplacing back Serializer in the std::vector
    Serializer(Serializer&& rserializer) = default;
    Serializer &operator=(Serializer&& rserializer) = default;


public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @brief Reads raw bytes as a value of the type currently held by @p value into @p value
     * 
     * @param[inout] value
     *    value to store deserialized bytes in (currentl variant determines target type of the
     *    CoE object)
     * 
     * @throws std::range_error 
     *    if size of the referenced byte buffer is too small to contain CoE object
     *    of type @p type
     */
    inline void read(Value &value) const;

    /**
     * @brief Reads raw bytes as a value of the given @p type
     * 
     * @tparam type
     *    type of the target object
     * @returns 
     *    value of the deserialize object of type @p type
     * 
     * @throws std::range_error 
     *    if size of the referenced byte buffer is too small to contain CoE object
     *    of type @p type
     */
    template<Type type>
    inline ValueType<type> read_as() const;

    /**
     * @brief Serializes @p value of th CoE raw bytes
     * 
     * @param value 
     *    value to be serialized
     * 
     * @throws std::range_error 
     *    if size of the referenced byte buffer is too small to contain CoE object
     *    of type @p type
     */
    inline void write(Value value);

    /**
     * @brief Serializes @p value of th CoE raw bytes as a value of the given @p type
     * 
     * @tparam type
     *    type of the target object
     * @param value 
     *    value to be serialized
     * 
     * @throws std::range_error 
     *    if size of the referenced byte buffer is too small to contain CoE object
     *    of type @p type
     */
    template<Type type>
    inline void write_as(ValueType<type> value);
    
protected: /* ---------------------------------------------- Protected data members ----------------------------------------------- */

    /// Serial image of the object
    ranges::span<uint8_t> data;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/objects/serializer/serializer.hpp"

/* ================================================================================================================================ */

#endif
