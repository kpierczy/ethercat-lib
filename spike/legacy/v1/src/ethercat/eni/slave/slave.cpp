/* ============================================================================================================================ *//**
 * @file       slave.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 3:33:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Slave class implementing parsing interface of the <Slave> tag of the ENI
 *             (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// Private includes
#include "ethercat/eni/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::vector<Slave::Pdo> Slave::get_pdos(Pdo::Direction direction) const {

    // Get <ProcessData> element
    auto process_data_elem = get_child("ProcessData");

    // Select target element based on the requested direction
    auto target_element_name = (direction == Pdo::Direction::Inputs) ? "TxPdo" : "RxPdo";

    std::vector<Pdo> ret;
    
    // Iterate over elements of <ProcessData> tag
    for(const auto &elem : process_data_elem) {
        // If target element met, add it to the resulting vector
        if(elem.first == target_element_name)
            ret.emplace_back(Pdo{ direction, elem.second });
    }

    return ret;
}


std::vector<Slave::Pdo> Slave::get_assigned_pdos(Pdo::Direction direction) const {
    
    std::vector<Pdo> ret;

    // Get all PDOs for the requested direction
    std::vector<Pdo> pdos = get_pdos(direction);
    // Filter out PDOs that are not assigned to a SyncManager
    std::remove_copy_if(pdos.begin(), pdos.end(), std::back_insert_iterator(ret),
        [](const Pdo &pdo) {
            return not pdo.is_assigned();
        }
    );

    return ret;
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni
