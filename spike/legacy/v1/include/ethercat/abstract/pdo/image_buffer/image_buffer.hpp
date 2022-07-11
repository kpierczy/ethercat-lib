/* ============================================================================================================================ *//**
 * @file       image_buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 12th May 2022 1:56:41 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the helper wrapper class around Process Data Image buffer poviding synchronisation
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_IMAGE_BUFFER_IMAGE_BUFFER_H__
#define __ETHERCAT_ABSTRACT_PDO_IMAGE_BUFFER_IMAGE_BUFFER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/pdo/image_buffer.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::pdo {

/* ========================================================= Public types ========================================================= */

std::vector<uint8_t> &ImageBuffer::Reference::get_data() {
    return data;
}


const std::vector<uint8_t> &ImageBuffer::Reference::get_data() const {
    return data;
}


ImageBuffer::Reference::Reference(const ImageBuffer &buffer) :
    data{ const_cast<ImageBuffer&>(buffer).data },
    lock{ const_cast<ImageBuffer&>(buffer).lock }
{
    lock.lock();
}


ImageBuffer::Reference::~Reference() {
    lock.unlock();
}

/* ======================================================== Public methods ======================================================== */

ImageBuffer::Reference ImageBuffer::acquire() {
    return Reference(*this);
}


const ImageBuffer::Reference ImageBuffer::acquire() const {
    return Reference(*this);
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

#endif
