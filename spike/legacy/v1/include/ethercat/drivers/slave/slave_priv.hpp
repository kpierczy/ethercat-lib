/* ============================================================================================================================ *//**
 * @file       slave_priv.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of private methods of the Slave class representing slave device on the EtherCAT bus
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DRIVERS_SLAVE_SLAVE_PRIV_H__
#define __ETHERCAT_DRIVERS_SLAVE_SLAVE_PRIV_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/drivers/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ============================================= Private implementation methods (CPRT) ============================================ */

template<typename ImplementationT>
ImplementationT &Slave<ImplementationT>::impl() {
    return *static_cast<ImplementationT*>(this);
}


template<typename ImplementationT>
const ImplementationT &Slave<ImplementationT>::impl() const {
    return *static_cast<const ImplementationT*>(this);
}

/* ================================================= Private access methods (PDO) ================================================= */

namespace details {

    template<bool flag = false>
    constexpr inline void find_pdo_entry_dir_no_match() {
        static_assert(flag, "[ethercat::drivers::Slave<ImplementationT>::find_pdo_entry] Invalid PDO direction given"); 
    }

}

template<typename ImplementationT>
template<abstract::pdo::Direction dir, abstract::objects::Type type>
std::optional<typename std::vector<abstract::pdo::Entry<dir>>::iterator> 
Slave<ImplementationT>::find_pdo_entry(std::string_view name) {

    std::vector<abstract::pdo::Entry<dir>> *pdos;

    // Pick related vector of PDO entries
    if constexpr(dir == abstract::pdo::Direction::Input)
        pdos = &inputs;
    else if constexpr(dir == abstract::pdo::Direction::Output)
        pdos = &outputs;
    else 
        details::find_pdo_entry_dir_no_match();

    // Find requested PDO
    auto pdo_found = std::find_if(pdos->begin(), pdos->end(), 
        [&name](const auto &entry){ return entry.get_name() == name; }
    );

    // If PDO has not been found, return empty
    if(pdo_found == pdos->end())
        return std::optional<typename std::vector<abstract::pdo::Entry<dir>>::iterator>{ };

    // If @tparam type does not match actual type of the PDO, throw error
    if(type != pdo_found->get_type())
        throw abstract::objects::TypeError{ type, pdo_found->get_type(), "ethercat::drivers::Slave<ImplementationT>::get_pdo_reference" };

    // Return reference to the PDO
    return pdo_found;

}

/* ===================================================== Private methods (PDO) ==================================================== */

namespace details {

    template<bool flag = false>
    constexpr inline void update_dir_no_match() {
        static_assert(flag, "[ethercat::Slave<ImplementationT>::update] Invalid PDO direction given"); 
    }

}

template<typename ImplementationT>
template<abstract::pdo::Direction dir>
void Slave<ImplementationT>::update() {
    
    /// Pointer to the pdo entries        
    std::vector<abstract::pdo::Entry<dir>> *pdos;
    
    // Select PDO set
    if constexpr (dir == abstract::pdo::Direction::Input)
        pdos = &inputs;
    else if constexpr (dir == abstract::pdo::Direction::Output)
        pdos = &outputs;
    else
        details::update_dir_no_match();

    // Call outputs-update handler
    if constexpr(dir == abstract::pdo::Direction::Output)
        handlers.at_outputs_update();

    // Iterate over PDO entries and update them
    for(auto &pdo : *pdos)
        pdo.update();

    // Call inputs-update handler
    if constexpr(dir == abstract::pdo::Direction::Input)
        handlers.at_inputs_update();

}

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

#endif
