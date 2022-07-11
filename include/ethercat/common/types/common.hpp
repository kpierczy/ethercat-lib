/* ============================================================================================================================ *//**
 * @file       common.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of common entities used by common base types describing EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_COMMON_H__
#define __ETHERCAT_COMMON_TYPES_COMMON_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <cstdint>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ============================================================= Types ============================================================ */

/// Type used to represent type ID
using IdType = std::size_t;

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
