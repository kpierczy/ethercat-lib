/* ============================================================================================================================ *//**
 * @file       bit.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of tag types providing an interface to specialize the default translator for a-bit-size booleans
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_TYPES_BIT_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_TYPES_BIT_H__

/* =========================================================== Includes =========================================================== */

// System includes
#include <type_traits>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ======================================================== Auxiliary types ======================================================= */

/**
 * @brief Auxiliary tag type used to identify translation with a boolean target using a single
 *    bit as a representation in the binary image
 */
struct bit { };

/* ================================================================================================================================ */

} // End namespace ethercat::types

#endif
