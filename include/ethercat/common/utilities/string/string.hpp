/* ============================================================================================================================ *//**
 * @file       string.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Friday, 1st July 2022 12:56:18 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_STRING_STRING_H__
#define __ETHERCAT_COMMON_UTILITIES_STRING_STRING_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <sstream>
#include <iomanip>
#include <cmath>
// Private includes
#include "ethercat/common/utilities/string.hpp"

/* =========================================================== Namespace ========================================================== */

namespace ethercat::common::utilities {

/* ========================================================== Definitions ========================================================= */

std::string byte_to_str(uint8_t byte) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << int(byte);
    return ss.str();
}


std::string buff_to_str(
    config::types::Span<const uint8_t> buff,
    std::size_t bytes_in_row,
    Color non_zeros_color,
    Color zeros_color
) {

    std::stringstream ss;

    // If empty buffer given, return empty string
    if(buff.empty())
        return ss.str();

    // Auxiliary functor returning ASCII color code
    auto get_color_code = [](Color color) {
        switch(color) {
            case Color::Red:     return "\033[31m";
            case Color::Green:   return "\033[32m";
            case Color::Yellow:  return "\033[33m";
            case Color::Blue:    return "\033[34m";
            case Color::Default: return "\033[0m";
            default:
                return "\033[0m";
        }
    };


    // Calculate number of (hexadecimal) digits of the highest byte address
    std::size_t addr_digits = static_cast<std::size_t>(std::log((double) buff.size()) / std::log(16) + 1);

    /// Loop over subsequent rows of the buffer
    for(std::size_t i = 0; i < static_cast<std::size_t>(buff.size()); i = i + bytes_in_row) {

        // Print address of the first byte in the row
        ss << "0x" << std::hex << std::setfill('0') << std::setw(addr_digits) << i << ": ";

        // Iterate over each 8 bytes in the row
        for(std::size_t j = 0; (j < bytes_in_row) && (i + j < static_cast<std::size_t>(buff.size())); j = j + 8) {
            // Iterate over byte in the octet
            for(std::size_t k = 0; (k < 8) && (i + j + k < static_cast<std::size_t>(buff.size())); ++k) {

                // Print the byte
                if(buff[i + j + k] == 0) {
                    ss << get_color_code(zeros_color) 
                       << byte_to_str(buff[i + j + k] )
                       << get_color_code(Color::Default) << " ";
                } else {
                    ss << get_color_code(non_zeros_color) 
                       << byte_to_str(buff[i + j + k] )
                       << get_color_code(Color::Default) << " ";
                }
            }

            // Print additionalspace between octets
            ss << " ";
        }

        // Print row-ending newline
        if(i + bytes_in_row < static_cast<std::size_t>(buff.size()))
            ss << "\n";
    }

    return ss.str();
}


std::string buff_to_array_str(config::types::Span<const uint8_t> buff) {

    std::stringstream ss;

    // Open array
    ss << "{ ";

    // Print all but last elements
    for(std::size_t i = 0; i < static_cast<std::size_t>(buff.size()) - 1; ++i)
        ss << byte_to_str(buff[i]) << ", ";
    // Print last element
    ss << byte_to_str(buff.back());

    // Close array
    ss << " }";

    return ss.str();
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities

#endif
