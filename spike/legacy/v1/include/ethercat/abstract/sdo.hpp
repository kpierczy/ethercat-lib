/* ============================================================================================================================ *//**
 * @file       sdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:11:53 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of data types and routines related to Service Data Objects of the EtherCAT protocol
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_SDO_H__
#define __ETHERCAT_ABSTRACT_SDO_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <cstdint>

/* ========================================================== Namespaces ========================================================== */

/**
 * @brief Definitions of types and routines modelling and managing SDO (Service Data Objects)
 */
namespace ethercat::abstract::sdo {

/* ========================================================== Data types ========================================================== */

/**
 * @brief Direction of the SDO
 */
enum class Direction {

    /**
     * @brief SDO downloading (writting) data into the slave device
     */
    Download,

    /**
     * @brief SDO uploading (reading) data from the slave device
     */
    Upload
    
};

/**
 * @brief Request structure for SDO access
 * @see doc/'Protocol API EtherCAT Master V4 V4.5.0', p. 74
 */
struct Request {

    /// Current operation mode of the bus (decides which addressing mode should be used in request)
    enum class OperationMode {
        BusScan,
        Normal
    };

    /// Current operation mode
    OperationMode operation_mode { OperationMode::Normal };
    /// Index of the object
    uint16_t index;
    /// Subindex of the object
    uint16_t subindex { 0 };

    /// Flag indicating whether full access should be made
    bool full_access { true };

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::sdo

#endif
