/* ============================================================================================================================ *//**
 * @file       master_prot.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 26th May 2022 5:38:41 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of protected methods of the Master class providing API entry for implementing hardware-specific drivers
 *             of EtherCAT master devices
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_MASTER_MASTER_PROT_H__
#define __ETHERCAT_MASTER_MASTER_PROT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ==================================================== Protected ctors & dtors =================================================== */

template<typename ImplementationT,typename SlaveImplementationT>
    template<typename SlaveFactoryT>
Master<ImplementationT, SlaveImplementationT>::Master(
    const std::filesystem::path &eni_path,
    SlaveFactoryT&& slave_factory
) :
    Master{ eni::configruation_from_file(eni_path), std::forward<SlaveFactoryT>(slave_factory) }
{ }


template<typename ImplementationT,typename SlaveImplementationT>
    template<typename SlaveFactoryT>
Master<ImplementationT, SlaveImplementationT>::Master(
    const std::string &eni,
    SlaveFactoryT&& slave_factory
) :
    Master{ eni::configruation_from_string(eni), std::forward<SlaveFactoryT>(slave_factory) }
{ }


template<typename ImplementationT,typename SlaveImplementationT>
    template<typename SlaveFactoryT>
Master<ImplementationT, SlaveImplementationT>::Master(
    std::basic_istream<char> &stream,
    SlaveFactoryT&& slave_factory
) :
    Master{ eni::configruation_from_stream(stream), std::forward<SlaveFactoryT>(slave_factory) }
{ }

/* ================================================================================================================================ */

} // End namespace ethercat

/* ================================================================================================================================ */

#endif
