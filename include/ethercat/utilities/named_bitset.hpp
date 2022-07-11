/* ============================================================================================================================ *//**
 * @file       named_bitset.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Thursday, 2nd June 2022 12:53:22 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the named_bitset class template extending std::bitset with capability to be indexed ([] operator)
 *             with enum class values
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_UTILITIES_NAMED_BITSET_H__
#define __ETHERCAT_UTILITIES_NAMED_BITSET_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <type_traits>
#include <bitset>
// Boost includes
#include <boost/preprocessor.hpp>
// Private includes
#include "ethercat/common/utilities/enum.hpp"
#include "ethercat/common/utilities/bit.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::utilities {

/* ====================================================== Auxiliary operators ===================================================== */

/**
 * @brief Pseudo-bitwise-NOT of enumerated constant.
 * @details This operator is introduced to help managing bitsets whose fields
 *    are named with some enumerated constants. When this operator is acessible
 *    notting enumerate constant e results in ~(1 << e)
 * 
 * @tparam Enum
 *    type of the enum
 * @param e
 *    operand
 * @returns  
 *    result of ~(1 << e_) where e_ is @p e casted to underlying integral type
 * 
 * @todo When C++20 is available require @p Enum to be enumeration
 */
template<typename Enum, 
    std::enable_if_t<std::is_enum_v<Enum>, bool> = true
> constexpr auto operator~(Enum e) {
    return ( ~( std::underlying_type_t<Enum>(1) << common::utilities::to_underlying(e) ) );
}

/**
 * @brief Pseudo-bitwise-OR of two enumerated constants.
 * @details This operator is introduced to help managing bitsets whose fields
 *    are named with some enumerated constants. When this operator is acessible
 *    oring two enumerated types e1, e2 results in ((1 << e1) | (1 << e2))
 * 
 * @tparam Enum1 
 *    type of the first enum
 * @tparam Enum2 
 *    type of the second enum
 * @param e1 
 *    left operand
 * @param e2 
 *    right operand
 * @returns  
 *    result of ((1 << e_1) | (1 << e_2)) where e_1 and e_2 are @p e1 and @p e2 
 *    casted to underlying integral types
 * 
 * @todo When C++20 is available require @p Enum1 and @p Enum2 to be enumeration
 */
template<typename Enum1, typename Enum2, 
    std::enable_if_t<std::is_enum_v<Enum1> and std::is_enum_v<Enum2>, bool> = true
> constexpr auto operator|(Enum1 e1, Enum2 e2) {
    return (
        ( std::underlying_type_t<Enum1>(1) << common::utilities::to_underlying(e1) ) | 
        ( std::underlying_type_t<Enum2>(1) << common::utilities::to_underlying(e2) )
    );
}

/**
 * @brief Pseudo-bitwise-OR of integral constant and enumerated constants.
 * @details This operator is introduced to enable OR-chaining with use of the
 *    previous operator's overload
 * 
 * @tparam Enum1 
 *    type of the first enum
 * @tparam Enum2 
 *    type of the second enum
 * @param e1 
 *    left operand
 * @param e2 
 *    right operand
 * @returns  
 *    result of (i | (1 << e_)) where e_ is @p e casted to underlying integral type
 * 
 * @todo When C++20 is available require @p Int to be integral and @p Enum to be enumeration
 */
template<typename Int, typename Enum, 
    std::enable_if_t<std::is_integral_v<Int> and std::is_enum_v<Enum>, bool> = true
> constexpr auto operator|(Int i, Enum e) {
    return (i | ( std::underlying_type_t<Enum>(1) << common::utilities::to_underlying(e) ));
}

/**
 * @brief Pseudo-bitwise-AND of two enumerated constants.
 * @details This operator is introduced to help managing bitsets whose fields
 *    are named with some enumerated constants. When this operator is acessible
 *    oring two enumerated types e1, e2 results in ((1 << e1) & (1 << e2))
 * 
 * @tparam Enum1 
 *    type of the first enum
 * @tparam Enum2 
 *    type of the second enum
 * @param e1 
 *    left operand
 * @param e2 
 *    right operand
 * @returns  
 *    result of ((1 << e_1) & (1 << e_2)) where e_1 and e_2 are @p e1 and @p e2 
 *    casted to underlying integral types
 * 
 * @todo When C++20 is available require @p Enum1 and @p Enum2 to be enumeration
 */
template<typename Enum1, typename Enum2, 
    std::enable_if_t<std::is_enum_v<Enum1> and std::is_enum_v<Enum2>, bool> = true
> constexpr auto operator&(Enum1 e1, Enum2 e2) {
    return (
        ( std::underlying_type_t<Enum1>(1) << common::utilities::to_underlying(e1) ) & 
        ( std::underlying_type_t<Enum2>(1) << common::utilities::to_underlying(e2) )
    );
}

/**
 * @brief Pseudo-bitwise-AND of integral constant and enumerated constants.
 * @details This operator is introduced to enable AND-chaining with use of the
 *    previous operator's overload
 * 
 * @tparam Enum1 
 *    type of the first enum
 * @tparam Enum2 
 *    type of the second enum
 * @param e1 
 *    left operand
 * @param e2 
 *    right operand
 * @returns  
 *    result of (i & (1 << e_)) where e_ is @p e casted to underlying integral type
 * 
 * @todo When C++20 is available require @p Int to be integral and @p Enum to be enumeration
 */
template<typename Int, typename Enum, 
    std::enable_if_t<std::is_integral_v<Int> and std::is_enum_v<Enum>, bool> = true
> constexpr auto operator&(Int i, Enum e) {
    return (i & ( std::underlying_type_t<Enum>(1) << common::utilities::to_underlying(e) ));
}

/* ============================================================ Macros ============================================================ */

/**
 * @brief Helper macor defining unary member operator of the class derived from the  
 *    derivation of std::bitset
 */
#define ETHERCAT_UTILITIES_WRAP_BITSET_UNARY_MEMBER_OPERATOR(op, baseclass, targetclass) \
    constexpr targetclass& operator op( ) noexcept {                  \
        baseclass::operator op(); return *this; }

/**
 * @brief Helper macor defining binary member operator of the class derived from the  
 *    derivation of std::bitset
 */
#define ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_MEMBER_OPERATOR(op, baseclass, targetclass)      \
    constexpr targetclass& operator BOOST_PP_CAT(op, =)( const targetclass& other ) noexcept { \
        baseclass::operator BOOST_PP_CAT(op, =)(other); return *this; }                        \
    constexpr targetclass& operator BOOST_PP_CAT(op, =)( const baseclass& other ) noexcept {   \
        baseclass::operator BOOST_PP_CAT(op, =)(other); return *this; }
    
/**
 * @brief Helper macor defining all basic member operators of the class derived from the  
 *    @p baseclass derivation of std::bitset
 */
#define ETHERCAT_UTILITIES_WRAP_BITSET_MEMBER_OPERATORS(baseclass, targetclass)      \
    ETHERCAT_UTILITIES_WRAP_BITSET_UNARY_MEMBER_OPERATOR(~,  baseclass, targetclass) \
    ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_MEMBER_OPERATOR(|, baseclass, targetclass) \
    ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_MEMBER_OPERATOR(&, baseclass, targetclass) \
    ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_MEMBER_OPERATOR(^, baseclass, targetclass)
    
/**
 * @brief Helper macor defining binary operator of the class derived from the  
 *    derivation of std::bitset
 */
#define ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(op, baseclass, targetclass)                           \
    constexpr targetclass operator op(const targetclass &lset, const targetclass &rset) { \
        return targetclass{ operator op(                                                  \
            static_cast<baseclass>(lset),                                                 \
            static_cast<baseclass>(rset)                                                  \
        ) };                                                                              \
    }
    
/**
 * @brief Helper macor defining all basic non-member binary operators of the class derived from
 *    the @p baseclass derivation of std::bitset
 */
#define ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATORS(baseclass, targetclass) \
    ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(|, baseclass, targetclass)   \
    ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(&, baseclass, targetclass)   \
    ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(^, baseclass, targetclass)
    
/**
 * @brief Helper macor defining all basic non-member binary operators of the class derived from
 *    the @p baseclass derivation of std::bitset
 */
#define ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATORS_TEMPLATE(header, baseclass, targetclass) \
    header ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(|, baseclass, targetclass);            \
    header ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(&, baseclass, targetclass);            \
    header ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATOR(^, baseclass, targetclass)

/* ========================================================= Named bitset ========================================================= */

/**
 * @brief Class template extending std::bitset with capability to be indexed with enum class
 *    values
 * 
 * @tparam N 
 *    size of the bitset 
 */
template<std::size_t N>
struct named_bitset : public std::bitset<N> {

public: /* ----------------------------------------------------- Public types ---------------------------------------------------- */

    /// Define 'base' type
    using base_type = std::bitset<N>;
    /// Define 'self' type
    using bitset_type = named_bitset<N>;

    /// Copy constructor from std::bitset
    constexpr named_bitset(const std::bitset<N> &set) : std::bitset<N>{ set } {  }

public: /* ----------------------------------------------------- Public ctors ---------------------------------------------------- */

    /// Forward all constructors from the std::bitset
    using std::bitset<N>::bitset;

    /**
     * @brief Construct bitset from enumerated constant treating it as index of the bit to be set
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param e 
     *    enumerated constant
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr named_bitset(Enum e) : 
        std::bitset<N>{ std::underlying_type_t<Enum>(1) << common::utilities::to_underlying(e) }
    { }

    // Forward basic constructors
    using std::bitset<N>::bitset;

public: /* --------------------------------------------------- Public operators -------------------------------------------------- */

    /// Wrap basic member operator of the bitset
    ETHERCAT_UTILITIES_WRAP_BITSET_MEMBER_OPERATORS(std::bitset<N>, named_bitset);

    /**
     * @brief Accesses reference to the bit of the bitset indexed by the underlying value 
     *    of the enumerated value
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param field 
     *    identifier of the field to be accessed
     * @returns 
     *    reference proxy to the bit
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr typename std::bitset<N>::reference operator[](Enum field) { 
        return std::bitset<N>::operator[](common::utilities::to_underlying(field)); 
    }

    /**
     * @brief Obtains value to the bit of the bitset indexed by the underlying value 
     *    of the enumerated value
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param field 
     *    identifier of the field to be obtained
     * @returns 
     *    value of the bit
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr bool operator[](Enum field) const { 
        return std::bitset<N>::operator[](common::utilities::to_underlying(field)); 
    }

    // Forward reference operators
    using std::bitset<N>::operator[];

public: /* ---------------------------------------------------- Public methods --------------------------------------------------- */

    /**
     * @brief Tests whether bit of the bitset is set. Bit's index given as enumeration
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param field 
     *    identifier of the field to be accessed
     * 
     * @retval true
     *    if requested bit is set
     * @retval false
     *    otherwise
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr bool test(Enum field) const { 
        return std::bitset<N>::test(common::utilities::to_underlying(field)); 
    }

    // Forward test method
    using std::bitset<N>::test;

    /**
     * @brief Sets bit of the bitset is set. Bit's index given as enumeration
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param field 
     *    identifier of the field to be accessed
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr void set(Enum field, bool value = true) { 
        std::bitset<N>::set(common::utilities::to_underlying(field), value); 
    }

    // Forward set method
    using std::bitset<N>::set;

    /**
     * @brief Resets bit of the bitset is set. Bit's index given as enumeration
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param field 
     *    identifier of the field to be accessed
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr void reset(Enum field) { 
        std::bitset<N>::reset(common::utilities::to_underlying(field)); 
    }

    // Forward reset method
    using std::bitset<N>::reset;

    /**
     * @brief Flips bit of the bitset is set. Bit's index given as enumeration
     * 
     * @tparam Enum 
     *    enum type that the class should be idnexed with
     * @param field 
     *    identifier of the field to be accessed
     * 
     * @todo When C++20 is available require @p Enum to be enumeration
     */
    template<typename Enum, 
        std::enable_if_t<std::is_enum_v<Enum>, bool> = true
    > constexpr void flip(Enum field) { 
        std::bitset<N>::flip(common::utilities::to_underlying(field)); 
    }

    // Forward flip method
    using std::bitset<N>::flip;

public: /* ---------------------------------------------------- Public methods --------------------------------------------------- */

    /**
     * @brief Converts bitset to the integral constant
     * 
     * @tparam T 
     *    target ty[e]
     * @returns 
     *    bitset converted to integral constant
     * 
     * @todo When C++20 is available require @p T to be integral
     */
    template<typename T, 
        std::enable_if_t<std::is_integral_v<T>, bool> = true
    > constexpr T to_value() const {

        T t { 0 };

        // Construct output by hand as std::bitset::to_ulong is not constexpr yet
        for(std::size_t i = 0; i < sizeof(T) * common::utilities::bit::BITS_IN_BYTE; ++i)
            t |= (T(std::bitset<N>::operator[](i)) << i);

        return t;
    }

    template<typename T, 
        std::enable_if_t<std::is_integral_v<T>, bool> = true
    > static constexpr named_bitset<N> from_value(T val) {
        return named_bitset<N>{ std::bitset<N>(static_cast<unsigned long long>(val)) };
    }

    /**
     * @brief Casts *this to std::bitset
     */
    constexpr std::bitset<N> &to_bitset() {
        return static_cast<std::bitset<N> &>(*this);
    }

    /**
     * @brief Casts *this to std::bitset
     */
    constexpr const std::bitset<N> &to_bitset() const {
        return static_cast<const std::bitset<N> &>(*this);
    }
    
};

/// Wrap binary operators of the bitset
ETHERCAT_UTILITIES_WRAP_BITSET_BINARY_OPERATORS_TEMPLATE(template<std::size_t N>, std::bitset<N>, named_bitset<N>);

/* ================================================================================================================================ */

} // End namespace ethercat::utilities

/* ================================================================================================================================ */

#endif
