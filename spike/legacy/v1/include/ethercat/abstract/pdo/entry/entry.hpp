/* ============================================================================================================================ *//**
 * @file       entry.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 12th May 2022 3:44:31 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the helper wrapper class around Process Data Image buffer poviding synchronisation
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_ENTRY_ENTRY_H__
#define __ETHERCAT_ABSTRACT_PDO_ENTRY_ENTRY_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/pdo/entry.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::pdo {

/* ========================================================= Public ctors ========================================================= */

template<Direction dir>
Entry<dir>::Entry(
    std::string_view name,
    abstract::objects::Type type, 
    ranges::span<uint8_t> data
) :
    name{ name },
    object{ type, data },
    handler{ type }
{ }


template<Direction dir>
template<typename Handler>
Entry<dir>::Entry(
    std::string_view name,
    abstract::objects::Type type, 
    ranges::span<uint8_t> data,
    Handler&& handler
) :
    name{ name },
    object{ type, data },
    handler{ type, std::forward<Handler>(handler) }
{ }


template<Direction dir>
Entry<dir>::Entry(Entry&& rentry) :
    name{ rentry.name },
    object{ std::move(rentry.object) },
    handler{ rentry.handler.get_type() }
{ 
    // Move handler in synchronised fashion
    std::lock_guard guard{ rentry.handler_lock };
    handler = rentry.handler;
}

/* ================================================= Public methods (client code) ================================================= */

template<Direction dir>
std::string_view Entry<dir>::get_name() const {
    return name;
}


template<Direction dir>
abstract::objects::Type Entry<dir>::get_type() const {
    return object.get_type();
}


template<Direction dir>
abstract::objects::ValueBuffer &Entry<dir>::get_reference() {
    return object.get_reference();
}


template<Direction dir>
const abstract::objects::ValueBuffer &Entry<dir>::get_reference() const {
    return object.get_reference();
}


template<Direction dir>
template<abstract::objects::Type type>
abstract::objects::ValueBufferReference<type> Entry<dir>::get_reference() const {
    return object.get_reference<type>();
}


template<Direction dir>
template<abstract::objects::Type type>
void Entry<dir>::register_pdo_handler(const HandlerType &handler) {
    std::lock_guard guard{ handler_lock };
    this->handler.set<type>(handler);
}


template<Direction dir>
template<typename Handler>
void Entry<dir>::register_pdo_handler(Handler handler) {
    std::lock_guard guard{ handler_lock };
    this->handler.set(handler);
}


template<Direction dir>
template<abstract::objects::Type type>
void Entry<dir>::unregister_pdo_handler() {
    std::lock_guard guard{ handler_lock };
    this->handler.set<type>(nullptr);
}


template<Direction dir>
void Entry<dir>::unregister_pdo_handler() {
    std::lock_guard guard{ handler_lock };
    this->handler.set(nullptr);
}

/* ================================================= Private methods (PDI manager) ================================================ */

namespace details {

    template<bool flag = false>
    inline constexpr void entry_dir_no_match() {
        static_assert(flag, "[ethercat::abstract::pdo::Entry::update] Invalid entry direction"); 
    }

}

template<Direction dir>
void Entry<dir>::update() {

    /// Update Input Entry
    if constexpr(dir == Direction::Input) {

        // Parse entry from the PDI
        auto value = object.read();

        // Synchronise access to the handler
        std::lock_guard guard{ handler_lock };
        // Call handler, if registered
        if(not handler.is_empty())
            handler.visit(value.get());

    /// Update Output Entry
    } else if constexpr(dir == Direction::Output) {

        // Synchronise access to the handler
        handler_lock.lock();

        // Call handler, if registered, to obtain a new value
        if(not handler.is_empty()) {

            // Create temporary variant to obtain a new value for the Entry from handler
            auto val = utilities::make_variant<abstract::objects::ValueStorage>( handler.get().index() );
            // Obtain new value from the handler
            handler.visit(val);
            // Free the handler
            handler_lock.unlock();

            // Set new value as the obejct's vale and serialize it to the PDI
            object.write(val);

        // Else, serialize current value of the entry
        } else
            object.serialize();

    // Invalid specialization (sanity check)
    } else 
        details::entry_dir_no_match();

}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

#endif
