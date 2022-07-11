/* ============================================================================================================================ *//**
 * @file       cyclic.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:12:57 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Cyclic class implementing parsing interface of the <Cyclic> tag of the ENI (EtherCAT Network
 *             Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_CYCLIC_H__
#define __ETHERCAT_COMMON_ENI_CYCLIC_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string>
#include <chrono>
// Boost includes
#include "boost/property_tree/ptree.hpp"
// Private includes
#include "ethercat/eni/common.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================= Class ============================================================ */

// Forward declare Configuration class
class Configuration;

/**
 * @brief Class providing basic parsing routines for the <Cyclic> description tag of the ENI 
 *    (EtherCAT Network Informations) file
 */
class Cyclic : protected Element {

    /// Make Configruaiton class a friend to let it spawn Cyclic parsers
    friend class Configuration;

public: /* ---------------------------------------------- Public management methods ----------------------------------------------- */

    /// Let user autonomize the element
    using Element::autonomize;

public: /* --------------------------------------------------- Public methods ----------------------------------------------------- */

    /**
     * @returns
     *    preconfigured bus cycle in [us]
     *
     * @throws eni::Error
     *    if <Cyclic> ENI elements does not contain <CycleTime> element
     */
    inline std::chrono::microseconds get_cycle_time() const;

protected: /* ---------------------------------------------- Protected ctors & dtors ---------------------------------------------- */

    /// Inherit all basic constructors
    using Element::Element;

};

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/cyclic/cyclic.hpp"

/* ================================================================================================================================ */

#endif
