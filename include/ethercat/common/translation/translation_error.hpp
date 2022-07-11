/* ============================================================================================================================ *//**
 * @file       translation_error.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declaration of the TranslationError exception class
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_TRANSLATION_ERROR_H__
#define __ETHERCAT_COMMON_TRANSLATION_TRANSLATION_ERROR_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <stdexcept>
// Private includes
#include "ethercat/common/translation/translator_traits.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation {

/* ======================================================= TranslationError ======================================================= */

/**
 * @brief Custom exception class describing error occuring during translation process
 */
class TranslationError : public std::runtime_error {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Direction of the performed translation at error occurance
    enum class Direction { Input, Output };

public: /* ------------------------------------------------ Public static methods ------------------------------------------------- */

    /**
     * @brief Construct a new TranslationError object describing error occuring when translating
     *    data from/to data buffer into/from object of type @p T using translator of type @p TranslatorT 
     * 
     * @tparam TranslatorT 
     *    type of the erronous translator
     * @tparam T 
     *    target type
     * @param what 
     *    optional information about the error (put at the end of the message into brackets)
     * @param context 
     *    optional context string (put at the beggining of the message into square brackets)
     */
    template<
        typename TranslatorT,
        typename T
    > static TranslationError make(std::string_view what = "", std::string_view context = "");

    /**
     * @brief Construct a new TranslationError object describing error occuring when translating
     *    data from/to data buffer into/from object of type @p T using translator of type @p TranslatorT 
     * 
     * @tparam dir 
     *    direction of the erronous translation
     * @tparam TranslatorT 
     *    type of the erronous translator
     * @tparam T 
     *    target type
     * @param what 
     *    optional information about the error (put at the end of the message into brackets)
     * @param context 
     *    optional context string (put at the beggining of the message into square brackets)
     */
    template<
        Direction dir,
        typename TranslatorT,
        typename T
    > static TranslationError make(std::string_view what = "", std::string_view context = "");
    
private: /* ---------------------------------------------------- Private ctors ---------------------------------------------------- */

    /// Make construction private
    using std::runtime_error::runtime_error;

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/common/translation/translation_error/translation_error.hpp"

/* ================================================================================================================================ */

#endif
