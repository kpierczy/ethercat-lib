/* ============================================================================================================================ *//**
 * @file       pdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the PDO nested class of the Slave interface
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_H__
#define __ETHERCAT_SLAVE_PDO_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni.hpp"
#include "ethercat/slave.hpp"
#include "ethercat/types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================== PDO ============================================================= */

/**
 * @brief A proxy class template providing an interface for managing Process Data Objects 
 *    (PDO) entries associated  with the slave device that are mapped in the Process Data 
 *    Image (PDI)
 * 
 * @tparam ImplementationT 
 *    type implementing hardware-specific part of the Slave driver
 * @tparam dir 
 *    communication direction for PDO interface 
 */
template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
class Slave<ImplementationT>::Pdo {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Auxiliary class providing interface for parsing and manipulating PDI-mapped
     *    entries of the given PDO
     */
    class Entry;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */
    
    /// Disable copy-construction
    inline Pdo(const Pdo &rpdo) = delete;
    /// Disable copy-asignment
    inline Pdo &operator=(const Pdo &rpdo) = delete;

    /// Enable move-construction (to enable storing slave in relocatable containers)
    inline Pdo(Pdo &&rpdo) = default;
    /// Enable move-asignment (to enable storing slave in relocatable containers)
    inline Pdo &operator=(Pdo &&rpdo) = default;

public: /* ---------------------------------------------------- Public getters ---------------------------------------------------- */

    /**
     * @returns 
     *    name of the PDO
     */
    inline std::string_view get_name() const;

    /**
     * @returns 
     *    reference to entries mapped into the PDO
     */
    inline std::vector<Entry> &get_entries();

    /**
     * @returns 
     *    const reference to entries mapped into the PDO
     */
    inline const std::vector<Entry> &get_entries() const;

    /**
     * @param name 
     *    name of the PDO entry
     * 
     * @retval true 
     *    if entry with the given @p name is mapped into the PDO
     * @retval false 
     *    otherwise
     */
    inline bool has_entry(std::string_view name) const;
    
    /**
     * @returns 
     *    reference to PDO entry entry with the given name
     */
    inline Entry &get_entry(std::string_view name);
    
    /**
     * @returns 
     *    const reference to PDO entry entry with the given name
     */
    inline const Entry &get_entry(std::string_view name) const;

protected: /* ------------------------------------------------ Protected ctors ---------------------------------------------------- */

    /// Make Master a friend to let it access update method
    template<typename MasterImplementationT,typename SlaveImplementationT>
    friend class Master;

    /**
     * @brief Construct a new Pdo object
     * 
     * @param pdo_description 
     *    ENI description of the PDO
     * @param pdi_variables 
     *    ENI description of PDI variables corresponding to PDO's entries
     * 
     * @throws eni::Error 
     *    if inconsistency between @p pdo_description and @p pdi_variables has been detected
     */
    Pdo(
        eni::Slave::Pdo pdo_description,
        const eni::ProcessImage::VariablesList &pdi_variables
    );

private: /* --------------------------------------------------- Private data ------------------------------------------------------ */

    /// Name of the Entry
    std::string name;
    /// Entries mapped into the PDO
    std::vector<Entry> entries;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/slave/pdo/pdo.hpp"
#include "ethercat/slave/pdo/entry.hpp"

/* ================================================================================================================================ */

#endif
