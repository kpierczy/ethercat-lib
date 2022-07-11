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

#ifndef __ETHERCAT_DRIVERS_MASTER_MASTER_PRIV_H__
#define __ETHERCAT_DRIVERS_MASTER_MASTER_PRIV_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// Private includes
#include "ethercat/drivers/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ============================================================ Helpers =========================================================== */

namespace details {
    
    /**
     * @brief Helper STL-like algorithm checking whether range whose elements has been transformed with 
     *    @p transformation stores only unique values (i.e. is a set) and returning iterator to the first
     *    non-unique element, if found
     * 
     * @tparam InputIt 
     *    type of range's iterators
     * @tparam TransformationT 
     *    type of the @p transformation
     * @param begin 
     *    begin of the range
     * @param end
     *    end of the range
     * @param transformation
     *    target transformation
     * 
     * @retval it 
     *    iterator to the first non-unique element, if found
     * @retval end 
     *    if no non-unique element found
     */
    template<typename InputIt, typename TransformationT>
    constexpr InputIt find_first_non_unique_transformed(InputIt begin, InputIt end, TransformationT transformation) {

        // Iterate over range
        for(auto it = begin; it != end; ++it) {
            
            // Transform current element
            auto it_transformed = transformation(*it);
            // Iterate over left elements
            for(auto rit = it; rit != end; ++it) {
                // If transformations are equal, return element
                if(it_transformed == transformation(*rit))
                    return it;
            }
        }
        
        // If no equality found, return end
        return end;
    }

}

/* ===================================================== Private ctors & dtors ==================================================== */


template<typename ImplementationT,typename SlaveImplementationT>
template<typename SlaveFactoryT>
Master<ImplementationT, SlaveImplementationT>::Master(
    eni::Configuration &&eni,
    SlaveFactoryT&& slave_factory
) :
    bus_cycle{ eni.get_cyclic().get_cycle_time() }
{ 
    // Clear PDI buffers
    {
        auto ipdi = input_pdi.acquire();
        std::fill(ipdi.get_data().begin(), ipdi.get_data().end(), static_cast<uint8_t>(0));
        auto opdi = output_pdi.acquire();
        std::fill(opdi.get_data().begin(), opdi.get_data().end(), static_cast<uint8_t>(0));
    }

    // Get list of slaves on the bus
    auto slave_eni_list = eni.get_slaves();
    // Verify that all slaves have unique names
    if(auto elem = details::find_first_non_unique_transformed(slave_eni_list.begin(), slave_eni_list.end(), 
        [](const auto &slave_config) { return slave_config.get_name(); }
    ); elem != slave_eni_list.end()) {

        // If no, throw error
        std::stringstream ss;
        ss << "[ethercat::drivers::Master::Master] Set of slaves listed in the ENI file contains more than one slave with the same name ("
           << elem->get_name()
           << ")";
        throw eni::Error{ ss.str() };
    }

    // Prepare storage for Slave interfaces
    slaves.reserve(slave_eni_list.size());

    // Get references to PDI buffers
    auto ipdi = input_pdi.acquire();
    auto opdi = output_pdi.acquire();

    // Create interface object for each slave described in the ENI
    for(auto &slave_config : slave_eni_list) {

        // Get slave's name
        auto slave_name = slave_config.get_name();
        // Get list of entries listed in the <ProcessImage> section of ENI that are related to the given slave
        auto mapped_slave_variables = eni.get_process_image().get_slave_variables(slave_name);

        // Create PDO-entries registration proxy for the given slave
        PdoEntryRegistrationProxy pdo_entry_registration_proxy {
            slave_name,
            ipdi.get_data(),
            opdi.get_data(),
            mapped_slave_variables
        };

        // Create slave interface
        slaves.emplace_back(slave_factory(slave_config, pdo_entry_registration_proxy));
        
    }

}

/* ============================================= Private implementation methods (CPRT) ============================================ */

template<typename ImplementationT,typename SlaveImplementationT>
ImplementationT &Master<ImplementationT, SlaveImplementationT>::impl() {
    return *static_cast<ImplementationT*>(this);
}


template<typename ImplementationT,typename SlaveImplementationT>
const ImplementationT &Master<ImplementationT, SlaveImplementationT>::impl() const {
    return *static_cast<const ImplementationT*>(this);
}

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

/* ================================================================================================================================ */

#endif
