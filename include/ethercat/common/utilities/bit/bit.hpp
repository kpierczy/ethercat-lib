/* ============================================================================================================================ *//**
 * @file       bit.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 13th June 2022 1:19:55 am
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of common utilities for handling bit-aligned data
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_UTILITIES_BIT_BIT_H__
#define __ETHERCAT_COMMON_UTILITIES_BIT_BIT_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <algorithm>
// Private includes
#include "ethercat/common/utilities/bit.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::common::utilities::bit {

/* ======================================================== Free functions ======================================================== */

template<typename ValueT>
constexpr auto get_bit(ValueT &&value, std::size_t position) {
    return bool(value & (0x1U << position));
}


template<typename ValueT>
constexpr auto set_bit(ValueT &value, std::size_t position) {
    value |= (0x1U << position);
}


template<typename ValueT>
constexpr auto clear_bit(ValueT &value, std::size_t position) {
    value &= (~(0x1U << position));
}


void copy_bytes(const uint8_t *src, uint8_t *dst, std::size_t n) {
    std::copy_n(src, n, dst);
}


void copy_bytes_from_bitshifted(const uint8_t *src, uint8_t *dst, std::size_t n, std::size_t bitoffset) {
    
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


void copy_bytes_to_bitshifted(const uint8_t *src, uint8_t *dst, std::size_t n, std::size_t bitoffset) {
    
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


void copy_bits(const uint8_t *src, uint8_t *dst, std::size_t n) {

    // Calculate number of full bytes to be copied
    std::size_t bytes = n / BITS_IN_BYTE;
    // Copy whole bytes
    copy_bytes(src, dst, bytes);
    
    // Calculate unaligned bits remainding from the number of bits to be copied
    std::size_t bits_remainder = n % BITS_IN_BYTE;
    // If all bits has been copied, return
    if(bits_remainder == 0)
        return;

    // Forward pointers by amount of copied bytes
    src += bytes;
    dst += bytes;

    // Copy remainding bits
    dst[0] = ((dst[0] & (0xFFU << (BITS_IN_BYTE - bits_remainder))) | (src[0] & (0xFFU >> bits_remainder)));
}


void copy_bits_from_bitshifted(const uint8_t *src, uint8_t *dst, std::size_t n, std::size_t bitoffset) {
    
    // Calculate number of full bytes to be copied
    std::size_t bytes = n / BITS_IN_BYTE;
    // Copy whole bytes
    copy_bytes_from_bitshifted(src, dst, bytes, bitoffset);
    
    // Calculate unaligned bits remainding from the number of bits to be copied
    std::size_t bits_remainder = n % BITS_IN_BYTE;
    // If all bits has been copied, return
    if(bits_remainder == 0)
        return;

    // Forward pointers by amount of copied bytes
    src += bytes;
    dst += bytes;

    // Calculate bitoffset (from LSBit) in current src[0] that left to be copied
    std::size_t src_bitoffset = (bitoffset % BITS_IN_BYTE);
    // If there is no offset from byte boundary, perform simplified copy
    if(src_bitoffset == 0) {
        dst[0] = ((dst[0] & (0xFFU << bits_remainder)) | (src[0] >> (BITS_IN_BYTE - bits_remainder)));
        return;
    } 

    // Calculate offset of the last bit to be copied from src[0] counting from LSBit of the src[0]
    std::size_t remainding_src_bits_lsb_offset = src_bitoffset + bits_remainder;

    // If all remaining bits reside in src[0]
    if(remainding_src_bits_lsb_offset < BITS_IN_BYTE) {

        // Select bits of src[0] that has to be copied and align them to the LSBit of src[0]
        uint8_t bits_to_copy = ((src[0] << (BITS_IN_BYTE - remainding_src_bits_lsb_offset)) >> (BITS_IN_BYTE - bits_remainder));

        // Copy bits to destination leaving bits that are not subject to be overwitten untouched
        dst[0] = ((dst[0] & (0xFFU << (BITS_IN_BYTE - bits_remainder))) | bits_to_copy);

    // Else, if remainding bits reside in both src[0] and src[1]
    } else {

        // Merge bits from src[0] and src[1] into 16-bit int
        uint16_t src_bits = ((src[1] << BITS_IN_BYTE) | src[0]);
        // Remove bits > 'remainding_src_bits_lsb_offset' and align bits form 'src_bitoffset' to 0
        src_bits = (src_bits << (BITS_IN_WORD - remainding_src_bits_lsb_offset)) >> (BITS_IN_WORD - bits_remainder);

        // Copy bits to destination leaving bits that are not subject to be overwitten untouched
        dst[0] = ((dst[0] & (0xFFU << (BITS_IN_BYTE - bits_remainder))) | (src_bits & 0xFFU));
        
    }

}


void copy_bits_to_bitshifted(const uint8_t *src, uint8_t *dst, std::size_t n, std::size_t bitoffset) {
    
    // Calculate number of full bytes to be copied
    std::size_t bytes = n / BITS_IN_BYTE;
    // Copy whole bytes
    copy_bytes_to_bitshifted(src, dst, bytes, bitoffset);
    
    // Calculate unaligned bits remainding from the number of bits to be copied
    std::size_t bits_remainder = n % BITS_IN_BYTE;
    // If all bits has been copied, return
    if(bits_remainder == 0)
        return;

    // Forward pointers by amount of copied bytes
    src += bytes;
    dst += bytes;

    // Calculate bitoffset (from LSBit) in current dst[0] of the first bit that has not been overwitten yet
    std::size_t dst_bitoffset = (bitoffset % BITS_IN_BYTE);
    // If there is no offset from byte boundary, perform simplified copy
    if(dst_bitoffset == 0) {
        dst[0] = ((dst[0] & (0xFFU << bits_remainder)) | (src[0] >> (BITS_IN_BYTE - bits_remainder)));
        return;
    } 

    // Calculate offset of the last bit to be overwritten in dst[0] counting from LSBit of the dst[0]
    std::size_t remainding_dst_bits_lsb_offset = dst_bitoffset + bits_remainder;

    // If all remaining bits reside in dst[0]
    if(remainding_dst_bits_lsb_offset < BITS_IN_BYTE) {

        // Select bits of dst[0] that has to be untouched
        uint8_t dst_bits_untouched = dst[0] & ~(
            (0xFFU << (BITS_IN_BYTE - remainding_dst_bits_lsb_offset)) 
                   >> (BITS_IN_BYTE - bits_remainder)
        );

        // Select bits of src[0] that has to be copied and align them to the bitshift of bits in dst[0] that has to be overwitten
        uint8_t bits_to_copy = (
            (src[0] << (BITS_IN_BYTE - bits_remainder)) 
                    >> (BITS_IN_BYTE - remainding_dst_bits_lsb_offset)
        );

        // Copy bits to destination leaving bits that are not subject to be overwitten untouched
        dst[0] = dst_bits_untouched | bits_to_copy;

    // Else, if remainding bits reside in both src[0] and src[1]
    } else {

        // Get remaining bits in src[0] and promote them to 16-bit integer; shift result to the offset of bits in dst[0] that has to be overwitten
        uint16_t src_bits_left = uint16_t(src[0] & (0xFF >> (BITS_IN_BYTE - bits_remainder))) << dst_bitoffset;
        
        // Merge bits in dst[0] and dst[1] into 16-bit ineger
        uint16_t dst_bits = uint16_t((dst[1] << BITS_IN_BYTE) | dst[0]);
        // Select bits of dst[0:1] that has to be left unchanged
        dst_bits = dst_bits & ~(
            (0xFFFFU << (BITS_IN_WORD - remainding_dst_bits_lsb_offset)) 
                     >> (BITS_IN_WORD - remainding_dst_bits_lsb_offset - dst_bitoffset)
        );

        // Merge src and dst bits
        dst_bits |= src_bits_left;
        // Dispatch dst bits into dst[0] and dst[1]
        dst[0] = ( dst_bits                  & 0xFFU);
        dst[1] = ((dst_bits >> BITS_IN_BYTE) & 0xFFU);
        
    }

}

/* ================================================================================================================================ */

} // End namespace ethercat::common::utilities::bit

#endif
