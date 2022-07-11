/* ============================================================================================================================ *//**
 * @file       configuration.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 3:33:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Configuration class implementing parsing interface of the <Configuration> tag of 
 *             the ENI (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/configuration.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::optional<Slave> Configuration::get_slave(std::string_view name) const {
    
    using namespace std::literals::string_literals;

    // Iterate over configruation nodes
    for (const auto &elem : *this) {

        // Check if <Slave> tag retrived
        if (elem.first != "Slave")
            continue;

        // Check whether slave found
        if (elem.second.get_child_value<std::string>("Info.Name") == name)
            return static_cast<Slave>(elem.second);
            
    }

    // If slave not found, return empty optional
    return std::optional<Slave>{};
}


std::vector<Slave> Configuration::get_slaves() const {

    // Preapre return vector
    std::vector<Slave> ret;

    // Iterate over configruation nodes
    for (const auto &elem : *this) {

        // Check if <Slave> tag retrived
        if (elem.first != "Slave")
            continue;

        // Add slave's description to the output
        ret.push_back(elem.second);
    }

    return ret;
}


std::vector<std::string> Configuration::list_slaves() const {

    using namespace std::literals::string_literals;

    // Preapre return vector
    std::vector<std::string> ret;

    // Iterate over configruation nodes
    for (const auto &elem : *this) {

        // Check if <Slave> tag retrived
        if (elem.first != "Slave")
            continue;

        // Parse slave's name
        auto name = elem.second.get_child_value<std::string>("Info.Name");
        //  Add slave's name to the result list
        ret.push_back(name);
        
    }

    return ret;
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni
