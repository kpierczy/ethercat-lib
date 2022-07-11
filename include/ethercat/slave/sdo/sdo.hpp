/* ============================================================================================================================ *//**
 * @file       sdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the SDO nested class of the Slave interface
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_SDO_SDO_H__
#define __ETHERCAT_SLAVE_SDO_SDO_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave/sdo.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================ Common ============================================================ */

namespace details {

    template<bool flag = false>
    inline constexpr void sdo_instantiation_failure() {
        static_assert(flag, 
            "[ethercat::Slave::Sdo][BUG] Class template has been instantiated although the translator type "
            "does not provide sizing method."
        ); 
    }

}

/* ======================================================== Public methods ======================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> enabler>
typename Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::Address 
Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::get_address() const {
    return address;
}

/* ====================================================== Public I/O methods ====================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> enabler>
template<bool enable,
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::download(
    ArgType object,
    std::chrono::milliseconds timeout,
    AccessType access_type 
) {
    // If static-sizing method is available
    if constexpr(SizingTranslatorTraits::static_sizing::is_available) {

        // Make buffer for binary image of the object
        auto buffer = WrapperType::make_buffer();
        // Translate object into the binary image
        WrapperType::translate_from(buffer, object);
        // Perform I/O
        slave->download_sdo(
            address.index,
            address.subindex,
            config::types::Span<const uint8_t>{ buffer },
            timeout,
            (access_type == AccessType::Complete) ? true : false
        );

    // Else, if dynamic-sizing method is available
    } else if constexpr(SizingTranslatorTraits::dynamic_sizing::is_available) {

        // Make buffer for binary image of the object
        auto buffer = WrapperType::make_buffer(object);
        // Translate object into the binary image
        WrapperType::translate_from(buffer, object);
        // Perform I/O
        slave->download_sdo(
            address.index,
            address.subindex,
            config::types::Span<const uint8_t>{ buffer },
            timeout,
            (access_type == AccessType::Complete) ? true : false
        );

    // Else, make compillation stop
    } else
        details::sdo_instantiation_failure();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> enabler>
template<bool enable,
    std::enable_if_t<enable, bool>>
typename Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::Type 
Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::upload(
    std::chrono::milliseconds timeout,
    AccessType access_type
) const {

    // If static-sizing method is available
    if constexpr(SizingTranslatorTraits::static_sizing::is_available) {

        Type object;
        
        // Make buffer for binary image of the object
        auto buffer = WrapperType::make_buffer();
        // Perform I/O
        slave->upload_sdo(
            address.index,
            address.subindex,
            config::types::Span<uint8_t>{ buffer },
            timeout,
            (access_type == AccessType::Complete) ? true : false
        );
        // Translate object into the binary image
        WrapperType::translate_to(buffer, object);

        return object;

    // Else, if dynamic-sizing method is available
    } else if constexpr(SizingTranslatorTraits::dynamic_sizing::is_available) {

        Type object;

        // Make buffer for binary image of the object
        auto buffer = WrapperType::make_buffer(object);
        // Perform I/O
        slave->upload_sdo(
            address.index,
            address.subindex,
            config::types::Span<uint8_t>{ buffer },
            timeout,
            (access_type == AccessType::Complete) ? true : false
        );
        // Translate object into the binary image
        WrapperType::translate_to(buffer, object);

        return object;

    // Else, make compillation stop
    } else
        details::sdo_instantiation_failure();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> enabler>
template<bool enable,
    std::enable_if_t<enable, bool>>
void Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::upload(
    Type &object,
    std::chrono::milliseconds timeout,
    AccessType access_type
) const {

    // If static-sizing method is available
    if constexpr(SizingTranslatorTraits::static_sizing::is_available) {

        // Make buffer for binary image of the object
        auto buffer = WrapperType::make_buffer();
        // Perform I/O
        slave->upload_sdo(
            address.index,
            address.subindex,
            config::types::Span<uint8_t>{ buffer },
            timeout,
            (access_type == AccessType::Complete) ? true : false
        );
        // Translate object into the binary image
        WrapperType::translate_to(buffer, object);

    // Else, if dynamic-sizing method is available
    } else if constexpr(SizingTranslatorTraits::dynamic_sizing::is_available) {

        // Make buffer for binary image of the object
        auto buffer = WrapperType::make_buffer(object);
        // Perform I/O
        slave->upload_sdo(
            address.index,
            address.subindex,
            config::types::Span<uint8_t>{ buffer },
            timeout,
            (access_type == AccessType::Complete) ? true : false
        );
        // Translate object into the binary image
        WrapperType::translate_to(buffer, object);

    // Else, make compillation stop
    } else
        details::sdo_instantiation_failure();
}

/* ==================================================== Protected ctors & dtors =================================================== */

template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> enabler>
template<typename TranslatorU,
    slave::traits::enable_if_stateless_sizing_translator_t<dir, TranslatorU, T>>
Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::Sdo(
    SlaveT *slave,
    Address address
) :
    slave{ slave },
    address{ address }
{ }


template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T> enabler>
template<typename... ArgsT, typename TranslatorU,
    slave::traits::enable_if_statefull_sizing_translator_t<dir, TranslatorU, T> ctor_enabler>
Slave<ImplementationT>::template Sdo<dir, TranslatorT, T, enabler>::Sdo(
    SlaveT *slave,
    Address address,
    ArgsT&&... args
) :
    common::translation::SizingTranslatorWrapper<
        static_cast<common::translation::TranslationDirection>(dir),
        TranslatorT,
        T
    >{ std::forward<ArgsT>(args)... },
    slave{ slave },
    address{ address }
{ }

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
