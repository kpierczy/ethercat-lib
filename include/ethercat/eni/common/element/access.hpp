/* ============================================================================================================================ *//**
 * @file       access.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 1st June 2022 7:28:12 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public inline methods the Element class related to accessing element's values
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ACCESS_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ACCESS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* =================================================== Public searching methods =================================================== */

template<class Type>
Type Element::get_value() const {
    return wrap_error([&]{ 
        return get_node().get_value<Type>();
    }, "ethercat::eni::Element::get_value()");
}


template<class Type>
Type Element::get_value_or(Type default_value) const {
    return get_node().get_value<Type>(default_value);
}


template<class Type>
std::optional<Type> Element::get_value_or_empty() const {
    if(auto ret = get_node().get_value_optional<Type>(); ret.has_value())
        return ret.get();
    else
        return std::optional<Type>{};
}


template<class Type>
Type Element::get_child_value(const path_type &path) const {
    return wrap_error([&]{ 
        return get_node().get<Type>(path);
    }, "ethercat::eni::Element::get_child_value()");
}


template<class Type>
Type Element::get_child_value_or(const path_type &path, Type default_value) const {
    return get_node().get<Type>(path, default_value);

}


template<class Type>
std::optional<Type> Element::get_child_value_or_empty(const path_type &path) const {
    if(auto ret = get_node().get_optional<Type>(path); ret.has_value())
        return ret.get();
    else
        return std::optional<Type>{};
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
