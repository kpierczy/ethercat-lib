/* ============================================================================================================================ *//**
 * @file       value_buffer_reference.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:02:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the ValueBufferReference class prividing type-precize access to the @ref ValueBuffer object
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_OBJECTS_VALUE_BUFFER_REFERENCE_H__
#define __ETHERCAT_ABSTRACT_OBJECTS_VALUE_BUFFER_REFERENCE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::objects {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Helper reference class providing type-precise access to the value stored in the @ref ValueBuffer object
 */
template<Type type>
class ValueBufferReference { 
public:
    
    /**
     * @brief Constructs reference to the given @p buffer
     * 
     * @param buffer 
     *    referenced buffer
     */
    ValueBufferReference(ValueBuffer &buffer);

    /// Default copy-construction
    ValueBufferReference(const ValueBufferReference &rreference) = default;
    /// Default move-construction
    ValueBufferReference(ValueBufferReference &&rreference) = default;
    
public:

    /**
     * @brief Reads referenced value
     * 
     * @returns 
     *    current value of the reference
     * 
     * @throws std::bad_variant_access 
     *    if underlying buffer stores value of the type not corresponding to @p type
     */
    inline const ValueType<type> get() const;
    
    /**
     * @brief Writes referenced value
     * 
     * @param value
     *    value to be set
     */
    inline void set(ValueType<type> value);

protected: /* ---------------------------------------------- Protected data members ----------------------------------------------- */

    /// Handler to the referenced buffer
    ValueBuffer &buffer;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::objects

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/objects/value_buffer_reference/value_buffer_reference.hpp"

/* ================================================================================================================================ */

#endif
