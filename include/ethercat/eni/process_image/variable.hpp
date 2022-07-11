/* ============================================================================================================================ *//**
 * @file       variable.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the Variable class implementing parsing interface of the <Variable> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_PROCESS_IMAGE_VARIABLE_H__
#define __ETHERCAT_COMMON_PROCESS_IMAGE_VARIABLE_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// External includes
#include <boost/algorithm/string.hpp>
// Private includes
#include "ethercat/eni/process_image.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* =========================================================== Constants ========================================================== */

namespace details {
namespace {
 
    /// Delimiter of the fields defined by the <Name> element of the <Variable> element of PDI
    constexpr auto VARIABLE_NAME_FIELDS_DELIMITER = ".";

    /**
     * @brief Specification of fields in the <Name> string of the <Variable> element
     *     associated with the Master device. Scheme of the <Name> content is:
     * 
     *       '<pdo-name>.<variable-name>'
     */
    struct MasterVariableNamingScheme {

        /// Index of the field defined by the <Name> element defining PDO name
        static constexpr auto PDO_NAME_FIELD_IDX = 0;
        /// Index of the field defined by the <Name> element defining variable name
        static constexpr auto VARIABLE_NAME_FIELD_IDX = 1;

    };

    /**
     * @brief Specification of fields in the <Name> string of the <Variable> element
     *     associated with the Slave device. Scheme of the <Name> content is:
     * 
     *       '<slave-name>.<pdo-name>.<variable-name>'
     */
    struct SlaveVariableNamingScheme {

        /// Index of the field defined by the <Name> element defining Slave name
        static constexpr auto SLAVE_NAME_FIELD_IDX = 0;
        /// Index of the field defined by the <Name> element defining PDO name
        static constexpr auto PDO_NAME_FIELD_IDX = 1;
        /// Index of the field defined by the <Name> element defining variable name
        static constexpr auto VARIABLE_NAME_FIELD_IDX = 2;

    };

    /// Number of bits in a byte
    constexpr std::size_t BITS_IN_BYTE = 8;

}
}

/* ============================================================ Helpers =========================================================== */

namespace details {

    /**
     * @brief Creates iterable view over fields of the @p name of the <Variable> element determined
     *    by the predefined delimtier
     * 
     * @param name 
     *    name to be split
     * @returns 
     *    created view
     * 
     * @note When C++20 std::ranges::view::split is available, switch to it
     */
    static inline auto split_variable_name(const std::string &name) {

        std::vector<std::string> ret;

        // Split  string by delimiter
        boost::algorithm::split(ret, name, boost::is_any_of(VARIABLE_NAME_FIELDS_DELIMITER));

        return ret;
    }

}

/* =================================================== Variable: Public methods =================================================== */

std::string ProcessImage::Variable::get_fq_name() const {

    /**
     * @note Existance of the <Name> tage is verified in the constructor
     */
    
    return get_child_value<std::string>("Name");
}


ProcessImage::Variable::Type ProcessImage::Variable::get_type() const {
    return type;
}


bool ProcessImage::Variable::is_master_variable() const {
    return type == Type::Master;
}


bool ProcessImage::Variable::is_slave_variable() const {
    return type == Type::Slave;
}


bool ProcessImage::Variable::is_other_variable() const {
    return type == Type::Other;
}


std::string ProcessImage::Variable::get_slave_name() const {

    // Check if slave-type variable
    if(type != Type::Slave)
        throw std::runtime_error{ "Variable is not related with any slave device" };
    
    // Else, get fully qualified name split into elements by '.' delimiter
    auto splitted_name = details::split_variable_name(get_fq_name());
    // Else, return name
    return splitted_name[details::SlaveVariableNamingScheme::SLAVE_NAME_FIELD_IDX];
}


std::string ProcessImage::Variable::get_pdo_name() const {
        
    // Else, get fully qualified name split into elements by '.' delimiter
    auto splitted_name = details::split_variable_name(get_fq_name());
    // Else, return PDO name
    return splitted_name[
        (type == Type::Master) ? 
            details::MasterVariableNamingScheme::PDO_NAME_FIELD_IDX : 
            details::SlaveVariableNamingScheme::PDO_NAME_FIELD_IDX
    ];

}


std::string ProcessImage::Variable::get_name() const {
        
    // Else, get fully qualified name split into elements by '.' delimiter
    auto splitted_name = details::split_variable_name(get_fq_name());
    // Else, return variable name
    return splitted_name[
        (type == Type::Master) ? 
            details::MasterVariableNamingScheme::VARIABLE_NAME_FIELD_IDX : 
            details::SlaveVariableNamingScheme::VARIABLE_NAME_FIELD_IDX 
    ];
    
}


types::Type ProcessImage::Variable::get_data_type() const {
    return types::Type::parse(
        get_child_value<std::string>("DataType"),
        get_bit_size()
    );
}


std::size_t ProcessImage::Variable::get_bit_size() const {
    return get_child_value<std::size_t>("BitSize");
}


std::size_t ProcessImage::Variable::get_bit_offset() const {
    return get_child_value<std::size_t>("BitOffs");
}


std::size_t ProcessImage::Variable::get_byte_size() const {
    return get_bit_size() / details::BITS_IN_BYTE;
}


std::size_t ProcessImage::Variable::get_byte_offset() const {
    return get_bit_offset() / details::BITS_IN_BYTE;
}

/* =================================================== Variable: Private methods ================================================== */

template<typename PredicateT>
std::optional<ProcessImage::Variable> ProcessImage::VariablesList::find_element(PredicateT &&predicate) const {
    if(auto it = std::find_if(this->begin(), this->end(), std::forward<PredicateT>(predicate)); it != this->end())
        return *it;
    else 
        return std::optional<Variable>{ };
}


template<typename PredicateT>
ProcessImage::VariablesList ProcessImage::VariablesList::filter_elements(PredicateT &&predicate) const {
    VariablesList ret;
    std::remove_copy_if(this->begin(), this->end(), std::back_insert_iterator(ret), std::forward<PredicateT>(predicate));
    return ret;
}

/* ================================================= VariablesList: Public methods ================================================ */

ProcessImage::VariablesList ProcessImage::VariablesList::get_master_variables() const {
    return filter_elements([](const Variable &var) { return (not var.is_master_variable()); });
}


ProcessImage::VariablesList ProcessImage::VariablesList::get_slave_variables() const {
    return filter_elements([](const Variable &var) { return (not var.is_slave_variable()); });
}


ProcessImage::VariablesList ProcessImage::VariablesList::get_slave_variables(std::string_view slave_name) const {
    return filter_elements(
        [slave_name](const Variable &var) { 
            return ((not var.is_slave_variable()) or (var.get_slave_name() != slave_name)); 
        });
}


ProcessImage::VariablesList ProcessImage::VariablesList::get_pdo_variables(std::string_view pdo_name) const {
    return filter_elements(
        [pdo_name](const Variable &var) { 
            return ((not var.is_slave_variable()) or (var.get_pdo_name() != pdo_name)); 
        });
}


ProcessImage::VariablesList ProcessImage::VariablesList::get_pdo_variables(std::string_view slave_name, std::string_view pdo_name) const {
    return filter_elements(
        [slave_name, pdo_name](const Variable &var) { 
            return ((not var.is_slave_variable()) or (var.get_slave_name() != slave_name) or (var.get_pdo_name() != pdo_name)); 
        });
}


std::optional<ProcessImage::Variable> ProcessImage::VariablesList::get_slave_variable(std::string_view name) const {
    if(auto it = std::find_if(this->begin(), this->end(), 
        [name](const Variable &var) { return (var.is_slave_variable() and (var.get_name() == name)); })
    ; it != this->end())
        return *it;
    else 
        return std::optional<Variable>{ };
}


std::optional<ProcessImage::Variable> ProcessImage::VariablesList::get_slave_variable(std::string_view slave_name, std::string_view name) const {
    return find_element(
        [slave_name, name](const Variable &var) { 
            return (var.is_slave_variable() and (var.get_slave_name() == slave_name) and (var.get_name() == name)); 
        });
}


std::optional<ProcessImage::Variable> ProcessImage::VariablesList::get_slave_variable(
    std::string_view slave_name,
    std::string_view pdo_name,
    std::string_view name
) const {
    return find_element(
        [slave_name, pdo_name, name](const Variable &var) { 
            return (
                var.is_slave_variable() and 
                (var.get_slave_name() == slave_name) and 
                (var.get_pdo_name() == pdo_name) and 
                (var.get_name() == name)
            ); 
        });
}


std::optional<ProcessImage::Variable> ProcessImage::VariablesList::get_variable(std::string_view name) const {
    return find_element(
        [name](const Variable &var) { 
            return (var.get_name() == name); 
        });
}


std::optional<ProcessImage::Variable> ProcessImage::VariablesList::get_variable_by_fq_name(std::string_view fq_name) const {
    return find_element(
        [fq_name](const Variable &var) { 
            return (var.get_fq_name() == fq_name); 
        });
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
