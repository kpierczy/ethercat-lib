/* ============================================================================================================================ *//**
 * @file       translator_wrapper.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 1st June 2022 7:14:09 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Spike programm testing functionality of TranslatorWrapper class template
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#include <iostream>
#include <map>
#include "ethercat/common/translation.hpp"

/* ========================================================== Namespaces ========================================================== */

using namespace ethercat::common::translation;
using namespace ethercat::config::types;

/* ========================================================== Translator ========================================================== */

struct Info {
    uint8_t id;
    int32_t value;
    // std::map<std::string, std::string> mp; // Uncomment to change object-passing-type from 'by value' to 'by reference' to reference
};

struct InfoD : public Info { };

struct Translator {
    
    using Type = InfoD;
    
    // Input translation
    template<typename T, std::enable_if_t<not std::is_same_v<T, InfoD>, bool> = true>
    inline void translate_to(Span<const uint8_t> data, T &obj, std::size_t offset) {
        std::copy_n(&data[0], 1, reinterpret_cast<uint8_t*>(&obj.id));
        std::copy_n(&data[1], 4, reinterpret_cast<uint8_t*>(&obj.value));
    }
    
    // Input translation (non-exact match but still choosen by the wrapper)
    inline void translate_to(Span<const uint8_t> data, Info &obj, std::size_t offset) {
        std::copy_n(&data[0], 1, reinterpret_cast<uint8_t*>(&obj.id));
        std::copy_n(&data[1], 4, reinterpret_cast<uint8_t*>(&obj.value));
    }
    
    // Output translation
    static inline void translate_from(Span<uint8_t> data, const InfoD &obj) {
        std::copy_n(reinterpret_cast<const uint8_t*>(&obj.id),    1, &data[0]);
        std::copy_n(reinterpret_cast<const uint8_t*>(&obj.value), 4, &data[1]);
    }
    
};

using Traits = ethercat::common::translation::TranslatorTraits<TranslationDirection::Bidirectional, Translator>;
using Wrapped = TranslatorWrapper<TranslationDirection::Bidirectional, Translator>;

/* ============================================================= Main ============================================================= */

int main(int argc, char const *argv[])
{
    std::cout << "===================================================== "       << std::endl;
    std::cout << "is_input_stateless:      " << Traits::is_input_stateless      << std::endl;
    std::cout << "is_input_statefull:      " << Traits::is_input_statefull      << std::endl;
    std::cout << "is_input_specialized:    " << Traits::is_input_specialized    << std::endl;
    std::cout << "is_input_generic:        " << Traits::is_input_generic        << std::endl;
    std::cout << "handles_input_bitoffset: " << Traits::handles_input_bitoffset << std::endl;

    std::cout << "is_output_stateless:      " << Traits::is_output_stateless      << std::endl;
    std::cout << "is_output_statefull:      " << Traits::is_output_statefull      << std::endl;
    std::cout << "is_output_specialized:    " << Traits::is_output_specialized    << std::endl;
    std::cout << "is_output_generic:        " << Traits::is_output_generic        << std::endl;
    std::cout << "handles_output_bitoffset: " << Traits::handles_output_bitoffset << std::endl;

    std::cout << "===================================================== "    << std::endl;

    std::cout << "TranslatorType: " << typeid(Traits::TranslatorType).name() << std::endl;
    std::cout << "Type:           " << typeid(Traits::Type).name()           << std::endl;
    std::cout << "ArgType:        " << typeid(Traits::ArgType).name()        << std::endl;
    std::cout << " - reference:   " << std::is_reference_v<Traits::ArgType>  << std::endl;
    
    std::cout << "===================================================== "    << std::endl;

    Wrapped wrapped;
    std::array<uint8_t, 5> data{ 0x5, 0x0, 0x1, 0x0, 0x0 };
    InfoD obj;

    wrapped.translate_to(data, obj, 0);
    std::cout << "Statefull input:  { .id = " << int(obj.id) << ", .value = " << obj.value << " }" << std::endl;
    
    obj.id    = 11;
    obj.value = -2;

    Wrapped::translate_from(data, obj);
    std::cout << "Stateless output: { .data = [";
    std::cout << int(data[0]) << ", ";
    std::cout << int(data[1]) << ", ";
    std::cout << int(data[2]) << ", ";
    std::cout << int(data[3]) << ", ";
    std::cout << int(data[4])        ;
    std::cout << "]"          << std::endl;
    
}
