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

#ifndef __ETHERCAT_SLAVE_H__
#define __ETHERCAT_SLAVE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <chrono>
// Private includes
#include "ethercat/common/utilities/crtp.hpp"
#include "ethercat/common/handlers/event_handler.hpp"
#include "ethercat/eni.hpp"
#include "ethercat/slave/translators_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

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
class Slave : public common::utilities::CRTP<ImplementationT> {

    /// Use CRTP methods in the private scope
    using common::utilities::CRTP<ImplementationT>::impl;

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
    
    /**
     * @brief Enumeration identifying direction of SDO objects
     */
    enum class SdoDirection {
        Upload        = slave::traits::Direction::Input,
        Download      = slave::traits::Direction::Output,
        Bidirectional = slave::traits::Direction::Bidirectional
    };

    /**
     * @brief Auxiliary class representing an SDO object of the slave
     * 
     * @tparam dir 
     *    target direction of the SDO
     * @tparam TranslatorT 
     *    translator type used to translate object between binary image and app-domain entity
     * @tparam T 
     *    type used to represent object in the app-domain ( automatically deduced from @p TranslatorT 
     *    if @c void )
     */
    template<SdoDirection dir, typename TranslatorT, typename T = void,
        slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> = true>
    class Sdo;

    /**
     * @brief Auxiliary alias for Sdo template specialized with the default translator for the given type
     */
    template<SdoDirection dir, typename T>
    using DefaultTranslatedSdo = 
        Sdo<dir, common::translation::DefaultTranslator<T>, T>;

    /**
     * @brief Auxiliary alias for Sdo template specialized with the default translator for the typed deduced
     *    from the ID of EtherCAT builtin type
     */
    template<SdoDirection dir, types::BuiltinType::ID type_id, std::size_t arity = 0>
    using BuiltinTypeSdo = 
        DefaultTranslatedSdo<dir, common::types::traits::TypeRepresentation<type_id, arity>>;
    
    /**
     * @brief Enumeration identifying direction of PDO objects
     */
    enum class PdoDirection {
        Input  = slave::traits::Direction::Input,
        Tx     = Input,
        Output = slave::traits::Direction::Output,
        Rx     = Output
    };

    /**
     * @brief Auxiliary class representing an PDO object of the slave
     * 
     * @tparam dir 
     *   target direction of the PDO
     */
    template<PdoDirection dir>
    class Pdo;

private: /* -------------------------------------------------- Private types ------------------------------------------------------ */

    /// Auxiliary trait chekcing whether @p T is specialization of Sdo template
    template <typename T>
    struct is_sdo : std::false_type { };
    
    /// Auxiliary trait chekcing whether @p T is specialization of Sdo template
    template <SdoDirection dir, typename TranslatorT, typename T>
    struct is_sdo<Sdo<dir, TranslatorT, T>> : std::true_type { };

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
    
    /// Disable copy-construction
    inline Slave(const Slave &rslave) = delete;
    /// Disable copy-asignment
    inline Slave &operator=(const Slave &rslave) = delete;

    /// Enable move-construction (to enable storing slave in relocatable containers)
    inline Slave(Slave &&rslave) = default;
    /// Enable move-asignment (to enable storing slave in relocatable containers)
    inline Slave &operator=(Slave &&rslave) = default;

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
     * @throws std::range_error
     *    if invalid state identifier has been returned by the hardware
     * @throws error 
     *    whatever error thrown by the underlying implementation
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
     * @throws error 
     *    whatever error thrown by the underlying implementation
     */
    inline void set_state(State state, std::chrono::milliseconds timeout = std::chrono::milliseconds{ 100 });

public: /* --------------------------------------------- Public access methods (SDO) ---------------------------------------------- */

    /**
     * @brief Constructs an SDO proxy object associated with @p this slave that can be used to
     *    read/write data from/to the device
     * @details This overload constructs Sdo using a custom translator to convert data
     *    between binary image and app-domain entity of an <i>automatically deduced</i> type
     * 
     * @param address 
     *    adress of the SDO object
     * @returns 
     *    SDO proxy object associated with @p this slave
     * 
     * @see See description of Sdo 
     * @see See description of translation namespace
     */
    template<SdoDirection dir, typename TranslatorT, typename... ArgsT,
        slave::traits::enable_if_sizing_translator_t<dir, TranslatorT> = true>
    inline Sdo<dir, TranslatorT> get_sdo(
        uint16_t index,
        uint16_t subindex = 0,
        ArgsT&&... args
    );

    /// @overload Sdo<dir, TranslatorT> get_sdo(Sdo<dir, TranslatorT>::Address, ArgsT&&...)
    template<SdoDirection dir, typename TranslatorT, typename... ArgsT,
        slave::traits::enable_if_sizing_translator_t<dir, TranslatorT> = true>
    inline const Sdo<dir, TranslatorT> get_sdo(
        uint16_t index,
        uint16_t subindex = 0,
        ArgsT&&... args
    ) const;

    /**
     * @overload Sdo<dir, TranslatorT> get_sdo(Sdo<dir, TranslatorT>::Address, ArgsT&&... args)
     * @details This overload constructs Sdo using a custom translator to convert data
     *    between binary image and app-domain entity of a <i>given</i> type
     */
    template<SdoDirection dir, typename TranslatorT, typename T, typename... ArgsT,
        slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> = true>
    inline Sdo<dir, TranslatorT, T> get_sdo(
        uint16_t index,
        uint16_t subindex = 0,
        ArgsT&&... args
    );

    /// @overload Sdo<dir, TranslatorT, T> get_sdo(Sdo<dir, TranslatorT, T>::Address, ArgsT&&...)
    template<SdoDirection dir, typename TranslatorT, typename T, typename... ArgsT,
        slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> = true>
    inline const Sdo<dir, TranslatorT, T> get_sdo(
        uint16_t index,
        uint16_t subindex = 0,
        ArgsT&&... args
    ) const;

    /**
     * @overload Sdo<dir, TranslatorT> get_sdo(Sdo<dir, TranslatorT>::Address, ArgsT&&... args)
     * @details This overload constructs Sdo using a <i>default stateless</i> translator to convert
     *     data between binary image and app-domain entity of an <i>given</i> type.
     * 
     * @note This overload is disabled if @p T is not supported by the default translator
     */
    template<SdoDirection dir, typename T,
        slave::traits::enable_if_default_translatable_t<T> = true>
    inline DefaultTranslatedSdo<dir, T> get_sdo(
        uint16_t index,
        uint16_t subindex = 0
    );

    /// @overload DefaultTranslatedSdo<dir, T> get_sdo(DefaultTranslatedSdo<dir, T>::Address)
    template<SdoDirection dir, typename T,
        slave::traits::enable_if_default_translatable_t<T> = true>
    inline const DefaultTranslatedSdo<dir, T> get_sdo(
        uint16_t index,
        uint16_t subindex = 0
    ) const;

    /**
     * @overload Sdo<dir, TranslatorT> get_sdo(Sdo<dir, TranslatorT>::Address, ArgsT&&... args)
     * @details This overload constructs Sdo using a <i>default stateless</i> translator to convert
     *     data between binary image and app-domain entity of a default type used to represent
     *     given <i>EtherCAT builtin type</i> type with the given @p arity
     * 
     * @tparam type_id 
     *    ID of the target EtherCAT builtin type
     * @tparam arity  
     *    arity of the target type ( @c 0 for scalars )
     * 
     * @note This overload is disabled if default representation of the given EtherCAT builtin type
     *    is not supported by the default translator (should not happen, but check it just in case)
     */
    template<SdoDirection dir, types::BuiltinType::ID type_id, std::size_t arity = 0,
        slave::traits::enable_if_builtin_default_translatable_t<type_id, arity> = true>
    inline BuiltinTypeSdo<dir, type_id, arity> get_sdo(
        uint16_t index,
        uint16_t subindex = 0
    );

    /// @overload BuiltinTypeSdo<dir, type_id, arity> get_sdo(BuiltinTypeSdo<dir, type_id, arity>::Address)
    template<SdoDirection dir, types::BuiltinType::ID type_id, std::size_t arity = 0,
        slave::traits::enable_if_builtin_default_translatable_t<type_id, arity> = true>
    inline const BuiltinTypeSdo<dir, type_id, arity> get_sdo(
        uint16_t index,
        uint16_t subindex = 0
    ) const;

    /**
     * @overload Sdo<dir, TranslatorT> get_sdo(Sdo<dir, TranslatorT>::Address, ArgsT&&... args)
     * @details This is helper overload enabling deduction of translator and target types from type 
     *    of the given @p SdoType type
     * 
     * @tparam SdoType 
     *    type of the target Sdo interface
     */
    template<typename SdoType, bool enable = 
            is_sdo<common::utilities::traits::remove_cvref_t<SdoType>>::value,
        std::enable_if_t<enable, bool> = true>
    inline auto get_sdo(
        uint16_t index,
        uint16_t subindex = 0
    );

    /// @overload auto get_sdo(typename SdoType::Address address)
    template<typename SdoType, bool enable = 
            is_sdo<common::utilities::traits::remove_cvref_t<SdoType>>::value,
        std::enable_if_t<enable, bool> = true>
    inline const auto get_sdo(
        uint16_t index,
        uint16_t subindex = 0
    ) const;
    
public: /* --------------------------------------------- Public access methods (PDO) ---------------------------------------------- */

    /**
     * @tparam dir 
     *    direction of obtained PDOs
     * 
     * @returns 
     *    reference to the set of slave's PDOs
     */
    template<PdoDirection dir>
    std::vector<Pdo<dir>> &get_pdos();

    /**
     * @tparam dir 
     *    direction of obtained PDOs
     * 
     * @returns 
     *    const reference to the set of slave's PDOs
     */
    template<PdoDirection dir>
    const std::vector<Pdo<dir>> &get_pdos() const;

    /**
     * @tparam dir 
     *    direction of obtained PDO
     * @param name
     *    name of the target PDO
     * 
     * @retval true 
     *    if slave has PDO named @p name
     * @retval false 
     *    otherwise
     */
    template<PdoDirection dir>
    bool has_pdo(std::string_view name) const;

    /**
     * @tparam dir 
     *    direction of obtained PDO
     * @param name
     *    name of the target PDO
     * 
     * @returns 
     *    reference to the slave's PDO with the given @p name
     * 
     * @throws std::out_of_range 
     *    if no PDO named @p name is associated with the slave
     */
    template<PdoDirection dir>
    Pdo<dir> &get_pdo(std::string_view name);

    /**
     * @tparam dir 
     *    direction of obtained PDO
     * @param name
     *    name of the target PDO
     * 
     * @returns 
     *    const reference to the slave's PDO with the given @p name
     * 
     * @throws std::out_of_range 
     *    if no PDO named @p name is associated with the slave
     */
    template<PdoDirection dir>
    const Pdo<dir> &get_pdo(std::string_view name) const;

    /**
     * @tparam dir 
     *    direction of obtained PDO entry
     * @param name
     *    name of the target PDO entry
     * 
     * @returns 
     *    reference to the first slave's PDO entry with the given @p name
     * 
     * @throws std::out_of_range 
     *    if no PDO entry named @p name is associated with the slave
     */
    template<PdoDirection dir>
    typename Pdo<dir>::Entry &get_pdo_entry(std::string_view name);

    /**
     * @tparam dir 
     *    direction of obtained PDO entry
     * @param name
     *    name of the target PDO entry
     * 
     * @returns 
     *    const reference to the first slave's PDO entry with the given @p name
     * 
     * @throws std::out_of_range 
     *    if no PDO entry named @p name is associated with the slave
     */
    template<PdoDirection dir>
    const typename Pdo<dir>::Entry &get_pdo_entry(std::string_view name) const;

protected: /* --------------------------------------------- Protected ctors & dtors ----------------------------------------------- */

    /// Make Master a friend to let it access constructor and update methods
    template<typename MasterImplementationT,typename SlaveImplementationT>
    friend class Master;

    /**
     * @brief Construct a new Slave interface
     * 
     * @param slave_eni 
     *    parsing interface for the slave's description present in ENI file
     * @param inputs 
     *    input PDOs associated with the slave
     * @param outputs 
     *    output PDOs associated with the slave
     */
    inline Slave(
        eni::Slave slave_eni,
        std::vector<Pdo<PdoDirection::Input>> &&inputs,
        std::vector<Pdo<PdoDirection::Output>> &&outputs
    );

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
    eni::Slave eni_description;

    /// Slave's name
    std::string name;

    /// Slave's fixed adress (physical address)
    uint16_t fixed_addr;
    /// Slave's auto-increment adress
    uint16_t auto_increment_addr;
    /// Slave's topological adress
    uint16_t topological_addr;

private: /* ----------------------------------------------- Private methods (PDO) ------------------------------------------------- */

    /**
     * @brief Notifies Slave that new bus cycle has started and it's entries should be updated. 
     * @details This method is called directly by the Master class when new bus cycle has started. 
     *    For Input direction this is @b after data has been read from the bus and PDO Entries 
     *    objects has been updated by the Master. For Output direction this is @b before PDO Entries 
     *    objects are requested by the Master to update PDI and data is written to the bus.
     * 
     * @tparam dir
     *    direction for which associated event occured
     */
    template<PdoDirection dir>
    inline void notify();

private: /* ------------------------------------------------ Private data (PDO) --------------------------------------------------- */

    /**
     * @brief Input PDO entries
     * @performance This vector is accessed on the critical path of the bus I/O loop. 
     *    switching implementation to statically-allocated container (e.g. 
     *    boost::static_vector can improve reference-locality and so cache 
     *    performance)
     */
    std::vector<Pdo<PdoDirection::Input>> inputs;

    /**
     * @brief Output PDO entries
     * @performance This vector is accessed on the critical path of the bus I/O loop. 
     *    switching implementation to statically-allocated container (e.g. 
     *    boost::static_vector can improve reference-locality and so cache 
     *    performance)
     */
    std::vector<Pdo<PdoDirection::Output>> outputs;

    /**
     * @brief Set of handlers for slave-related events
     */
    struct {

        /// Handler triggerred after updating input PDOs of the slave
        common::handlers::EventHandler at_inputs_update;
        /// Handler triggerred before updating output PDOs of the slave
        common::handlers::EventHandler at_outputs_update;

    } handlers;

};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/slave/sdo.hpp"
#include "ethercat/slave/pdo.hpp"
#include "ethercat/slave/slave_priv.hpp"
#include "ethercat/slave/slave_prot.hpp"
#include "ethercat/slave/slave_pub.hpp"

/* ================================================================================================================================ */

#endif
