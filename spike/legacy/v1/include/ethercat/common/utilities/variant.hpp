/* ============================================================================================================================ *//**
 * @file       variant.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 11th May 2022 1:09:32 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of common utilities for the cifx-ethercat library related to std::variant
 * @details   
 *   
 *   Next to the convinient tools for creating std::variant at runtime, the file provides a small utility 
 *   managin std::visit-based storage for per-alterntive handler functors. Aim of such an utility is to provide 
 *   handler corresponding to each alternative possible to store by some external data-holding std::variant 
 *   (<b>data variant</b>). Such a <b>variant handler</b>, should be automatically derivable from the mentioned 
 *   data variant. Also auxiliary utilities for checking both variant's coherency (i.e. coherency of currently 
 *   stored alternatives) as well as calling handler variant with the given data variant. Structure of these twin 
 *   types can be described in the following way:
 *   
 *       Data ( std::variant<T...> )         Handler ( std::variant<T_dependent_functor...> )
 *     
 *                [  type1 ]                             [  handler_for_type1 ]
 *                [  type2 ]                             [  handler_for_type2 ]
 *                [ *type3 ]         ------>             [ *handler_for_type3 ]
 *                [   ...  ]                             [         ...        ]
 *                [  typeN ]                             [  handler_for_typeN ]
 *
 *   Such an utility may be used provide efficient (due to local memory usage) manner to implement dynamically
 *   polimorphic handler for std::variant-base runtime polimorphic types.
 *   
 *   In particular, submodule introduces classes of handlers related to specific kind of data processing that the 
 *   handler type provides (e.g. consuming, producing, processing data). On top of the provided data types convinient 
 *   utilities imitating std::visit behaviour are provided. Specific type of the handler determines uctual result
 *   of visiting.
 *   
 *   The whole utility is implemented in a modern-cpp style and should very likely be inlined into stdandar library's
 *   calls in most use cases.
 *   
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_UTILITIES_VARIANT_H__
#define __ETHERCAT_UTILITIES_VARIANT_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include <functional>
#include <variant>
#include <type_traits>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::utilities {

/* ======================================================== Visitor helpers ======================================================= */

/// @brief Helper type for the in-place (lambda-based) std::visitor
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
/// @brief Explicit deduction guide for @ref overloaded class template (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

/* ======================================================= Dynamic dispatch ======================================================= */

/**
 * @brief Helper function producing std::variant initialized with an alternative associated with index
 *    given at runtime
 * 
 * @tparam Var
 *    type of the variant
 * @tparam Args 
 *    types of arguments to be passed to the std::variant
 * @param index 
 *    index of the target alternative
 * @param args 
 *    arguments to be passed to the std::variant
 * @returns 
 *    constructed variant
 * 
 * @note @p index is limited by the underlying @ref boost::mp11::mp_with_index implementation
 */
template<class Var, class... Args>
inline constexpr Var make_variant( std::size_t index, Args&&... args );

/* ==================================================== Variant transformations =================================================== */

/**
 * @brief Helper type alias template providing std::variant specialization holding specializations
 *    of the template @p T for all types listed in @p Args...
 * 
 * @tparam T
 *    template to be specialized
 * @tparam Args 
 *    types for specializaing the @p T template
 */
template<template<typename> typename T, typename... Args>
using SpecializationVariant = std::variant<T<Args>...>;

namespace details {

    /**
     * @brief Helper meta-function guiding deduction of Args... from the std::variant declval for 
     *    the @ref SpecializationVariant alias
     */
    template<template<typename> typename T, typename... Args>
    SpecializationVariant<T, Args...> specialization_variant_helper(std::variant<Args...> v);

    /**
     * @brief Helper alias template dispaching @ref specialization_variant_helper()
     */
    template<template<typename> typename T, typename Var>
    using SpecializationVariantHelper = decltype(specialization_variant_helper<T>(std::declval<Var>()));

}

/**
 * @brief Helper type alias template providing a std::variant specialization defined by
 *    dispatching subsequent types held by the given @p Var std::variant as a template
 *    parameters for specializations of the given @p T template
 * 
 * @tparam T
 *    template to dispatch @p Var into
 * @tparam Var
 *    variant to be dispatched
 */
template<template<typename> typename T, typename Var>
using VariantDispatch = details::SpecializationVariantHelper<T, Var>;

/* ================================================== Variant handlers (visitors) ================================================= */

namespace details {

    /**
     * @brief Helper guide template providing partial specialization of @p F template for the 
     *     functional variant visitors. Such a visitor processes corresponding variant's 
     *     alternatiive and produces some arbitrary value of type @p R
     * 
     * @tparam F 
     *    functor template to be used
     * @tparam R
     *   return type of the functor's specialization
     */
    template<template<typename> typename F, typename R>
    struct FunctionGuide {

        template<typename Arg> using ArgValue    = F<R(Arg)>;
        template<typename Arg> using ArgRef      = F<R(Arg&)>;
        template<typename Arg> using ArgConstRef = F<R(const Arg&)>;

    };
    
    /**
     * @brief Helper guide template providing partial specialization of @p F template for the 
     *     consumer-type variant visitors. Such a visitor consumes corresponding variant's 
     *     alternative.
     * 
     * @tparam F 
     *    functor template to be used
     */
    template<template<typename> typename F>
    struct ConsumerGuide {

        template<typename Arg> using ArgValue    = F<void(Arg)>;
        template<typename Arg> using ArgRef      = F<void(Arg&)>;
        template<typename Arg> using ArgConstRef = F<void(const Arg&)>;

    };

    /**
     * @brief Helper guide template providing partial specialization of @p F template for the 
     *     producer-type variant visitors. Such a visitor produces a new value for the corresponding 
     *     variant's alternative.
     * 
     * @tparam F 
     *    functor template to be used
     */
    template<template<typename> typename F>
    struct ProducerGuide {

        template<typename Arg> using Function = std::function<Arg(void)>;

    };

    /**
     * @brief Helper guide template providing partial specialization of @p F template for the 
     *     processor-type variant visitors. Such a visitor processes corresponding variant's 
     *     alternative and produces a new one.
     * 
     * @tparam F 
     *    functor template to be used
     */
    template<template<typename> typename F>
    struct ProcessorGuide {

        template<typename Arg> using ArgValue    = F<Arg(Arg)>;
        template<typename Arg> using ArgRef      = F<Arg(Arg&)>;
        template<typename Arg> using ArgConstRef = F<Arg(const Arg&)>;

    };

    /**
     * @brief Functor template used to store variant's handlers
     * @note At the moment, the functor is fixed and set to @ref std::function . 
     *    This is done for simplicity in the design process. However, in the future,
     *    the implementation may provide a custom selection of the functor type
     *    (e.g. for perfoemance improvement)
     */
    template<typename T>
    using Functor = std::function<T>;

    /**
     * @brief Helper enumeration providing compile-time tagging of visitor types
     */
    enum class VisitorType {
        Function,
        Consumer,
        Producer,
        Processor
    };

    /**
     * @brief Helper tagging wrapper providing type of the wrapped visitor
     */
    template<template<typename> typename HandlerTemplate, typename Var, VisitorType viritor_tag>
    struct VisitorWrapper : public VariantDispatch<HandlerTemplate, Var> {

        /// Wrapped type
        using Data = Var;
        /// Wrapped type
        using Visitor = VariantDispatch<HandlerTemplate, Var>;
        /// Type of the visitor
        static constexpr VisitorType tag { viritor_tag };

        /// Forward constructors
        using VariantDispatch<HandlerTemplate, Var>::VariantDispatch;

    };
    
}

/**
 * @brief Templated type providing function-type variant-matching visitor types
 * 
 * @tparam R
 *    return type of visitor's alternative
 * @tparam Var
 *    variant associated with the visitor type
 */
template<typename R, typename Var>
struct VariantFunction {
    
    using ArgValue    = details::VisitorWrapper<details::FunctionGuide<details::Functor, R>::template ArgValue,    Var, details::VisitorType::Function>;
    using ArgRef      = details::VisitorWrapper<details::FunctionGuide<details::Functor, R>::template ArgRef,      Var, details::VisitorType::Function>;
    using ArgConstRef = details::VisitorWrapper<details::FunctionGuide<details::Functor, R>::template ArgConstRef, Var, details::VisitorType::Function>;

};

/**
 * @brief Templated type providing consumer-type variant-matching visitor types
 * 
 * @tparam Var
 *    variant associated with the visitor type
 */
template<typename Var>
struct VariantConsumer {
    
    using ArgValue    = details::VisitorWrapper<details::ConsumerGuide<details::Functor>::template ArgValue,    Var, details::VisitorType::Consumer>;
    using ArgRef      = details::VisitorWrapper<details::ConsumerGuide<details::Functor>::template ArgRef,      Var, details::VisitorType::Consumer>;
    using ArgConstRef = details::VisitorWrapper<details::ConsumerGuide<details::Functor>::template ArgConstRef, Var, details::VisitorType::Consumer>;

};

/**
 * @brief Consumer-type variant-matching visitor type
 * 
 * @tparam Var
 *    variant associated with the visitor type
 */
template<typename Var>
struct VariantProducer {
    
    using Type = details::VisitorWrapper<details::ProducerGuide<details::Functor>::template Function, Var, details::VisitorType::Producer>;
    
};

/**
 * @brief Templated type providing processor-type variant-matching visitor types
 * 
 * @tparam Var
 *    variant associated with the visitor type
 */
template<typename Var>
struct VariantProcessor {
    
    using ArgValue    = details::VisitorWrapper<details::ProcessorGuide<details::Functor>::template ArgValue,    Var, details::VisitorType::Processor>;
    using ArgRef      = details::VisitorWrapper<details::ProcessorGuide<details::Functor>::template ArgRef,      Var, details::VisitorType::Processor>;
    using ArgConstRef = details::VisitorWrapper<details::ProcessorGuide<details::Functor>::template ArgConstRef, Var, details::VisitorType::Processor>;

};

/**
 * @brief Helper std::visit-like function visiting @p var data variant and calling corresponding
 *    alternative of the @p visitor handler variant (alternative with corresponding index). Visitation 
 *    result depends on the @p visitor type:
 * 
 *         - for @ref VariantFunction-like types corresponding functor is called with the
 *           visited alternative passed as argument and the resulting value is returned
 *         - for @ref VariantConsumer-like types corresponding functor is called with the
 *           visited alternative passed as argument
 *         - for @ref VariantProducer types corresponding functor is called with no argument 
 *           and the visited alternative is set to the invocation result
 *         - for @ref VariantProcessor-like types corresponding functor is called with the
 *           visited alternative passed as argument and the result is stored back in the visited
 *           alternative
 * 
 *    In contrast to the @ref visit() , this function does not produce runtime-dispatchable jumptable
 *    but rather accesses both variant directly using the given idnex. 
 *    
 * @tparam I 
 *    index of the alternatives to be used
 * @tparam Vis 
 *    type of the visiting functor; this must be speciaization of one of types listed in the @brief 
 *    section; generic F type is used to provide automatic deduction of handlers' return type
 * @tparam Alts... 
 *    variants' alternative types
 * @param var 
 *    variant to be visited
 * @param visitor 
 *    functor visitor to be used
 * @returns 
 *    return value of the visitation result
 * 
 * @note If alternatives stored by @p var and @p visitor are of a different index, the invalid 
 *    access to std::varint is performed and a standard @ref std::bad_variant_access exception 
 *    is thrown. To check whether @p var can be visited with @p visitor use @ref is_visitable().
 * @note Size of @p Alts is limited by the underlying @ref boost::mp11::mp_with_index 
 *    implementation
 */
template<std::size_t I, typename Vis, typename... Alts>
inline constexpr auto handle(std::variant<Alts...> &var, Vis visitor);

/**
 * @overload constexpr auto handle(std::variant<Alts...> &var, Vis visitor)
 * @note This version of the function results in compilation error if variant-moifying visitor
 *    is given.
 */
template<std::size_t I, typename Vis, typename... Alts>
inline constexpr auto handle(const std::variant<Alts...> &var, Vis visitor);

/**
 * @brief Helper std::visit-like function visiting @p var data variant and calling corresponding
 *    alternative of the @p visitor handler variant (alternative with corresponding index). Visitation 
 *    result depends on the @p visitor type:
 * 
 *         - for @ref VariantFunction-like types corresponding functor is called with the
 *           visited alternative passed as argument and the resulting value is returned
 *         - for @ref VariantConsumer-like types corresponding functor is called with the
 *           visited alternative passed as argument
 *         - for @ref VariantProducer types corresponding functor is called with no argument 
 *           and the visited alternative is set to the invocation result
 *         - for @ref VariantProcessor-like types corresponding functor is called with the
 *           visited alternative passed as argument and the result is stored back in the visited
 *           alternative
 *    
 * @tparam Vis 
 *    type of the visiting functor; this must be speciaization of one of types listed in the @brief 
 *    section; generic F type is used to provide automatic deduction of handlers' return type
 * @tparam Alts... 
 *    variants' alternative types
 * @param var 
 *    variant to be visited
 * @param visitor 
 *    functor visitor to be used
 * @returns 
 *    return value of the visitation result
 * 
 * @note If alternatives stored by @p var and @p visitor are of a different index, the invalid 
 *    access to std::varint is performed and a standard @ref std::bad_variant_access exception 
 *    is thrown. To check whether @p var can be visited with @p visitor use @ref is_visitable().
 * @note Size of @p Alts is limited by the underlying @ref boost::mp11::mp_with_index 
 *    implementation
 */
template<typename Vis, typename... Alts>
inline constexpr auto visit(std::variant<Alts...> &var, Vis visitor);

/**
 * @overload constexpr auto visit(std::variant<Alts...> &var, Vis visitor)
 * @note This version of the function results in compilation error if variant-moifying visitor
 *    is given.
 */
template<typename Vis, typename... Alts>
inline constexpr auto visit(const std::variant<Alts...> &var, Vis visitor);

/**
 * @brief Helper predicate function checking whether @p var can be visited with @p visitor using
 *     @ref visit(var, visitor) call
 *    
 * @tparam Alts... 
 *    variants' alternative types
 * @tparam Vis 
 *    type of the visiting functor; this must be speciaization of one of types listed in the @brief 
 *    section of the @ref visit() function; generic F type is used to provide automatic deduction
 *    of handlers' return type
 * @param var 
 *    variant to be visited
 * @param visitor 
 *    functor visitor to be used
 * @returns 
 *    @retval @c true if indeces of alternatives stored by @p var and @p visitor match and 
 *       so @p var can be visited with @p visitor
 *    @retval @c false otherwise
 * 
 * @see visit()
 */
template<typename Vis, typename... Alts>
inline constexpr bool is_visitable(const std::variant<Alts...> &var, Vis visitor);

/* ================================================================================================================================ */

} // End namespace ethercat::utilities

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/utilities/variant/variant.hpp"

/* ================================================================================================================================ */

#endif
