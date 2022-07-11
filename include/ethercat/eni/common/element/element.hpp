/* ============================================================================================================================ *//**
 * @file       element.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of inline public methods the Element class
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ELEMENT_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ELEMENT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================= Public operators ======================================================= */

template<typename ElementType, std::enable_if_t<std::is_base_of_v<Element, ElementType>, bool>>
Element::operator ElementType() const {
    return ElementType{ root, *node };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
