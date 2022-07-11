/* ============================================================================================================================ *//**
 * @file       slave.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:11:53 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Slave class representing slave device on the EtherCAT bus
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DRIVERS_SLAVE_H__
#define __ETHERCAT_DRIVERS_SLAVE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <chrono>
// External includes
#include <range/v3/span.hpp>
// Private includes
#include "ethercat/abstract/pdo.hpp"
#include "ethercat/abstract/sdo.hpp"
#include "ethercat/eni.hpp"
#include "ethercat/drivers/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ========================================================== Data types ========================================================== */

// Foward declare Master
template<typename MasterImplementationT,typename SlaveImplementationT>
class Master;

/**
 * @brief Interface class template representing a slave device on the EtherCAt bus
 * 
 * @tparam ImplementationT
 *    type of the class implementing hardware-specifc routines related to Slave device 
 *    management; this class neds to derive from Slave class; for interface required to
 *    be implemented see @ref ethercat::drivers::impl::Slave
 * 
 * @note Template utilizes CRTP pattern
 */
template<typename ImplementationT>
class Slave {

public: /* ---------------------------------------------------- Public types ------------------------------------------------------ */

    /// Slave's state (in EtherCAT State Machine [ECM])
    enum class State {
        Init,
        Preop,
        Boot,
        Safeop,
        Op
    };

    /**
     * @brief Enumeration of bus-related events that the user code can register handlers for
     */
    enum class Event {

        /// Event triggerred after updating input PDOs of the slave
        InputsUpdate,
        /// Event triggerred before updating output PDOs of the slave
        OutputsUpdate,

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

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */
    
    /**
     * @brief Move constructor (to enable storing slave in dynamic containers)
     * 
     * @param rslave 
     *    slaved to be moved
     */
    inline Slave(Slave &&rslave);
    
    /**
     * @brief Move-asign operator
     * 
     * @param rslave 
     *    slaved to be moved
     */
    inline Slave &operator=(Slave &&rslave);

public: /* ------------------------------------------------ Public common methods ------------------------------------------------- */

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
     *    ENI configuration of the slave
     */
    inline eni::Slave get_eni() const;

    /**
     * @returns 
     *    name of the slave
     */
    inline std::string_view get_name() const;
    
    /**
     * @returns 
     *    fixed address of the slave
     */
    inline uint16_t get_fixed_addr() const;
    
    /**
     * @returns 
     *    auto-increment address of the slave
     */
    inline uint16_t get_auto_increment_addr() const;
    
    /**
     * @returns 
     *    topological address of the slave
     */
    inline uint16_t get_topological_addr() const;

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
     *    if invalid state identifier has been returned by the hardware
     */
    inline State get_state(std::chrono::milliseconds timeout = std::chrono::milliseconds{ 100 });
    
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
    inline void set_state(State state, std::chrono::milliseconds timeout = std::chrono::milliseconds{ 100 });

public: /* --------------------------------------------- Public access methods (SDO) ---------------------------------------------- */

    /**
     * @brief Downloads (writes) @p data to the slave's object via SDO access
     * 
     * @tparam type 
     *    type of the target object
     * @param request 
     *    request structure
     * @param data 
     *    data to be downloaded into the object
     * @param timeout 
     *    operation timeout
     * 
     * @throws Error 
     *    on error or timeout
     */
    template<abstract::objects::Type type>
    inline void download_object(
        const abstract::sdo::Request &request,
        abstract::objects::ValueType<type> data,
        std::chrono::milliseconds timeout = std::chrono::seconds{ 1 }
    );

    /**
     * @brief Uploads (reads) @p data from the slave's object via SDO access
     * 
     * @tparam type 
     *    type of the target object
     * @param request 
     *    request structure
     * @param timeout 
     *    operation timeout
     * @returns 
     *    current value of the uploaded object
     * 
     * @throws Error 
     *    on error or timeout
     */
    template<abstract::objects::Type type>
    inline abstract::objects::ValueType<type> upload_object(
        const abstract::sdo::Request &request,
        std::chrono::milliseconds timeout = std::chrono::seconds{ 1 }
    );

public: /* --------------------------------------------- Public access methods (PDO) ---------------------------------------------- */

    /**
     * @brief Obtains reference object providing access to the PDO obejct of the slave device
     * 
     * @tparam dir 
     *    direction of the PDI (Process Data Image) that the obejct is mapped into
     * @tparam type 
     *    data type of the object
     * @param name 
     *    name of the object (used in ENI file)
     * 
     * @retval reference
     *    thread-safe reference to the given PDO on success
     * @retval empty
     *    empty optional if PDO named @p name has not been mapped into the given PDI
     * 
     * @throws abstract::objects::TypeError
     *    when @p type does not match actual type of the PDO named @p name
     */
    template<abstract::pdo::Direction dir, abstract::objects::Type type>
    inline std::optional<abstract::objects::ValueBufferReference<type>> get_pdo_reference(std::string_view name);

    /**
     * @brief Registers update-handler for the given PDO. For input PDOs (Tx direction) handler is called
     *    each time AFTER Master reads PDO from the bus. Read value is passes as an argument. For output
     *    PDOs (Rx direction) handler is called each time BEFORE Master writes PDOs to the bus. Return
     *    value of the handler is treated as a new value for the PDO.
     * 
     * @tparam dir 
     *    PDO's direction
     * @tparam type 
     *    PDO type
     * @param name 
     *    PDO name 
     * @param handler 
     *    handler to be set
     * 
     * @retval true 
     *    if register was successfully registered
     * @retval false 
     *    if no object with the given @p name is configured in the slave's pdo
     * 
     * @throws abstract::objects::TypeError
     *    when @p type does not match actual type of the PDO named @p name
     */
    template<abstract::pdo::Direction dir, abstract::objects::Type type>
    inline bool register_pdo_handler(std::string_view name, abstract::pdo::HandlerType<dir, type> handler);

    /**
     * @brief Unregisters update-handler for the given PDO. For input PDOs (Tx direction) handler is called
     *    each time AFTER Master reads PDO from the bus. Read value is passes as an argument. For output
     *    PDOs (Rx direction) handler is called each time BEFORE Master writes PDOs to the bus. Return
     *    value of the handler is treated as a new value for the PDO.
     * 
     * @tparam dir 
     *    PDO's direction
     * @tparam type 
     *    PDO type
     * @param name 
     *    PDO name 
     * 
     * @retval true
     *   if register was successfully unregistered
     * @retval false
     *   if no object with the given @p name is configured in the slave's pdo
     * 
     * @throws abstract::objects::TypeError
     *    when @p type does not match actual type of the PDO named @p name
     */
    template<abstract::pdo::Direction dir, abstract::objects::Type type>
    inline bool unregister_pdo_handler(std::string_view name);

protected: /* --------------------------------------------- Protected ctors & dtors ----------------------------------------------- */

    /// Make Master a friend to let it access constructor and update methods
    template<typename MasterImplementationT,typename SlaveImplementationT>
    friend class Master;

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
     * 
     * 
     * @details To increase decoupling between @ref Slave and @ref Master class, Slave interfaces are constructed
     *    only with access to it's own description parsed from the ENI file. Such a slave interfaces inspects
     *    the description looking for these of PDOs descibed there that has been assigned to a SyncManager
     *    (i.e. are present in the Process Data Image). Next, it can decide whether it wants to handle all 
     *    Objects mapped to assigned PDOs or only some of them (see [1]). 
     * 
     *    Slave cannot directly manage Process Data Image (PDI), as it could manipulated data that should be
     *    otherwise managed by other slaves and it could result in safetu issueas. This is why Slave needs to 
     *    @b request Master, who is responsible for managing PDI, to register handlers to the mapped objects 
     *    (see [2]) on behalf of the slave. Direct request would directly couple Slave with Master's API which
     *    is unnecessary. To avoid this Slave requires only access to some arbitrary @b proxy object that can
     *    provide such entries registrations on behalf of the Master. This implementation decouples Slave from
     *    Master completely (the other way around only minimal API coupling resulting from @p PdoEntryRegistrationProxyT 
     *    API requirements is held)
     * 
     *    In the simples case such a proxy object can take Slave's request and create reference to the requested
     *    object. It however provides also a convinient entry to verify whether parsed ENI configuration files
     *    is consistent. Proxy object (as it can potentially access whole ENI description) can verify that both -
     *    objects mapping and assignment desscribed in the <Slave> section are consisent with objects described
     *    in the <ProcessImage>.
     * 
     * @note [1] At the momen Slave::Slave constructor always handles all objects mapped into assigned PDOs
     *    but to let it change in the future in a way that is decoupled from the Master class, described approach
     *    is used.
     * @note [2] Such a registered @a handle is modeled by @ref abstract::pdo::Entry class
     * 
     * @todo When C++20 is available put `require` on requirements concerning @p PdoEntryRegistrationProxyT
     * @note Form of the operator of the @p PdoEntryRegistrationProxyT was designed with C++20 lambda templates
     *    in mind. At the moment it must be implemented as an explicit functor type, but in the
     *    future it migh be implemented as a lambda template indeed an then slave's construction
     *    will be able to look like that:
     * 
     *          Slave(eni, [...]<abstract::pdo::Direction dir>(
     *              std::string_view pdo_name,
     *              eni::Slave::Pdo::Entry entry
     *          ){ ... })
     *          
     */
    template<typename PdoEntryRegistrationProxyT>
    Slave(eni::Slave slave_eni, PdoEntryRegistrationProxyT&& make_entry);

    /// Disable copy semantic
    Slave(const Slave &rslave) = delete;
    Slave &operator=(const Slave &rslave) = delete;

protected: /* ------------------------------------------------ Protected data ----------------------------------------------------- */

    /**
     * @brief ENI description of the Slave device
     * @performance Currently the ENI description of the slave is sored entirely in the memory 
     *    for entire lifetime of the Slave driver. It MAY be unnecesary performance hit in 
     *    memory-constained environments. An alternative is to make Master pass to the Slave's 
     *    constructor a functor object that would load slave-specific ENI element on-demand.
     *    This however would require per-access I/O operation that would affect performance
     *    in different way.
     */
    const eni::Slave eni_description;

    /// Slave's name
    const std::string name;

    /// Slave's fixed adress (physical address)
    const uint16_t fixed_addr;
    /// Slave's auto-increment adress
    const uint16_t auto_increment_addr;
    /// Slave's topological adress
    const uint16_t topological_addr;

private: /* ---------------------------------------- Private implementation methods (CPRT) ---------------------------------------- */

    /**
     * @brief Casts *this to the implementation type
     */
    inline ImplementationT &impl();

    /**
     * @brief Casts *this to the implementation type (const)
     */
    inline const ImplementationT &impl() const;

private: /* ---------------------------------------- Private implementation methods (PDO) ----------------------------------------- */

    /**
     * @brief Finds PDO entry correspodning to the object named @p name of type @p type
     * 
     * @tparam dir 
     *     direction of the PDI (Process Data Image) that the pdo is assigned to
     * @tparam type 
     *     type of the obejct
     * @param name 
     *     name of the object
     * 
     * @retval iterator
     *    iterator to the found pdo on success
     * @retval empty
     *    empty optional on failure
     * 
     * @throws abstract::objects::TypeError
     *    when @p type does not match actual type of the PDO named @p name
     */
    template<abstract::pdo::Direction dir, abstract::objects::Type type>
    std::optional<typename std::vector<abstract::pdo::Entry<dir>>::iterator> find_pdo_entry(std::string_view name);

private: /* ----------------------------------------------- Private methods (PDO) ------------------------------------------------- */

    /**
     * @brief Called by the master after reading data from/before writting data to the bus.
     *    Updates all PDO entry associated with the slave based on the (Input/Output) Process
     *    Data Image
     * @note This method is quaranted to be called by the master when the underlying Process
     *    Data Image can be safely accessed (i.e. associated lock is taken)
     */
    template<abstract::pdo::Direction dir>
    inline void update();

private: /* ------------------------------------------------ Private data (PDO) --------------------------------------------------- */

    /**
     * @brief Input PDO entries
     * @performance This vector is accessed on the critical path of the bus I/O loop. 
     *    switching implementation to statically-allocated container (e.g. 
     *    boost::static_vector can improve reference-locality and so cache 
     *    performance)
     */
    std::vector<abstract::pdo::Entry<abstract::pdo::Direction::Input>> inputs;

    /**
     * @brief Output PDO entries
     * @performance This vector is accessed on the critical path of the bus I/O loop. 
     *    switching implementation to statically-allocated container (e.g. 
     *    boost::static_vector can improve reference-locality and so cache 
     *    performance)
     */
    std::vector<abstract::pdo::Entry<abstract::pdo::Direction::Output>> outputs;

    /**
     * @brief Set of handlers for slave-related events
     */
    struct {

        /// Handler triggerred after updating input PDOs of the slave
        EventHandler at_inputs_update;
        /// Handler triggerred before updating output PDOs of the slave
        EventHandler at_outputs_update;

    } handlers;

};

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/drivers/slave/slave_priv.hpp"
#include "ethercat/drivers/slave/slave_prot.hpp"
#include "ethercat/drivers/slave/slave_pub.hpp"

/* ================================================================================================================================ */

#endif
