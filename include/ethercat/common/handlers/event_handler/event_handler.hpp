/* ============================================================================================================================ *//**
 * @file       event_handler.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 10:01:18 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the EventHandler class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_HANDLERS_EVENT_HANDLER_EVENT_HANDLER_H__
#define __ETHERCAT_COMMON_HANDLERS_EVENT_HANDLER_EVENT_HANDLER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/handlers/event_handler.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::handlers {

/* ========================================================= EventHandler ========================================================= */

EventHandler::EventHandler(EventHandler &&rhandler) :
    handler{ std::move(rhandler.handler) }
{ }


EventHandler &EventHandler::operator=(EventHandler &&rhandler) {
    handler = std::move(rhandler.handler);
    return *this;
}


void EventHandler::operator()() const {
    std::lock_guard guard{ lock };
    if(handler)
        handler();
}

/* ================================================================================================================================ */

} // End namespace ethercat::common::handlers

#endif