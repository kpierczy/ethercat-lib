/* ============================================================================================================================ *//**
 * @file       pdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 11:27:14 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the PDO nested class of the Slave interface
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_PDO_H__
#define __ETHERCAT_SLAVE_PDO_PDO_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave/pdo.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ======================================================== Public getters ======================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
std::string_view Slave<ImplementationT>::template Pdo<dir>::get_name() const {
    return name;
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
std::vector<typename Slave<ImplementationT>::template Pdo<dir>::Entry> &
Slave<ImplementationT>::template Pdo<dir>::get_entries() {
    return entries;
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
const std::vector<typename Slave<ImplementationT>::template Pdo<dir>::Entry> &
Slave<ImplementationT>::template Pdo<dir>::get_entries() const {
    return entries;
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
bool Slave<ImplementationT>::template Pdo<dir>::has_entry(std::string_view name) const {
    return (entries.end() != 
        std::find_if(entries.begin(), entries.end(),
            [&name](const auto &entry){ return (entry.get_name() == name); }
        )
    );
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
typename Slave<ImplementationT>::template Pdo<dir>::Entry &
Slave<ImplementationT>::template Pdo<dir>::get_entry(std::string_view name) {

    // Try to find the entry
    auto entry = std::find_if(entries.begin(), entries.end(),
        [&name](const auto &entry){ return (entry.get_name() == name); }
    );

    // If not found, throw
    if(entry == entries.end()) {
        std::stringstream ss;
        ss << "[ethercat::Slave::Pdo::get_entry] No entry named "
            << "'" << name << "' "
            << "in '" << this->name << "' PDO found";
        throw std::out_of_range{ ss.str() };
    }

    // Else, return entry
    return *entry;
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
const typename Slave<ImplementationT>::template Pdo<dir>::Entry &
Slave<ImplementationT>::template Pdo<dir>::get_entry(std::string_view name) const {
    return const_cast<Pdo*>(this)->get_entry(name);
}

/* ==================================================== Protected ctors & dtors =================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
Slave<ImplementationT>::template Pdo<dir>::Pdo(
    eni::Slave::Pdo pdo_description,
    const eni::ProcessImage::VariablesList &pdi_variables
) :
    name{ pdo_description.get_name() }
{
    // Reserve memory for entries
    entries.reserve(pdo_description.get_entries().size());

    // Iterate over descriptions of PDO's entries and create subsequent Entries
    for(const auto entry_description : pdo_description.get_entries()) {

        // Find description of corresponding PDI variable
        auto variable_description = pdi_variables.get_variable(entry_description.get_name());
        // If no corresponding description, throw error
        if(not variable_description.has_value()) {
            std::stringstream ss;
            ss << "[ethercat::Slave::Pdo::Pdo] Incoherent ENI description found when parsing PDO entry named "
               << "'" << entry_description.get_name() << "' "
               << "in '" << name << "' PDO "
               << "(no matching variable in the PDI description)";
            throw eni::Error{ ss.str() };
        }

        // Construct a new Entry
        entries.emplace_back(Entry{ entry_description, *variable_description });

    }
}

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
