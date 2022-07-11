/* ============================================================================================================================ *//**
 * @file       types.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:37:44 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of common utilities related to EtherCAT (CoE) data types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 * @note This header file should be included entirely by downstream files. In particular no files from ethercat/common/types/ 
 *    subdirectory should be included individually. It is required to provide proper includsion order
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_H__
#define __ETHERCAT_COMMON_TYPES_H__

/* =========================================================== Includes =========================================================== */

#include "ethercat/common/types/common.hpp"
#include "ethercat/common/types/builtin.hpp"
#include "ethercat/common/types/structural.hpp"
#include "ethercat/common/types/type.hpp"
#include "ethercat/common/types/traits.hpp"

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/types/builtin/builtin.hpp"
#include "ethercat/common/types/structural/structural.hpp"
#include "ethercat/common/types/type/type.hpp"

/* ================================================================================================================================ */

#endif
