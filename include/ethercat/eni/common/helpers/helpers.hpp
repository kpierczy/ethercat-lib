/* ============================================================================================================================ *//**
 * @file       helpers.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 17th May 2022 2:10:20 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of the helpers routines used across the module
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_HELPERS_HELPERS_H__
#define __ETHERCAT_COMMON_ENI_COMMON_HELPERS_HELPERS_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <regex>
// Private includes
#include "ethercat/eni/common/helpers.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ========================================================== Definitions ========================================================= */

std::size_t parse_index(const std::string &index_string) {
    
    // Regex pattern that <Exclude> element is expected to match
    const std::regex pattern("^#x([0-9a-fA-F]+)$", std::regex_constants::ECMAScript);

    std::smatch match;

    // Check if content matches expected format (pattern matches and subelement is found)
    if(auto matches = std::regex_match(index_string, match, pattern); matches && match.size() == 2) {
        
        std::stringstream ss;

        // On match, parse sub-match of the result as hexadecimal number
        ss << std::hex << match[1];

        std::size_t index;

        // Convert string representation to the number
        ss >> index;

        return index;
        
    }

    // If content has invalid form, throw error
    throw std::runtime_error{ "<Exclude> element has invalid form (" + index_string + ")" };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
