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

#ifndef __ETHERCAT_DRIVERS_SLAVE_SLAVE_PUB_H__
#define __ETHERCAT_DRIVERS_SLAVE_SLAVE_PUB_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/drivers/slave.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::drivers {

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
    EventHandler *phandler;
    
    // Select requested handler
    switch(event) {
        case Event::InputsUpdate:  phandler = &handlers.at_inputs_update;  break;
        case Event::OutputsUpdate: phandler = &handlers.at_outputs_update; break;
        default:
            using namespace std::literals::string_literals;
            throw std::out_of_range{ 
                "[ethercat::drivers::Slave::register_event_handler] Invalid event ID given " 
                  "("s
                + std::to_string(utilities::to_underlying(event))
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
    EventHandler *phandler;

    // Select requested handler
    switch(event) {
        case Event::InputsUpdate:  phandler = &handlers.at_inputs_update;  break;
        case Event::OutputsUpdate: phandler = &handlers.at_outputs_update; break;
        default:
            using namespace std::literals::string_literals;
            throw std::out_of_range{ 
                "[ethercat::drivers::Slave::unregister_event_handler] Invalid event ID given " 
                  "("s
                + std::to_string(utilities::to_underlying(event))
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
Slave<ImplementationT>::get_state(std::chrono::milliseconds timeout) {
    return impl().get_state_impl(timeout);
}

template<typename ImplementationT>
void Slave<ImplementationT>::set_state(State state, std::chrono::milliseconds timeout) {
    impl().set_state_impl(state, timeout);
}

/* ================================================== Public access methods (SDO) ================================================= */

template<typename ImplementationT>
template<abstract::objects::Type type>
void Slave<ImplementationT>::download_object(
    const abstract::sdo::Request &request,
    abstract::objects::ValueType<type> data,
    std::chrono::milliseconds timeout
) {
    // Call underlying function with type dispatched
    impl().download_object(
        request,
        ranges::span{ static_cast<const uint8_t*>(static_cast<const void*>(&data)), sizeof(data) },
        timeout
    );

    /**
     * @warning Direct casting @p data to the array of bytes can be safely performed only on Little
     *    Endian systems. It is because data passed to the implementation function is directly inserted
     *    into the SDO request which - as EtherCAT protocol in general - utilizes Little Endian format.
     *    If platform change was to be introduced, the endianness-conversion would be required.
     */
}


template<typename ImplementationT>
template<abstract::objects::Type type>
abstract::objects::ValueType<type> 
Slave<ImplementationT>::upload_object(
    const abstract::sdo::Request &request,
    std::chrono::milliseconds timeout
) {
    // Prepare buffer for response
    std::array<uint8_t, sizeof(abstract::objects::ValueType<type>)> data;

    // Call underlying function with type dispatched
    impl().upload_object(
        request,
        ranges::span{ data.data(), data.size() },
        timeout
    );

    abstract::objects::ValueType<type> ret;

    // Parse response
    std::copy(
        data.begin(),
        data.end(),
        static_cast<uint8_t*>(static_cast<void*>(&ret))
    );

    return ret;

    /**
     * @warning Direct copying array of bytes to the @p data can be safely performed only on Little
     *    Endian systems. It is because array passed to the implementation function is directly filled
     *    with bytes from the SDO response which - as EtherCAT protocol in general - utilizes Little 
     *    Endian format. If platform change was to be introduced, the endianness-conversion would be 
     *    required.
     */
}

/* ================================================== Public access methods (PDO) ================================================= */

template<typename ImplementationT>
template<abstract::pdo::Direction dir, abstract::objects::Type type>
std::optional<abstract::objects::ValueBufferReference<type>> 
Slave<ImplementationT>::get_pdo_reference(std::string_view name) {

    // Find PDO in the entries' list
    auto pdo = find_pdo_entry<dir, type>(name);
    // On failure, return
    if(not pdo.has_value())
        return std::optional<abstract::objects::ValueBufferReference<type>>{ };

    // Return reference to the PDO
    return (*pdo)->template get_reference<type>();
}


template<typename ImplementationT>
template<abstract::pdo::Direction dir, abstract::objects::Type type>
bool Slave<ImplementationT>::register_pdo_handler(
    std::string_view name,
    abstract::pdo::HandlerType<dir, type> handler
) {

    // Find PDO in the entries' list
    auto pdo = find_pdo_entry<dir, type>(name);
    // On failure, return
    if(not pdo.has_value())
        return false;

    // Register handler
    (*pdo)->template register_pdo_handler<type>(handler);

    return true;
}


template<typename ImplementationT>
template<abstract::pdo::Direction dir, abstract::objects::Type type>
bool Slave<ImplementationT>::unregister_pdo_handler(std::string_view name) {

    // Find PDO in the entries' list
    auto pdo = find_pdo_entry<dir, type>(name);
    // On failure, return
    if(not pdo.has_value())
        return false;

    // Register handler
    (*pdo)->template unregister_pdo_handler<type>();

    return true;
}

/* ================================================================================================================================ */

} // End namespace ethercat::drivers

#endif
