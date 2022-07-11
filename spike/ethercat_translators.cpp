/* ============================================================================================================================ *//**
 * @file       ethercat_translators.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 1st June 2022 7:14:09 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Program testing possibility of implementing compile-time dispatched hierarchy of classes translatin
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

// Boost includes
#include <boost/mp11.hpp>
#include <iostream>
#include <cstdint>
#include <regex>
#include <bitset>
#include <variant>
#include <type_traits>
#include <range/v3/span.hpp>
#include <boost/property_tree/ptree.hpp>
#include <experimental/type_traits>

/* ============================================================= Main ============================================================= */

struct StatelessGeneric {

    template<typename T>
    static T to(const std::vector<uint8_t> &data) {
        T t;
        std::memcpy(
            reinterpret_cast<void*>(&t),
            reinterpret_cast<const void*>(&data[0]),
            sizeof(T)
        );
        std::cout << "(StatelessGeneric) ";
        return t;
    }

};

struct StatelessSpecific {

    static int to(const std::vector<uint8_t> &data) {
        int t;
        std::memcpy(
            reinterpret_cast<void*>(&t),
            reinterpret_cast<const void*>(&data[0]),
            sizeof(int)
        );
        std::cout << "(StatelessSpecific) ";
        return t;
    }

};

struct StatefullGeneric {

    template<typename T>
    T to(const std::vector<uint8_t> &data) {
        T t;
        std::memcpy(
            reinterpret_cast<void*>(&t),
            reinterpret_cast<const void*>(&data[0]),
            sizeof(T)
        );
        std::cout << "(StatefullGeneric) ";
        return t;
    }

};

struct StatefullSpecific {

    int to(const std::vector<uint8_t> &data) {
        int t;
        std::memcpy(
            reinterpret_cast<void*>(&t),
            reinterpret_cast<const void*>(&data[0]),
            sizeof(int)
        );
        std::cout << "(StatefullSpecific) ";
        return t;
    }

};

/* =========================================================== Reference ========================================================== */

template<typename TranslatorT, typename T>
struct StatelessGenericReference {

    StatelessGenericReference(const std::vector<uint8_t> &data) : data{ data } { }

    auto get()       { return TranslatorT::template to<T>(data); }
    auto get() const { return TranslatorT::template to<T>(data); }

    const std::vector<uint8_t> &data;
};

template<typename TranslatorT>
struct StatelessSpecificReference {

    StatelessSpecificReference(const std::vector<uint8_t> &data) : data{ data } { }

    auto get()       { return TranslatorT::to(data); }
    auto get() const { return TranslatorT::to(data); }

    const std::vector<uint8_t> &data;
};

template<typename TranslatorT, typename T>
struct StatefullGenericReference {

    template<typename... ArgsT>
    StatefullGenericReference(const std::vector<uint8_t> &data, ArgsT... args) : tr{ std::forward<ArgsT>(args)... }, data{ data } { }

    auto get()       { return tr.template to<T>(data); }
    auto get() const { return tr.template to<T>(data); }

    TranslatorT tr;
    const std::vector<uint8_t> &data;
};

template<typename TranslatorT>
struct StatefullSpecificReference {

    template<typename... ArgsT>
    StatefullSpecificReference(const std::vector<uint8_t> &data, ArgsT... args) : tr{ std::forward<ArgsT>(args)... }, data{ data } { }

    auto get()       { return tr.to(data); }
    auto get() const { return tr.to(data); }

    TranslatorT tr;
    const std::vector<uint8_t> &data;
};

/* =========================================================== Reference ========================================================== */

template<typename TranslatorT>
using has_stateless_specific_translation = decltype(TranslatorT::to(std::declval<std::vector<uint8_t>>()));

template<typename TranslatorT, typename T>
using has_stateless_generic_translation = decltype(TranslatorT::template to<T>(std::declval<std::vector<uint8_t>>()));

template<typename TranslatorT>
using has_statefull_specific_translation = decltype(std::declval<TranslatorT>().to(std::declval<std::vector<uint8_t>>()));

template<typename TranslatorT, typename T>
using has_statefull_generic_translation = decltype(std::declval<TranslatorT>().template to<T>(std::declval<std::vector<uint8_t>>()));

template<typename TranslatorT, typename T = void>
struct CustomReference : 
    public std::conditional_t<
        // If generic-type translator is given
        std::is_same_v<T, void>,
        // If is generic translator
        std::conditional_t<std::experimental::is_detected_v<has_stateless_specific_translation, TranslatorT>, 
            StatelessSpecificReference<TranslatorT>,
            StatefullSpecificReference<TranslatorT>>,
        // If is generic translator
        std::conditional_t<std::experimental::is_detected_v<has_stateless_generic_translation, TranslatorT>, 
            StatelessGenericReference<TranslatorT, T>,
            StatefullGenericReference<TranslatorT, T>>
    >
{ 
    static_assert(
        std::is_same_v<T, void> && std::experimental::is_detected_v<has_stateless_specific_translation, TranslatorT   > ||
        std::is_same_v<T, void> && std::experimental::is_detected_v<has_statefull_specific_translation, TranslatorT   > ||
                                   std::experimental::is_detected_v<has_stateless_generic_translation,  TranslatorT, T> ||
                                   std::experimental::is_detected_v<has_statefull_generic_translation,  TranslatorT, T>,
        "Invalid <TranslatorT> passed to the CustomReference specialization"
    );
};

/* ============================================================= Tests ============================================================ */

void test_underlying() {
    
    std::vector<uint8_t> data { 0x1, 0x0, 0x0, 0x0 };
    
    StatelessGenericReference<StatelessGeneric, int> slgr{ data };
    StatelessSpecificReference<StatelessSpecific>    slsr{ data };
    StatefullGenericReference<StatefullGeneric, int> sfgr{ data };
    StatefullSpecificReference<StatefullSpecific>    sfsr{ data };

    std::cout << "slgr: " << slgr.get() << std::endl;
    std::cout << "slsr: " << slsr.get() << std::endl;
    std::cout << "sfgr: " << sfgr.get() << std::endl;
    std::cout << "sfsr: " << sfsr.get() << std::endl;
}

void test_reference() {
    
    std::vector<uint8_t> data { 0x1, 0x0, 0x0, 0x0 };
    
    CustomReference<StatelessGeneric, int> slgr{ data };
    CustomReference<StatelessSpecific>     slsr{ data };
    CustomReference<StatefullGeneric, int> sfgr{ data };
    CustomReference<StatefullSpecific>     sfsr{ data };

    std::cout << "slgr: " << slgr.get() << std::endl;
    std::cout << "slsr: " << slsr.get() << std::endl;
    std::cout << "sfgr: " << sfgr.get() << std::endl;
    std::cout << "sfsr: " << sfsr.get() << std::endl;
}

/* ============================================================= Main ============================================================= */

int main(int argc, char const *argv[])
{
    test_underlying();
    test_reference();
}
