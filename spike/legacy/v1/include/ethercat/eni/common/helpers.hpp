/* ============================================================================================================================ *//**
 * @file       helpers.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 17th May 2022 2:09:09 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of the helpers routines used across the module
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_HELPERS_H__
#define __ETHERCAT_COMMON_ENI_COMMON_HELPERS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================ Macros ============================================================ */

/// Macro defining default copying semantics
#define ETHERCAT_ENI_DEFAULT_COPY(type)         \
    type(const type &relem) = default;          \
    type &operator=(const type &relem) = default

/// Macro defining default move semantics
#define ETHERCAT_ENI_DEFAULT_MOVE(type)    \
    type(type &&relem) = default;          \
    type &operator=(type &&relem) = default

/// Macro defining default copy and move semantics
#define ETHERCAT_ENI_DEFAULT_COPY_MOVE(type) \
    ETHERCAT_ENI_DEFAULT_COPY(type);         \
    ETHERCAT_ENI_DEFAULT_MOVE(type)

/* ============================================================ Helpers =========================================================== */

/**
 * @brief Parses @p index_string containing ENI-specific formatted index of the CoE obejct
 *    into numerical form
 * 
 * @param index_string 
 *    string to be converted
 * @returns 
 *    parse index
 * 
 * @throws std::runtime_error 
 *    if @p index_string does not contain properly formated index
 */
inline std::size_t parse_index(const std::string &index_string);

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/common/helpers/helpers.hpp"

/* ================================================================================================================================ */

#endif
