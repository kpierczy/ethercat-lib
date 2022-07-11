/* ============================================================================================================================ *//**
 * @file       master.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:12:57 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Master class implementing parsing interface of the <Master> tag of the ENI (EtherCAT Network
 *             Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_MASTER_H__
#define __ETHERCAT_COMMON_ENI_MASTER_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string>
// Boost includes
#include <boost/property_tree/ptree.hpp>
// Private includes
#include "ethercat/eni/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================= Class ============================================================ */

// Forward declare Configuration class
class Configuration;

/**
 * @brief Class providing basic parsing routines for the <Master> description tag of the ENI
 *    (EtherCAT Network Informations) file
 */
class Master : protected Element {

    /// Make Configruaiton class a friend to let it spawn Master parsers
    friend class Configuration;

public: /* ---------------------------------------------- Public management methods ----------------------------------------------- */

    /// Let user autonomize the element
    using Element::autonomize;

public: /* --------------------------------------------------- Public methods ----------------------------------------------------- */

    /**
     * @returns
     *    name of the master device
     *
     * @throws eni::Error
     *    if <Master> ENI elements does not contain <Info>/<Name> element
     */
    inline std::string get_name() const;

protected: /* ---------------------------------------------- Protected ctors & dtors ---------------------------------------------- */

    /// Inherit all basic constructors
    using Element::Element;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/master/master.hpp"

/* ================================================================================================================================ */

#endif
