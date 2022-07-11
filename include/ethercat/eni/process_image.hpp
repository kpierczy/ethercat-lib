/* ============================================================================================================================ *//**
 * @file       process_image.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:11:21 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the ProcessImage class implementing parsing interface of the <ProcessImage> tag of the ENI 
 *             (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_PROCESS_IMAGE_H__
#define __ETHERCAT_COMMON_ENI_PROCESS_IMAGE_H__

/* =========================================================== Includes =========================================================== */

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
 * @brief Class providing basic parsing routines for the <ProcessImage> description tag of the ENI 
 *    (EtherCAT Network Informations) file
 */
class ProcessImage : protected Element {

    /// Make Configruaiton class a friend to let it spawn ProcessImage parsers
    friend class Configuration;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Auxiliary enumeration identifying Input and Output PDI (Process Data Image)
     */
    enum class Direction {
        Inputs,
        Outputs
    };

    /**
     * @brief Auxiliary class parsing a single entry of the list stored by <Inputs>/<Outputs> element
     *    of the <ProcessImage>
     */
    class Variable : protected Element{

        // Make ProcessImage class a friend to let it spawn ProcessImage parsers
        friend class ProcessImage;

    public: /* Public types */

        /**
         * @brief Auxiliary enumeration identifying type of the variable
         */
        enum class Type {
            
            /**
             * @brief 'Master' variables are not related to any slave on the bus in particular. These are contain
             *   general informations / control general parameters related to the whole communication process.
             *   Such variables are automatically generated and mapped into the Process Data Image by the
             *   TwinCAT software. They can be distinguished by nique naming scheme:
             * 
             *       '<pdo/mapping-name>.<variable-name>;
             * 
             * @note Such a naming convention is not standardized by EtherCAT and is TwinCAT specific behaviour.
             *    Implementation takes advantage of this behaviour to simplify design.
             */
            Master,

            /**
             * @brief 'Slave' variables are related to a specific slave on the bus. These are contain
             *    readings / control writes to the slave device. Such variables are generated and mapped into the
             *    Process Data Image by the TwinCAT software based on the slave's description file and prepared net 
             *    configuration. They can be distinguished by nique naming scheme:
             * 
             *       '<slave-name>.<pdo/mapping-name>.<variable-name>;
             * 
             * @note Such a naming convention is not standardized by EtherCAT and is TwinCAT specific behaviour.
             *    Implementation takes advantage of this behaviour to simplify design.
             */
            Slave,

            /**
             * @brief 'Other' variables are those unsupported directly by the parser. Example of such a PDO
             *    can be one named:
             * 
             *       'SyncUnits.<default>.<unreferenced>.WcState.WcState'
             * 
             *    Such PDOs are mapped automatically by the TwinCAT software
             */
            Other
            
        };

    public: /* Public management methods */

        /// Let user autonomize the element
        using Element::autonomize;

    public: /* Public methods */

        /**
         * @returns 
         *    fully qualified name of the variable given in the ENI file
         */
        inline std::string get_fq_name() const;

        /**
         * @returns 
         *    type of the variable
         */
        inline Type get_type() const;

        /**
         * @returns 
         *    @retval @c true if variable is associated with the master device
         *    @retval @c false if variable is associated with the slave device
         */
        inline bool is_master_variable() const;

        /**
         * @returns 
         *    @retval @c true if variable is associated with the slave device
         *    @retval @c false if variable is associated with the master device
         */
        inline bool is_slave_variable() const;

        /**
         * @returns 
         *    @retval @c true if variable is of unsupported type
         *    @retval @c false if variable is of unsupported type
         */
        inline bool is_other_variable() const;

        /**
         * @returns 
         *    name of the slave associated with the variable
         *
         * @throws std::runtime_error
         *    if variable is of no @c Type::Slave type
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <Name> element
         */
        inline std::string get_slave_name() const;

        /**
         * @returns 
         *    name of the PDO/mapping that the variable is associated with
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <Name> element or namng scheme is invalid
         */
        inline std::string get_pdo_name() const;

        /**
         * @returns 
         *    specific name of the mapped variable
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <Name> element or namng scheme is invalid
         */
        inline std::string get_name() const;

        /**
         * @returns 
         *    data type of the variable
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <DataType> element or type is not supported
         */
        inline types::Type get_data_type() const;

        /**
         * @returns 
         *    bit size of the variable
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <BitSize> element or size is invalid
         */
        inline std::size_t get_bit_size() const;

        /**
         * @returns 
         *    byte size of the variable (rounded down)
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <BitSize> element or size is invalid
         */
        inline std::size_t get_bit_offset() const;

        /**
         * @returns 
         *    bit offset of the variable
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <BitOffs> element or offset is invalid
         */
        inline std::size_t get_byte_size() const;

        /**
         * @returns 
         *    byte offset of the variable (rounded down)
         *
         * @throws eni::Error
         *    if <Variable> ENI element does not contain <BitOffs> element or offset is invalid
         */
        inline std::size_t get_byte_offset() const;

    protected: /* Protected ctors & dtors */

        /// Inherit all basic constructors
        using Element::Element;

        /// Custom from-element constructor
        Variable(const Element &elem);

    protected: /* Protected data */

        /// Type of the variable
        Type type{ };

    };

    /**
     * @brief Auxiliary wrapper around vector of PDI variables descriptions providing additional methods
     *    for searching the list
     */
    struct VariablesList : public std::vector<Variable> {
    private: /* Private methods */

        /**
         * @brief Finds first element in the list for which @p predicate returns @c true
         * 
         * @tparam PredicateT 
         *    type of the predicate
         * @param predicate 
         *    predicate to be used
         * 
         * @retval first
         *    subitem meeting the requirement
         * @retval empty
         *    optional if none of subelements meets the requirement
         */
        template<typename PredicateT>
        inline std::optional<Variable> find_element(PredicateT &&predicate) const;

        /**
         * @brief Filters out subelement for which @p predicate returns @c true
         * 
         * @tparam PredicateT 
         *    type of the predicate
         * @param predicate 
         *    predicate to be used
         * 
         * @returns 
         *    list of subitems for which @p predicate returned @c false
         */
        template<typename PredicateT>
        inline VariablesList filter_elements(PredicateT &&predicate) const;

    public: /* Public methods */

        /**
         * @returns 
         *    sublist containing only variables associated with master devices
         */
        inline VariablesList get_master_variables() const;
        
        /**
         * @returns 
         *    sublist containing only variables associated with slave devices
         */
        inline VariablesList get_slave_variables() const;

        /**
         * @param slave_name 
         *    name of the target slave
         * 
         * @returns 
         *    sublist containing variables associated with the slave with the given @p slave_name
         */
        inline VariablesList get_slave_variables(std::string_view slave_name) const;

        /**
         * @param pdo_name 
         *    name of the target pdo
         * 
         * @returns 
         *    sublist containing variables associated with PDO(s) with the given @p pdo_name
         */
        inline VariablesList get_pdo_variables(std::string_view pdo_name) const;

        /**
         * @param slave_name 
         *    name of the target slave
         * @param pdo_name 
         *    name of the target pdo
         * 
         * @returns 
         *    sublist containing variables associated with PDO with the given @p pdo_name 
         *    of the slaved named @p slave_name
         */
        inline VariablesList get_pdo_variables(std::string_view slave_name, std::string_view pdo_name) const;
        
        /**
         * @param name 
         *    name of the target variable
         * 
         * @returns 
         *    PDI slave variable with the given @p name
         */
        inline std::optional<Variable> get_slave_variable(std::string_view name) const;
        
        /**
         * @param slave 
         *    name of the target slave
         * @param name 
         *    name of the target variable
         * 
         * @returns 
         *    PDI variable associated with a slave named @p slave_name with the given @p name
         */
        inline std::optional<Variable> get_slave_variable(std::string_view slave_name, std::string_view name) const;
        
        /**
         * @param slave 
         *    name of the target slave
         * @param pdo 
         *    name of the target PDO
         * @param name 
         *    name of the target variable
         * 
         * @returns 
         *    PDI variable associated with a slave named @p slave_name  and PDO named @p pdo_name 
         *    with the given @p name
         */
        inline std::optional<Variable> get_slave_variable(
            std::string_view slave_name,
            std::string_view pdo_name,
            std::string_view name
        ) const;
        
        /**
         * @param name 
         *    name of the target variable
         * 
         * @returns 
         *    PDI variable with the given @p name
         */
        inline std::optional<Variable> get_variable(std::string_view name) const;

        /**
         * @param fq_name 
         *    fully qualified name of the target variable
         * 
         * @returns 
         *    PDI variable with the given fully qualified @p fq_name
         */
        inline std::optional<Variable> get_variable_by_fq_name(std::string_view fq_name) const;

    };

    /**
     * @brief Auxiliary pair-like structure holding lists of input and output variables
     *    associated according to some criterion
     */
    struct VariablesSet {

        /// Input variables
        VariablesList inputs;
        /// Output variables
        VariablesList outputs;

    };

public: /* ---------------------------------------------- Public management methods ----------------------------------------------- */

    /// Let user autonomize the element
    using Element::autonomize;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @param direction 
     *    direction of the image to be inspected
     * @returns 
     *    size of the input/output PDI (Process Data Image)
     *
     * @throws eni::Error
     *    if <ProcessImage> ENI elements does not contain <Inputs/Outputs>.<ByteSize> element
     */
    inline std::size_t get_size(Direction direction) const;

    /**
     * @param direction 
     *    direction of the image to be inspected
     * @returns 
     *    list of variables mapped into the PDI of the given @p direction
     *
     * @throws eni::Error
     *    if <ProcessImage> ENI elements does not contain <Inputs/Outputs> element
     */
    VariablesList get_variables(Direction direction) const;

    /**
     * @returns 
     *    list of input and output variables mapped into the PDIs
     *
     * @throws eni::Error
     *    if <ProcessImage> ENI elements does not contain <Inputs/Outputs> element
     */
    inline VariablesSet get_variables() const;

    /**
     * @param direction 
     *    direction of the image to be inspected
     * @param name 
     *    name of the slave device
     * @returns 
     *    list of input/output variables related to the slave device named @p name
     */
    inline VariablesList get_slave_variables(Direction direction, std::string_view name) const;

    /**
     * @param name 
     *    name of the slave device
     * @returns 
     *    list of input and output variables related to the slave device named @p name
     */
    inline VariablesSet get_slave_variables(std::string_view name) const;

protected: /* ---------------------------------------------- Protected ctors & dtors ---------------------------------------------- */

    /// Inherit all basic constructors
    using Element::Element;

};

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/process_image/variable.hpp"
#include "ethercat/eni/process_image/process_image.hpp"

/* ================================================================================================================================ */

#endif
