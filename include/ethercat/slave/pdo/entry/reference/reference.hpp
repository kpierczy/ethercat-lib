/* ============================================================================================================================ *//**
 * @file       reference.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 13th June 2022 3:43:33 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the Reference class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_ENTRY_REFERENCE_REFERENCE_H__
#define __ETHERCAT_SLAVE_PDO_ENTRY_REFERENCE_REFERENCE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave/pdo/entry/reference.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ==================================================== Public getters/setters ==================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T>
template<bool enable, 
    std::enable_if_t<enable, bool>>
typename Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>::Type 
Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>::get() const {
    std::scoped_lock guard{ buffer->lock };
    Type object;
    WrapperType::translate_to(buffer->buffer, object);
    return object;
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T>
template<bool enable, 
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>::get(Type &object) const {
    std::scoped_lock guard{ buffer->lock };
    WrapperType::translate_to(buffer->buffer, object);
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T>
template<bool enable, 
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>::set(ArgType object) {
    std::scoped_lock guard{ buffer->lock };
    WrapperType::translate_from(buffer->buffer, object);
}

/* ======================================================== Protected ctors ======================================================= */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T>
template<bool enable,
    std::enable_if_t<enable, bool>>
Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>::Reference(
    const types::Type &type,
    Buffer &buffer
) :
    buffer{ &buffer }
{ }


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T>
template<bool enable, typename... ArgsT,
    std::enable_if_t<enable, bool>>
Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>::Reference(
    const types::Type &type,
    Buffer &buffer,
    ArgsT&&... args
) :
    common::translation::TranslatorWrapper<slave::traits::to_translation_dir<dir>, TranslatorT, T>{ std::forward<ArgsT>(args)... },
    buffer{ &buffer }
{ }

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
