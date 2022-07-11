/* ============================================================================================================================ *//**
 * @file       common.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 11:34:53 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of methods of common utilities used by all specializations of the DefaultTranslator
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_COMMON_H__
#define __ETHERCAT_COMMON_TRANSLATION_DEFAULT_TRANSLATOR_COMMON_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/common/translation/translation_error.hpp"
#include "ethercat/common/translation/default_translator.hpp"
#include "ethercat/common/translation/default_translator/supported_types.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::translation::details {

/* =========================================================== Utilities ========================================================== */

template<bool flag = false>
inline constexpr void cannot_report_translation_error() {
    static_assert(flag, 
        "[ethercat::common::translation::DefaultTranslator][BUG] Configruation of the DefaultTranslator  "
        "is invalid: transllator would not be able to report error condition during translation (EnableVerification "
        "is enabled but neither EnableBooleanReturn nor EnableExceptions is enabled)"
    ); 
}

/* ================================================ DefaultTranslator - common base =============================================== */

/**
 * @brief Common base class for all specializations of the default translator
 * 
 * @tparam TranslatorT 
 *    specialized type
 * @tparam T 
 *    target type of translation
 */
template<typename TranslatorT, typename T>
struct DefaultTranslatorBase {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /// Self type
    using SelfType = TranslatorT;
    /// Target type
    using Type = T;
    /// Argument-efficient type
    using ArgType = translation::ArgType<T>;

protected: /* -------------------------------------------------- Protected types -------------------------------------------------- */

    /// Direction of translation
    enum class Direction { 
        Input,
        Output,
        None
    };

protected: /* ---------------------------------------- Protected members (data validation) ---------------------------------------- */

    /// Number of bits in a byte
    static inline constexpr std::size_t BITS_IN_BYTE = 8;

    /**
     * @brief Rounds up @p num_to_round to the nearest multiple of @p multiple
     */
    template<std::size_t multiple>
    static constexpr std::size_t round_up_to_multiple_of(std::size_t num_to_round) {
        
        if constexpr (multiple == 0)
            return num_to_round;

        // Calculate remindes
        int remainder = num_to_round % multiple;
        // If remainder is zero, no rounding is needed
        if (remainder == 0)
            return num_to_round;
        // Otherwise, round
        return num_to_round + multiple - remainder;
    }

    /**
     * @brief Auxiliary function calculating whether the @p buffer provides anough room to 
     *    fit @p bytes number of bytes
     */
    template<typename BufferT>
    static auto is_enough_size_to_fit(BufferT &&buffer, std::size_t bytes) -> bool {
        return not (buffer.size() < bytes);
    }

    /**
     * @brief Auxiliary function calculating whether for the given @p bitoffset the @p buffer provides
     *    anough room to fit @p bytes number of bytes
     */
    template<typename BufferT>
    static constexpr auto is_enough_size_to_fit(BufferT &&buffer, std::size_t bytes, std::size_t bitoffset) -> bool {
        return not ((buffer.size() * BITS_IN_BYTE - bitoffset) < (bytes * BITS_IN_BYTE));
    }

    /**
     * @brief Auxiliary function calculating whether the @p buffer provides minimal amount of room to 
     *    fit @p bytes number of bytes
     */
    template<typename BufferT>
    static constexpr auto is_minimal_size_to_fit(BufferT &&buffer, std::size_t bytes) -> bool {
        return (buffer.size() == bytes);
    }

    /**
     * @brief Auxiliary function calculating whether for the given @p bitoffset the @p buffer provides
     *    minimal amount of room to fit @p bytes number of bytes
     */
    template<typename BufferT>
    static constexpr auto is_minimal_size_to_fit(BufferT &&buffer, std::size_t bytes, std::size_t bitoffset) -> bool {
        return ((buffer.size() * BITS_IN_BYTE) == (bytes * BITS_IN_BYTE + round_up_to_multiple_of<BITS_IN_BYTE>(bitoffset)));
    }

protected: /* ----------------------------------------- Protected members (data copying) ------------------------------------------ */

    /// Gets value of the bit at position @p position
    template<typename ValueT>
    static constexpr auto get_bit(ValueT &&value, std::size_t position) { return bool(value & (0x1U << position)); }

    /// Sets @p position bit in the @p value
    template<typename ValueT>
    static constexpr auto set_bit(ValueT &value, std::size_t position) { value |= (0x1U << position); }

    /// Clears @p position bit in the @p value
    template<typename ValueT>
    static constexpr auto clear_bit(ValueT &value, std::size_t position) { value &= (~(0x1U << position)); }

    /**
     * @brief Copies @p n bytes from @p src to @p dst
     */
    static inline auto copy_bytes(const uint8_t *src, uint8_t *dst, std::size_t n) {
        std::copy_n(src, n, dst);
    }

    /**
     * @brief Copies @p n bytes from @p src to @p dst given the @p bitoffset of data in @p src
     */
    static inline auto copy_bytes_from_bitshifted(const uint8_t *src, uint8_t *dst, std::size_t n, std::size_t bitoffset) {
        
        // Calculate bitoffset from the byte boundry
        std::size_t bitoffset_remainder = bitoffset % BITS_IN_BYTE;
        // Calculate number of full bytes in the offset
        std::size_t bytes_offset = bitoffset / BITS_IN_BYTE;

        // Skip bytes offset in the source
        src += bytes_offset;

        // If bitoffset does not cross byte boundary, copy raw bytes
        if(bitoffset_remainder == 0)
            return copy_bytes(src, dst, n);

        // Compute distance from the LSBit (of the source byte) to the bitoffset boundary
        std::size_t lsb_bitoffset = bitoffset_remainder;
        // Compute distance from the MSBit (of the source byte) to the bitoffset boundary
        std::size_t msb_bitoffset = (BITS_IN_BYTE - bitoffset_remainder);

        // Copy data to the output buffer
        for(std::size_t i = 0; i < n; ++i)
            dst[i] = ((src[i + 1] << msb_bitoffset) | (src[i] >> lsb_bitoffset));
    }

    /**
     * @brief Copies @p n bytes from @p src to @p dst with the required @p bitoffset of data in @p dst
     */
    static inline auto copy_bytes_to_bitshifted(const uint8_t *src, uint8_t *dst, std::size_t n, std::size_t bitoffset) {
        
        // Calculate bitoffset from the byte boundry
        std::size_t bitoffset_remainder = bitoffset % BITS_IN_BYTE;
        // Calculate number of full bytes in the offset
        std::size_t bytes_offset = bitoffset / BITS_IN_BYTE;

        // Skip bytes offset in the destination
        dst += bytes_offset;

        // If bitoffset does not cross byte boundary, copy raw bytes
        if(bitoffset_remainder == 0)
            return copy_bytes(src, dst, n);

        // Compute distance from the LSBit (of the destination byte) to the bitoffset boundary
        std::size_t lsb_bitoffset = bitoffset_remainder;
        // Compute distance from the MSBit (of the destination byte) to the bitoffset boundary
        std::size_t msb_bitoffset = (BITS_IN_BYTE - bitoffset_remainder);

        // Handle the first element of the output manually preserving [bitoffset_remainder] LSBits
        dst[0] = ((src[0] << lsb_bitoffset) | (dst[0] & (0xFFU >> msb_bitoffset)));

        // Copy data to all (n-1) 'full' bytes of the destination (i.e. bytes that will hold 8 bits from the source buffer each)
        for(std::size_t i = 0; i < n - 1; ++i)
            dst[i + 1] = (src[i + 1] << lsb_bitoffset) | (src[i] >> msb_bitoffset);

        // Handle the last element of the output manually preserving [8 - bitoffset_remainder] MSBits
        dst[n - 1] = ((dst[n - 1] & (0xFFU << lsb_bitoffset)) | (src[n - 1] >> msb_bitoffset));
    }

protected: /* ---------------------------------------- Protected members (errors reporting) --------------------------------------- */

    /**
     * @brief Auxiliary function for reporting an error
     */
    template<Direction dir, typename ErrorMessageT>
    __attribute__((always_inline)) static constexpr auto report_error (ErrorMessageT &&msg)  {
        
        // Throw exception, if requested by configuration
        if constexpr(config::translation::default_translators::EnableExceptions) {
            if constexpr(dir == Direction::Input)
                throw TranslationError::make<TranslationError::Direction::Input, SelfType, T>(msg);
            else if constexpr(dir == Direction::Output)
                throw TranslationError::make<TranslationError::Direction::Output, SelfType, T>(msg);
            else
                throw TranslationError::make<SelfType, T>(msg);
        // If boolean return is enabled, return error
        } else if constexpr(config::translation::default_translators::EnableBooleanReturn)
            return false;
        // Else, report compile-time error as translator has not means to report the error
        else
            cannot_report_translation_error();
    
    }

    /**
     * @brief Auxiliary skeleton of the translating function providing common part of all basic 
     *    translation routines
     * 
     * @tparam dir 
     *     translation direction
     * @param is_buffer_enough_size 
     *     functor checking whether binary-image buffer is big enough
     * @param is_buffer_minimal_size_to_fit 
     *     functor checking whether binary-image buffer is exact size of the serialize object
     * @param translate 
     *     functor performing actual translation
     * 
     * @returns 
     *    either void or boolean status depending on the configruation of the library
     */
    template<
        Direction dir,
        typename IsBufferEnoughSizeT,
        typename IsBufferMinimalSizeToFitT,
        typename TranslateT>
    __attribute__((always_inline)) static inline auto translate
    (
        IsBufferEnoughSizeT &&is_buffer_enough_size,
        IsBufferMinimalSizeToFitT &&is_buffer_minimal_size_to_fit,
        TranslateT &&translate
    ) {

        // If verification is enabled
        if constexpr(config::translation::default_translators::EnableVerification) {

            // If partial translation is disabled make sure that binary buffer provides exact amount of data to construct target object
            if constexpr(not config::translation::default_translators::AllowPartialTranslation) {
                
                if(not is_buffer_minimal_size_to_fit()) {
                    if constexpr(dir == Direction::Input)
                        return report_error<dir>("Binary image provides more data than is required to construct target object");
                    else
                        return report_error<dir>("Binary image provides more space than is required to serialize target object");                    
                }
            
            } // Otherwise, check whether there is enough binary data to construct target object
            else {
                
                if(not is_buffer_enough_size()) {
                    if constexpr(dir == Direction::Input)
                        return report_error<dir>("Not enough data in the binary image to construct target object");
                    else
                        return report_error<dir>("Not enough space in the binary image to serialize target object");
                }
                
            }

        }

        // Translate data
        translate();

        // If boolean return is enabled, return
        if constexpr(config::translation::default_translators::EnableBooleanReturn)
            return true;
    }

};

/* ================================================================================================================================ */

} // End namespace ethercat::common::translation::details

#endif
