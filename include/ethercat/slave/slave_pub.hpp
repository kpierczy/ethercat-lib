/* ============================================================================================================================ *//**
 * @file       slave_pub.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public methods of the Slave class representing slave device on the EtherCAT bus
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_SLAVE_PUB_H__
#define __ETHERCAT_SLAVE_SLAVE_PUB_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ===================================================== Public static methods ==================================================== */

template<typename ImplementationT>
constexpr std::string_view Slave<ImplementationT>::state_to_str(State state) {
    switch(state) {
        case State::Init:   return "Init";
        case State::Preop:  return "Preop";
        case State::Boot:   return "Boot";
        case State::Safeop: return "Safeop";
        case State::Op:     return "Op";
        default:
            return "<Unknown>";
    }
}

/* ===================================================== Public common methods ==================================================== */

template<typename ImplementationT>
template<typename HandlerT>
void Slave<ImplementationT>::register_event_handler(
    Event event,
    HandlerT &&handler
) {
    common::handlers::EventHandler *phandler;
    
    // Select requested handler
    switch(event) {
        case Event::InputsUpdate:  phandler = &handlers.at_inputs_update;  break;
        case Event::OutputsUpdate: phandler = &handlers.at_outputs_update; break;
        default:
            using namespace std::literals::string_literals;
            throw std::out_of_range{ 
                "[ethercat::Slave::register_event_handler] Invalid event ID given " 
                  "("s
                + std::to_string(common::utilities::to_underlying(event))
                + ")" 
            };
    }

    // Lock the handler
    std::lock_guard guard{ phandler->lock };
    // Set the new handler
    phandler->handler = handler;
}


template<typename ImplementationT>
void Slave<ImplementationT>::unregister_event_handler(
    Event event
) {
    common::handlers::EventHandler *phandler;

    // Select requested handler
    switch(event) {
        case Event::InputsUpdate:  phandler = &handlers.at_inputs_update;  break;
        case Event::OutputsUpdate: phandler = &handlers.at_outputs_update; break;
        default:
            using namespace std::literals::string_literals;
            throw std::out_of_range{ 
                "[ethercat::Slave::unregister_event_handler] Invalid event ID given " 
                  "("s
                + std::to_string(common::utilities::to_underlying(event))
                + ")" 
            };
    }

    // Lock the handler
    std::lock_guard guard{ phandler->lock };
    // Reset the handler
    phandler->handler = nullptr;
}

/* ================================================ Public EtherCAT common methods ================================================ */

template<typename ImplementationT>
eni::Slave Slave<ImplementationT>::get_eni() const {
    return eni_description;
}


template<typename ImplementationT>
std::string_view Slave<ImplementationT>::get_name() const {
    return name;
}


template<typename ImplementationT>
uint16_t Slave<ImplementationT>::get_fixed_addr() const {
    return fixed_addr;
}


template<typename ImplementationT>
uint16_t Slave<ImplementationT>::get_auto_increment_addr() const {
    return auto_increment_addr;
}


template<typename ImplementationT>
uint16_t Slave<ImplementationT>::get_topological_addr() const {
    return topological_addr;
}


template<typename ImplementationT>
typename Slave<ImplementationT>::State 
Slave<ImplementationT>::get_state(std::chrono::milliseconds timeout) const {
    return impl().get_state_impl(timeout);
}

template<typename ImplementationT>
void Slave<ImplementationT>::set_state(State state, std::chrono::milliseconds timeout) {
    impl().set_state_impl(state, timeout);
}

/* ================================================== Public access methods (SDO) ================================================= */

// Custom translator overload (auto-deduced target type)
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename... ArgsT,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT>>
typename Slave<ImplementationT>::template Sdo<dir, TranslatorT> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex,
    ArgsT&&... args
) {
    return Sdo<dir, TranslatorT>{ &impl(), typename Sdo<dir, TranslatorT>::Address{ index, subindex }, args... };
}

// Custom translator overload (auto-deduced target type) [const]
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename... ArgsT,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT>>
const typename Slave<ImplementationT>::template Sdo<dir, TranslatorT> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex,
    ArgsT&&... args
) const {
    return Sdo<dir, TranslatorT>{ &impl(), typename Sdo<dir, TranslatorT>::Address{ index, subindex }, args... };
}

// Custom translator overload (manually-deduced target type)
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T, typename... ArgsT,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T>>
typename Slave<ImplementationT>::template Sdo<dir, TranslatorT, T> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex,
    ArgsT&&... args
) {
    return Sdo<dir, TranslatorT, T>{ &impl(), typename Sdo<dir, TranslatorT, T>::Address{ index, subindex }, args... };
}

// Custom translator overload (manually-deduced target type) [const]
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T, typename... ArgsT,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T>>
const typename Slave<ImplementationT>::template Sdo<dir, TranslatorT, T> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex,
    ArgsT&&... args
) const {
    return Sdo<dir, TranslatorT, T>{ &impl(), typename Sdo<dir, TranslatorT, T>::Address{ index, subindex }, args... };
}

// Default translator overload
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename T,
    slave::traits::enable_if_default_translatable_t<T>>
typename Slave<ImplementationT>::template DefaultTranslatedSdo<dir, T> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex
) {
    return DefaultTranslatedSdo<dir, T>{ &impl(), typename DefaultTranslatedSdo<dir, T>::Address{ index, subindex } };
}

// Default translator overload [const]
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename T,
    slave::traits::enable_if_default_translatable_t<T>>
const typename Slave<ImplementationT>::template DefaultTranslatedSdo<dir, T> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex
) const {
    return DefaultTranslatedSdo<dir, T>{ &impl(), typename DefaultTranslatedSdo<dir, T>::Address{ index, subindex } };
}

// Default translator overload (type given as ID of the EtherCAT builtin type)
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, types::BuiltinType::ID type_id, std::size_t arity,
    slave::traits::enable_if_builtin_default_translatable_t<type_id, arity>>
typename Slave<ImplementationT>::template BuiltinTypeSdo<dir, type_id, arity> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex
) {
    return BuiltinTypeSdo<dir, type_id, arity>{ &impl(), typename BuiltinTypeSdo<dir, type_id, arity>::Address{ index, subindex } };
}

// Default translator overload (type given as ID of the EtherCAT builtin type) [const]
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, types::BuiltinType::ID type_id, std::size_t arity,
    slave::traits::enable_if_builtin_default_translatable_t<type_id, arity>>
const typename Slave<ImplementationT>::template BuiltinTypeSdo<dir, type_id, arity> Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex
) const {
    return BuiltinTypeSdo<dir, type_id, arity>{ &impl(), typename BuiltinTypeSdo<dir, type_id, arity>::Address{ index, subindex } };
}

// Auto-deduction overload
template<typename ImplementationT>
template<typename SdoType, bool enable,
    std::enable_if_t<enable, bool>>
auto Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex
) {
    using SdoT = common::utilities::traits::remove_cvref_t<SdoType>;
    return get_sdo<
        SdoType::Dir,
        typename SdoType::TranslatorType,
        typename SdoType::Type
    >(index, subindex);
}

// Auto-deduction overload [const]
template<typename ImplementationT>
template<typename SdoType, bool enable,
    std::enable_if_t<enable, bool>>
const auto Slave<ImplementationT>::get_sdo(
    uint16_t index,
    uint16_t subindex
) const {
    using SdoT = common::utilities::traits::remove_cvref_t<SdoType>;
    return get_sdo<
        SdoType::Dir,
        typename SdoType::TranslatorType,
        typename SdoType::Type
    >(index, subindex);
}

/* ================================================== Public access methods (PDO) ================================================= */

namespace details {

    template<bool flag = false>
    constexpr inline void slave_dir_no_match() {
        static_assert(flag, "[ethercat::Slave] Invalid PDO direction given"); 
    }

}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
std::vector<typename Slave<ImplementationT>::template Pdo<dir>> &Slave<ImplementationT>::get_pdos() {
    if constexpr(dir == PdoDirection::Input)
        return inputs;
    else if constexpr(dir == PdoDirection::Output)
        return outputs;
    else
        details::slave_dir_no_match();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
const std::vector<typename Slave<ImplementationT>::template Pdo<dir>> &Slave<ImplementationT>::get_pdos() const {
    return const_cast<Slave*>(this)->get_pdos();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
bool Slave<ImplementationT>::has_pdo(std::string_view name) const {
    if constexpr(dir == PdoDirection::Input) {
        return (std::find_if(inputs.begin(), inputs.end(),   [name](const auto &pdo){ return (pdo.get_name() == name); }) != inputs.end());
    } else if constexpr(dir == PdoDirection::Output) {
        return (std::find_if(outputs.begin(), outputs.end(), [name](const auto &pdo){ return (pdo.get_name() == name); }) != outputs.end());
    } else
        details::slave_dir_no_match();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
typename Slave<ImplementationT>::template Pdo<dir> &Slave<ImplementationT>::get_pdo(std::string_view name) {
    if constexpr(dir == PdoDirection::Input) {

        auto it = std::find_if(inputs.begin(), inputs.end(), [name](const auto &pdo){ return (pdo.get_name() == name); });
        if(it == inputs.end())
            throw std::out_of_range{ "[ethercat::Slave::get_pdo] No input PDO named '" + std::string{ name } + "'" };
        return *it;

    } else if constexpr(dir == PdoDirection::Output) {

        auto it = std::find_if(outputs.begin(), outputs.end(), [name](const auto &pdo){ return (pdo.get_name() == name); });
        if(it == outputs.end())
            throw std::out_of_range{ "[ethercat::Slave::get_pdo] No output PDO named '" + std::string{ name } + "'" };
        return *it;
        
    } else
        details::slave_dir_no_match();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
const typename Slave<ImplementationT>::template Pdo<dir> &Slave<ImplementationT>::get_pdo(std::string_view name) const {
    return const_cast<Slave*>(this)->get_pdo(name);
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
typename Slave<ImplementationT>::template Pdo<dir>::Entry &Slave<ImplementationT>::get_pdo_entry(std::string_view name) {
    if constexpr(dir == PdoDirection::Input) {

        for(auto &pdo : inputs) {
            if(pdo.has_entry(name))
                return pdo.get_entry(name);
        }

        throw std::out_of_range{ "[ethercat::Slave::get_pdo_entry] No input PDO entry named '" + std::string{ name } + "'" };

    } else if constexpr(dir == PdoDirection::Output) {

        for(auto &pdo : outputs) {
            if(pdo.has_entry(name))
                return pdo.get_entry(name);
        }

        throw std::out_of_range{ "[ethercat::Slave::get_pdo_entry] No output PDO entry named '" + std::string{ name } + "'" };
        
    } else
        details::slave_dir_no_match();
}


template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
const typename Slave<ImplementationT>::template Pdo<dir>::Entry &Slave<ImplementationT>::get_pdo_entry(std::string_view name) const {
    return const_cast<Slave*>(this)->get_pdo(name);
}

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
