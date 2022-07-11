/* ============================================================================================================================ *//**
 * @file       pdo_entry_registration_proxy.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 26th May 2022 8:09:49 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the Master::PdoEntryRegistrationProxy class.
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DRIVERS_MASTER_PDO_ENTRY_REGISTRATION_PROXY_PDO_ENTRY_REGISTRATION_PROXY_H__
#define __ETHERCAT_DRIVERS_MASTER_PDO_ENTRY_REGISTRATION_PROXY_PDO_ENTRY_REGISTRATION_PROXY_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/drivers/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ========================================================= Public ctors ========================================================= */

PdoEntryRegistrationProxy::PdoEntryRegistrationProxy(
    std::string_view slave_name,
    std::vector<uint8_t> &input_pdi,
    std::vector<uint8_t> &output_pdi,
    const eni::ProcessImage::VariablesSet &mapped_slave_variables
) :
    slave_name { slave_name },
    input_pdi { input_pdi },
    output_pdi { output_pdi },
    mapped_slave_variables { mapped_slave_variables }
{ }

/* ============================================================ Helpers =========================================================== */

namespace details {

    /**
     * @brief Finds object in the @p pdi_variables list with the name matching to the slave's @p pdo_entry
     * 
     * @param name
     *    slave's name
     * @param pdo_name
     *    name of the PDO that the requested entry is mapped into
     * @param pdo_entry
     *    PDO entry to be matched
     * @param pdi_variables 
     *    list of variables in the PDI that match should be found in
     * @returns 
     *    reference to the matched obejct
     * 
     * @throws ethercat::eni::Error
     *    if match could not be found
     */
    static inline eni::ProcessImage::Variable match_pdo_to_pdi_variable(
        std::string_view name,
        std::string_view pdo_name,
        eni::Slave::Pdo::Entry pdo_entry,
        const std::vector<eni::ProcessImage::Variable> &pdi_variables
    ) {
        // Find matchin entry in the PDI
        auto pdi_match =  std::find_if(pdi_variables.begin(), pdi_variables.end(),
            [&pdo_name, &pdo_entry](const auto &object) { 
                return (object.get_pdo_name() == pdo_name            ) and
                       (object.get_name()     == pdo_entry.get_name());
        });
        
        // If match could not be found, ENI configuration is corrupted - throw error
        if(pdi_match == pdi_variables.end()) {

            std::stringstream ss;

            // Format error message
            ss << "[ethercat::drivers::PdoEntryRegistrationProxy::operator()] "
            << "Corrupted ENI configuration "
            << "(slave '" << name << "': PDO entry '" << pdo_entry.get_name() << "' has no match in the Process Data Image)";

            // Throw error
            throw ethercat::eni::Error( ss.str() );
        
        }

        return *pdi_match;
    }

    /**
     * @brief Verifies that data types and size of @p pdo_entry and @p pdi_variable match
     * 
     * @param name
     *    slave's name
     * @param pdo_entry
     *    PDO entry
     * @param pdi_variable 
     *    matched PDI variable
     * 
     * @throws ethercat::eni::Error
     *    if data is not consistent
     */
    static inline void verify_pdo_consistency(
        std::string_view name,
        eni::Slave::Pdo::Entry pdo_entry,
        eni::ProcessImage::Variable pdi_variable
    ) {
        // Check whether matched obejcts has the same data type
        bool is_object_type_matched = (pdo_entry.get_data_type() == pdi_variable.get_data_type());
        // Check whether matched obejcts has the same size
        bool is_object_size_matched = (pdo_entry.get_bit_len()   == pdi_variable.get_bit_size() );
            
        // If not, throw error
        if((not is_object_type_matched) or (not is_object_size_matched)) {

            std::stringstream ss;

            // Format error message
            ss << "[ethercat::drivers::PdoEntryRegistrationProxy::operator()] "
                << "Corrupted ENI configuration "
                << "(slave '" << name << "': PDO entry '" << pdo_entry.get_name() << "' has not matching "
                << ((not is_object_type_matched) ? "types" : "size")
                << " in the Process Data Image)";

            // Throw error
            throw ethercat::eni::Error( ss.str() );
        }
    }

    /// Number of bits in byte
    constexpr std::size_t BITS_IN_BYTE = 8;

    /**
     * @brief Verifies that alignment of @p pdi_variable in the PDI is supported
     * 
     * @param name 
     *    slave's name
     * @param pdi_variable 
     *    matched PDI entry
     * 
     * @throws ethercat::eni::Error
     *    if @p pdi_variable is not byte-aligned
     */
    static inline void verify_entry_alignment(
        std::string_view name,
        eni::ProcessImage::Variable pdi_variable
    ) {

        // Check whether byte-aligned object has been found
        bool is_object_alignment_valid = 
            ( pdi_variable.get_bit_size()   / BITS_IN_BYTE != pdi_variable.get_byte_size()   ) or 
            ( pdi_variable.get_bit_offset() / BITS_IN_BYTE != pdi_variable.get_byte_offset() );
        // If not, throw error
        if(not is_object_alignment_valid) {

            std::stringstream ss;

            // Format error message
            ss << "[ethercat::drivers::PdoEntryRegistrationProxy::operator()] "
            << "Invalid PDO alignment "
            << "(slave '" << name << "': PDO entry '" << pdi_variable.get_name() << "' is not byte-aligned)";

            /**
             * @note In general, EtherCAT PDOs can be bit-aligned inside the Process Data Image.
             *    At the moment driver supports only byte-aligned objects but it may change in 
             *    the future
             */

            // Throw error
            throw ethercat::eni::Error( ss.str() );
        }
    }

    /**
     * @brief Verifies that the @p pdi_variable fits into the target PDI buffer
     * 
     * @param name 
     *    slave's name
     * @param pdi_variable 
     *    PDI variable to fit in @p pdi_buffer
     * @param buffer_size 
     *    size of the target PDI buffer
     * 
     * @throws ethercat::eni::Error
     *    if variable does not fit
     */
    static inline void verify_object_placement(
        std::string_view name,
        eni::ProcessImage::Variable pdi_variable,
        std::size_t buffer_size
    ) {
        // Check whether PDO fits into the given buffer
        bool object_fits_in_buffer = (pdi_variable.get_byte_offset() + pdi_variable.get_byte_size()) <= buffer_size;
        // If not, throw error
        if(not object_fits_in_buffer) {

            std::stringstream ss;

            // Format error message
            ss << "[ethercat::drivers::PdoEntryRegistrationProxy::operator()] "
            << "PDO buffer too small to store PDO entry"
            << "(slave '" << name << "': PDO entry '" << pdi_variable.get_name() << "' "
            << "of size " << pdi_variable.get_byte_size() << " at offset " << pdi_variable.get_byte_offset() << " "
            << "does not fit into the PDI buffer)";

            /**
             * @note In general, EtherCAT PDOs can be bit-aligned inside the Process Data Image.
             *    At the moment driver supports only byte-aligned objects but it may change in 
             *    the future
             */

            // Throw error
            throw ethercat::eni::Error( ss.str() );
        }
    }

    template<bool flag = false>
    constexpr inline void entry_maker_dir_no_match() {
        static_assert(flag, "[ethercat::drivers::PdoEntryRegistrationProxy::operator()] Invalid PDO direction given"); 
    }

}

/* ======================================================== Public methods ======================================================== */

template<abstract::pdo::Direction dir>
abstract::pdo::Entry<dir> PdoEntryRegistrationProxy::operator()(
    std::string_view pdo_name,
    eni::Slave::Pdo::Entry entry
) {
    std::vector<uint8_t> *pdi;
    const std::vector<eni::ProcessImage::Variable> *mapped_variables;

    // Pick PDI and list of mapped objects depending on the PDO direction requested
    if constexpr(dir == abstract::pdo::Direction::Input) {
        pdi              = &input_pdi;
        mapped_variables = &mapped_slave_variables.inputs;
    } else if constexpr(dir == abstract::pdo::Direction::Output) {
        pdi              = &output_pdi;
        mapped_variables = &mapped_slave_variables.outputs;
    } else 
        details::entry_maker_dir_no_match();

    // Match slave's PDO entry with the variable in the Process Data Image
    auto pdi_entry_matched = details::match_pdo_to_pdi_variable(slave_name, pdo_name, entry, *mapped_variables);

    // Verify that matched entries are consistent
    details::verify_pdo_consistency(slave_name, entry, pdi_entry_matched);
    // Verify that the entry is properly alligned in the PDI
    details::verify_entry_alignment(slave_name, pdi_entry_matched);
    // Verify that the entry fits in the PDI buffer
    details::verify_object_placement(slave_name, pdi_entry_matched, pdi->size());
    
    // Assert that the Entries are move-constructible
    static_assert(std::is_move_constructible_v<abstract::pdo::Entry<dir>>, 
        "[ethercat::drivers::PdoEntryRegistrationProxy::operator()] PDO ethercat::abstract::pdo::Entry is "
        "required to be move-constructible. Entry is quarenteed to be move-constructible as long as "
        "synchronisation lock used to synchronise internal ethercat::abstract::objects::ValueBuffer is "
        "move-constructible. By default it is not the case. Make sure that this synchronsiation primitive "
        "is move-constructible"
    );

    // If valid match found, return entry reference
    abstract::pdo::Entry<dir>{
        pdi_entry_matched.get_name(),
        pdi_entry_matched.get_data_type(),
        ranges::span<uint8_t>{
            &(*pdi)[pdi_entry_matched.get_byte_offset()],
            static_cast<ranges::span<uint8_t>::index_type>(pdi_entry_matched.get_byte_size())
        }
    };
    
}

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

/* ================================================================================================================================ */

#endif
