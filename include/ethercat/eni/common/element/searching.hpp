/* ============================================================================================================================ *//**
 * @file       searching.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 1st June 2022 7:28:12 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public inline methods the Element class related to searching element's children
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_SEARCHING_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_SEARCHING_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* =================================================== Public searching methods =================================================== */

Element::size_type Element::count(const key_type &key) const {
    return get_node().count(key);
}


bool Element::has_child(const key_type &child) const {
    return (get_node().find(child) != get_node().not_found());
}


Element Element::get_child(const path_type &path) {
    return wrap_error([&]{ 
        return make_subelement(get_node().get_child(path));
    }, "ethercat::eni::Element::get_child()");
}


const Element Element::get_child(const path_type &path) const {
    return const_cast<Element*>(this)->get_child(path);
}


Element Element::get_child_or(const path_type &path, Element default_element) {
    return make_subelement(get_node().get_child(path, default_element.get_node()));
}


const Element Element::get_child_or(const path_type &path, const Element default_element) const {
    return const_cast<Element*>(this)->get_child_or(path, const_cast<Element&>(default_element));
}


std::optional<Element> Element::get_child_or_empty(const path_type &path) {
    if(auto ret = get_node().get_child_optional(path); ret.has_value())
        return make_subelement(ret.get());
    else
        return std::optional<Element>{};
}


std::optional<const Element> Element::get_child_or_empty(const path_type &path) const {
    return const_cast<Element*>(this)->get_child_or_empty(path);
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
