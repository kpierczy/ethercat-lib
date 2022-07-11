/* ============================================================================================================================ *//**
 * @file       entry.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Entry class implementing parsing interface of the <Entry> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_SLAVE_PDO_ENTRY_H__
#define __ETHERCAT_COMMON_SLAVE_PDO_ENTRY_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* =========================================================== Constants ========================================================== */

namespace details {

    /// Number of bits in a byte
    constexpr std::size_t BITS_IN_BYTE = 8;

}

/* ======================================================== Public methods ======================================================== */

std::size_t Slave::Pdo::Entry::get_index() const {
    return parse_index(get_child_value<std::string>("Index"));
}

std::size_t Slave::Pdo::Entry::get_subindex() const {
    return get_child_value<std::size_t>("SubIndex");
}


std::size_t Slave::Pdo::Entry::get_bit_len() const {
    return get_child_value<std::size_t>("BitLen");
}


std::size_t Slave::Pdo::Entry::get_byte_len() const {
    return get_bit_len() / details::BITS_IN_BYTE;
}


std::string Slave::Pdo::Entry::get_name() const {
    return get_child_value<std::string>("Name");
}


abstract::objects::Type Slave::Pdo::Entry::get_data_type() const {
    return abstract::objects::parse_type(get_child_value<std::string>("DataType"));
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
