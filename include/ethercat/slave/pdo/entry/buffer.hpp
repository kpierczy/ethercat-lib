/* ============================================================================================================================ *//**
 * @file       buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the auxiliary Buffer nested class of the Entry class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_ENTRY_BUFFER_H__
#define __ETHERCAT_SLAVE_PDO_ENTRY_BUFFER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/config.hpp"
#include "ethercat/slave/pdo/entry.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================= Class ============================================================ */

/**
 * @brief A wrapper class managing byte-image buffer holding copy of current data associated
 *    witht he entry in the Process Data Image (PDI)
 * 
 * @tparam ImplementationT 
 *    type implementing hardware-specific part of the Slave driver
 * @tparam dir 
 *    communication direction for PDO interface 
 */
template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
class Slave<ImplementationT>::template Pdo<dir>::Entry::Buffer {
    
public: /* ----------------------------------------------------- Public data ------------------------------------------------------ */

    // Lock used to synchronise the buffer
    mutable ethercat::config::types::QuickLock lock;

    // Actual data buffer
    std::vector<uint8_t> buffer;
    
    // Bitsize of the entry
    std::size_t bitsize;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /// Disable copy-construction
    inline Buffer(const Buffer &rbuffer) = delete;
    /// Disable copy-asignment
    inline Buffer &operator=(const Buffer &rbuffer) = delete;

    /// Enable move-construction (to enable storing slave in relocatable containers)
    inline Buffer(Buffer &&rbuffer);
    /// Enable move-asignment (to enable storing slave in relocatable containers)
    inline Buffer &operator=(Buffer &&rbuffer);

public: /* ------------------------------------------------ Public helper methods ------------------------------------------------- */
    
    /**
     * @brief Updates buffer from the current Process Data Image @p pdi buffer
     * 
     * @param pdi 
     *   reference to the current PDI buffer
     * 
     * @note This overload is enabled only for Input entries 
     */
    template<bool enable = common::translation::is_input_dir_v<dir>,
        std::enable_if_t<enable, bool> = true>
    inline void update(config::types::Span<const uint8_t> pdi);

    /**
     * @brief Updates current Process Data Image @p pdi buffer with data stored in the buffer
     * 
     * @param pdi 
     *   reference to the current PDI buffer
     * 
     * @note This overload is enabled only for Output entries 
     */
    template<bool enable = common::translation::is_output_dir_v<dir>,
        std::enable_if_t<enable, bool> = true>
    inline void update(config::types::Span<uint8_t> pdi) const;

protected: /* ------------------------------------------------ Protected ctors ---------------------------------------------------- */

    /// Make Entry a friend to let it access constructor
    friend class Entry;
    
    /**
     * @brief Construct a new Buffer with given parameters
     * 
     * @param bitsize 
     *    bitsize of the entry
     * @param bitoffset 
     *    bitoffset of the entry in the PDI image
     */
    inline Buffer(
        std::size_t bitsize,
        std::size_t bitoffset
    );
    
private: /* --------------------------------------------------- Private data ------------------------------------------------------ */

    // Bitoffset of the entry in the Process Data Image
    std::size_t bitoffset;

};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/slave/pdo/entry/buffer/buffer.hpp"

/* ================================================================================================================================ */

#endif
