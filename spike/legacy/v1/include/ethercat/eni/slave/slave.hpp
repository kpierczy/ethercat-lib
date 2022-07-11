/* ============================================================================================================================ *//**
 * @file       slave.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Slave class implementing parsing interface of the <Slave> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_SLAVE_SLAVE_H__
#define __ETHERCAT_COMMON_SLAVE_SLAVE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::string Slave::get_name() const {
    return get_child_value<std::string>("Info.Name");
}


std::size_t Slave::get_physical_addr() const {
    return get_child_value<std::size_t>("Info.PhysAddr");
}


std::size_t Slave::get_auto_increment_addr() const {
    return get_child_value<std::size_t>("Info.AutoIncAddr");
}


Slave::PdosSet Slave::get_pdos() const {
    return PdosSet {
        .inputs  = get_pdos(Pdo::Direction::Inputs),
        .outputs = get_pdos(Pdo::Direction::Outputs)
    };
}


Slave::PdosSet Slave::get_assigned_pdos() const {
    return PdosSet {
        .inputs  = get_assigned_pdos(Pdo::Direction::Inputs),
        .outputs = get_assigned_pdos(Pdo::Direction::Outputs)
    };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
