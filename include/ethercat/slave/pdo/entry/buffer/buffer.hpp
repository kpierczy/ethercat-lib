/* ============================================================================================================================ *//**
 * @file       buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 13th June 2022 3:43:33 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the Buffer class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_ENTRY_BUFFER_BUFFER_H__
#define __ETHERCAT_SLAVE_PDO_ENTRY_BUFFER_BUFFER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/bit.hpp"
#include "ethercat/slave/pdo/entry/buffer.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ================================================ Public EtherCAT common methods ================================================ */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
Slave<ImplementationT>::template Pdo<dir>::Entry::Buffer::Buffer(Buffer &&rbuffer) :
    buffer{ std::move(rbuffer.buffer) },
    bitsize{ std::move(rbuffer.bitsize) },
    bitoffset{ std::move(rbuffer.bitoffset) }
{ }


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
typename Slave<ImplementationT>::template Pdo<dir>::Entry::Buffer &
Slave<ImplementationT>::template Pdo<dir>::Entry::Buffer::operator=(Buffer &&rbuffer) {
    buffer    = std::move(rbuffer.buffer);
    bitsize   = std::move(rbuffer.bitsize);
    bitoffset = std::move(rbuffer.bitoffset);
    return *this;
}

/* ===================================================== Public helper methods ==================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<bool enable,
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Pdo<dir>::Entry::Entry::Buffer::update(config::types::Span<uint8_t> pdi) const {
    std::scoped_lock guard{ lock };
    common::utilities::bit::copy_bits_to_bitshifted(buffer.data(), pdi.data(), bitsize, bitoffset);
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<bool enable,
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Pdo<dir>::Entry::Entry::Buffer::update(config::types::Span<const uint8_t> pdi) {
    std::scoped_lock guard{ lock };
    common::utilities::bit::copy_bits_from_bitshifted(pdi.data(), buffer.data(), bitsize, bitoffset);
}

/* ======================================================== Protected ctors ======================================================= */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
Slave<ImplementationT>::template Pdo<dir>::Entry::Entry::Buffer::Buffer(
    std::size_t bitsize,
    std::size_t bitoffset
) :
    // Keep entry parameters
    bitsize{ bitsize },
    bitoffset{ bitoffset },
    // Prepare entry buffer (zero-initialize)
    buffer(
        ((bitsize % common::utilities::bit::BITS_IN_BYTE) == 0) ?
            bitsize / common::utilities::bit::BITS_IN_BYTE :
            bitsize / common::utilities::bit::BITS_IN_BYTE + 1,
        static_cast<uint8_t>(0)
    )
{ }

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
