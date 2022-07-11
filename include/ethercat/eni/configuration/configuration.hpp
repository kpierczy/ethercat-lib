/* ============================================================================================================================ *//**
 * @file       configuration.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 1:13:40 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Configuration class implementing parsing interface of the <Configuration> tag of the ENI
 *             (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_CONFIGRUATION_ENI_CONFIGRUATION_H__
#define __ETHERCAT_COMMON_ENI_CONFIGRUATION_ENI_CONFIGRUATION_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/configuration.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

Master Configuration::get_master() const {
    return get_child("Master");
}


std::size_t Configuration::get_slaves_num() const {
    return count("Slave");
}


Cyclic Configuration::get_cyclic() const {
    return get_child("Cyclic");
}


ProcessImage Configuration::get_process_image() const {
    return get_child("ProcessImage");
}

/* ==================================================== Auxiliary I/O functions =================================================== */

namespace details {

    /// Path to the <Config> element in the ENI file
    constexpr auto ENI_CONFIG_ELEMENT_PATH = "EtherCATConfig.Config";

}


Configuration configruation_from_file(const std::filesystem::path &path) {
    return element_from_file(path).get_child(details::ENI_CONFIG_ELEMENT_PATH);
}


Configuration configruation_from_string(const std::string &eni) {
    return element_from_string(eni).get_child(details::ENI_CONFIG_ELEMENT_PATH);
}


Configuration configruation_from_stream(std::basic_istream<char> &stream) {
    return element_from_stream(stream).get_child(details::ENI_CONFIG_ELEMENT_PATH);
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
