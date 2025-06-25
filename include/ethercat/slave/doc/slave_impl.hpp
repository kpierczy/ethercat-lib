/* ============================================================================================================================ *//**
 * @file       slave_impl.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 25th May 2022 7:37:33 pm
 * @modified   Wednesday, 25th June 2025 3:15:10 pm
 * @project    ethercat-lib
 * @brief      Example interface of the class implementing abstract EtherCAT slave driver provided by the @a ethercat library
 *
 *
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifdef DOXYGEN_ONLY

/* =========================================================== Includes =========================================================== */

#include "ethercat/slave.hpp"

/* =========================================================== Namespace ========================================================== */

namespace ethercat::impl {

/* ======================================================== Implementation ======================================================== */

/**
 * @brief Example interface of the class implementing abstract EtherCAT slave driver
 *    provided by the @a ethercat library
 */
class Slave : public ethercat::Slave<Slave> {

    /// Make base class a friend to let it access implementation methods
    friend class ethercat::Slave<Slave>;

protected: /* ------------------------------ Protected EtherCAT common methods (implementations) ---------------------------------- */

    /**
     * @brief Reads current state of the slave device in the ESM (EtherCAT slave machine)
     *
     * @param timeout
     *    access timeout
     * @returns
     *    current state of the slave device in the ESM (EtherCAT slave machine)
     *
     * @throws cifx::Error
     *    on error
     * @throws std::range_error
     *    if invalid state identifier has been returned by the hardware
     */
    State get_state_impl(std::chrono::milliseconds timeout);

    /**
     * @brief Requestes state change of the slave device in the ESM (EtherCAT slave machine)
     *
     * @param state
     *    target state
     * @param timeout
     *    access timeout
     *
     * @throws cifx::Error
     *    on error
     * @throws std::range_error
     *    if invalid state has been requested
     */
    void set_state_impl(State state, std::chrono::milliseconds timeout);

protected: /* --------------------------------------------- Protected ctors & dtors ----------------------------------------------- */

    /**
     * @brief Construct a new Slave interface
     *
     * @param channel
     *    reference tot he CIFX channel used to communicate with CIFX master device
     * @param slave_eni
     *    parsing interface for the slave's description present in ENI file
     * @param inputs
     *    input PDOs associated with the slave
     * @param outputs
     *    output PDOs associated with the slave
     *
     * @throws cifx::ethercat::eni::Error
     *    if initialization fails for some reason
     */
    inline Slave(
        cifx::Channel &channel,
        ::ethercat::eni::Slave slave_eni,
        std::vector<Pdo<PdoDirection::Input>> &&inputs,
        std::vector<Pdo<PdoDirection::Output>> &&outputs
    );

    /// Disable copy semantic
    Slave(const Slave &rslave) = delete;
    Slave &operator=(const Slave &rslave) = delete;

protected: /* -------------------------------------- Protected implementation methods (SDO) --------------------------------------- */

    /**
     * @brief Type-independent implementation of the @ref download_sdo(...) method template
     *
     * @param index
     *    index of the object
     * @param subindex
     *    subindex of the object
     * @param data
     *    data to be written into the object
     * @param timeout
     *    access timeout
     * @param complete_access
     *    @c true if complete access is to be performed
     */
    void download_sdo(
        uint16_t index,
        uint16_t subindex,
        std::span<const uint8_t> data,
        std::chrono::milliseconds timeout,
        bool complete_access
    );

    /**
     * @brief Type-independent implementation of the @ref upload_object(...) method template
     *
     * @param index
     *    index of the object
     * @param subindex
     *    subindex of the object
     * @param[inout] data
     *    data to be written into the object
     * @param timeout
     *    access timeout
     * @param complete_access
     *    @c true if complete access is to be performed
     */
    void upload_sdo(
        uint16_t index,
        uint16_t subindex,
        std::span<uint8_t> data,
        std::chrono::milliseconds timeout,
        bool complete_access
    );

};

/* ================================================================================================================================ */

} // End namespace ethercat::impl

#endif
