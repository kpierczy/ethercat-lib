/** ==================================================================================================================================
 * @file       slave_prot.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 10:31:09 am
 * @modified   Friday, 20th May 2022 8:25:48 pm
 * @project    ethercat-lib
 * @brief
 *    
 *    Definition of protected
 * @copyright Krzysztof Pierczyk © 2022
 * ================================================================================================================================ */

#ifndef __ETHERCAT_DRIVERS_SLAVE_SLAVE_PROT_H__
#define __ETHERCAT_DRIVERS_SLAVE_SLAVE_PROT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni.hpp"
#include "ethercat/drivers/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ==================================================== Protected ctors & dtors =================================================== */

template<typename ImplementationT>
template<typename PdoEntryRegistrationProxyT>
Slave<ImplementationT>::Slave(
    eni::Slave slave_eni,
    PdoEntryRegistrationProxyT&& make_entry
) :
    // Store slave's ENI description as an autonomous object
    eni_description{ slave_eni },
    // Initialize slave's configruation
    name{ slave_eni.get_name() },
    // Initialize slave's configruation (these will be actually set in the body)
    fixed_addr{ },
    auto_increment_addr{ },
    topological_addr{ }
{
    // Autonomize slave's ENI description form the rest of ENI tree
    const_cast<eni::Slave&>(eni_description).autonomize();
    // Prase slave's configuration
    const_cast<uint16_t&>(fixed_addr)          = eni_description.get_physical_addr();
    const_cast<uint16_t&>(auto_increment_addr) = eni_description.get_auto_increment_addr();
    const_cast<uint16_t&>(topological_addr)    = 0x1 - eni_description.get_auto_increment_addr();
    
    /**
     * @brief Addressing schemes are described in [1]
     * @see [1] 'EtherCAT Master V4 Protocol API 06 EN.pdf', p.18-19
     */

    // Get list of slave's PDOs that have been assigned in the PDI
    auto assigned_pdos = eni_description.get_assigned_pdos();
    
    /* ------------------------- Register input PDOs entries ------------------------- */

    /// Allocate memory for input PDOs entries
    inputs.reserve(assigned_pdos.inputs.size());

    /// Register entries
    for(const auto &pdo : assigned_pdos.inputs)
        for(const auto &obj : pdo.get_entries()) 
            inputs.emplace_back(
                make_entry.template operator()<abstract::pdo::Direction::Input>(
                    pdo.get_name(), obj
                )
            );
    
    /* ------------------------- Register input PDOs entries ------------------------- */

    /// Allocate memory for output PDOs entries
    outputs.reserve(assigned_pdos.outputs.size());

    /// Register entries
    for(const auto &pdo : assigned_pdos.outputs)
        for(const auto &obj : pdo.get_entries()) 
            outputs.emplace_back(
                make_entry.template operator()<abstract::pdo::Direction::Output>(
                    pdo.get_name(), obj
                )
            );

}

template<typename ImplementationT>
Slave<ImplementationT>::Slave(Slave &&rslave) :
    
    // Protected members
    eni_description     { rslave.eni_description     },
    name                { rslave.name                },
    fixed_addr          { rslave.fixed_addr          },
    auto_increment_addr { rslave.auto_increment_addr },
    topological_addr    { rslave.topological_addr    },
    // Private members
    inputs  { std::move(rslave.inputs ) },
    outputs { std::move(rslave.outputs) }

{ }


template<typename ImplementationT>
Slave<ImplementationT> &
Slave<ImplementationT>::operator=(Slave &&rslave) {

    // Protected members
    const_cast<eni::Slave&>(eni_description)   = rslave.eni_description;
    const_cast<std::string&>(name)             = rslave.name;
    const_cast<uint16_t&>(fixed_addr)          = rslave.fixed_addr;
    const_cast<uint16_t&>(auto_increment_addr) = rslave.auto_increment_addr;
    const_cast<uint16_t&>(topological_addr)    = rslave.topological_addr;
    // Private members
    inputs  = std::move(rslave.inputs );
    outputs = std::move(rslave.outputs);

    return *this;
}

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

#endif
