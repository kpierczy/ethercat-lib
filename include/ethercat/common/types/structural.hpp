/* ============================================================================================================================ *//**
 * @file       structural.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of common base types describing structural EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TYPES_STRUCTURAL_H__
#define __ETHERCAT_COMMON_TYPES_STRUCTURAL_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/types/builtin.hpp"
#include "ethercat/config.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::types {

/* ======================================================== Structural type ======================================================= */

/**
 * @brief Base type for types describing custom structural type of EtherCAT (CoE) objects
 * 
 * @note StructuralTypeBase does not support nested structural types (however I'm not sure 
 *    whether CoE support such types at all)
 */
template<template<typename> typename SubitemsCollectionT, typename StringT, typename BuiltinTypeBase>
class StructuralTypeBase {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Structure describing type of the subitem of the structural type
     */
    struct Subitem {

        /// Subindex of the subobject
        std::size_t subindex;
        /// Name of the subitem
        StringT name;
        /// Type of the subitem
        BuiltinTypeBase type;
        /// Bitoffset of the item in the object's footprint
        std::size_t bitoffset;

        /**
         * @retval true 
         *    if both objects represent the same item
         * @retval false 
         *    otherwise
         */
        inline bool operator==(const Subitem &ritem) const;

        /**
         * @retval true 
         *    if objects represent different items
         * @retval false 
         *    otherwise
         */
        inline bool operator!=(const Subitem &ritem) const;
        
    };

public: /* ---------------------------------------------------- Public operators -------------------------------------------------- */

    /**
     * @retval true 
     *    if both objects represent the same type
     * @retval false 
     *    otherwise
     */
    inline bool operator==(const StructuralTypeBase &rtype) const;

    /**
     * @retval true 
     *    if objects represent different types
     * @retval false 
     *    otherwise
     */
    inline bool operator!=(const StructuralTypeBase &rtype) const;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */
    
    /**
     * @returns 
     *    bitesize of the described numeric type
     */
    constexpr std::size_t get_bitsize() const;
    
    /**
     * @returns 
     *    bytesize (rounded down) of the described numeric type
     */
    constexpr std::size_t get_bytesize() const;

public: /* ----------------------------------------------------- Public members --------------------------------------------------- */

    /// Name of the type
    StringT name;
    /// Subitems of the structure
    SubitemsCollectionT<Subitem> subitems { };
    /// Bitsize of the type 
    std::size_t bitsize { calculate_bitsize(subitems) };

public: /* ------------------------------------------------- Private static methods ----------------------------------------------- */

    /**
     * @brief Calculates expected size of the structure based on the list of subitems
     * 
     * @param items 
     *    list of structure's subitems
     * @returns 
     *    expected size of the structure
     */
    constexpr static std::size_t calculate_bitsize(config::types::Span<const Subitem> items);

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::types

#endif
