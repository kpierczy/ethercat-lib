/* ============================================================================================================================ *//**
 * @file       string.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 2:56:09 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Auxiliary utilities related to strings
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_STRING_H__
#define __ETHERCAT_COMMON_UTILITIES_STRING_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/config.hpp"

/* ====================================================== Namespace patterns ====================================================== */

namespace ethercat::common::utilities {

/* =========================================================== Constants ========================================================== */

/**
 * @brief Enumeration fo strings' colors
 */
enum class Color {
    Default,
    Red,
    Green,
    Yellow,
    Blue
};

/* ========================================================= Declarations ========================================================= */

/**
 * @brief Converts a byte to human-readable hexadecimal representation
 * 
 * @param byte 
 *    byte to be converted
 * @returns 
 *    byte converted to human-readable hexadecimal representation
 */
inline std::string byte_to_str(uint8_t byte);

/**
 * @brief Converts bytes-buffer into human-readable string
 * 
 * @param buff 
 *    buffer to be converted
 * @param bytes_in_row 
 *    number of bytes printed in a single row
 * @param non_zeros_color 
 *    output color of non-zero bytes
 * @param zeros_color 
 *    output color of zero bytes
 * 
 * @returns 
 *    human-readble representation fo @p buff
 */
inline std::string buff_to_str(
    config::types::Span<const uint8_t> buff,
    std::size_t bytes_in_row = 32,
    Color non_zeros_color = Color::Default,
    Color zeros_color = Color::Default
);

/**
 * @brief Converts bytes-buffer into human-readable array string
 * 
 * @param buff 
 *    buffer to be converted
 * 
 * @returns 
 *    human-readble representation fo @p buff
 */
inline std::string buff_to_array_str(config::types::Span<const uint8_t> buff);

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/utilities/string/string.hpp"

/* ================================================================================================================================ */

#endif
