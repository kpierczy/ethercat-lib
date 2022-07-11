/* ============================================================================================================================ *//**
 * @file       variant.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 5:37:44 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of common utilities for the cifx-ethercat library related to std::variant
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_VARIANT_VARIANT_H__
#define __ETHERCAT_COMMON_UTILITIES_VARIANT_VARIANT_H__

/* =========================================================== Includes =========================================================== */

// Boost includes
#include <boost/mp11.hpp>
// Private includes
#include "ethercat/common/utilities/variant.hpp"
#include "ethercat/common/traits.hpp"

/* ====================================================== Standard namespace ====================================================== */

namespace std {

    /// Specialization of @ref variant_size for visitor variant wrapper
    template<template<typename> typename HandlerTemplate, ::ethercat::utilities::details::VisitorType type, typename... _Args>
    struct variant_size<::ethercat::utilities::details::VisitorWrapper<HandlerTemplate, variant<_Args...>, type>> : 
        public variant_size<::ethercat::utilities::VariantDispatch<HandlerTemplate, variant<_Args...>>> {};

    /// Specialization of @ref variant_alternative for visitor variant wrapper
    template<template<typename> typename HandlerTemplate, ::ethercat::utilities::details::VisitorType type, size_t _Np, typename... _Args>
    struct variant_alternative<_Np, ::ethercat::utilities::details::VisitorWrapper<HandlerTemplate, variant<_Args...>, type>> : 
        public variant_alternative<_Np, ::ethercat::utilities::VariantDispatch<HandlerTemplate, variant<_Args...>>> {};

}

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::utilities {

/* ======================================================= Dynamic dispatch ======================================================= */

template<class Var, class... Args>
constexpr Var make_variant( std::size_t index, Args&&... args ) {
    return boost::mp11::mp_with_index<std::variant_size_v<Var>>( index, [&](auto I){
        
        /**
         * @note @p I is specialization of the @ref std::intergal_constant and so can bed 
         *    used in the compile time context
         */

        return Var( std::in_place_index<I>, std::forward<Args>( args )... );
    });
}

/* ================================================== Variant handlers (visitors) ================================================= */

namespace details {

    template<bool flag = false>
    inline constexpr void visitor_type_no_match() {
        static_assert(flag, "[ethercat::utilities::details::visit_impl] Invalid visitor type tag"); 
    }

    template<std::size_t I, typename R, typename Var, typename Vis>
    inline constexpr R handle_impl(Var var, Vis visitor) {
        
        // Get type of the visitor
        using visitor_t = traits::remove_cvref_t<Vis>;

        // Visit the variant (Function case)
        if constexpr (visitor_t::tag == details::VisitorType::Function) {
            return std::get<I>(visitor)(std::get<I>(var));
        // Visit the variant (Consumer case)
        } else if constexpr (visitor_t::tag == details::VisitorType::Consumer) {
            std::get<I>(visitor)(std::get<I>(var));
        // Visit the variant (Producer case)
        } else if constexpr (visitor_t::tag == details::VisitorType::Producer) {
            std::get<I>(var) = std::get<I>(visitor)();
        // Visit the variant (Processor case)
        } else if constexpr (visitor_t::tag == details::VisitorType::Processor) {
            auto &data = std::get<I>(var);
            data = std::get<I>(visitor)(data);
        // Invalid tag
        } else
            visitor_type_no_match();
    }

    template<typename R, typename Var, typename Vis>
    inline constexpr R visit_impl(Var var, Vis visitor) {
        
        // Dispatch the visitation with the jumptable
        return boost::mp11::mp_with_index<std::variant_size_v<traits::remove_cvref_t<Var>>>( var.index(), [&](auto I){
            return handle_impl<I, R, Var, Vis>(var, visitor);
        });

    }

    /* ---------------------------------------------------------------------------------------------------------------------------- */

    #define visit_variant_def(...)                                                           \
        template<std::size_t I, typename R, typename... Alts>                                \
        inline constexpr R visit(std::variant<Alts...> &var, const __VA_ARGS__&fun) {       \
            return handle_impl<I, R, decltype(var), decltype(fun)>(var, fun);                \
        }                                                                                    \
        template<std::size_t I, typename R, typename... Alts>                                \
        inline constexpr R visit(const std::variant<Alts...> &var, const __VA_ARGS__&fun) { \
            return handle_impl<I, R, decltype(var), decltype(fun)>(var, fun);                \
        }                                                                                    \
        template<typename R, typename... Alts>                                               \
        inline constexpr R visit(std::variant<Alts...> &var, const __VA_ARGS__&fun) {       \
            return visit_impl<R, decltype(var), decltype(fun)>(var, fun);                    \
        }                                                                                    \
        template<typename R, typename... Alts>                                               \
        inline constexpr R visit(const std::variant<Alts...> &var, const __VA_ARGS__&fun) { \
            return visit_impl<R, decltype(var), decltype(fun)>(var, fun);                    \
        }
        
    /* -------------------------------------------- Function-typed visitation functions ------------------------------------------- */

    visit_variant_def( typename VariantFunction<R, std::variant<Alts...>>::ArgValue    )
    visit_variant_def( typename VariantFunction<R, std::variant<Alts...>>::ArgRef      )
    visit_variant_def( typename VariantFunction<R, std::variant<Alts...>>::ArgConstRef )

    /* -------------------------------------------- Consumer-typed visitation functions ------------------------------------------- */

    visit_variant_def( typename VariantConsumer<std::variant<Alts...>>::ArgValue    )
    visit_variant_def( typename VariantConsumer<std::variant<Alts...>>::ArgRef      )
    visit_variant_def( typename VariantConsumer<std::variant<Alts...>>::ArgConstRef )

    /* -------------------------------------------- Producer-typed visitation functions ------------------------------------------- */

    visit_variant_def( typename VariantProducer<std::variant<Alts...>>::Type )
    
    /* ------------------------------------------- Processor-typed visitation functions ------------------------------------------- */

    visit_variant_def( typename VariantProcessor<std::variant<Alts...>>::ArgValue    )
    visit_variant_def( typename VariantProcessor<std::variant<Alts...>>::ArgRef      )
    visit_variant_def( typename VariantProcessor<std::variant<Alts...>>::ArgConstRef )

    /* ---------------------------------------------------------------------------------------------------------------------------- */

    #undef visit_variant_def

}

template<std::size_t I, typename Vis, typename... Alts>
inline constexpr auto handle(std::variant<Alts...> &var, Vis visitor) {
    return details::visit<
        I, traits::result_t<
            traits::remove_cvref_t<
                std::variant_alternative_t<0, traits::remove_cvref_t<Vis>>
            >
        >, Alts...>(var, visitor);
}

template<std::size_t I, typename Vis, typename... Alts>
inline constexpr auto handle(const std::variant<Alts...> &var, Vis visitor) {
    return details::visit<
        I, traits::result_t<
            traits::remove_cvref_t<
                std::variant_alternative_t<0, traits::remove_cvref_t<Vis>>
            >
        >, Alts...>(var, visitor);
}

template<typename Vis, typename... Alts>
constexpr auto visit(std::variant<Alts...> &var, Vis visitor) {
    return details::visit<
        traits::result_t<
            traits::remove_cvref_t<
                std::variant_alternative_t<0, traits::remove_cvref_t<Vis>>
            >
        >, Alts...>(var, visitor);
}

template<typename Vis, typename... Alts>
constexpr auto visit(const std::variant<Alts...> &var, Vis visitor) {
    return details::visit<
        traits::result_t<
            traits::remove_cvref_t<
                std::variant_alternative_t<0, traits::remove_cvref_t<Vis>>
            >
        >, Alts...>(var, visitor);
}

namespace details {

    template<typename Var, typename Vis>
    inline constexpr bool is_visitable_impl(Var var, Vis visitor) {

        // Get alternatives held by both variants
        auto var_alternative = var.index();
        auto visitor_alternative = visitor.index();

        // Check if both variants match held indeces
        return 
            var_alternative     != std::variant_npos and
            visitor_alternative != std::variant_npos and
            var_alternative     == visitor_alternative;
    }

    /* ---------------------------------------------------------------------------------------------------------------------------- */

    #define is_visitable_variant_def(...)                                                             \
        template<typename R, typename... Alts>                                                        \
        inline constexpr bool is_visitable(const std::variant<Alts...> &var, const __VA_ARGS__&fun) { \
            return is_visitable_impl<decltype(var), decltype(fun)>(var, fun);                         \
        }
        
    /* -------------------------------------------- Function-typed visitation functions ------------------------------------------- */

    is_visitable_variant_def( typename VariantFunction<R, std::variant<Alts...>>::ArgValue    )
    is_visitable_variant_def( typename VariantFunction<R, std::variant<Alts...>>::ArgRef      )
    is_visitable_variant_def( typename VariantFunction<R, std::variant<Alts...>>::ArgConstRef )

    /* -------------------------------------------- Consumer-typed visitation functions ------------------------------------------- */

    is_visitable_variant_def( typename VariantConsumer<std::variant<Alts...>>::ArgValue    )
    is_visitable_variant_def( typename VariantConsumer<std::variant<Alts...>>::ArgRef      )
    is_visitable_variant_def( typename VariantConsumer<std::variant<Alts...>>::ArgConstRef )

    /* -------------------------------------------- Producer-typed visitation functions ------------------------------------------- */

    is_visitable_variant_def( typename VariantProducer<std::variant<Alts...>>::Type )
    
    /* ------------------------------------------- Processor-typed visitation functions ------------------------------------------- */

    is_visitable_variant_def( typename VariantProcessor<std::variant<Alts...>>::ArgValue    )
    is_visitable_variant_def( typename VariantProcessor<std::variant<Alts...>>::ArgRef      )
    is_visitable_variant_def( typename VariantProcessor<std::variant<Alts...>>::ArgConstRef )

    /* ---------------------------------------------------------------------------------------------------------------------------- */

    #undef is_visitable_variant_def

}

template<typename Vis, typename... Alts>
constexpr bool is_visitable(const std::variant<Alts...> &var, Vis visitor) {
    return details::is_visitable<
        traits::result_t<
            traits::remove_cvref_t<
                std::variant_alternative_t<0, traits::remove_cvref_t<Vis>>
            >
        >, Alts...>(var, visitor);
}

/* ================================================================================================================================ */

} // End namespace ethercat::utilities

#endif
