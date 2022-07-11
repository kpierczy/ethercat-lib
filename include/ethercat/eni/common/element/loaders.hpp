/* ============================================================================================================================ *//**
 * @file       loaders.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public static inline methods of the Element class providing routines loading ENI file
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_LOADERS_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_LOADERS_H__

/* =========================================================== Includes =========================================================== */

// Boost includes
#include "boost/property_tree/xml_parser.hpp"
// Private includes
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ==================================================== Protected ctors & dtors =================================================== */

Element element_from_file(const std::filesystem::path &path) {

    // Allocate property tree in the memory
    auto ret = std::make_shared<Element::property_tree_type>();

    // Load property tree from XML
    wrap_error([&]{

        // Load tree from the file
        boost::property_tree::read_xml(path.string(), *ret);

    }, "ethercat::eni::element_from_file()");

    // Return wrapping element
    return Element(ret);
}


Element element_from_string(const std::string &eni) {

    // Allocate property tree in the memory
    auto ret = std::make_shared<Element::property_tree_type>();

    // Create stream from string
    std::istringstream stream{ eni };
    // Load property tree from XML
    wrap_error([&]{

        // Load tree from the stream
        boost::property_tree::read_xml(stream, *ret);

    }, "ethercat::eni::element_from_string()");

    // Return wrapping element
    return Element(ret);
}


Element element_from_stream(std::basic_istream<char> &stream) {

    // Allocate property tree in the memory
    auto ret = std::make_shared<Element::property_tree_type>();

    // Load property tree from XML
    wrap_error([&]{

        // Load tree from the stream
        boost::property_tree::read_xml(stream, *ret);

    }, "ethercat::eni::element_from_stream()");

    // Return wrapping element
    return Element(ret);
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
