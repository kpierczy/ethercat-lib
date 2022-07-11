/* ============================================================================================================================ *//**
 * @file       pdo_entry_registration_proxy.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 26th May 2022 8:09:49 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Master::PdoEntryRegistrationProxy class.
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DRIVERS_MASTER_PDO_ENTRY_REGISTRATION_PROXY_H__
#define __ETHERCAT_DRIVERS_MASTER_PDO_ENTRY_REGISTRATION_PROXY_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/drivers/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

/* ============================================================= Class ============================================================ */

/**
 * @brief Auxiliary functor type providing lambda-like routine object to whose aim is to construct
 *    @ref abstract::pdo::Entry objects requested by @ref Slave interfaces on construction. Instances
 *    of this class are passed directly to the constructor of Slave class ( see @ref Slave::Slave ).
 *    
 * @see Slave::Slave
 * @todo When C++20 think about switching EntryMarker to lambda template:
 *      
 *      [&slave_name, &input_pdi, &output_pdi, &mapped_slave_variables]
 *          <abstract::pdo::Direction dir>
 *      (std::string_view pdo_name, const eni::Slave::Pdo::Entry &entry) {
 *          ...
 *      };
 */
class PdoEntryRegistrationProxy {
    
public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Construct a new PdoEntryRegistrationProxy object
     * 
     * @param slave_name 
     *    name of the slave that the maker will construct entries for
     * @param input_pdi 
     *    reference to the input Process Data Image buffer
     * @param output_pdi 
     *    reference to the output Process Data Image buffer
     * @param mapped_slave_variables 
     *    set of slave's objects actually mapped into the Process Data Image
     */
    inline PdoEntryRegistrationProxy(
        std::string_view slave_name,
        std::vector<uint8_t> &input_pdi,
        std::vector<uint8_t> &output_pdi,
        const eni::ProcessImage::VariablesSet &mapped_slave_variables
    );

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /**
     * @brief Registers handler of the @p entry mapped in the PDO named @p pdo_name for the
     *    associated slave interface. Verifies that the requested @p entry is in fact present
     *    in the Process Data Image mapping
     * 
     * @tparam dir 
     *    direction of the requested entry
     * @param pdo_name 
     *    name of the PDO that the entry is mapped into
     * @param entry 
     *    description of the entry to be registered
     * @returns 
     *    object enabling manipulation of the registered entry
     * 
     * @throws ethercat::eni::Error 
     *    if inconsistency between slave's request and objects actuall actually mapped into PDI
     *    (based on the <ProcessImage> section of ENI) is found 
     */
    template<abstract::pdo::Direction dir>
    abstract::pdo::Entry<dir> operator()(
        std::string_view pdo_name,
        eni::Slave::Pdo::Entry entry
    );
    
private: /* ----------------------------------------------------- Private data ---------------------------------------------------- */

    /// Name of the slave that the maker will construct entries for
    std::string_view slave_name;
    /// Reference to the input Process Data Image buffer
    std::vector<uint8_t> &input_pdi;
    /// Reference to the output Process Data Image buffer
    std::vector<uint8_t> &output_pdi;
    /// Set of slave's objects actually mapped into the Process Data Image
    const eni::ProcessImage::VariablesSet &mapped_slave_variables;

};

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/drivers/master/pdo_entry_registration_proxy/pdo_entry_registration_proxy.hpp"

/* ================================================================================================================================ */

#endif