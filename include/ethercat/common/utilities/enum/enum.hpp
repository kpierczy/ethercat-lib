/* ============================================================================================================================ *//**
 * @file       enum.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:37:44 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of common utilities for the cifx-ethercat library related to enums
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_ENUM_ENUM_H__
#define __ETHERCAT_COMMON_UTILITIES_ENUM_ENUM_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/enum.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities {

/* ======================================================== Free functions ======================================================== */

template<typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}


template<typename Enum>
constexpr Enum to_enum(std::underlying_type_t<Enum> val) {
    return static_cast<Enum>(val);
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities

#endif
