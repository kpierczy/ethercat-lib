/* ============================================================================================================================ *//**
 * @file       configruation.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:12:57 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the ProcessImage class implementing parsing interface of the <Configuration> tag of the ENI 
 *             (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_CONFIGRUATION_H__
#define __ETHERCAT_COMMON_ENI_CONFIGRUATION_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <optional>
#include <memory>
#include <sstream>
#include <filesystem>
#include <string>
// Boost includes
#include <boost/property_tree/ptree.hpp>
// Private includes
#include "ethercat/eni/common.hpp"
#include "ethercat/eni/master.hpp"
#include "ethercat/eni/slave.hpp"
#include "ethercat/eni/cyclic.hpp"
#include "ethercat/eni/process_image.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================= Class ============================================================ */

/**
 * @brief Class representing entrypoint for parsing the ENI (EtherCAT Network Informations) files.
 *    In particular it provides direct parsing of subelements of the <Configuration> tag along
 *    with interface for loading the XML description from various input sources.
 *
 * @note [1] Implementation of the ENI parser has been based on the ENI-generation style of the TwinCAT
 *    software. This results in some assumptions being taken about naming conventions of some parts
 *    of the file. When possible, these conventions has been adopted as a 'special case' and the
 *    generalized forms of names are allowed. However there are some special-cases where strings
 *    stored by particular tags of the ENI file are required to hold some special form. These are
 *    among others:
 *
 *        - names of objects mapped into the PDI (Process Data Image) that are described by the
 *          <ProcessImage/Inputs> and <ProcessImage/Outputs> tags are assummed to be composed of
 *          two or three dot-delimited parts in shape of
 *          
 *              - <pdo-name.object-name> (master's objects)
 *              - <slave-name.pdo-name.object-name> (slaves' objects)
 *
 *    All assumptions concerning naming conventions are extensively described in the comments to the
 *    related parts of API. Mentioned assumptions has been taken to ease the implementetion, as the
 *    main purpose of the parser at the design stage was to work with TwinCAT-generated descriptions
 *    exclusively. In the future, if wider-class ENI format support is needed, implementation can be
 *    (relatively) easily extended to properly handle generalized naming conventions.
 *
 * @note [2] All ENI-parsing objects share common instance of the XML file parser. Access to this instance
 *    is not thread-safe in the current implementation. 
 *
 * @note [3] ENI-parsing objects provide lazy evaluation implementation. In result actual content of the XML
 *    tree is not verified until it's parsing is requested by the client code.
 * @note [4] Configruation object must outlive all sub-parsers produced by it's API
 * 
 * @todo Extend @ref Element class with shared pointer to the main XML structure to prolong it's live
 *    to the longest lifetime of it's references ( see [4] )
 */
class Configuration : protected Element {

public: /* ------------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /// Inherit all basic constructors
    using Element::Element;

public: /* ---------------------------------------------- Public management methods ----------------------------------------------- */

    /// Let user autonomize the element
    using Element::autonomize;

public: /* --------------------------------------------------- Public methods ----------------------------------------------------- */

    /**
     * @returns
     *    parser of the <Master> tag of the ENI file
     *
     * @throws eni::Error
     *    if <Config> ENI elements does not contain <Master> element
     */
    inline Master get_master() const;

    /**
     * @param name
     *    name of the slave whose description should be returned (content of the slave's
     *    <Info/Name> tag)
     *
     * @returns
     *    parser object of the <Slave> tag of the ENI file describing Slave named @p name
     *
     * @throws eni::Error
     *    if any of <Slave> ENI elements does not contain <Info>/<Name> element
     */
    std::optional<Slave> get_slave(std::string_view name) const;

    /**
     * @returns
     *    parser objects of all <Slave> tags of the ENI file
     */
    std::vector<Slave> get_slaves() const;

    /**
     * @returns
     *    parser number of slave devices on the bus
     */
    inline std::size_t get_slaves_num() const;

    /**
     * @returns
     *    names of all slaves described by the ENI file
     */
    std::vector<std::string> list_slaves() const;

    /**
     * @returns
     *    parser object of the <Cyclic> tag of the ENI file
     *
     * @throws eni::Error
     *    if <Config> ENI elements does not contain <Cyclic> element
     */
    inline Cyclic get_cyclic() const;

    /**
     * @returns
     *    parser object of the <ProcessImage> tag of the ENI file
     *
     * @throws eni::Error
     *    if <Config> ENI elements does not contain <ProcessImage> element
     */
    inline ProcessImage get_process_image() const;
    
};

/* ==================================================== Auxiliary I/O functions =================================================== */

/**
 * @brief Loads ENI property tree from the file located at @p path . Obtains <Config> element from it.
 *
 * @param path
 *    path to the ENI file to be parsed
 * @returns 
 *    target element loaded from XML tree
 * 
 * @throws eni::Error
 *    if ENI file could not be loaded
 */
inline Configuration configruation_from_file(const std::filesystem::path &path);

/**
 * @brief Loads ENI property tree read into the runtime string . Obtains <Config> element from it.
 *
 * @param eni
 *    string object containing ENI description
 * @returns 
 *    target element loaded from XML tree
 * 
 * @throws eni::Error
 *    if ENI file could not be loaded
 */
inline Configuration configruation_from_string(const std::string &eni);

/**
 * @brief Loads ENI property from tree the given input @p stream . Obtains <Config> element from it.
 *
 * @param stream
 *    stream to load ENI from
 * @returns 
 *    target element loaded from XML tree
 * 
 * @throws eni::Error
 *    if ENI file could not be loaded
 */
inline Configuration configruation_from_stream(std::basic_istream<char> &stream);

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/configuration/configuration.hpp"

/* ================================================================================================================================ */

#endif
