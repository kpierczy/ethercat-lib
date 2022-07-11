/* ============================================================================================================================ *//**
 * @file       management.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of inline public methods of the Element class related to general management
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_MANAGEMENT_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_MANAGEMENT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* =================================================== Public management methods ================================================== */

void Element::autonomize() {

    // Create copy of the currently referenced element
    auto new_root = std::make_shared<property_tree_type>(*node);
    // Store reference to the created obejct as a new root
    root = new_root;
    // Store reference to the root object as node reference
    node = root.get();
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
