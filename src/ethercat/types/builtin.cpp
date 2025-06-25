/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 9th May 2022 6:02:53 pm
 * @modified   Wednesday, 25th June 2025 3:17:39 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Type type describing type of the CoE object at runtime
 *
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <sstream>
#include <regex>
#include <charconv>
// Private includes
#include "ethercat/types/builtin.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::types {

/* ============================================================ Helpers =========================================================== */

namespace details {

    static constexpr bool starts_with(std::string_view str, std::string_view key) {
        return (str.size() >= key.size()) and (str.substr(0, key.size()) == key);
    }

}

/* ========================================================== BuiltinType ========================================================= */

std::optional<BuiltinType> BuiltinType::parse(std::string_view type_string) {

    /// Helper function parsing base type identifier
    auto parse_base = [](std::string_view type_string) -> std::optional<BuiltinType> {

        // Iterate over possible types
        for(std::size_t i = 0; i < TYPES_NUM; ++i) {

            // Get possible names of the type
            auto possible_name = common::types::traits::coe_names(common::utilities::to_enum<ID>(i));

            // If non-string type is matched, check if whole string matches
            if(i != common::utilities::to_underlying(ID::String)) {

                // Iterate over possible types
                for(const auto &name : possible_name) {
                    // If name matches, return
                    if(type_string == name) {
                        return BuiltinType { NumericType{ common::utilities::to_enum<NumericType::ID>(i) } };
                    }
                }

            // If non-string type is matched, check if whole string matches
            } else {

                // Iterate over possible types
                for(std::string_view name : possible_name)
                    // If name matches, return
                    if(details::starts_with(type_string, name)) {

                        // Get size substring
                        auto size_spec_string = type_string.substr(name.size());

                        // Check if size string has ([0-9]+) format
                        if(size_spec_string.front() != '(' or size_spec_string.back() != ')')
                            return std::optional<BuiltinType>{ };

                        std::size_t size { };

                        // Parse number substring
                        auto size_string = size_spec_string.substr(1, size_spec_string.size() - 1);

                        // If so, try to parse size
                        try { std::from_chars(
                            size_string.data(), size_string.data() + size_string.size(), size
                        ); }
                        /// On error, return
                        catch(...) { return std::optional<BuiltinType>{ }; }

                        // On success, return specifier
                        return BuiltinType { StringType { size } };

                    }
            }
        };

        // If no match found, return empty
        return std::optional<BuiltinType>{ };
    };

    // Cast input to std::string, as std::regex does not support std::string_view
    std::string type_str{ type_string };

    // Regex pattern that matches standard name of the array of base types
    const std::regex array_pattern("^ARRAY \\[[0-9]+\\.\\.([0-9])+\\] (of|OF) ([0-9a-zA-Z\\(\\)]+)$", std::regex_constants::ECMAScript);
    // Amount of keys that would be matched in the array string
    constexpr std::size_t ARRAY_PATTERN_MATCH_KEYS = 3;

    std::smatch match;

    // Check if content matches array format (pattern matches and subelement is found)
    if(auto matches = std::regex_match(type_str, match, array_pattern); matches && match.size() == ARRAY_PATTERN_MATCH_KEYS + 1) {

        std::size_t arity;

        /// Get type's arity
        try { arity = static_cast<std::size_t>(std::stoi(match[1])); }
        /// On error, return
        catch(...) { return std::optional<BuiltinType>{ }; }

        // Parse type
        auto base = parse_base(match[3].str());
        // If parsing failed, return empty
        if(not base.has_value())
            return std::optional<BuiltinType>{ };

        // Else, add arity to the type descriptor
        base->arity = arity;
        // Return result
        return *base;

    }

    // Otherwise, try to parse scalar type
    auto base = parse_base(type_string);
    // If parsing failed, return empty
    if(not base.has_value())
        return std::optional<BuiltinType>{ };

    // Else, return result
    return *base;
}

/* ================================================================================================================================ */

} // End namespace ethercat::types
