/* ============================================================================================================================ *//**
 * @file       pdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 10th May 2022 5:10:46 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of data types and routines related to Process Data Objects of the EtherCAT protocol
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_H__
#define __ETHERCAT_ABSTRACT_PDO_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/objects.hpp"

/* ========================================================== Namespaces ========================================================== */

/**
 * @brief Definitions of types and routines modelling and managing PDO (Process Data Objects)
 */
namespace ethercat::abstract::pdo {

/* ========================================================= Enumerations ========================================================= */

/// PDO direction
enum class Direction {
    
    /// Input PDOs (sent from Slave to Master)
    Input = 0,
    /// Input PDOs (sent from Slave to Master)
    Tx = 0,

    /// Output PDOs (sent from Master to Slave)
    Output = 1,
    /// Output PDOs (sent from Master to Slave)
    Rx = 1,

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

/* ====================================================== Submodule includes ====================================================== */

#include "ethercat/abstract/pdo/image_buffer.hpp"
#include "ethercat/abstract/pdo/handler.hpp"
#include "ethercat/abstract/pdo/entry.hpp"

/* ================================================================================================================================ */

#endif
