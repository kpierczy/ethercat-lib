/* ============================================================================================================================ *//**
 * @file       entry.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 12th May 2022 3:43:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the helper wrapper class around Process Data Image buffer poviding synchronisation
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_ENTRY_H__
#define __ETHERCAT_ABSTRACT_PDO_ENTRY_H__

/* =========================================================== Includes =========================================================== */

// External includes
#include "range/v3/span.hpp"
// Private includes
#include "ethercat/abstract/objects.hpp"
#include "ethercat/abstract/pdo.hpp"

/* ========================================================= Declarations ========================================================= */

// Forward declare Slave class responsible for managin the Entry
namespace ethercat::drivers { 
    template<typename ImplementationT>
    class Slave;
}

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::pdo {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Helper class representing named PDO entry (cyclical data object) with thread-safe access 
 *    and associated update handler
 * @details Entry class provides complete functionality for managing cyclical objects mapped in the
 *    Process Data Image. It covers 
 *     
 *         - automatic, type-safe serialization/deserialization of objects
 *         - thread-safe buffer isolating client code from the underlying Process
 *           Data Image buffer ( such a buffer decouples manager of the Process
 *           Data Image that controls cyclical bus I/O from client code that
 *           processes the Entry, see @performance [1] )
 *         - managin associated handler automatically called at after update
 *           of the Entry in Output PDI / before updating the Entry from the 
 *           Input PDI
 * 
 * @tparam dir 
 *    PDO direction
 * 
 * @note Class does NOT provide synchronisation of the access to the underlying PDI. It's synchronisation
 *    is required by the client code.
 * @performance [1] Entry class ( in particular update() method ) is utilized on the critical 
 *    path of the bus I/O loop. It's implementation is fully thread-safe and provides unbound 
 *    access to the PDO value from both sides (bus Master and Host app) due to 3-bufferred 
 *    implementation. As a consequence, execution performance suffers from 
 *    
 *      - additional data copying in both Input and Output direction; it especially
 *        influence Output direction as output data are copied at every bus Write 
 *        and not only when Host updates output value
 *      - additional locking synchronising the buffer (this is probably small drawback
 *        as for such short critical sections spinlocks can be used)
 */
template<Direction dir>
class Entry {

public: /* --------------------------------------------------- Public constants --------------------------------------------------- */

    /// Direction of the PDOs handled by the type
    static constexpr Direction Dir { dir };

public: /* ---------------------------------------------------- Deleted types ----------------------------------------------------- */

    /// Handler type utilized by the class
    using HandlerType = abstract::pdo::Handler<dir>;

public: /* --------------------------------------------------- Deleted methods ---------------------------------------------------- */

    /// Disable copying
    Entry(const Entry& rentry) = delete;
    Entry &operator=(const Entry& rentry) = delete;
    // Disable move-assignment
    Entry &operator=(Entry&& rentry) = delete;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Constructs new PDO entry of type @p type refereing to data in @p data buffer
     * 
     * @param name 
     *    name of the entry enabling it's identification in the client code
     * @param type 
     *    type of the entry data 
     * @param data 
     *    reference to the buffer in the PDI that the entry is mapped into
     */
    inline Entry(
        std::string_view name,
        abstract::objects::Type type, 
        ranges::span<uint8_t> data
    );

    /**
     * @brief Constructs new PDO entry of type @p type refereing to data in @p data buffer with
     *    the initial @p handle
     * 
     * @tparam Handler
     *    type of the @p handler ; this needs to be specialization of the abstract::pdo::Handler<dir, type>
     *    template corresponding to the @p type 
     * @param name 
     *    name of the entry enabling it's identification in the client code
     * @param type 
     *    type of the entry data 
     * @param data 
     *    reference to the buffer in the PDI that the entry is mapped into
     * @param handler 
     *    initial handler to be set 
     */
    template<typename Handler>
    inline Entry(
        std::string_view name,
        abstract::objects::Type type, 
        ranges::span<uint8_t> data,
        Handler&& handler
    );

    /**
     * @brief Enable moving to allow emplacing back Entries in the std::vector
     * 
     * @param rentry 
     *    entry to be moved
     * 
     * @warning Entry contains internal lock that synchronisin access to the handler
     *    functor. Usually synchronisation primitives should not be moved as it implies need
     *    of some communication protocol between synchronised threads to ensure that all
     *    of them refer to the same primitve. The move-constructor for the Entry class 
     *    has been introduced @b only to allow storing such locks in dynamically-allocated
     *    containers that can be reallocated during it's lifetime. Entry should not be
     *    moved it any other context.
     */
    inline Entry(Entry&& rentry);

public: /* --------------------------------------------- Public methods (client code) --------------------------------------------- */

    /**
     * @returns 
     *    name of the PDO entry
     */
    inline std::string_view get_name() const;

    /**
     * @returns 
     *    type of the PDO entry
     */
    inline abstract::objects::Type get_type() const;

    /**
     * @returns 
     *    reference to the buffer utilized by the entry
     */
    inline abstract::objects::ValueBuffer &get_reference();

    /**
     * @returns 
     *    reference to the buffer utilized by the entry
     */
    inline const abstract::objects::ValueBuffer &get_reference() const;

    /**
     * @returns 
     *    reference to the buffer utilized by the entry
     */
    template<abstract::objects::Type type>
    inline abstract::objects::ValueBufferReference<type> get_reference() const;

    /**
     * @brief Registers given update @p handler for the entry dispatching handler @p type at compile time
     * 
     * @tparam type 
     *    type of the Entry (must match one given at construction)
     * @param handler 
     *   handler to be set
     */
    template<abstract::objects::Type type>
    inline void register_pdo_handler(const HandlerType &handler);

    /**
     * @brief Registers given update @p handler for the entry dispatching handler type at runtime
     * 
     * @tparam Handler 
     *    type of the @p handler
     * @param handler 
     *   handler to be set
     */
    template<typename Handler>
    inline void register_pdo_handler(Handler handler);
    
    /**
     * @brief Unregisters current PDO handler using compile-time known type of the Entry
     * 
     * @tparam type 
     *    type of the Entry (must match one given at construction)
     */
    template<abstract::objects::Type type>
    inline void unregister_pdo_handler();
    
    /**
     * @brief Unregisters current PDO handler figuring out type of the Entry at runtime
     */
    inline void unregister_pdo_handler();

private: /* -------------------------------------------- Private methods (PDI manager) -------------------------------------------- */

    /// Make Slave class a friend to let it manage entrie's updates
    template<typename ImplementationT>
    friend class ::ethercat::drivers::Slave;

    /**
     * @brief Updates internal entry buffer from raw data. Calls handler, if registered
     * 
     * @note When updating, make sure that the reference PDI can be safely accessed by the object
     */
    void update();

private: /* ---------------------------------------------------- Private data ----------------------------------------------------- */

    /// Name of the PDO Entry in the ENI file
    std::string name;
    /// Object represented by the entry
    abstract::objects::Object object;

    /**
     * @var handler_lock
     * @performance Think through synchronisation model of the @a handler_lock and whether
     *    it should by implemented in a blocking or non-blocking amnenr
     */

    /// Lock used to synchronise access to the handler
    config::Lock handler_lock;
    /// Optional handler called at PDO update
    HandlerType handler;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/pdo/entry/entry.hpp"

/* ================================================================================================================================ */

#endif
