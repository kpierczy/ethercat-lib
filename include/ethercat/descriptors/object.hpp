/* ============================================================================================================================ *//**
 * @file       object.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:10:31 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Object type providigng compile-time description of the CoE object
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DESCRIPTORS_OBJECT_H__
#define __ETHERCAT_DESCRIPTORS_OBJECT_H__

/* =========================================================== Includes =========================================================== */

#include "ethercat/descriptors/types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::descriptors {

/* ============================================================ Object ============================================================ */

/**
 * @brief Class providigng compile-time description of the CoE object
 * 
 * @tparam SubitemsN 
 *    number of subitems
 */
template<std::size_t SubitemsN = 0>
struct Object {

public: /* ------------------------------------------------------ Public data ----------------------------------------------------- */

    /// Index of the object in the Objects Dictionary
    uint16_t index;
    /// Name of the object
    std::string_view name;
    /// Type of the object
    types::Type<SubitemsN> type;

};

/* ================================================================================================================================ */

} // End namespace ethercat::descriptors

/* ================================================================================================================================ */

#endif
