/* ============================================================================================================================ *//**
 * @file       reference.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the auxiliary Reference nested class of the Entry class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_ENTRY_REFERENCE_H__
#define __ETHERCAT_SLAVE_PDO_ENTRY_REFERENCE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave/translators_traits.hpp"
#include "ethercat/slave/pdo/entry.hpp"
#include "ethercat/common/translation/translator_wrapper.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================= Class ============================================================ */

/**
 * @brief Auxiliary proxy type providing type-safe access to the PDO entry object
 * 
 * @tparam ImplementationT 
 *    type implementing hardware-specific part of the Slave driver
 * @tparam dir 
 *    communication direction for PDO interface 
 * @tparam TranslatorT 
 *    translator type used to translate object between binary image and app-domain entity
 * @tparam T 
 *    type used to represent object in the app-domain ( automatically deduced from @p TranslatorT 
 *    if @c void )
 */
template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
template<typename TranslatorT, typename T = void>
class Slave<ImplementationT>::template Pdo<dir>::Entry::Reference :
    protected common::translation::TranslatorWrapper<slave::traits::to_translation_dir<dir>, TranslatorT, T>
{
    // Alis for wrapper base type
    using WrapperType = common::translation::TranslatorWrapper<slave::traits::to_translation_dir<dir>, TranslatorT, T>;

    // Alis for wrapper traits
    using WrapperTraits = typename WrapperType::Traits;
    // Translation-compatible direction
    static constexpr auto Direction = slave::traits::to_translation_dir<dir>;

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Translator type the template has been specialized with
    using TranslatorType = TranslatorT;
    // Type of the app-domain entity
    using Type = typename WrapperTraits::Type;
    // Type of the app-domain entity (efficient to pass to function)
    using ArgType = typename WrapperTraits::ArgType;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */
    
    /// Disable copy-construction
    inline Reference(const Reference &rreference) = delete;
    /// Disable copy-asignment
    inline Reference &operator=(const Reference &rreference) = delete;

    /// Enable move-construction (to enable storing slave in relocatable containers)
    inline Reference(Reference &&rreference) = default;
    /// Enable move-asignment (to enable storing slave in relocatable containers)
    inline Reference &operator=(Reference &&rreference) = default;

public: /* ------------------------------------------------ Public getters/setters ------------------------------------------------ */

    /**
     * @brief Translated current binary data of the entry to the object of the reference @p Type
     */
    template<bool enable = 
            common::translation::is_input_dir_v<Direction> and 
            std::is_default_constructible_v<Type>
        , std::enable_if_t<enable, bool> = true>
    inline Type get() const;

    /**
     * @brief Translated current binary data of the entry to the object of the reference @p Type
     */
    template<bool enable = 
            common::translation::is_input_dir_v<Direction>
        , std::enable_if_t<enable, bool> = true>
    inline void get(Type &object) const;

    /**
     * @brief Translated object of the reference @p Type into new content of the entirie's binary-image
     */
    template<bool enable = 
            common::translation::is_output_dir_v<Direction>
        , std::enable_if_t<enable, bool> = true>
    inline void set(ArgType object);

protected: /* ------------------------------------------------ Protected ctors ---------------------------------------------------- */
    
    /// Make Entry a friend to let it access update method
    friend class Entry;

    /**
     * @brief Construct a new Reference to the given binary-image buffer
     * 
     * @param type 
     *    type of the entry
     * @param buffer 
     *    buffer to be referenced
     * 
     * @todo [Important] Reference type shall verify whether given @p type can be reasonably 
     *   represented by the @ref Type . As @p type is an object while @ref Type is a type it would 
     *   require some type of type-tagging (.e.g by creating ethercat::common::utilities::traits::TypeTag
     *   structure that would describe base type of the type and arity). Such a tag could be created
     *   for both @ref Type and represenation type of the @p type and could be compared. In case 
     *   of complete mismatch, an error should be thrown.
     */
    template<bool enable = WrapperTraits::template is_stateless_in_dir<Direction>,
        std::enable_if_t<enable, bool> = true>
    inline Reference(const types::Type &type, Buffer &buffer);
    
    /**
     * @brief Construct a new Reference to the given binary-image buffer
     * 
     * @param type 
     *    type of the entry
     * @param buffer 
     *    buffer to be referenced
     * @param args 
     *    additional arguments to be passed to the constructor of @p TranslatorT
     * 
     * @todo [Important] Reference type shall verify whether given @p type can be reasonably 
     *   represented by the @ref Type . As @p type is an object while @ref Type is a type it would 
     *   require some type of type-tagging (.e.g by creating ethercat::common::utilities::traits::TypeTag
     *   structure that would describe base type of the type and arity). Such a tag could be created
     *   for both @ref Type and represenation type of the @p type and could be compared. In case 
     *   of complete mismatch, an error should be thrown.
     */
    template<bool enable = WrapperTraits::template is_statefull_in_dir<Direction>, typename... ArgsT,
        std::enable_if_t<enable, bool> = true>
    inline Reference(const types::Type &type, Buffer &buffer, ArgsT&&... args);

private: /* --------------------------------------------------- Private data ------------------------------------------------------ */
    
    // Pointer to the binary-data buffer of the Entry
    Buffer *buffer;

};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/slave/pdo/entry/reference/reference.hpp"

/* ================================================================================================================================ */

#endif
