/* ============================================================================================================================ *//**
 * @file       master_pub.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 26th May 2022 5:38:41 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of public methods of the Master class providing API entry for implementing hardware-specific drivers
 *             of EtherCAT master devices
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_MASTER_MASTER_PUB_H__
#define __ETHERCAT_MASTER_MASTER_PUB_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// Private includes
#include "ethercat/master.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ===================================================== Public static methods ==================================================== */

template<typename ImplementationT,typename SlaveImplementationT>
constexpr std::string_view Master<ImplementationT, SlaveImplementationT>::state_to_str(State state) {
    switch(state) {
        case State::Init:   return "Init";
        case State::Preop:  return "Preop";
        case State::Safeop: return "Safeop";
        case State::Op:     return "Op";
        default:
            return "<Unknown>";
    }
}

/* ===================================================== Public common methods ==================================================== */

template<typename ImplementationT,typename SlaveImplementationT>
std::vector<std::string_view> Master<ImplementationT, SlaveImplementationT>::list_slaves() const {
    std::vector<std::string_view> ret;
    ret.reserve(slaves.size());
    for(const auto &slave : slaves)
        ret.push_back(slave.get_name());
    return ret;
}


template<typename ImplementationT,typename SlaveImplementationT>
std::vector<typename Master<ImplementationT, SlaveImplementationT>::SlaveT*> 
Master<ImplementationT, SlaveImplementationT>::get_slaves() {
    std::vector<SlaveT*> ret;
    ret.reserve(slaves.size());
    for(auto &slave : slaves)
        ret.push_back(&slave);
    return ret;
}


template<typename ImplementationT,typename SlaveImplementationT>
typename Master<ImplementationT, SlaveImplementationT>::SlaveT &
Master<ImplementationT, SlaveImplementationT>::get_slave(std::string_view name) {
    
    // Find slave on the list registered devices
    auto slave = std::find_if(slaves.begin(), slaves.end(), 
        [name](const auto &slave) { return (slave.get_name() == name); });
    
    // If slave not found, throw
    if(slave == slaves.end()) {
        std::stringstream ss;
        ss << "[ethercat::Master::get_slave] Non-existing slave requested "
           << "(" << name << ")";
        throw std::out_of_range{ ss.str() };
    }

    return *slave;
}


template<typename ImplementationT,typename SlaveImplementationT>
const typename Master<ImplementationT, SlaveImplementationT>::SlaveT &
Master<ImplementationT, SlaveImplementationT>::get_slave(std::string_view name) const {
    return (const_cast<Master*>(this)->get_slave(name));
}


template<typename ImplementationT,typename SlaveImplementationT>
template<typename HandlerT>
void Master<ImplementationT, SlaveImplementationT>::register_event_handler(
    Event event,
    HandlerT &&handler
) {
    common::handlers::EventHandler *phandler;
    
    // Select requested handler
    switch(event) {
        case Event::ReadBusStart:                 phandler = &handlers.at_read_bus_start;                   break;
        case Event::ReadBusComplete:              phandler = &handlers.at_read_bus_complete;                break;
        case Event::ReadBusSlavesUpdateComplete:  phandler = &handlers.at_read_bus_slaves_update_complete;  break;
        case Event::WriteBusStart:                phandler = &handlers.at_write_bus_start;                  break;
        case Event::WriteBusSlavesUpdateComplete: phandler = &handlers.at_write_bus_slaves_update_complete; break;
        case Event::WriteBusComplete:             phandler = &handlers.at_write_bus_complete;               break;
        default:
            using namespace std::literals::string_literals;
            throw std::out_of_range{ 
                "[ethercat::Master::register_event_handler] Invalid event ID given " 
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


template<typename ImplementationT,typename SlaveImplementationT>
void Master<ImplementationT, SlaveImplementationT>::unregister_event_handler(
    Event event
) {
    common::handlers::EventHandler *phandler;

    // Select requested handler
    switch(event) {
        case Event::ReadBusStart:                 phandler = &handlers.at_read_bus_start;                   break;
        case Event::ReadBusComplete:              phandler = &handlers.at_read_bus_complete;                break;
        case Event::ReadBusSlavesUpdateComplete:  phandler = &handlers.at_read_bus_slaves_update_complete;  break;
        case Event::WriteBusStart:                phandler = &handlers.at_write_bus_start;                  break;
        case Event::WriteBusSlavesUpdateComplete: phandler = &handlers.at_write_bus_slaves_update_complete; break;
        case Event::WriteBusComplete:             phandler = &handlers.at_write_bus_complete;               break;
        default:
            using namespace std::literals::string_literals;
            throw std::out_of_range{ 
                "[ethercat::Master::unregister_event_handler] Invalid event ID given " 
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

template<typename ImplementationT,typename SlaveImplementationT>
std::chrono::microseconds Master<ImplementationT, SlaveImplementationT>::get_bus_cycle() const {
    return bus_cycle;
}


template<typename ImplementationT,typename SlaveImplementationT>
typename Master<ImplementationT, SlaveImplementationT>::State 
Master<ImplementationT, SlaveImplementationT>::get_state(
    std::chrono::milliseconds timeout
) const {
    return impl().get_state_impl(timeout);
}


template<typename ImplementationT,typename SlaveImplementationT>
void Master<ImplementationT, SlaveImplementationT>::set_state(
    State state,
    std::chrono::milliseconds timeout
) {
    impl().set_state_impl(state, timeout);
}

/* ================================================== Public EtherCAT I/O methods ================================================= */

template<typename ImplementationT,typename SlaveImplementationT>
std::vector<uint8_t> &Master<ImplementationT, SlaveImplementationT>::_get_input_buffer() {
    return input_pdi.data;
}


template<typename ImplementationT,typename SlaveImplementationT>
const std::vector<uint8_t> &Master<ImplementationT, SlaveImplementationT>::_get_input_buffer() const {
    return input_pdi.data;
}


template<typename ImplementationT,typename SlaveImplementationT>
std::vector<uint8_t> &Master<ImplementationT, SlaveImplementationT>::_get_output_buffer() {
    return output_pdi.data;
}


template<typename ImplementationT,typename SlaveImplementationT>
const std::vector<uint8_t> &Master<ImplementationT, SlaveImplementationT>::_get_output_buffer() const {
    return output_pdi.data;
}


template<typename ImplementationT,typename SlaveImplementationT>
void Master<ImplementationT, SlaveImplementationT>::read_bus(std::chrono::milliseconds timeout) {

    // Call 'start' handler
    handlers.at_read_bus_start();
    
    {
        // Acquire input PDI
        std::scoped_lock guard{ input_pdi.lock };
        
        // Perform I/O
        impl().read_bus_impl(input_pdi.data, timeout);
        
        // Update input PDO entries of all slaves with incoming PDI
        for(auto &slave : slaves) {

            // Update all input PDO entries of the slave
            for(auto &pdo : slave.template get_pdos<SlaveT::PdoDirection::Input>())
                for(auto &entry : pdo.get_entries())
                    entry.update(input_pdi.data);

        }
    }

    // Call 'I/O end' handler
    handlers.at_read_bus_complete();

    // Notify slaves that their Input PDOs has been updated
    for(auto &slave : slaves)
        slave.template notify<SlaveT::PdoDirection::Input>();
        
    // Call 'Slaves update end' handler
    handlers.at_read_bus_slaves_update_complete();
}


template<typename ImplementationT,typename SlaveImplementationT>
void Master<ImplementationT, SlaveImplementationT>::write_bus(std::chrono::milliseconds timeout) {

    // Call 'start' handler
    handlers.at_write_bus_start();
        
    // Notify all slave's that their Output PDOs will be written to the bus
    for(auto &slave : slaves)
        slave.template notify<SlaveT::PdoDirection::Output>();

    // Call 'Slaves update end' handler
    handlers.at_write_bus_slaves_update_complete();
    
    {
        // Acquire output PDI
        std::scoped_lock guard{ output_pdi.lock };

        // Update outgoing PDI with output PDO entries of all slaves
        for(auto &slave : slaves) {

            // Update all input PDO entries of the slave
            for(auto &pdo : slave.template get_pdos<SlaveT::PdoDirection::Output>())
                for(auto &entry : pdo.get_entries())
                    entry.update(output_pdi.data);

        }
        
        // Perform I/O
        impl().write_bus_impl(
            const_cast<const std::vector<uint8_t>&>(output_pdi.data),
            timeout
        );
    }

    // Call 'I/O end' handler
    handlers.at_write_bus_complete();

}

/* ================================================================================================================================ */

} // End namespace ethercat

#endif
