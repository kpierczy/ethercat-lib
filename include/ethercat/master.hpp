/* ============================================================================================================================ *//**
 * @file       master.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 11:18:57 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Master class providing an interface for managig Master node of the bus
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_MASTER_H__
#define __ETHERCAT_MASTER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/config.hpp"
#include "ethercat/common/utilities/crtp.hpp"
#include "ethercat/common/handlers/event_handler.hpp"
#include "ethercat/eni/configuration.hpp"
#include "ethercat/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================ Master ============================================================ */

/**
 * @brief Interface class template representing a master device on the EtherCAt bus
 * 
 * @tparam ImplementationT
 *    type of the class implementing hardware-specifc routines related to Master device 
 *    management; this class needs to derive from Slave class; for interface required to
 *    be implemented see @ref ethercat::drivers::impl::Master
 * @tparam SlaveImplementationT
 *    type of the slave interface objects; this type must be derivced from 
 *    @ref Slave<SlaveImplementationT> base class
 * 
 * @note Template utilizes CRTP pattern
 * @todo When C++20 is available make @p SlaveImplementationT required to be derived from
 *    Slave<SlaveImplementationT>
 */
template<typename ImplementationT,typename SlaveImplementationT>
class Master : public common::utilities::CRTP<ImplementationT> {

    /// Use CRTP methods in the private scope
    using common::utilities::CRTP<ImplementationT>::impl;

public: /* ---------------------------------------------------- Public types ------------------------------------------------------ */

    /// Type of the associated slave interface
    using SlaveT = SlaveImplementationT;

    /// State of the Master in the ESM (EtherCAT State Machine)
    enum class State {
        Init,
        Preop,
        Safeop,
        Op
    };

    /**
     * @brief Enumeration of bus-related events that the user code can register handlers for
     */
    enum class Event {

        /// Event triggerred at the entry to @ref read_bus method
        ReadBusStart,
        /// Event triggerred when the bus-read action is completed
        ReadBusComplete,
        /// Event triggerred when the slave's input PDOs are succesfully updated after bus-read action completed
        ReadBusSlavesUpdateComplete,

        /// Event triggerred at the entry to @ref write_bus method
        WriteBusStart,
        /// Event triggerred when the slave's output PDOs are succesfully updated before bus-write action
        WriteBusSlavesUpdateComplete,
        /// Event triggerred when the bus-write action is completed
        WriteBusComplete,

    };
    
public: /* ------------------------------------------------ Public static methods ------------------------------------------------- */

    /**
     * @brief Converts @p state to the human-readable string
     * 
     * @param state 
     *    state to be converted
     * @returns 
     *    human-readable name of the state
     */
    static constexpr std::string_view state_to_str(State state);

public: /* ------------------------------------------------ Public common methods ------------------------------------------------- */

    /**
     * @brief Lists slaves registered in the Master interface
     * @returns 
     *    list of names of registered slaves
     */
    inline std::vector<std::string_view> list_slaves() const;

    /**
     * @brief Lists slaves registered in the Master interface
     * @returns 
     *    list of names of registered slaves
     */
    inline std::vector<SlaveT*> get_slaves();

    /**
     * @param name 
     *    name of the target slave
     * @returns 
     *    reference to the interface of the slave device named @p name
     * 
     * @throws std::out_of_range 
     *    if no slave named @p name is present on the bus
     */
    inline SlaveT &get_slave(std::string_view name);

    /**
     * @param name 
     *    name of the target slave
     * @returns 
     *    const reference to the interface of the slave device named @p name
     * 
     * @throws std::out_of_range 
     *    if no slave named @p name is present on the bus
     */
    inline const SlaveT &get_slave(std::string_view name) const;

    /**
     * @brief Registers custom handler for the given @p event
     * 
     * @tparam HandlerT 
     *    type of the handler functor
     * @param event 
     *    target event
     * @param handler 
     *    handler to be registered
     * 
     * @throws std::out_of_range 
     *    if invalid @p event given
     */
    template<typename HandlerT>
    void register_event_handler(Event event, HandlerT &&handler);
    
    /**
     * @brief Unregsters custom handler for the given @p event
     * 
     * @param event 
     *    target event
     * 
     * @throws std::out_of_range 
     *    if invalid @p event given
     */
    void unregister_event_handler(Event event);

public: /* -------------------------------------------- Public EtherCAT common methods -------------------------------------------- */

    /**
     * @returns 
     *     bus cycle configured in the ENI file
     */
    inline std::chrono::microseconds get_bus_cycle() const;

    /**
     * @brief Reads current state of the slave device in the ESM (EtherCAT slave machine)
     * 
     * @param timeout 
     *    access timeout
     * @returns 
     *    current state of the slave device in the ESM (EtherCAT slave machine)
     * 
     * @throws std::range_error
     *    if invalid state identifier has been returned by the master device
     * @throws implementation-specific
     *    eror on failure
     */
    inline State get_state(std::chrono::milliseconds timeout = std::chrono::milliseconds{ 100 }) const;
    
    /**
     * @brief Requestes state change of the slave device in the ESM (EtherCAT slave machine)
     * 
     * @param state 
     *    target state
     * @param timeout 
     *    access timeout
     * 
     * @throws std::range_error
     *    if invalid state has been requested
     * @throws implementation-specific
     *    eror on failure
     */
    inline void set_state(State state, std::chrono::milliseconds timeout = std::chrono::milliseconds{ 100 });

public: /* --------------------------------------------- Public EtherCAT I/O methods ---------------------------------------------- */

    
    /**
     * @returns 
     *    reference to the input buffer storing cyclical data
     * 
     * @note This metho is only for debugging purposes
     */
    inline std::vector<uint8_t> &_get_input_buffer();
    
    /**
     * @returns 
     *    reference to the input buffer storing cyclical data
     * 
     * @note This metho is only for debugging purposes
     */
    inline const std::vector<uint8_t> &_get_input_buffer() const;
    
    /**
     * @returns 
     *    reference to the output buffer storing cyclical data
     * 
     * @note This metho is only for debugging purposes
     */
    inline std::vector<uint8_t> &_get_output_buffer();
    
    /**
     * @returns 
     *    reference to the output buffer storing cyclical data
     * 
     * @note This metho is only for debugging purposes
     */
    inline const std::vector<uint8_t> &_get_output_buffer() const;

    /**
     * @brief Reads Input Process Data Image from the bus updating slave's input PDOs after I/O.
     * @details Behavioural procedure is as follows:
     * 
     *       1) call 'read start' handler
     *       4) perform reading
     *       5) call 'read end' handler
     *       2) update all slaves from input PDI
     *       3) call 'input PDO update end' handler
     * 
     * @param[in] timeout
     *    timeout of the I/O operation
     * 
     * @throws implementation-specific
     *    eror on failure
     */
    void read_bus(std::chrono::milliseconds timeout);

    /**
     * @brief Writes Output Process Data Image to the bus buffer (firstly updates slave's output PDOs)
     * @details Behavioural procedure is as follows:
     * 
     *       1) call 'write start' handler
     *       2) update output PDI from all slaves
     *       3) call 'output PDO update end' handler
     *       4) perform writting
     *       5) call 'write end' handler
     * 
     * @param[in] timeout
     *    timeout of the I/O operation
     * 
     * @throws implementation-specific
     *    eror on failure
     */
    void write_bus(std::chrono::milliseconds timeout);

protected: /* --------------------------------------------- Protected ctors & dtors ----------------------------------------------- */

    /**
     * @brief Construct a new Master interface
     *
     * @tparam SlaveFactoryT 
     *    type of the @p slave_factory functor
     * @param eni_path
     *    path to the ENI file describing the bus to be parsed
     * @param slave_factory 
     *    factory-like functor provided by the implementation class that can construct
     *    implementation-specific slave interfaces (see [1])
     *
     * @throws eni::Error
     *    if ENI file could not be loaded
     * 
     * @see [1] Master(eni::Configuration &&, SlaveFactoryT)
     */
    template<typename SlaveFactoryT>
    inline Master(const std::filesystem::path &eni_path, SlaveFactoryT&& slave_factory);

    /**
     * @brief Construct a new Master interface
     *
     * @tparam SlaveFactoryT 
     *    type of the @p slave_factory functor
     * @param eni
     *    string object containing ENI describing the bus
     * @param slave_factory 
     *    factory-like functor provided by the implementation class that can construct
     *    implementation-specific slave interfaces (see [1])
     *
     * @throws eni::Error
     *    if ENI file could not be loaded
     * 
     * @see [1] Master(eni::Configuration &&, SlaveFactoryT)
     */
    template<typename SlaveFactoryT>
    inline Master(const std::string &eni, SlaveFactoryT&& slave_factory);

    /**
     * @brief Construct a new Master interface
     *
     * @tparam SlaveFactoryT 
     *    type of the @p slave_factory functor
     * @param stream
     *    stream to load ENI file describing the bus from
     * @param slave_factory 
     *    factory-like functor provided by the implementation class that can construct
     *    implementation-specific slave interfaces (see [1])
     *
     * @throws eni::Error
     *    if ENI file could not be loaded
     * 
     * @see [1] Master(eni::Configuration &&, SlaveFactoryT)
     */
    template<typename SlaveFactoryT>
    inline Master(std::basic_istream<char> &stream, SlaveFactoryT&& slave_factory);

    /// Disable copy-construction semantic
    Master(const Master &rmaster) = delete;
    /// Disable copy-asignment semantic
    Master &operator=(const Master &rmaster) = delete;
    /// Disable move-construction semantic
    Master(Master &&rmaster) = delete;
    /// Disable move-asignment semantic
    Master &operator=(Master &&rmaster) = delete;

protected: /* ------------------------------------------------ Protected data ----------------------------------------------------- */

    /// Bus cycle in [us]
    const std::chrono::microseconds bus_cycle;

private: /* ------------------------------------------------ Private ctors & dtors ------------------------------------------------ */

    /**
     * @brief Actual constructor used underthehood by all protected ones after
     *    loading an ENI configuration
     *
     * @tparam SlaveFactoryT 
     *    type of the @p slave_factory functor
     * @param eni
     *    parsed eni configuration
     * @param slave_factory 
     *    factory-like functor provided by the implementation class that can construct
     *    implementation-specific slave interfaces. It should provide call operator with
     *    the following signature:
     * 
     *        template<typename PdoEntryRegistrationProxyT>
     *        SlaveT operator()(
     *            eni::Slave slave_eni,
     *            PdoEntryRegistrationProxyT make_entry
     *        );
     * 
     * @throws eni::Error 
     *    if inconsistency has been found in the @p eni configuration
     * @throws error  
     *    whatever @p slave_factory throws
     */
    template<typename SlaveFactoryT>
    Master(eni::Configuration &&eni, SlaveFactoryT&& slave_factory);

private: /* -------------------------------------------------- Private types ------------------------------------------------------ */

    /**
     * @brief Auxiliary class wrapping a dynamic-size bytes-buffer storing process data 
     *    exchanged with the bus
     */
    struct ProcessDataImageBuffer {

        /// Synchronisation lock
        config::types::Lock lock;
        /// Data bufffer
        std::vector<uint8_t> data;

        /// Constructs buffer of the given size with data bytes reset to @c 0
        inline ProcessDataImageBuffer(std::size_t size) :
            data(size, static_cast<uint8_t>(0))
        { }

    };

private: /* --------------------------------------------------- Private data ------------------------------------------------------ */

    /// Input Process Data Image buffer
    ProcessDataImageBuffer input_pdi;
    /// Output Process Data Image buffer
    ProcessDataImageBuffer output_pdi;

    /// List of slave interfaces representing devices on the bus
    std::vector<SlaveT> slaves;

    /**
     * @brief Set of handlers for master-related events
     */
    struct {

        /// Handler triggerred at the entry to @ref read_bus method
        common::handlers::EventHandler at_read_bus_start;
        /// Handler triggerred when the bus-read action is completed
        common::handlers::EventHandler at_read_bus_complete;
        /// Handler triggerred when the slave's input PDOs are succesfully updated after bus-read action
        common::handlers::EventHandler at_read_bus_slaves_update_complete;

        /// Handler triggerred at the entry to @ref write_bus method
        common::handlers::EventHandler at_write_bus_start;
        /// Handler triggerred when the slave's output PDOs are succesfully updated before bus-write action
        common::handlers::EventHandler at_write_bus_slaves_update_complete;
        /// Handler triggerred when the bus-write action is completed
        common::handlers::EventHandler at_write_bus_complete;

    } handlers;

};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/master/master_priv.hpp"
#include "ethercat/master/master_prot.hpp"
#include "ethercat/master/master_pub.hpp"

/* ================================================================================================================================ */

#endif
