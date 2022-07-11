/* ============================================================================================================================ *//**
 * @file       master_impl.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 26th May 2022 9:08:14 pm
 * @modified   Monday, 11th July 2022 9:02:15 pm
 * @project    ethercat-lib
 * @brief      Example interface of the class implementing abstract EtherCAT master driver provided by the @a ethercat library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifdef DOXYGEN_ONLY

/* =========================================================== Includes =========================================================== */

#include "ethercat/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::impl {

/* ============================================================= Class ============================================================ */

/**
 * @brief Example interface of the class implementing abstract EtherCAT master driver 
 *    provided by the @a ethercat library (based on the CIFX-specific implementation)
 */
class Master : public ::ethercat::Master<Master> {

    /// Make base class a friend to let it access implementation methods
    friend class ::ethercat::Master<Master>;

protected: /* ---------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /**
     * @brief Creates EtherCAT master driver operating on the given @p channel of the
     *    CIFX interface
     * 
     * @param channel 
     *    channel to be used for EtherCAT communication
     */
    Master(cifx::Channel &channel);

    /// Disabled copy semantic
    Master(const Master &rmaster) = delete;
    Master &operator=(const Master &rmaster) = delete;
    /// Disabled moving semantic
    Master(Master &&rmaster) = delete;
    Master &operator=(Master &&rmaster) = delete;

protected: /* -------------------------------- Protected EtherCAT common methods (implementation) --------------------------------- */

    /**
     * @brief Reads current state of the slave device in the ESM (EtherCAT slave machine)
     * 
     * @param timeout 
     *    access timeout
     * @returns 
     *    current state of the slave device in the ESM (EtherCAT slave machine)
     * 
     * @throws Error 
     *    on error
     * @throws std::range_error
     *    if invalid state identifier has been returned by the CIFX device
     */
    inline State get_state_impl(std::chrono::milliseconds timeout);
    
    /**
     * @brief Requestes state change of the slave device in the ESM (EtherCAT slave machine)
     * 
     * @param state 
     *    target state
     * @param timeout 
     *    access timeout
     * 
     * @throws Error 
     *    on error
     * @throws std::range_error
     *    if invalid state has been requested
     */
    inline void set_state_impl(State state, std::chrono::milliseconds timeout);
    
protected: /* -------------------------------- Protected EtherCAT I/O methods (implementation) ------------------------------------ */

    /**
     * @brief Reads Input Process Data Image from the bus (secondly updates slave's input PDOs)
     * 
     * @param[out] pdi_buffer
     *    PDI buffer to read data to
     * 
     * @throws cifx::Error
     *    on failure
     */
    void read_bus_impl(::ethercat::config::types::Span<uint8_t> pdi_buffer);

    /**
     * @brief Writes Output Process Data Image to the bus buffer (firstly updates slave's output PDOs)
     * 
     * @param[in] pdi_buffer
     *    PDI buffer to write data from
     * 
     * @throws cifx::Error
     *    on failure
     */
    void write_bus_impl(::ethercat::config::types::Span<const uint8_t> pdi_buffer);

};

/* ================================================================================================================================ */

} // End namespace ethercat::impl

/* ================================================================================================================================ */

#endif
