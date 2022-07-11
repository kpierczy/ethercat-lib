/* ============================================================================================================================ *//**
 * @file       sizing_translator_traits.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 4:29:06 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the SizingTranslationTraits traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/translator_traits.hpp"
#include "ethercat/common/translation/sizing_translator_traits/sizing_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ======================================================= TranslationTraits ====================================================== */

/**
 * @brief Traits structure providing compile-time informations about the @p TranslatorT 
 *   sizing-translator type
 * @details This traits structure extends @ref TranslatorTraits with boolean flags describing
 *   traits of the @p TranslatorT in context of provided <i>sizing</i> method [1]
 * 
 * @tparam TranslatorT 
 *    translator to be examined
 * @tparam T (optional, default: void)
 *    target type; if @c void traits try to deduce it automatically from thefinition of the
 *    @p TranslatorT type
 * 
 * @see [1] Namespace ethercat::common::translation
 */
template<typename TranslatorT, typename T = void>
struct SizingTranslatorTraits : 
    public TranslatorTraits<TranslatorT, T>
{
private: /* --------------------------------------------------- Private types ----------------------------------------------------- */

    /// Alias for Sizing traits implementation class
    using SizingTraits = details::SizingTraits<TranslatorT, typename TranslatorTraits<TranslatorT, T>::Type>;

public: /* ---------------------------------------------------- Public types ------------------------------------------------------ */

    /// Alias for Traits base class
    using BaseTraits = TranslatorTraits<TranslatorT, T>;

public: /* --------------------------------------------------- Public constants --------------------------------------------------- */

    /**
     * @brief Structure describing properties of the static-sizing capabilities of the translator
     */
    struct static_sizing {

        /// @c true if @p TranslatorT provides a stateless static-sizing @c make_buffer() method
        static constexpr bool is_stateless { SizingTraits::is_static_stateless };
        /// @c true if @p TranslatorT provides a statefull static-sizing @c make_buffer() method
        static constexpr bool is_statefull { SizingTraits::is_static_statefull };
        /// @c true if @p TranslatorT provides a static-sizing @c make_buffer() method
        static constexpr bool is_available { is_stateless or is_statefull };
        /// @c true if @p TranslatorT provides a specialized static-sizing @c make_buffer() method
        static constexpr bool is_specialized { SizingTraits::is_static_specialized };
        /// @c true if @p TranslatorT provides a generic static-sizing @c make_buffer() method
        static constexpr bool is_generic { SizingTraits::is_static_generic };

    };

    /**
     * @brief Structure describing properties of the dynamic-sizing capabilities of the translator
     */
    struct dynamic_sizing {

        /// @c true if @p TranslatorT provides a stateless dynamic-sizing @c make_buffer() method
        static constexpr bool is_stateless { SizingTraits::is_dynamic_stateless };
        /// @c true if @p TranslatorT provides a statefull dynamic-sizing @c make_buffer() method
        static constexpr bool is_statefull { SizingTraits::is_dynamic_statefull };
        /// @c true if @p TranslatorT provides a dynamic-sizing @c make_buffer() method
        static constexpr bool is_available { is_stateless or is_statefull };
        /// @c true if @p TranslatorT provides a specialized dynamic-sizing @c make_buffer() method
        static constexpr bool is_specialized { SizingTraits::is_dynamic_specialized };
        /// @c true if @p TranslatorT provides a generic dynamic-sizing @c make_buffer() method
        static constexpr bool is_generic { SizingTraits::is_dynamic_generic };

    };

    /**
     * @brief Structure describing properties of the sizing capabilities of the translator
     */
    struct sizing {

        /// @c true if @p TranslatorT provides only stateless sizing @c make_buffer() method(s) (if any)
        static constexpr bool is_stateless { static_sizing::is_stateless and dynamic_sizing::is_stateless };
        /// @c true if @p TranslatorT provides at least one statefull sizing @c make_buffer() method (if any)
        static constexpr bool is_statefull { static_sizing::is_statefull or dynamic_sizing::is_statefull };
        /// @c true if @p TranslatorT provides a sizing @c make_buffer() method
        static constexpr bool is_available { static_sizing::is_available or dynamic_sizing::is_available };

        /// @c true if @p TranslatorT is stateless for the target @a Type in the given direction
        template<TranslationDirection dir> static constexpr bool is_stateless_in_dir { 
            BaseTraits::template is_stateless_in_dir<dir>                         and 
                sizing::is_available                                              and
                (not static_sizing::is_available  or static_sizing::is_stateless) and 
                (not dynamic_sizing::is_available or dynamic_sizing::is_stateless)
        };

        /// @c true if @p TranslatorT is statefull for the target @a Type in the given direction
        template<TranslationDirection dir> static constexpr bool is_statefull_in_dir { 
            BaseTraits::template is_translator_in_dir<dir>    and 
            sizing::is_available                              and (
                BaseTraits::template is_statefull_in_dir<dir> or
                static_sizing::is_statefull                   or
                dynamic_sizing::is_statefull
            )
        };

        /// @c true if @p TranslatorT is sizing translator for the target @a Type in the given direction
        template<TranslationDirection dir> static constexpr bool is_translator_in_dir { 
            BaseTraits::template is_translator_in_dir<dir> and sizing::is_available
        };

    };
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

#endif
