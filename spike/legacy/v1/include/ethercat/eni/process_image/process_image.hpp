/* ============================================================================================================================ *//**
 * @file       process_image.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of inline methods of the ProcessImage class implementing parsing interface of the <ProcessImage> tag of the ENI
*              (EtherCAT Network Informations) tag
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_PROCESS_IMAGE_PROCESS_IMAGE_H__
#define __ETHERCAT_COMMON_PROCESS_IMAGE_PROCESS_IMAGE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/process_image.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ======================================================== Public methods ======================================================== */

std::size_t ProcessImage::get_size(Direction direction) const {
    return ( direction == Direction::Inputs ) ?
        get_child_value<std::size_t>("Inputs.ByteSize") :
        get_child_value<std::size_t>("Outputs.ByteSize");
}


ProcessImage::VariablesSet ProcessImage::get_variables() const {
    return VariablesSet {
        .inputs  = get_variables(Direction::Inputs),
        .outputs = get_variables(Direction::Outputs)
    };
}


ProcessImage::VariablesSet ProcessImage::get_slave_variables(std::string_view name) const {
    return VariablesSet {
        .inputs  = get_slave_variables(Direction::Inputs,  name),
        .outputs = get_slave_variables(Direction::Outputs, name)
    };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
