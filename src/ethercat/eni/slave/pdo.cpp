/* ============================================================================================================================ *//**
 * @file       pdo.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 3:33:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Pdo class implementing parsing interface of the <TxPdo/RxPdo> tag of the ENI
 *             (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
#include <regex>
// Private includes
#include "ethercat/eni/common/helpers.hpp"
#include "ethercat/eni/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::optional<std::size_t> Slave::Pdo::get_sync_manager() const {
    
    /// Check if PDO element has XML attributes
    if(has_child(ATTRIBUTES_CHILD)) {

        // Get reference to the XML attributes node
        auto &attributes = get_child(ATTRIBUTES_CHILD);
        
        // Name of the XML attribute of the <TxPdo/RxPdo> determining index of the SyncManager PDO is assigned to
        constexpr auto SYNC_MANAGER_ATTRIBUTE_NAME = "Sm";

        // If element has <Sm> attribute, return parsed value
        if(attributes.has_child(SYNC_MANAGER_ATTRIBUTE_NAME)) {
            return std::optional<std::size_t>{ 
                attributes.get_child_value<std::size_t>(SYNC_MANAGER_ATTRIBUTE_NAME)
            };
        }
    }

    // Otherwise return empty result
    return std::optional<std::size_t>{ };
}


bool Slave::Pdo::is_fixed() const {

    /// Check if PDO element has XML attributes
    if(has_child(ATTRIBUTES_CHILD)) {

        // Get reference to the XML attributes node
        auto &attributes = get_child(ATTRIBUTES_CHILD);

        // Name of the XML attribute of the <TxPdo/RxPdo> determining whether PDO id fixed
        constexpr auto FIXED_ATTRIBUTE_NAME = "Fixed";

        // If element has <Fixed> attribute, return parsed value
        if(attributes.has_child(FIXED_ATTRIBUTE_NAME))
            return (attributes.get_child_value<std::string>(FIXED_ATTRIBUTE_NAME) == "true");

    }

    // Otherwise return false
    return false;
}


std::vector<std::size_t> Slave::Pdo::get_excludes() const {
        
    std::vector<std::size_t> ret;
    
    // Iterate over <Exclude> subelements of the PDO
    for(const auto &elem : *this) {
        // If <Exclude> element found, parse it
        if(elem.first == "Exclude")
            ret.push_back(parse_index(elem.second.get_value<std::string>()));
    }

    // Return result
    return ret;
}


std::vector<Slave::Pdo::Entry> Slave::Pdo::get_entries() const {

    std::vector<Entry> ret;
    
    // Iterate over subelements of the PDO
    for(const auto &elem : *this) {
        // If <Entry> element found, parse it
        if(elem.first == "Entry")
            ret.emplace_back(elem.second);
    }

    return ret;
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni
