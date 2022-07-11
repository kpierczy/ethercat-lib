/* ============================================================================================================================ *//**
 * @file       master_priv.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 26th May 2022 5:38:41 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of private methods of the Master class providing API entry for implementing hardware-specific drivers
 *             of EtherCAT master devices
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_MASTER_MASTER_PRIV_H__
#define __ETHERCAT_MASTER_MASTER_PRIV_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ===================================================== Private ctors & dtors ==================================================== */

template<typename ImplementationT,typename SlaveImplementationT>
template<typename SlaveFactoryT>
Master<ImplementationT, SlaveImplementationT>::Master(
    eni::Configuration &&eni,
    SlaveFactoryT&& slave_factory
) :
    // Parse ENI configuration
    bus_cycle { eni.get_cyclic().get_cycle_time() },
    // Initialize PDI buffers with zeros
    input_pdi  { eni.get_process_image().get_size(eni::ProcessImage::Direction::Inputs)  },
    output_pdi { eni.get_process_image().get_size(eni::ProcessImage::Direction::Outputs) }
{ 
    
    // Verify that all slaves have unique names
    if(auto slave = eni.are_slaves_unique(); slave.has_value()) {

        // If no, throw error
        std::stringstream ss;
        ss << "[ethercat::Master::Master] Set of slaves listed in the ENI file contains more than " 
           << "one slave with the non-unique name/address (" << slave->get_name() << ")";
        throw eni::Error{ ss.str() };
    }
 
    // Get list of slaves on the bus
    auto slave_eni_list = eni.get_slaves();
    // Prepare storage for Slave interfaces
    slaves.reserve(slave_eni_list.size()); 

    /*
     * @brief Auxiliary function constructing list of PDO objects for the single slave
     * @param dir
     *    std::integral_constant of type SlaveT::PdoDirection representing target direction of created PDOs
     * @param slave_pdos_config
     *    list of ENI descriptions of slave's PDOs defined for the given @p dir
     * @param sslave_variables
     *    list of ENI descriptions of slave-related PDI variables present in the description of Process Data
     *    Image of the given @p dir
     */
    auto make_pdos = [](
        auto dir,
        const eni::Slave::PdosList &slave_pdos_config,
        const eni::ProcessImage::VariablesList &slave_variables
    ) { 

        using PdoType = typename SlaveT::template Pdo<dir>;

        // Prepare list of slave's PDOs
        std::vector<PdoType> pdos;
        // Get ENI configurations of slave's mapped PDOs
        auto assigned_pdos_config = slave_pdos_config.get_assigned();
        // Reserve memory for PDOs
        pdos.reserve(assigned_pdos_config.size());

        // Iterate over descriptions of mapped PDOs and construct PDO objects
        for(const auto &pdo_config : assigned_pdos_config)
            pdos.emplace_back(PdoType{ pdo_config, slave_variables.get_pdo_variables(pdo_config.get_name()) });
            
        return pdos;
    };

    // Create interface object for each slave described in the ENI
    for(auto &slave_config : slave_eni_list) {

        // Get list of slave's mapped pdos
        auto pdos_config = slave_config.get_pdos();
        // Get list of slave-related PDI variables
        auto variables_config = eni.get_process_image().get_slave_variables(slave_config.get_name());

        // Construct the slave
        slaves.emplace_back(
            slave_factory(slave_config, 
                make_pdos( /* Input PDOs */
                    std::integral_constant<typename SlaveT::PdoDirection, SlaveT::PdoDirection::Input>{},
                    pdos_config.inputs,
                    variables_config.inputs),
                make_pdos( /* Output PDOs */
                    std::integral_constant<typename SlaveT::PdoDirection, SlaveT::PdoDirection::Output>{},
                    pdos_config.outputs,
                    variables_config.outputs)
            )
        );
    }
}

/* ================================================================================================================================ */

} // End namespace ethercat

/* ================================================================================================================================ */

#endif
