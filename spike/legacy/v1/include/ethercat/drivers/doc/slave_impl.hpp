/* ============================================================================================================================ *//**
 * @file       slave_impl.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 25th May 2022 7:37:33 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Example interface of the class implementing abstract EtherCAT slave driver provided by the @a ethercat library
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifdef DOXYGEN_ONLY

/* =========================================================== Includes =========================================================== */

#include "ethercat/drivers/slave.hpp"

/* =========================================================== Namespace ========================================================== */

namespace ethercat::drivers::impl {

/* ======================================================== Implementation ======================================================== */

/**
 * @brief Example interface of the class implementing abstract EtherCAT slave driver 
 *    provided by the @a ethercat library
 */
class Slave : public ethercat::drivers::Slave<Slave> {

    /// Make base class a friend to let it access implementation methods
    friend class ethercat::drivers::Slave<Slave>;

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
     * @tparam EntryRegistrationProxyT 
     *    type of the @p make_entry . @p EntryRegistrationProxyT is required to define a method template
     *    with signatre:
     * 
     *        template<abstract::pdo::Direction dir>
     *        abstract::pdo::Entry<dir> operator()(
     *            std::string_view pdo_name,
     *            const eni::Slave::Pdo::Entry &entry
     *        );
     * 
     *    that creates PDO entry for the requested object named @p object_name mapped into the
     *    PDO object named @p pdo_name .
     * 
     * @param slave_eni 
     *    parsing interface for the slave's description present in ENI file
     * @param process_image_eni 
     *    reference to the describtor of set of slave-related variables mapped into
     *    rhe Process Data Image
     * @param make_entry
     *    proxy object providing registration mechanism for slave's object entries mapped into
     *    the Process Data Image
     * 
     * @throws error 
     *    whatever @p make_entry throws
     */
    template<typename PdoEntryRegistrationProxyT>
    Slave(eni::Slave slave_eni, PdoEntryRegistrationProxyT make_entry);

    // Disable copy semantic
    Slave(const Slave &rslave) = delete;
    Slave &operator=(const Slave &rslave) = delete;
    // Disable moving semantic
    Slave(Slave &&rslave) = delete;
    Slave &operator=(Slave &&rslave) = delete;

protected: /* -------------------------------------- Protected implementation methods (SDO) --------------------------------------- */

    /**
     * @brief Type-independent implementation of the @ref download_sdo(...) method template
     * 
     * @param request 
     *    request structure
     * @param data 
     *    data to be written into the object
     * @param timeout 
     *    access timeout
     */
    void download_object_impl(
        const ::ethercat::sdo::Request &request,
        ranges::span<const uint8_t> data,
        std::chrono::milliseconds timeout
    );
    
    /**
     * @brief Type-independent implementation of the @ref upload_object(...) method template
     * 
     * @param request 
     *    request structure
     * @param[inout] data 
     *    data buffer to write read object's value into
     * @param timeout 
     *    access timeout
     */
    void upload_object_impl(
        const ::ethercat::sdo::Request &request,
        ranges::span<uint8_t> data,
        std::chrono::milliseconds timeout
    );

};

/* ================================================================================================================================ */

} // End namespace ethercat::drivers::impl

#endif
