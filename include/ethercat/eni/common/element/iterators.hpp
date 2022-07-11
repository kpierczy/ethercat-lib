/* ============================================================================================================================ *//**
 * @file       iterators.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public iterator types of the Element class
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ITEARTORS_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ITEARTORS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* =========================================================== Namespace ========================================================== */

namespace ethercat::eni {

/* =========================================================== Iterators ========================================================== */

class Element::iterator : 
    public details::element_iterator<Element, property_tree_type::iterator> 
{ 
    using details::element_iterator<Element, property_tree_type::iterator>::element_iterator;
};


class Element::const_iterator : 
    public details::const_element_iterator<Element, property_tree_type::const_iterator> 
{ 
    using details::const_element_iterator<Element, property_tree_type::const_iterator>::const_element_iterator;
};


class Element::reverse_iterator : 
    public details::element_iterator<Element, property_tree_type::reverse_iterator> 
{ 
    using details::element_iterator<Element, property_tree_type::reverse_iterator>::element_iterator;
};


class Element::const_reverse_iterator : 
    public details::const_element_iterator<Element, property_tree_type::const_reverse_iterator> 
{ 
    using details::const_element_iterator<Element, property_tree_type::const_reverse_iterator>::const_element_iterator;
};

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
