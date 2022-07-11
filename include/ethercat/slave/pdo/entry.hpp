/* ============================================================================================================================ *//**
 * @file       entry.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Entry nested class of the Slave::Pdo interface
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_PDO_ENTRY_H__
#define __ETHERCAT_SLAVE_PDO_ENTRY_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave/pdo.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================== PDO ============================================================= */

/**
 * @brief A proxy class template providing an interface for managing Process Data Objects 
 *    (PDO) entry associated with the slave device that are mapped in the Process Data 
 *    Image (PDI)
 * 
 * @tparam ImplementationT 
 *    type implementing hardware-specific part of the Slave driver
 * @tparam dir 
 *    communication direction for PDO interface 
 */
template<typename ImplementationT>
template<typename Slave<ImplementationT>::PdoDirection dir>
class Slave<ImplementationT>::template Pdo<dir>::Entry {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Auxiliary proxy type providing type-safe access to the PDO entry object
     * 
     * @tparam TranslatorT 
     *    translator type used to translate object between binary image and app-domain entity
     * @tparam T 
     *    type used to represent object in the app-domain ( automatically deduced from @p TranslatorT 
     *    if @c void )
     */
    template<typename TranslatorT, typename T = void>
    class Reference;

    /**
     * @brief Auxiliary alias for Reference template specialized with the default translator for the given type
     */
    template<typename T>
    using DefaultTranslatedReference = 
        Reference<common::translation::DefaultTranslator<T>, T>;

    /**
     * @brief Auxiliary alias for Reference template specialized with the default translator for the typed deduced
     *    from the ID of EtherCAT builtin type
     */
    template<types::BuiltinType::ID type_id, std::size_t arity = 0>
    using BuiltinDefaultTranslatedReference = 
        DefaultTranslatedReference<common::types::traits::TypeRepresentation<type_id, arity>>;

private: /* -------------------------------------------------- Private types ------------------------------------------------------ */

    /// Auxiliary trait chekcing whether @p T is specialization of Sdo template
    template <typename T>
    struct is_reference : std::false_type { };
    
    /// Auxiliary trait chekcing whether @p T is specialization of Sdo template
    template <typename TranslatorT, typename T>
    struct is_reference<Reference<TranslatorT, T>> : std::true_type { };

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */
    
    /// Disable copy-construction
    inline Entry(const Entry &rentry) = delete;
    /// Disable copy-asignment
    inline Entry &operator=(const Entry &rentry) = delete;

    /// Enable move-construction (to enable storing slave in relocatable containers)
    inline Entry(Entry &&rentry) = default;
    /// Enable move-asignment (to enable storing slave in relocatable containers)
    inline Entry &operator=(Entry &&rentry) = default;

public: /* ---------------------------------------------------- Public getters ---------------------------------------------------- */

    /**
     * @returns 
     *    name of the entry
     */
    inline std::string_view get_name() const;

    /**
     * @returns 
     *    type of the entry
     */
    inline const types::Type &get_type() const;

public: /* ------------------------------------------ Public methods (entry-referencing) ------------------------------------------ */

    /**
     * @brief Constructs an proxy object associated with @p this entry that can be used to
     *    read/write data from/to it
     * @details This overload constructs Reference using a custom translator to convert data
     *    between binary image and app-domain entity of an <i>automatically deduced</i> type
     * 
     * @param args
     *    additional arguments ot be passed to the constructor of @p TranslatorT (enabled only 
     *    if statefull translator is used)
     * 
     * @returns 
     *    proxy object associated with @p this entry
     */
    template<typename TranslatorT, typename... ArgsT,
        slave::traits::enable_if_translator_t<dir, TranslatorT> = true>
    inline Reference<TranslatorT> get_reference(
        ArgsT&&... args
    );

    /// @overload Reference<TranslatorT> get_reference(ArgsT&&...)
    template<typename TranslatorT, typename... ArgsT,
        slave::traits::enable_if_translator_t<dir, TranslatorT> = true>
    inline const Reference<TranslatorT> get_reference(
        ArgsT&&... args
    ) const;

    /**
     * @overload Reference<TranslatorT> get_reference(ArgsT&&...)
     * @details This overload constructs Reference using a custom translator to convert data
     *    between binary image and app-domain entity of a <i>given</i> type
     */
    template<typename TranslatorT, typename T, typename... ArgsT,
        slave::traits::enable_if_translator_t<dir, TranslatorT, T> = true>
    inline Reference<TranslatorT, T> get_reference(
        ArgsT&&... args
    );

    /// @overload Reference<TranslatorT, T> get_reference(ArgsT&&...)
    template<typename TranslatorT, typename T, typename... ArgsT,
        slave::traits::enable_if_translator_t<dir, TranslatorT, T> = true>
    inline const Reference<TranslatorT, T> get_reference(
        ArgsT&&... args
    ) const;

    /**
     * @overload Reference<TranslatorT> get_reference(ArgsT&&...)
     * @details This overload constructs Reference using a custom translator to convert data
     *    between binary image and app-domain entity of an <i>given</i> type.
     * 
     * @note This overload is disabled if @p T is not supported by the default translator
     */
    template<typename T,
        slave::traits::enable_if_default_translatable_t<T> = true>
    inline DefaultTranslatedReference<T> get_reference();

    /// @overload Reference<dir, T> get_sdo()
    template<typename T,
        slave::traits::enable_if_default_translatable_t<T> = true>
    inline const DefaultTranslatedReference<T> get_reference() const;

    /**
     * @overload Reference<TranslatorT> get_reference(ArgsT&&...)
     * @details This overload constructs Reference using a <i>default stateless</i> translator to convert
     *     data between binary image and app-domain entity of a default type used to represent
     *     given <i>EtherCAT builtin type</i> type with the given @p arity
     * 
     * @tparam type_id 
     *    ID of the target EtherCAT builtin type
     * @tparam arity  
     *    arity of the target type ( @c 0 for scalars )
     * 
     * @note This overload is disabled if default representation of the given EtherCAT builtin type
     *    is not supported by the default translator (should not happen, but check it just in case)
     */
    template<types::BuiltinType::ID type_id, std::size_t arity = 0,
        slave::traits::enable_if_builtin_default_translatable_t<type_id, arity> = true>
    inline BuiltinDefaultTranslatedReference<type_id, arity> get_reference();

    /// @overload BuiltinDefaultTranslatedReference<dir, type_id, arity> get_reference();
    template<types::BuiltinType::ID type_id, std::size_t arity = 0,
        slave::traits::enable_if_builtin_default_translatable_t<type_id, arity> = true>
    inline const BuiltinDefaultTranslatedReference<type_id, arity> get_reference() const;

    /**
     * @overload BuiltinDefaultTranslatedReference<dir, type_id, arity> get_reference();
     * @details This is helper overload enabling deduction of translator and target types from type 
     *    of the given @p ReferenceType type
     * 
     * @tparam ReferenceType 
     *    type of the target Pdo interface
     */
    template<typename ReferenceType, bool enable = 
            is_reference<common::utilities::traits::remove_cvref_t<ReferenceType>>::value,
        std::enable_if_t<enable, bool> = true>
    inline auto get_reference();

    /// @overload auto get_reference()
    template<typename ReferenceType, bool enable = 
            is_reference<common::utilities::traits::remove_cvref_t<ReferenceType>>::value,
        std::enable_if_t<enable, bool> = true>
    inline const auto get_reference() const;

protected: /* ------------------------------------------------ Protected ctors ---------------------------------------------------- */

    /// Make Pdo a friend to let it access constructor
    friend class Pdo;

    /**
     * @brief Construct a new Entry object
     * 
     * @param entry_description 
     *    ENI description of the Entry
     * @param pdi_variable
     *    ENI description of PDI variable corresponding to the entry
     * 
     * @throws eni::Error 
     *    if inconsistency between @p entry_description and @p pdi_variable has been detected
     */
    Entry(
        eni::Slave::Pdo::Entry entry_description,
        eni::ProcessImage::Variable pdi_variable
    );
    
private: /* ------------------------------------------------- Private methods ----------------------------------------------------- */

    /// Make Master a friend to let it access update method
    template<typename MasterImplementationT,typename SlaveImplementationT>
    friend class Master;
    
    /**
     * @brief Updates internal buffer of the entry from the current Process Data Image @p pdi buffer
     * @details This method is called by the Master driver each time Input Process Data Image is
     *    updated with data from the bus
     * 
     * @param pdi 
     *   reference to the current PDI buffer
     * 
     * @note This overload is enabled only for Input entries 
     */
    template<bool enable = common::translation::is_input_dir_v<dir>,
        std::enable_if_t<enable, bool> = true>
    inline void update(config::types::Span<const uint8_t> pdi);

    /**
     * @brief Updates current Process Data Image @p pdi buffer with data stored in the entrie's internal buffer
     * @details This method is called by the Master driver each time Output Process Data Image needs to be
     *    updated before sending new data to the bus
     * 
     * @param pdi 
     *   reference to the current PDI buffer
     * 
     * @note This overload is enabled only for Output entries 
     */
    template<bool enable = common::translation::is_output_dir_v<dir>,
        std::enable_if_t<enable, bool> = true>
    inline void update(config::types::Span<uint8_t> pdi) const;

private: /* -------------------------------------------------- Private types ------------------------------------------------------ */

    /**
     * @brief Class wrapping a binary-image buffer storing copy of the current content of 
     *    the Entry in the PDI
     */
    class Buffer;

private: /* --------------------------------------------------- Private data ------------------------------------------------------ */

    /// Make referene class a friend to let it access internal buffer
    friend class Pdo;
    
    /// Name of the PDO
    std::string name;
    /// Type descriptor of the entry
    types::Type type;

    /// Binary-image buffer of the entry
    Buffer buffer;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/slave/pdo/entry/buffer.hpp"
#include "ethercat/slave/pdo/entry/reference.hpp"
#include "ethercat/slave/pdo/entry/entry.hpp"

/* ================================================================================================================================ */

#endif
