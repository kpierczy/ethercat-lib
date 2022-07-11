/* ============================================================================================================================ *//**
 * @file       enum.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 11th May 2022 1:09:32 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of common utilities for the cifx-ethercat library related to enums
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_UTILITIES_ENUM_H__
#define __ETHERCAT_UTILITIES_ENUM_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include <type_traits>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::utilities {

/* ====================================================== Auxiliary function ====================================================== */

/**
 * @brief Converts enumerated value to it's underlying representation
 * @param e
 *    value to be converted
 */
template<typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e);

/**
 * @brief Converts value to it's corresponding enumeration
 * @param val
 *    value to be converted
 */
template<typename Enum>
constexpr Enum to_enum(std::underlying_type_t<Enum> val);

/* ================================================================================================================================ */

} // End namespace ethercat::utilities

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/utilities/enum/enum.hpp"

/* ================================================================================================================================ */

#endif
