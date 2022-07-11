/* ============================================================================================================================ *//**
 * @file       entry.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the Entry class representing a PDO entry
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_ENTRY_ENTRY_H__
#define __ETHERCAT_SLAVE_PDO_ENTRY_ENTRY_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/utilities/bit.hpp"
#include "ethercat/slave/pdo/entry.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ================================================ Public EtherCAT common methods ================================================ */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
std::string_view Slave<ImplementationT>::template Pdo<dir>::Entry::get_name() const {
    return name;
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
const types::Type &Slave<ImplementationT>::template Pdo<dir>::Entry::get_type() const {
    return type;
}

/* ============================================== Public methods (entry-referencing) ============================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename... ArgsT,
    slave::traits::enable_if_translator_t<dir, TranslatorT>>
typename Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT> 
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference(
    ArgsT&&... args
) {
    return Reference<TranslatorT>{ type, buffer, args... };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename... ArgsT,
    slave::traits::enable_if_translator_t<dir, TranslatorT>>
const typename Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT> 
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference(
    ArgsT&&... args
) const {
    return Reference<TranslatorT>{ type, const_cast<Buffer&>(buffer), args... };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T, typename... ArgsT,
    slave::traits::enable_if_translator_t<dir, TranslatorT, T>>
typename Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference(
    ArgsT&&... args
) {
    return Reference<TranslatorT, T>{ type, buffer, args... };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T, typename... ArgsT,
    slave::traits::enable_if_translator_t<dir, TranslatorT, T>>
const typename Slave<ImplementationT>::template Pdo<dir>::Entry::template Reference<TranslatorT, T>
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference(
    ArgsT&&... args
) const {
    return Reference<TranslatorT, T>{ type, const_cast<Buffer&>(buffer), args... };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename T,
    slave::traits::enable_if_default_translatable_t<T>>
typename Slave<ImplementationT>::template Pdo<dir>::Entry::template DefaultTranslatedReference<T>
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference() {
    return DefaultTranslatedReference<T>{ type, buffer };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename T,
    slave::traits::enable_if_default_translatable_t<T>>
const typename Slave<ImplementationT>::template Pdo<dir>::Entry::template DefaultTranslatedReference<T>
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference() const {
    return DefaultTranslatedReference<T>{ type, const_cast<Buffer&>(buffer) };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<types::BuiltinType::ID type_id, std::size_t arity,
    slave::traits::enable_if_builtin_default_translatable_t<type_id, arity>>
typename Slave<ImplementationT>::template Pdo<dir>::Entry::template BuiltinDefaultTranslatedReference<type_id, arity>
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference() {
    return BuiltinDefaultTranslatedReference<type_id, arity>{ type, buffer };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<types::BuiltinType::ID type_id, std::size_t arity,
    slave::traits::enable_if_builtin_default_translatable_t<type_id, arity>>
const typename Slave<ImplementationT>::template Pdo<dir>::Entry::template BuiltinDefaultTranslatedReference<type_id, arity>
Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference() const {
    return BuiltinDefaultTranslatedReference<type_id, arity>{ type, const_cast<Buffer&>(buffer) };
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename ReferenceType, bool enable,
    std::enable_if_t<enable, bool>>
auto Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference() {
    using ReferenceT = common::utilities::traits::remove_cvref_t<ReferenceType>;
    return get_reference<
        typename ReferenceT::TranslatorType,
        typename ReferenceT::Type
    >();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename ReferenceType, bool enable,
    std::enable_if_t<enable, bool>>
const auto Slave<ImplementationT>::template Pdo<dir>::Entry::get_reference() const {
    using ReferenceT = common::utilities::traits::remove_cvref_t<ReferenceType>;
    return get_reference<
        typename ReferenceT::TranslatorType,
        typename ReferenceT::Type
    >();
}

/* ======================================================== Protected ctors ======================================================= */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
Slave<ImplementationT>::template Pdo<dir>::Entry::Entry(
    eni::Slave::Pdo::Entry entry_description,
    eni::ProcessImage::Variable pdi_variable
) :
    name{ entry_description.get_name() },
    type{ entry_description.get_data_type() },
    buffer{ pdi_variable.get_bit_size() , pdi_variable.get_bit_offset() }
{
    // Check if data types match
    if(entry_description.get_data_type() != pdi_variable.get_data_type()) {

        std::stringstream ss;
        ss << "[ethercat::Slave::Pdo::Entry::Entry] PDO Entry "
            << "'" << entry_description.get_name() << "' "
            << "has different type in <Slave> description (" << entry_description.get_data_type().get_name() << ")' "
            << "and in <ProcessImage> description (" << pdi_variable.get_data_type().get_name() << ")";
        throw eni::Error{ ss.str() };

    // Check if bit sizes match
    } else if(entry_description.get_bit_len() != pdi_variable.get_bit_size()) {

        std::stringstream ss;
        ss << "[ethercat::Slave::Pdo::Entry::Entry] PDO Entry "
            << "'" << entry_description.get_name() << "' "
            << "has different bitsize in <Slave> description (" << entry_description.get_bit_len() << ")' "
            << "and in <ProcessImage> description (" << pdi_variable.get_bit_size() << ")";
        throw eni::Error{ ss.str() };

    }

    using namespace common::utilities::bit;

    // Check if data is byte-aligned in PDI (if bit-aligned data support is disabled)
    if constexpr(not config::BitAlignedPdoSupport) {
        if(pdi_variable.get_bit_size() % BITS_IN_BYTE != 0) {

            std::stringstream ss;
            ss << "[ethercat::Slave::Pdo::Entry::Entry] PDO Entry "
                << "'" << entry_description.get_name() << "' "
                << "has not byte-aligned size (" << pdi_variable.get_bit_size() << ")' "
                << "although bit-aligned data support is disabled";
            throw eni::Error{ ss.str() };

        } else if(pdi_variable.get_bit_offset() % BITS_IN_BYTE != 0) {

            std::stringstream ss;
            ss << "[ethercat::Slave::Pdo::Entry::Entry] PDO Entry "
                << "'" << entry_description.get_name() << "' "
                << "has not byte-aligned bitoffset in the PDI (" << pdi_variable.get_bit_offset() << ")' "
                << "although bit-aligned data support is disabled";
            throw eni::Error{ ss.str() };
            
        }
    }
            
}

/* ======================================================== Private methods ======================================================= */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<bool enable,
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Pdo<dir>::Entry::Entry::update(config::types::Span<const uint8_t> pdi) {
    buffer.update(pdi);
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<bool enable,
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Pdo<dir>::Entry::Entry::update(config::types::Span<uint8_t> pdi) const {
    buffer.update(pdi);
}

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
