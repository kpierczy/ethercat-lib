/* ============================================================================================================================ *//**
 * @file       stl.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public inline STL-like methods the Element class
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_STL_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_STL_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ==================================================== Protected ctors & dtors =================================================== */

Element::size_type Element::size() const {
    return get_node().size();
}


bool Element::empty() const {
    return get_node().empty();
}


Element::iterator Element::begin() {
    return iterator(root, get_node().begin());
}


Element::const_iterator Element::begin() const {
    return const_iterator(root, get_node().begin());
}


Element::iterator Element::end() {
    return iterator(root, get_node().end());
}


Element::const_iterator Element::end() const {
    return const_iterator(root, get_node().end());
}


Element::reverse_iterator Element::rbegin() {
    return reverse_iterator(root, get_node().rbegin());
}


Element::const_reverse_iterator Element::rbegin() const {
    return const_reverse_iterator(root, get_node().rbegin());
}


Element::reverse_iterator Element::rend() {
    return reverse_iterator(root, get_node().rend());
}


Element::const_reverse_iterator Element::rend() const {
    return const_reverse_iterator(root, get_node().rend());
}


Element::value_type Element::front() {
    auto front = get_node().front();
    return value_type{ front.first, make_subelement(front.second) };
}


const Element::value_type Element::front() const {
    auto front = get_node().front();
    return value_type{ front.first, make_subelement(front.second) };
}


Element::value_type Element::back() {
    auto back = get_node().back();
    return value_type{ back.first, make_subelement(back.second) };
}


const Element::value_type Element::back() const {
    auto back = get_node().back();
    return value_type{ back.first, make_subelement(back.second) };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
