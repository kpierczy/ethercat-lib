/* ============================================================================================================================ *//**
 * @file       handler.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 11th May 2022 4:40:01 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the auxiliary class providing dynamically dispatchable handler for incoming/outgoin PDO-mapped objects
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_IMAGE_HANDLER_H__
#define __ETHERCAT_ABSTRACT_PDO_IMAGE_HANDLER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/pdo.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::pdo {

/* ========================================================== Base types ========================================================== */

/**
 * @brief Helper type storing an arbitrary callable handling cyclical input object
 */
using InputHandlerStorage = typename utilities::VariantConsumer<abstract::objects::ValueStorage>::ArgValue;

/// Helper meta-function converting @ref DataType enumeration into corresponding data handler type
template<abstract::objects::Type type>
using InputHandlerType = std::variant_alternative_t<abstract::objects::TypeIndex<type>, InputHandlerStorage>;

/**
 * @brief Helper type storing an arbitrary callable handling cyclical output object
 */
using OutputHandlerStorage = typename utilities::VariantProducer<abstract::objects::ValueStorage>::Type;

/// Helper meta-function converting @ref DataType enumeration into corresponding data handler type
template<abstract::objects::Type type>
using OutputHandlerType = std::variant_alternative_t<abstract::objects::TypeIndex<type>, OutputHandlerStorage>;

namespace details {

    /* -------------- Handler-storage traits -------------- */

    template<Direction dir>
    struct HandlerStorageTraits { };

    template<>
    struct HandlerStorageTraits<Direction::Input> {
        using storage = InputHandlerStorage;
    };

    template<>
    struct HandlerStorageTraits<Direction::Output> {
        using storage = OutputHandlerStorage;
    };

    /* ------------------ Handler traits ------------------ */

    template<Direction dir, abstract::objects::Type type>
    struct HandlerTraits { };

    template<abstract::objects::Type type>
    struct HandlerTraits<Direction::Input, type> {

        /// Handler's storage type
        using storage = HandlerStorageTraits<Direction::Input>::storage;

        /// Handler's type
        using handler_type = InputHandlerType<type>;
        /// Handler's traits
        using function_traits = traits::function_traits<traits::function_traits<handler_type>>;
        /// Handler's arg type
        template<std::size_t I>
        using arg_t = traits::arg_t<handler_type, I>;
        /// Handler's result type type
        using result_t = traits::result_t<handler_type>;
    };

    template<abstract::objects::Type type>
    struct HandlerTraits<Direction::Output, type> {

        /// Handler's storage type
        using storage = HandlerStorageTraits<Direction::Output>::storage;

        /// Handler's type
        using handler_type = OutputHandlerType<type>;
        /// Handler's traits
        using function_traits = traits::function_traits<traits::function_traits<handler_type>>;
        /// Handler's arg type
        template<std::size_t I>
        using arg_t = traits::arg_t<handler_type, I>;
        /// Handler's result type type
        using result_t = traits::result_t<handler_type>;
    };

}

/// Helper meta-function converting @ref DataType enumeration into corresponding data handler storage type
template<Direction dir>
using HandlerStorage = typename details::HandlerStorageTraits<dir>::storage;

/// Handler's traits
template<Direction dir, abstract::objects::Type type>
struct HandlerTraits : public details::HandlerTraits<dir, type> { };

/// Helper meta-function converting @ref DataType enumeration into corresponding data handler type
template<Direction dir, abstract::objects::Type type>
using HandlerType = typename HandlerTraits<dir, type>::handler_type;

/**
 * @brief Helper meta-function converting @ref Type enumeration into index of the corresponding 
 *    @ref Value alternative
 */
template<abstract::objects::Type type>
constexpr std::size_t HandlerIndex = abstract::objects::TypeIndex<type>;

/**
 * @brief Helper meta-function converting @ref Type enumeration into corresponding data type
 */
template<abstract::objects::Type type>
constexpr auto InPlaceHandlerIndex = abstract::objects::TypeIndex<type>;

/* ======================================================= Getters & setters ====================================================== */

/// Converts @p type to alternative's idnex
inline constexpr std::size_t handler_index(abstract::objects::Type type);

/// Helper getter for object value storage
template<abstract::objects::Type type>
inline InputHandlerType<type> &get(InputHandlerStorage &handler_storage);
/// Helper getter for object value storage (const)
template<abstract::objects::Type type>
inline const InputHandlerType<type> &get(const InputHandlerStorage &handler_storage);

/// Helper getters for object value storage
template<abstract::objects::Type type>
inline OutputHandlerType<type> &get(OutputHandlerStorage &handler_storage);
/// Helper getters for object value storage (const)
template<abstract::objects::Type type>
inline const OutputHandlerType<type> &get(const OutputHandlerStorage &handler_storage);

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Helper wrapper around @ref HandlerStorage
 */
template<Direction dir>
class Handler {

public: /* --------------------------------------------------- Public constants --------------------------------------------------- */

    /// Direction of the PDOs handled by the type
    static constexpr Direction Dir { dir };

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Handler's traits
    template<abstract::objects::Type type> 
    using Traits = HandlerTraits<dir, type>;

    /// Storage type used by the type
    using Storage = HandlerStorage<dir>;
    /// Data object type corresponding to the handler
    using DataStorage = typename Storage::Data;

    /// Handler's type for the given object @p type
    template<abstract::objects::Type type> 
    using Type = typename Traits<type>::handler_type;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /**
     * @brief Constructs an empty handler associated with a cyclical object of type @p type 
     * 
     * @param type 
     *    type of the cyclical object associated with the handler
     */
    inline Handler(abstract::objects::Type type);

    /**
     * @brief Constructs a handler associated with a cyclical object of type @p type initialized
     *    with the given @p handler
     * 
     * @tparam HandlerType
     *    type of the handler to initialize object with; this must be coherent with the given @p type ,
     *    in particular it must be specialization of the Handler::Type alias template
     * @param type 
     *    type of the cyclical object associated with the handler
     * @param handler 
     *    handler routine to initialize object with
     * 
     * @note The generic handler type is used due to lack of alias templates-based deduction guide 
     *    in C++17.
     */
    template<typename HandlerType>
    inline Handler(abstract::objects::Type type, HandlerType&& handler);

    /// Default copying
    Handler(const Handler& rhandler) = default;
    Handler &operator=(const Handler& rhandler) = default;
    /// Enable moving to allow emplacing back Handlers in the std::vector
    Handler(Handler&& rhandler) = default;
    Handler &operator=(Handler&& rhandler) = default;

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /**
     * @brief Casts *this to the Storage
     */
    inline operator Storage() const;

    /**
     * @brief Casts *this to the Storage reference
     */
    inline operator Storage&();

    /**
     * @brief Casts *this to the const Storage reference
     */
    inline operator const Storage&() const;

    /**
     * @brief Assigns new @p handler to the handler object
     * 
     * @tparam H 
     *    type of the handle
     * @param handler 
     *    handler to be assigned
     * @returns 
     *    reference to *this
     */
    template<typename HandlerType>
    inline Handler &operator=(HandlerType handler);

public: /* ------------------------------------------------ Public handle methods ------------------------------------------------- */

    /**
     * @brief Calls handler for the given @p data assuming data @p type
     * 
     * @param data 
     *    data to be handled
     * @returns 
     *    handler's result value
     * 
     * @note Method is enabled for input cyclical objects' handler
     * @todo Switch to C++20 concepts when C++20 is available
     */
    template<Direction actual_dir = Dir, std::enable_if_t<actual_dir == Direction::Input, bool> = true>
    inline auto handle(const DataStorage &data) const;

    /**
     * @brief Calls handler for the given @p data assuming data @p type
     * 
     * @param data 
     *    data to be handled
     * @returns 
     *    handler's result value
     * 
     * @note Method is enabled for output cyclical objects' handler
     * @todo Switch to C++20 concepts when C++20 is available
     */
    template<Direction actual_dir = Dir, std::enable_if_t<actual_dir == Direction::Output, bool> = true>
    inline auto handle(DataStorage &data) const;

    /**
     * @brief Calls handler for the given @p data dynamically dispatching handler's alternative
     *    based on currently held @p data alternative.
     * 
     * @param data 
     *    data to be handled
     * @returns 
     *    handler's result value
     * 
     * @note Method is enabled for input cyclical objects' handler
     * @todo Switch to C++20 concepts when C++20 is available
     */
    template<Direction actual_dir = Dir, std::enable_if_t<actual_dir == Direction::Input, bool> = true>
    inline auto visit(const DataStorage &data) const;

    /**
     * @brief Calls handler for the given @p data dynamically dispatching handler's alternative
     *    based on currently held @p data alternative.
     * 
     * @param data 
     *    data to be handled
     * @returns 
     *    handler's result value
     * 
     * @note Method is enabled for output cyclical objects' handler
     * @todo Switch to C++20 concepts when C++20 is available
     */
    template<Direction actual_dir = Dir, std::enable_if_t<actual_dir == Direction::Output, bool> = true>
    inline auto visit(DataStorage &data) const;

public: /* ----------------------------------------------- Public getters methods ------------------------------------------------- */

    /// @returns data type of the currently set handler
    inline abstract::objects::Type get_type();

    /// Sets handler by @p type to @p handler
    template<abstract::objects::Type type>
    inline void set(const Type<type> &handler);

    /// Accesses handler by @p type 
    template<abstract::objects::Type type>
    inline Type<type> &get();

    /// Accesses handler by @p type 
    template<abstract::objects::Type type>
    inline const Type<type> &get() const;

    /// Sets handler as storage
    inline void set(const Storage &handler);

    /// Sets handler to @p handler dispatching handler type at runtime
    template<typename HandlerT>
    inline void set(HandlerT handler);

    /// Accesses handler storage
    inline Storage &get();

    /// Accesses handler storage
    inline const Storage &get() const;

public: /* ------------------------------------------------ Public helper methods ------------------------------------------------- */

    /**
     * @brief Checks whether handler can be used to visit @p data based on indeces of currently
     *    stored alternatives by both handler and @p data
     * 
     * @param data 
     *    data to be verified
     * @retval true
     *    if indeces of alternatives stored by @p var and @p visitor match and so @p var can be
     *    visited with @p visitor
     * @retval false
     *    otherwise
     */
    inline bool is_visitable(const DataStorage &data) const;

    /**
     * @brief Checks whether handler is desired to handle cyclical objects of type @p type 
     *   ( i.e. whether it stored handler alternative corresopnding to the type @p type )
     * 
     * @tparam type 
     *    requested type of cyclical data object
     * 
     * @retval true 
     *    if handler stored alternative corresponding to the given @p type
     * @retval false 
     *    otherwise
     */
    template<abstract::objects::Type type>
    inline bool handles() const;

    /**
     * @brief Checks whether handler is desired to handle cyclical objects of type @p type 
     *   ( i.e. whether it stored handler alternative corresopnding to the type @p type )
     * 
     * @param type 
     *    requested type of cyclical data object
     * 
     * @retval true
     *    if handler stored alternative corresponding to the given @p type
     * @retval false
     *    otherwise
     */
    inline bool handles(abstract::objects::Type type) const;

    /**
     * @brief Checks whether object stores handler for the given object @p type that is empty
     * 
     * @tparam type 
     *    requested cyclical object data type
     * 
     * @retval true
     *    if handler stores alternative corresponding to cyclical data of the given @p type that
     *    is empty
     * @retval false
     *    otherwise
     * 
     * @throws TypeError
     *    if handler does no correspond to data of type @p type
     */
    template<abstract::objects::Type type>
    inline bool is_empty() const;

    /**
     * @brief Checks whether object stores handler for the given object @p type that is empty
     * 
     * @param type 
     *    requested cyclical object data type
     * @retval true
     *    if handler stores alternative corresponding to cyclical data of the given @p type that
     *    is empty
     * @retval false
     *    otherwise
     * 
     * @throws TypeError
     *    if handler does no correspond to data of type @p type
     */
    inline bool is_empty(abstract::objects::Type type) const;

    /**
     * @brief Checks whether object stores empty handler
     * 
     * @retval true
     *    if handler stores alternative that is empty
     * @retval false
     *    otherwise
     */
    inline bool is_empty() const;

private: /* ---------------------------------------------------- Private data ----------------------------------------------------- */

    /// Handler storage
    Storage handler;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/pdo/handler/handler.hpp"

/* ================================================================================================================================ */

#endif
