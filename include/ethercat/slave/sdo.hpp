/* ============================================================================================================================ *//**
 * @file       sdo.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2022 9:50:06 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the SDO nested class of the Slave interface
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_SLAVE_SDO_H__
#define __ETHERCAT_SLAVE_SDO_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/slave.hpp"
#include "ethercat/slave/translators_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat {

/* ============================================================== SDO ============================================================= */

/**
 * @brief A proxy class template proiding an interface for uploading and downloading 
 *    Service Data Objects (SDO) from/to memory of the slave device performing automated
 *    data (de)serialization
 * 
 * @tparam ImplementationT 
 *    type implementing hardware-specific part of the Slave driver
 * @tparam dir 
 *    communication direction for SDO interface 
 * @tparam TranslatorT 
 *    translator type used to perform automatic data (de)serialization 
 * @tparam T 
 *    target type used to represent the object in the app-domain ( if @c void the type 
 *    will be deduced automatically from definition of the @p TranslatorT )
 * 
 * @note Template is inactive if @p TranslatorT is not a sizing translator of type @p T for 
 *    the given @p dir .
 * @note Sdo class must have access to implementation of SDO communication methods. or this
 *    reason class deriving from @ref Slave shall make Sdo clas template a friend
 */
template<typename ImplementationT>
template<typename Slave<ImplementationT>::SdoDirection dir, typename TranslatorT, typename T,
    slave::traits::enable_if_sizing_translator_t<dir, TranslatorT, T>>
class Slave<ImplementationT>::Sdo :
    protected common::translation::SizingTranslatorWrapper<
        static_cast<common::translation::TranslationDirection>(dir), TranslatorT, T
    >
{
    /// Type of the base class used to wrap the translator
    using WrapperType = common::translation::SizingTranslatorWrapper<
        static_cast<common::translation::TranslationDirection>(dir), TranslatorT, T
    >;

    /// Translator traits
    using TranslatorTraits = typename WrapperType::Traits;
    /// Sizing translator traits
    using SizingTranslatorTraits = typename WrapperType::SizingTraits;

public: /* ---------------------------------------------------- Public types ------------------------------------------------------ */

    /// Type of the associated slave
    using SlaveT = ImplementationT;

    /// Direction for which template has been specialized
    static constexpr Slave<ImplementationT>::SdoDirection Dir = dir;
    
    /// Translator type
    using TranslatorType = typename WrapperType::TranslatorType;
    /// Target type
    using Type = typename WrapperType::Type;
    /// Efficient argument type
    using ArgType = typename WrapperType::ArgType;

    /// SDO direction
    using Direction = typename Slave<ImplementationT>::SdoDirection;

    /**
     * @brief Structure describing address of the object in the Objects Dictionary of the
     *    slave device
     */
    struct Address {

        /// Index of the object
        uint16_t index;
        /// Subindex of the object
        uint16_t subindex { 0 };

    };

    /**
     * @brief Type of the CoE access to be performed
     */
    enum class AccessType { 

        // Complete access (access all subitems with indeces starting from the given one)
        Complete,
        // Limited access (access only a subitem with the given subindex)
        Limited
        
    };

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /// Enable copy-construction
    inline Sdo(const Sdo &rsdo) = default;
    /// Enable copy-asignment
    inline Sdo &operator=(const Sdo &rsdo) = default;

    /// Enable move-construction
    inline Sdo(Sdo &&rsdo) = default;
    /// Enable move-asignment
    inline Sdo &operator=(Sdo &&rsdo) = default;

public: /* --------------------------------------------------- Public methods ----------------------------------------------------- */

    /**
     * @returns 
     *    address of the SDO
     */
    inline Address get_address() const;

public: /* ------------------------------------------------- Public I/O methods --------------------------------------------------- */

    /**
     * @brief Downloads @p object to the slave's memory
     * 
     * @param object 
     *    object to be written to the slave
     * @param timeout 
     *    I/O timeout
     * @param access_type 
     *    type of the access
     * 
     * @throws error 
     *    whatever error thrown by implementation
     * 
     * @note This method is enabled only for output/bidirectional SDO interface
     */
    template<bool enable = 
            common::translation::is_at_least_input_dir_v<dir> and 
            SizingTranslatorTraits::sizing::is_available 
        , std::enable_if_t<enable, bool> = true>
    void download(
        ArgType object,
        std::chrono::milliseconds timeout = std::chrono::milliseconds{ 1000 },
        AccessType access_type = AccessType::Limited 
    );

    /**
     * @brief Uploads daa from the slave's memory
     * 
     * @param timeout 
     *    I/O timeout
     * @param access_type 
     *    type of the access
     * @returns 
     *    deserialized data read from the device
     * 
     * @throws error 
     *    whatever error thrown by implementation
     * 
     * @note This method is enabled only for input/bidirectional SDO interface
     * @note This overload creates binary-image for incoming data based on the static
     *    sizing method of the translator. If such a method is not defined, this overload
     *    is disabled
     */
    template<bool enable = 
            common::translation::is_at_least_output_dir_v<dir> and 
            std::is_default_constructible_v<Type>        and 
            SizingTranslatorTraits::sizing::is_available
        , std::enable_if_t<enable, bool> = true>
    Type upload(
        std::chrono::milliseconds timeout = std::chrono::milliseconds{ 1000 },
        AccessType access_type = AccessType::Limited
    ) const;

    /**
     * @brief Uploads daa from the slave's memory
     * 
     * @param object 
     *    reference to the object that data will be parsed into
     * @param timeout 
     *    I/O timeout
     * @param access_type 
     *    type of the access
     * @returns 
     *    deserialized data read from the device
     * 
     * @throws error 
     *    whatever error thrown by implementation
     * 
     * @note This method is enabled only for input/bidirectional SDO interface
     * @note This overload creates binary-image for incoming data based on the dynamic
     *    sizing method of the translator. If such a method is not defined, this overload
     *    is disabled
     */
    template<bool enable = 
            common::translation::is_at_least_output_dir_v<dir> and 
            SizingTranslatorTraits::sizing::is_available
        , std::enable_if_t<enable, bool> = true>
    void upload(
        Type &object,
        std::chrono::milliseconds timeout = std::chrono::milliseconds{ 1000 },
        AccessType access_type = AccessType::Limited
    ) const;

protected: /* --------------------------------------------- Protected ctors & dtors ----------------------------------------------- */
    
    /// Make Slave a friend to let it access constructor
    friend class Slave<SlaveT>;

    /**
     * @brief Construct a new SDO object with the given stateless translator
     * 
     * @tparam ArgsT 
     *    types of @p args
     * @param slave 
     *    pointer to the associated slave interface
     * @param address
     *    address of the SDO
     * 
     * @note Constructor is enabled only if wrapped translator is stateless
     */
    template<typename TranslatorU = TranslatorT,
        slave::traits::enable_if_stateless_sizing_translator_t<dir, TranslatorU, T> = true>
    inline Sdo(SlaveT *slave, Address address);
    
    /**
     * @brief Construct a new SDO object with the given statefull translator
     * 
     * @tparam ArgsT 
     *    types of @p args
     * @param slave 
     *    pointer to the associated slave interface
     * @param address
     *    address of the SDO
     * @param args 
     *    arguments to be passed to the wrapped translator's constructor
     * 
     * @note Constructor is enabled only if wrapped translator is statefull
     */
    template<typename... ArgsT, typename TranslatorU = TranslatorT,
        slave::traits::enable_if_statefull_sizing_translator_t<dir, TranslatorU, T> = true>
    inline Sdo(SlaveT *slave, Address address, ArgsT&&... args);
    
private: /* --------------------------------------------------- Private data ------------------------------------------------------ */

    /// Reference to the Slave interface
    SlaveT *slave;
    /// Address of the SDO
    Address address;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/slave/sdo/sdo.hpp"

/* ================================================================================================================================ */

#endif
