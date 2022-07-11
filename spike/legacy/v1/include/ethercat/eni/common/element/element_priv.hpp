/* ============================================================================================================================ *//**
 * @file       element_priv.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of inline private methods the Element class
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ELEMENT_PRIV_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ELEMENT_PRIV_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ===================================================== Private ctors & dtors ==================================================== */

Element::Element(std::shared_ptr<property_tree_type> root) :
    root{ root },
    node{ root.get() }
{ }


Element::Element(std::shared_ptr<property_tree_type> root, property_tree_type &node) :
    root{ root },
    node{ &node }
{ }

/* ======================================================== Private methods ======================================================= */

Element::property_tree_type &Element::get_node() {
    return *node;
}


const Element::property_tree_type &Element::get_node() const {
    return const_cast<const property_tree_type &>(*node);
}


Element Element::make_subelement(property_tree_type &subnode) const {
    return Element(root, subnode);
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
