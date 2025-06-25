/* ============================================================================================================================ *//**
 * @file       default_parser_test.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2021 1:54:23 am
 * @modified   Wednesday, 25th June 2025 3:19:41 pm
 * @project    ethercat-lib
 * @brief      Unit tests for the default parser of binary data
 *
 *
 * @copyright Krzysztof Pierczyk © 2022
 * @todo Condition tests depending on the current configuration of the library
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// System includes
#include <array>
#include <iostream>
// Tetsing includes
#include "gtest/gtest.h"
// Private includes
#include "ethercat/common/translation/default_translator.hpp"

/* ========================================================== Namespaces ========================================================== */

using namespace ethercat::common::utilities::traits;
using namespace ethercat::common::translation;

/* ======================================================= Common functions ======================================================= */

/// Log start string
constexpr auto log_start = "[    LOG   ] ";

/// Auxiliary logger class
struct Log {
    template<typename T>
    std::ostream &operator<<(T &&v) { return (std::cout << log_start << v); }
};

/// Auxiliary logger instance
Log logger;

// Stringifies an array
template<typename T, std::size_t N>
auto to_str(const std::array<T, N> &array) {
    std::stringstream ss;
    ss << "{ ";
    if constexpr(std::is_same_v<T, std::string>) {
        for(std::size_t i = 0; i < array.size() - 1; ++i)
            ss << array[i] << ", ";
        ss << array[array.size() - 1] << " }";
    } else {
        for(std::size_t i = 0; i < array.size() - 1; ++i)
            ss << "0x" << std::hex << int(array[i]) << ", ";
        ss << "0x" << std::hex << int(array[array.size() - 1]) << " }";
    }
    return ss.str();
}

// Stringifies a vector
template<typename T>
auto to_str(const std::vector<T> &array) {
    std::stringstream ss;
    ss << "{ ";
    if constexpr(std::is_same_v<T, std::string>) {
        for(std::size_t i = 0; i < array.size() - 1; ++i)
            ss << array[i] << ", ";
        ss << array[array.size() - 1] << " }";
    } else {
        for(std::size_t i = 0; i < array.size() - 1; ++i)
            ss << "0x" << std::hex << int(array[i]) << ", ";
        ss << "0x" << std::hex << int(array[array.size() - 1]) << " }";
    }
    return ss.str();
}

// Stringifies an object
template<typename T>
auto to_str(const T &obj) {
    std::stringstream ss;
    ss << "{ ";
    if constexpr(std::is_same_v<T, std::string>) {
        ss << obj << " }";
    } else {
        ss << "0x" << std::hex << int(obj) << " }";
    }
    return ss.str();
}

/* ======================================================= Common functions ======================================================= */

template<typename DataT, typename ObjT>
inline void translate_to(DataT &&data, ObjT &&obj, std::size_t offset = 0) {
    DefaultTranslatorOfQualified<ObjT>::translate_to(std::forward<DataT>(data), std::forward<ObjT>(obj), offset);
}

template<typename DataT, typename ObjT>
inline void translate_from(DataT &&data, ObjT &&obj, std::size_t offset = 0) {
    DefaultTranslatorOfQualified<ObjT>::translate_from(std::forward<DataT>(data), std::forward<ObjT>(obj), offset);
}

/* ========================================================= Common macros ======================================================== */

#define IDENTITY(...) (__VA_ARGS__)
#define INPUT(...)     __VA_ARGS__
#define OUTPUT(...)    __VA_ARGS__
#define OFFSET(...)    __VA_ARGS__
#define EXPECTED(...)  __VA_ARGS__

#define ASSERT_NOTHROW_WITH_MESSAGE(...)                                          \
    ASSERT_NO_THROW(                                                              \
    try                       { __VA_ARGS__; }                                    \
    catch(std::exception &ex) {                                                   \
        std::cout << log_start << "Exception thrown: " << ex.what() << std::endl; \
        throw ex;                                                                 \
    })

#define TEST_INPUT(data, obj, expected)                            \
    {                                                              \
        std::array<uint8_t, std::size(data)> buffer data;          \
        remove_cvref_t<decltype(obj)> object{ obj };               \
        ASSERT_NOTHROW_WITH_MESSAGE(translate_to(buffer, object)); \
        ASSERT_TRUE(object == IDENTITY(expected))                  \
        << "Object: " << to_str(object) << std::endl;              \
    }

#define TEST_INPUT_WITH_OFFSET(data, obj, offset, expected)                             \
    {                                                                                   \
        std::array<uint8_t, std::size(data)> buffer data;                               \
        remove_cvref_t<decltype(obj)> object{ obj };                                    \
        ASSERT_NOTHROW_WITH_MESSAGE(translate_to(buffer, object, std::size_t{offset})); \
        ASSERT_TRUE(object == IDENTITY(expected))                                       \
        << "Object: " << to_str(object) << std::endl;                                   \
    }

#define TEST_OUTPUT(obj, data, expected)                                                    \
    {                                                                                       \
        std::array<uint8_t, std::size(data)> buffer data;                                   \
        ASSERT_NOTHROW_WITH_MESSAGE(translate_from(buffer, obj));                           \
        ASSERT_TRUE(buffer == IDENTITY(std::array<uint8_t, std::size(expected)>{expected})) \
        << "Buffer: " << to_str(buffer) << std::endl;                                       \
    }

#define TEST_OUTPUT_WITH_OFFSET(obj, data, offset, expected)                                 \
    {                                                                                        \
        std::array<uint8_t, std::size(data)> buffer data;                                    \
        ASSERT_NOTHROW_WITH_MESSAGE(translate_from(buffer, obj, std::size_t{offset}));       \
        ASSERT_TRUE(buffer == IDENTITY(std::array<uint8_t, std::size(expected)>{expected}))  \
        << "Buffer: " << to_str(buffer) << std::endl;                                        \
    }

/* =========================================================== Literals =========================================================== */

bool operator""_b(unsigned long long value) { return static_cast<bool>(value); }

/* =========================================================== Booleans =========================================================== */

TEST(DefaultParserTest, Bool) {

    // Translate [IN]
    TEST_INPUT(
        INPUT   (    { 0x1   }),
        OUTPUT  (bool{ false }),
        EXPECTED(bool{ true  })
    )

    // Translate with offset [IN]
    TEST_INPUT_WITH_OFFSET(
        INPUT   (            { 0x2, 0x0 }),
        OUTPUT  (bool        { false    }),
        OFFSET  (std::size_t { 1        }),
        EXPECTED(bool        { 0x1      })
    )

    // Translate [OUT]
    TEST_OUTPUT(
        INPUT   (bool{ true }),
        OUTPUT  (    { 0x0  }),
        EXPECTED(    { 0x1u })
    )

    // Translate with offset [OUT]
    TEST_OUTPUT_WITH_OFFSET(
        INPUT   (bool        { true     }),
        OUTPUT  (            { 0x0, 0x0 }),
        OFFSET  (std::size_t { 1        }),
        EXPECTED(            { 0x2, 0x0 })
    )
}

TEST(DefaultParserTest, BoolArray) {

    // Translate [IN]
    TEST_INPUT(
        INPUT   (          { 0x1, 0x0, 0x0, 0x1         }),
        OUTPUT  (std::array{ false, false, false, false }),
        EXPECTED(std::array{ 0x1_b, 0x0_b, 0x0_b, 0x1_b })
    )

    // Translate with offset [IN]
    TEST_INPUT_WITH_OFFSET(
        INPUT   (            { 0x8, 0x0, 0x0, 0x8, 0x0    }),
        OUTPUT  (std::array  { 0x0_b, 0x0_b, 0x0_b, 0x0_b }),
        OFFSET  (std::size_t { 3                          }),
        EXPECTED(std::array  { 0x1_b, 0x0_b, 0x0_b, 0x1_b })
    )

    // Translate [OUT]
    TEST_OUTPUT(
        INPUT   (std::array{ true, false, true, false }),
        OUTPUT  (          { 0x1, 0x0, 0x0, 0x1       }),
        EXPECTED(          { 0x1, 0x0, 0x1, 0x0       })
    )

    // Translate with offset [OUT]
    TEST_OUTPUT_WITH_OFFSET(
        INPUT   (std::array  { true, false, true, false         }),
        OUTPUT  (            { 0x0,   0x0,   0x0,    0x0,   0x0 }),
        OFFSET  (std::size_t { 3                                }),
        EXPECTED(            { 0x8,   0x0,   0x8,    0x0,   0x0 })
    )

}

TEST(DefaultParserTest, BoolVector) {

    // Translate [IN]
    TEST_INPUT(
        INPUT   (           { 0x1, 0x0, 0x0, 0x1         }),
        OUTPUT  (std::vector{ false, false, false, false }),
        EXPECTED(std::vector{ 0x1_b, 0x0_b, 0x0_b, 0x1_b })
    )

    std::array<uint8_t, 5> data     { 0x8,   0x0,   0x0,   0x8, 0x0 };
    std::vector            object   { 0x0_b, 0x0_b, 0x0_b, 0x0_b    };
    std::vector            expected { 0x0_b, 0x0_b, 0x0_b, 0x0_b    };
    translate_to                    (data, object, 3);

    // Translate with offset [IN]
    TEST_INPUT_WITH_OFFSET(
        // INPUT   (             { 0x8,   0x0,   0x0,   0x8, 0x0 }),
        INPUT   (             { 0x8,   0x0,   0x0,   0x8, 0x0 }),
        OUTPUT  (std::vector  { 0x0_b, 0x0_b, 0x0_b, 0x0_b    }),
        OFFSET  (std::size_t  { 3                             }),
        EXPECTED(std::vector  { 0x1_b, 0x0_b, 0x0_b, 0x1_b    })
    )

    // Translate [OUT]
    TEST_OUTPUT(
        INPUT   (std::vector{ true, false, true, false }),
        OUTPUT  (           { 0x1, 0x0, 0x0, 0x1       }),
        EXPECTED(           { 0x1, 0x0, 0x1, 0x0       })
    )

    // Translate with offset [OUT]
    TEST_OUTPUT_WITH_OFFSET(
        INPUT   (std::vector { true, false, true, false         }),
        OUTPUT  (            { 0x0,   0x0,   0x0,    0x0,   0x0 }),
        OFFSET  (std::size_t { 3                                }),
        EXPECTED(            { 0x8,   0x0,   0x8,    0x0,   0x0 })
    )

}

/* ================================================================================================================================ */