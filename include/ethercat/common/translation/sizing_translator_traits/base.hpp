/* ============================================================================================================================ *//**
 * @file       base.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Sunday, 12th June 2022 4:58:38 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the SizingTraits traits
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_BASE_H__
#define __ETHERCAT_COMMON_TRANSLATION_SIZING_TRANSLATOR_TRAITS_BASE_H__

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* ====================================================== NoSizingTraitsBase ====================================================== */

/**
 * @brief Indirect base class of the *SizingTraits hierarchy describing type that is not a translator
 */
struct NoSizingTraitsBase
{ 
    static constexpr bool is_statefull   = false;
    static constexpr bool is_stateless   = false;
    static constexpr bool is_specialized = false;
    static constexpr bool is_generic     = false;
};

/* ===================================================== Statefull SizingTraits =================================================== */

/**
 * @brief Indirect base class of the *SizingTraits hierarchy describing statefull translator type
 */
struct StatefullSizingTraitsBase
{ 
    static constexpr bool is_statefull = true;
    static constexpr bool is_stateless = not is_statefull;
};

/// Specialization of @ref StatefullSizingTraitsBase for specialized translators
struct StatefullSpecializedSizingTraitsBase :
    public StatefullSizingTraitsBase
{ 
    static constexpr bool is_specialized = true;
    static constexpr bool is_generic     = not is_specialized;
};

/// Specialization of @ref StatefullSizingTraitsBase for generic translators
struct StatefullGenericSizingTraitsBase :
    public StatefullSizingTraitsBase
{ 
    static constexpr bool is_specialized = false;
    static constexpr bool is_generic     = not is_specialized;
};

/* ===================================================== Stateless SizingTraits =================================================== */

/**
 * @brief Indirect base class of the *SizingTraits hierarchy describing statefull translator type
 */
struct StatelessSizingTraitsBase
{ 
    static constexpr bool is_statefull = false;
    static constexpr bool is_stateless = not is_statefull;
};

/// Specialization of @ref StatelessSizingTraitsBase for specialized translators
struct StatelessSpecializedSizingTraitsBase :
    public StatelessSizingTraitsBase
{ 
    static constexpr bool is_specialized = true;
    static constexpr bool is_generic     = not is_specialized;
};

/// Specialization of @ref StatelessSizingTraitsBase for generic translators
struct StatelessGenericSizingTraitsBase :
    public StatelessSizingTraitsBase
{ 
    static constexpr bool is_specialized = false;
    static constexpr bool is_generic     = not is_specialized;
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
