/* ============================================================================================================================ *//**
 * @file       slave.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:12:57 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Slave class implementing parsing interface of the <Slave> tag of the ENI (EtherCAT Network 
 *             Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_SLAVE_H__
#define __ETHERCAT_COMMON_ENI_SLAVE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <optional>
// Boost includes
#include "boost/property_tree/ptree.hpp"
// Private includes
#include "ethercat/types.hpp"
#include "ethercat/eni/common/error.hpp"
#include "ethercat/eni/common/element.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================= Class ============================================================ */

// Forward declare Configuration class
class Configuration;

/**
 * @class Slave
 * @brief Class providing basic parsing routines for the <Slave> description tag of the ENI 
 *    (EtherCAT Network Informations) file
 */
class Slave : protected Element {

    /// Make Configruaiton class a friend to let it spawn Slave parsers
    friend class Configuration;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Auxiliary type implementing parsing interface for <TxPdo/RxPdo> elements of the 
     *    <Slave> description
     */
    class Pdo : protected Element { 
    
        /// Make Slave class a friend to let it spawn Pdo parsers
        friend class Slave;
            
    public: /* Public types */
    
        /**
         * @brief Auxiliary enumeration identifying Input and Output PDOs (Process Data Object)
         */
        enum class Direction {
            Inputs,
            Outputs
        };
        
        /**
         * @brief Auxiliary type implementing parsing interface for <Entry> elements of the 
         *    <TxPdo/RxPdo> description
         */
        class Entry : protected Element {

            /// Make Pdo class a friend to let it spawn Entry parsers
            friend class Pdo;

        public: /* Public management methods */

            /// Let user autonomize the element
            using Element::autonomize;
            
        public: /* Public methods */

            /**
             * @returns 
             *    index of the mapped object
             *
             * @throws eni::Error
             *    if <Entry> ENI element does not contain <Index>
             */
            inline std::size_t get_index() const;

            /**
             * @returns 
             *    subindex of the mapped object
             *
             * @throws eni::Error
             *    if <Entry> ENI element does not contain <SubIndex>
             */
            inline std::size_t get_subindex() const;

            /**
             * @returns 
             *    bit length of the mapped object
             *
             * @throws eni::Error
             *    if <Entry> ENI element does not contain <BitLen>
             */
            inline std::size_t get_bit_len() const;

            /**
             * @returns 
             *    byte length of the mapped object (rounded down)
             *
             * @throws eni::Error
             *    if <Entry> ENI element does not contain <BitLen>
             */
            inline std::size_t get_byte_len() const;

            /**
             * @returns 
             *    name of the mapped object
             *
             * @throws eni::Error
             *    if <Entry> ENI element does not contain <Name>
             */
            inline std::string get_name() const;

            /**
             * @returns 
             *    data type of the mapped object
             *
             * @throws eni::Error
             *    if <Entry> ENI element does not contain <Name>
             */
            inline types::Type get_data_type() const;
                    
        protected: /* Protected ctors */

            /// Inherit all basic constructors
            using Element::Element;
            
        };

    public: /* Public management methods */

        /// Let user autonomize the element
        using Element::autonomize;
        
    public: /* Public methods */

        /**
         * @returns 
         *    direction of the PDO (from perspective of the Master)
         */
        inline Direction get_direction() const;

        /**
         * @returns 
         *    @retval SyncManager that the PDO is assigned to if it is actually assigned
         *    @retval empty optional otherwise
         */
        std::optional<std::size_t> get_sync_manager() const;

        /**
         * @returns 
         *    @retval @c true if PDO is assigned to the corresponding SM (SyncManager)
         *    @retval @c false otherwise
         */
        inline bool is_assigned() const;

        /**
         * @returns 
         *    @retval @c true if PDO mapping is fixed
         *    @retval @c false otherwise
         */
        bool is_fixed() const;

        /**
         * @returns 
         *    index of the PDO object
         *
         * @throws eni::Error
         *    if <TxPdo/RxPdo> ENI element does not contain <Index>
         */
        inline std::size_t get_index() const;
        
        /**
         * @returns 
         *    name of the PDO object
         *
         * @throws eni::Error
         *    if <TxPdo/RxPdo> ENI element does not contain <Name>
         */
        inline std::string get_name() const;
        
        /**
         * @returns 
         *    list of indeces of PDOs that cannot be mapped simultaneously with this PDO
         */
        std::vector<std::size_t> get_excludes() const;
        
        /**
         * @returns 
         *    list of entries mapped in this PDO
         */
        std::vector<Entry> get_entries() const;

    protected: /* Protected ctors */

        /// Inherit all basic constructors
        using Element::Element;

        /**
         * @brief Construct a new Pdo parser object wrapping given @p elem element
         *    of the XML tree resulting from parsing ENI file
         * 
         * @param direction
         *    PDO's direction 
         * @param elem 
         *    reference to the <TxPdo/RxPdo> node of the ENI tree
         */
        inline Pdo(Direction direction, const Element &elem);
        
    private: /* Private data */

        /// PDO's direction
        Direction direction;

    };

    /**
     * @brief Auxiliary wrapper around vector of PDOs descriptions providing additional methods
     *    for searching the list
     */
    struct PdosList : public std::vector<Pdo> {

        /**
         * @returns 
         *    sublist of PDOs that are asigned to some SyncManager
         */
        inline PdosList get_assigned() const;

    };

    /**
     * @brief Auxiliary pair-like structure holding lists of input and output PDOs
     */
    struct PdosSet {

        /// Input variables
        PdosList inputs;
        /// Output variables
        PdosList outputs;

    };

public: /* ---------------------------------------------- Public management methods ----------------------------------------------- */

    /// Let user autonomize the element
    using Element::autonomize;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @returns 
     *    name of the slave
     *
     * @throws eni::Error
     *    if <Info>.<Name> ENI element does not exist
     */
    inline std::string get_name() const;

    /**
     * @returns 
     *    physical address of the slave
     *
     * @throws eni::Error
     *    if <Info>.<PhysAddr> ENI element does not exist
     */
    inline std::size_t get_physical_addr() const;

    /**
     * @returns 
     *    auto-increment address of the slave
     *
     * @throws eni::Error
     *    if <Info>.<AutoIncAddr> ENI element does not exist
     */
    inline std::size_t get_auto_increment_addr() const;

    /**
     * @param direction 
     *    direction of the PDOs to be inspected
     * @returns 
     *    list of pdos for the given @p direction defined by the slave
     */
    PdosList get_pdos(Pdo::Direction direction) const;

    /**
     * @returns 
     *    set of pdos defined by the slave
     */
    inline PdosSet get_pdos() const;

    /**
     * @param direction 
     *    direction of the PDOs to be inspected
     * @returns 
     *    list of pdos for the given @p direction defined by the slave assigned to the associated
     *    SM (SyncManager)
     */
    inline PdosList get_assigned_pdos(Pdo::Direction direction) const;

    /**
     * @returns 
     *    set of pdos defined by the slave assigned to the associated SM (SyncManager)
     */
    inline PdosSet get_assigned_pdos() const;

protected: /* ---------------------------------------------- Protected ctors & dtors ---------------------------------------------- */

    /// Inherit all basic constructors
    using Element::Element;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/slave/entry.hpp"
#include "ethercat/eni/slave/pdo.hpp"
#include "ethercat/eni/slave/slave.hpp"

/* ================================================================================================================================ */

#endif
