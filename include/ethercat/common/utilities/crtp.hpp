/* ============================================================================================================================ *//**
 * @file       crtp.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 9th June 2022 2:56:09 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Header file of the base class for classes implementing CRTP static polymorphism
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_CRTP_H__
#define __ETHERCAT_COMMON_UTILITIES_CRTP_H__

/* ====================================================== Namespace patterns ====================================================== */

namespace ethercat::common::utilities {

/* ========================================================= Declarations ========================================================= */

/**
 * @class CRTP
 * @brief Universal base class providing static cast methods used to implement 
 *    CRTP interface classes
 * @tparam T 
 *    target (derived) class of the CRTP pattern
 * 
 * @example 
 *    
 *    template<typename Derived> 
 *    class CRTPInterface : public CRTP<Derived> {
 *    private:
 *        using CRTP<Derived>::impl;
 *    public:
 *        
 *        void foo() { 
 *            impl().foo_impl(); 
 *        }
 *        
 *        void foo() const { 
 *            impl().foo_const_impl(); 
 *        }
 * 
 *    };
 * 
 */
template<typename T>
class CRTP {
protected:

    /**
     * @brief Static cast of @a this into the target class
     */
    constexpr T &impl();

    /**
     * @brief Static cast of @a this into the target class (const version)
     */
    constexpr const T &impl() const;
    
};

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/utilities/crtp/crtp.hpp"

/* ================================================================================================================================ */

#endif
