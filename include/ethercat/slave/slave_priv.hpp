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

#ifndef __ETHERCAT_SLAVE_SLAVE_PRIV_H__
#define __ETHERCAT_SLAVE_SLAVE_PRIV_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ======================================================== Private methods ======================================================= */

namespace details {

    template<bool flag = false>
    constexpr inline void notify_dir_no_match() {
        static_assert(flag, "[ethercat::Slave::notify][BUG] Invalid PDO direction given"); 
    }

}

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
void Slave<ImplementationT>::notify() {
    if constexpr (dir == PdoDirection::Input) {        
        handlers.at_inputs_update();
    } else if constexpr (dir == PdoDirection::Output) {        
        handlers.at_outputs_update();
    } else
        details::notify_dir_no_match();
}

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
