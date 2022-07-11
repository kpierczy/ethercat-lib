/* ============================================================================================================================ *//**
 * @file       pdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Pdo class implementing parsing interface of the <Pdo> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_SLAVE_PDO_H__
#define __ETHERCAT_COMMON_SLAVE_PDO_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/common/helpers.hpp"
#include "ethercat/eni/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ====================================================== Pdo: Public methods ===================================================== */

Slave::Pdo::Direction Slave::Pdo::get_direction() const {
    return direction;
}


bool Slave::Pdo::is_assigned() const {
    return get_sync_manager().has_value();
}


std::size_t Slave::Pdo::get_index() const {
    return parse_index(get_child_value<std::string>("Index"));
}


std::string Slave::Pdo::get_name() const {
    return get_child_value<std::string>("Name");
}

/* ================================================= Pdo: Protected ctors & dtors ================================================= */

Slave::Pdo::Pdo(Direction direction, const Element &elem) :
    Element{ elem },
    direction{ direction }
{ }

/* =================================================== PdosList: Public methods =================================================== */

Slave::PdosList Slave::PdosList::get_assigned() const {
    
    PdosList ret;

    // Copy PDOs that are asigned to some SyncManager
    std::remove_copy_if(this->begin(), this->end(), std::back_insert_iterator(ret),
        [](const Pdo &pdo){ return not pdo.is_assigned(); });

    return ret;
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
