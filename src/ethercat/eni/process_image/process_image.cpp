/* ============================================================================================================================ *//**
 * @file       process_image.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 3:33:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the ProcessImage class implementing parsing interface of the <ProcessImage> tag of 
 *             the ENI (EtherCAT Network Informations) tag
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

/* ======================================================== Public methods ======================================================== */

ProcessImage::VariablesList ProcessImage::get_variables(Direction direction) const {

    VariablesList ret;

    // Select target PDI
    auto pdi_element = (direction == Direction::Inputs) ? "Inputs" : "Outputs";

    // Iterate over child elements
    for(const auto &element : get_child(pdi_element)) {
        // If mapping element given, add entry to the return value
        if(element.first == "Variable")
            ret.emplace_back(static_cast<Variable>(element.second));
    }

    return ret;
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni
