/* ============================================================================================================================ *//**
 * @file       event_handler.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 10:01:18 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the EventHandler class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_HANDLERS_EVENT_HANDLER_H__
#define __ETHERCAT_COMMON_HANDLERS_EVENT_HANDLER_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <functional>
// Private includes
#include "ethercat/config.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::handlers {

/* ========================================================= EventHandler ========================================================= */

/**
 * @brief Auxiliary pair-like structure storing event handler functor along with it's
 *    synchronising long
 */
struct EventHandler {

    /**
     * @brief Synchronising lock
     * @performance Think through non-blocking synchronisation
     */
    mutable config::types::Lock lock;
    
    /**
     * @brief Handler
     * @performance Handlers utilize std::function<> to implement callbacks used in the critical
     *    path of the bus I/O loop. This utilizes dynamic memory and dynamic dispatch that can
     *    corrupt performance (std::function<> is not inlined very well on the GCC). Think about
     *    moving to other implementation of function<> template
     */
    std::function<void(void)> handler;

    /// Use default default constructor
    EventHandler() = default;

    /**
     * @brief Enable copy-construction to let handler be stored in relocable containers
     * @see operator=()
     */
    inline EventHandler(EventHandler &&rhandler);

    /**
     * @brief Enable copy-asignment to let handler be stored in relocable containers
     * @warning EventHandler stored a locking primitive. Usually such object are not movable as
     *    synchronized thread must keep valid location of the primitive in the memory. Move
     *    semantic is enabled for EventHandler @p only to let collection of handlers to be
     *    constructed in a dynamic-size containers (e.g. std::vector) <b>at the beggining
     *    of the program execution</b>. In particular objects of @ref Slave class template 
     *    that stored some EventHandler objects are stored by the @ref Master class in the 
     *    std::vector that is constructed (and not resized, thus no relocated) during object's
     *    lifetime. Move semantics for EventHandler should NOT be used after storing container
     *    is initially constructed.
     */
    inline EventHandler &operator=(EventHandler &&rhandler);

    /**
     * @brief Calls the handler if it is registered
     * @synchronised
     */
    inline void operator()() const;

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::handlers

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/handlers/event_handler/event_handler.hpp"

/* ================================================================================================================================ */

#endif