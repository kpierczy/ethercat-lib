/* ============================================================================================================================ *//**
 * @file       image_buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 12th May 2022 1:56:41 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of the auxiliary class providing dynamically dispatchable handler for incoming/outgoin PDO-mapped objects
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_HANDLER_HANDLER_H__
#define __ETHERCAT_ABSTRACT_PDO_HANDLER_HANDLER_H__

/* =========================================================== Includes =========================================================== */

// Boost includes
#include <boost/mp11.hpp>
// Private includes
#include "ethercat/abstract/pdo/handler.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::pdo {

/* ======================================================= Getters & setters ====================================================== */

constexpr std::size_t handler_index(abstract::objects::Type type) {
    return utilities::to_underlying(type);
}


template<abstract::objects::Type type>
InputHandlerType<type> &get(InputHandlerStorage &handler_storage) {
    return std::get<HandlerIndex<type>>(handler_storage);
}


template<abstract::objects::Type type>
const InputHandlerType<type> &get(const InputHandlerStorage &handler_storage) {
    return std::get<HandlerIndex<type>>(handler_storage);
}


template<abstract::objects::Type type>
OutputHandlerType<type> &get(OutputHandlerStorage &handler_storage) {
    return std::get<HandlerIndex<type>>(handler_storage);
}


template<abstract::objects::Type type>
const OutputHandlerType<type> &get(const OutputHandlerStorage &handler_storage) {
    return std::get<HandlerIndex<type>>(handler_storage);
}

/* ========================================================= Public ctors ========================================================= */

template<Direction dir>
Handler<dir>::Handler(abstract::objects::Type type) :
    handler{ utilities::make_variant<Storage>(handler_index(type)) }
{ }

template<Direction dir>
template<typename HandlerType>
Handler<dir>::Handler(abstract::objects::Type type, HandlerType&& handler) :
    handler{ utilities::make_variant<Storage>(handler_index(type), handler) }
{ }

/* ======================================================= Public operators ======================================================= */


template<Direction dir>
Handler<dir>::operator Storage() const {
    return handler;
}


template<Direction dir>
Handler<dir>::operator Storage&() {
    return handler;
}


template<Direction dir>
Handler<dir>::operator const Storage&() const {
    return handler;
}


template<Direction dir>
template<typename HandlerType>
Handler<dir> &Handler<dir>::operator=(HandlerType handler) {
    this->handler = handler;
    return *this;
}

/* ===================================================== Public handle methods ==================================================== */

template<Direction dir>
template<Direction actual_dir, std::enable_if_t<actual_dir == Direction::Input, bool>>
auto Handler<dir>::handle(const DataStorage &data) const {
    return utilities::handle(data, handler);
}


template<Direction dir>
template<Direction actual_dir, std::enable_if_t<actual_dir == Direction::Output, bool>>
auto Handler<dir>::handle(DataStorage &data) const {
    return utilities::handle(data, handler);
}


template<Direction dir>
template<Direction actual_dir, std::enable_if_t<actual_dir == Direction::Input, bool>>
auto Handler<dir>::visit(const DataStorage &data) const {
    return utilities::visit(data, handler);
}


template<Direction dir>
template<Direction actual_dir, std::enable_if_t<actual_dir == Direction::Output, bool>>
auto Handler<dir>::visit(DataStorage &data) const {
    return utilities::visit(data, handler);
}

/* ============================================================ Helpers =========================================================== */

namespace details {

    /**
     * @brief Custom implementation of the std::variant  
     * @details According to [1] visting types inherited from std::variant is broken in GCC < 11.3.
     *    For such compiler versions the custom boost::mp-based implementation of the std::visit is 
     *    defined
     * 
     * @see [1] https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90943
     */
    template <class Visitor, class Variant>
    inline constexpr auto visit_derived_variant( Visitor&& vis, Variant &&var ) {

        // If newer GCC available (GCC >= 11.3), fallback to std::visit
        #if __GNUC__ > 11 || (__GNUC__ == 11 && __GNUC_MINOR__ >= 3 )
        return std::visit(std::forward<Visitor>(vis), std::forward<Variant>(var)...);
        // For older versions, do custom implementation
        #else
        return boost::mp11::mp_with_index<std::variant_size_v<traits::remove_cvref_t<Variant>>>( var.index(), [&](auto I){
            return std::invoke(std::forward<Visitor>(vis), std::get<I>(std::forward<Variant>(var)));
        });
        #endif
    }

}

/* ==================================================== Public getters methods ==================================================== */

template<Direction dir>
abstract::objects::Type Handler<dir>::get_type() {
    return objects::index_type(handler.index());
}


template<Direction dir>
template<abstract::objects::Type type>
void Handler<dir>::set(const Type<type> &handler) {
    get<type>(this->handler) = handler;
}


template<Direction dir>
template<abstract::objects::Type type>
typename Handler<dir>::template Type<type> &Handler<dir>::get() {
    return get<type>(handler);
}


template<Direction dir>
template<abstract::objects::Type type>
const typename Handler<dir>::template Type<type> &Handler<dir>::get() const {
    return get<type>(handler);
}


template<Direction dir>
void Handler<dir>::set(const Storage &handler) {
    this->handler = handler;
}


template<Direction dir>
template<typename HandlerT>
void Handler<dir>::set(HandlerT handler) {
    details::visit_derived_variant( [&handler](auto &ref){ ref = handler; }, this->handler );
}


template<Direction dir>
typename Handler<dir>::Storage &Handler<dir>::get() {
    return handler;
}


template<Direction dir>
const typename Handler<dir>::Storage &Handler<dir>::get() const {
    return handler;
}

/* ===================================================== Public helper methods ==================================================== */

template<Direction dir>
bool Handler<dir>::is_visitable(const DataStorage &data) const {
    return utilities::is_visitable(data, handler);
}


template<Direction dir>
template<abstract::objects::Type type>
bool Handler<dir>::handles() const {
    return ( handler.index() == HandlerIndex<type> );
}


template<Direction dir>
bool Handler<dir>::handles(abstract::objects::Type type) const {
    return ( handler.index() == handler_index(type) );
}


template<Direction dir>
template<abstract::objects::Type type>
bool Handler<dir>::is_empty() const {
    return bool(this->get<type>());
}


template<Direction dir>
bool Handler<dir>::is_empty(abstract::objects::Type type) const {
    return boost::mp11::mp_with_index<std::variant_size_v<Storage>>( handler_index(type), [this](auto I){
        return bool(std::get<I>(handler));
    });
}


template<Direction dir>
bool Handler<dir>::is_empty() const {
    return details::visit_derived_variant([](const auto &h) { return bool(h); }, handler);
}

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

#endif
