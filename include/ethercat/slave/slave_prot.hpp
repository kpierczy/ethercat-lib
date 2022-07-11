/** ==================================================================================================================================
 * @file       slave_prot.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 10:31:09 am
 * @modified   Friday, 20th May 2022 8:25:48 pm
 * @project    ethercat-lib
 * @brief      Definition of protected methods of the Slave class representing slave device on the EtherCAT bus
 *    
 *    
 * @copyright Krzysztof Pierczyk © 2022
 * ================================================================================================================================ */

#ifndef __ETHERCAT_SLAVE_SLAVE_PROT_H__
#define __ETHERCAT_SLAVE_SLAVE_PROT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni.hpp"
#include "ethercat/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ==================================================== Protected ctors & dtors =================================================== */

template<typename ImplementationT>
Slave<ImplementationT>::Slave(
    eni::Slave slave_eni,
    std::vector<Pdo<PdoDirection::Input>> &&inputs,
    std::vector<Pdo<PdoDirection::Output>> &&outputs
) :
    // Store slave's ENI description as an autonomous object
    eni_description{ slave_eni },
    // Initialize slave's configruation
    name{ slave_eni.get_name() },
    // Initialize slave's configruation
    fixed_addr{ eni_description.get_physical_addr() },
    auto_increment_addr{ eni_description.get_auto_increment_addr() },
    topological_addr{ 0x1 - eni_description.get_auto_increment_addr() },

    // Initialize PDO lists
    inputs{ std::move(inputs) },
    outputs{ std::move(outputs) }
{
    // Autonomize slave's ENI description form the rest of ENI tree
    eni_description.autonomize();
    
    /**
     * @brief Addressing schemes are described in [1]
     * @see [1] 'EtherCAT Master V4 Protocol API 06 EN.pdf', p.18-19
     */
}

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
