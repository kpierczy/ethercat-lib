/* ============================================================================================================================ *//**
 * @file       variable.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 3:33:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Variable class implementing parsing interface of the <Variable> tag of the ENI
 *             (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// Private includes
#include "ethercat/eni/process_image.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ==================================================== Protected ctors & dtors =================================================== */

ProcessImage::Variable::Variable(const Element &elem) :
    Element{ elem }
{ 
    // Get name string
    auto name_str = get_child_value<std::string>("Name");
    // Count dot-delimiters in the string
    auto delimiters_num = std::count(name_str.begin(), name_str.end(), '.');

    // Determine variable's type
    switch(delimiters_num) {

        /**
         * If 2-elements name is used (assummed format '<pdo-name>.<variable-name>'), 
         * mark variable as master-related
         */
        case 1: type = Type::Master; break;
        
        /**
         * If 3-elements name is used (assummed format '<pdo-name>.<variable-name>'), 
         * mark variable as master-related
         */
        case 2: type = Type::Slave; break;

        /**
         * Otherwise, mark PDO as unsupported
         */
        default: type = Type::Other; break;

    }
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni
