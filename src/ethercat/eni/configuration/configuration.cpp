/* ============================================================================================================================ *//**
 * @file       configuration.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 3:33:15 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of methods of the Configuration class implementing parsing interface of the <Configuration> tag of 
 *             the ENI (EtherCAT Network Informations) tag
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/eni/configuration.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================ Helpers =========================================================== */

namespace details {
    
    /**
     * @brief Helper STL-like algorithm checking whether range whose elements has been transformed with 
     *    @p transformation stores only unique values (i.e. is a set) and returning iterator to the first
     *    non-unique element, if found
     * 
     * @tparam InputIt 
     *    type of range's iterators
     * @tparam TransformationT 
     *    type of the @p transformation
     * @param begin 
     *    begin of the range
     * @param end
     *    end of the range
     * @param transformation
     *    target transformation
     * 
     * @retval it 
     *    iterator to the first non-unique element, if found
     * @retval end 
     *    if no non-unique element found
     */
    template<typename InputIt, typename TransformationT>
    static constexpr InputIt find_first_non_unique_transformed(InputIt begin, InputIt end, TransformationT transformation) {

        // Iterate over range
        for(auto it = begin; it != end; ++it) {
            
            // Transform current element
            auto it_transformed = transformation(*it);
            // Iterate over left elements (@note copy of it is made to limti requirements on InputIt to being a forward iterator)
            for(auto it_copy = it, rit = ++it_copy; rit != end; ++rit) {
                // If transformations are equal, return element
                if(it_transformed == transformation(*rit))
                    return it;
            }
        }
        
        // If no equality found, return end
        return end;
    }

}

/* ======================================================== Public methods ======================================================== */

std::optional<Slave> Configuration::get_slave(std::string_view name) const {
    
    using namespace std::literals::string_literals;

    // Iterate over configruation nodes
    for (const auto &elem : *this) {

        // Check if <Slave> tag retrived
        if (elem.first != "Slave")
            continue;

        // Check whether slave found
        if (elem.second.get_child_value<std::string>("Info.Name") == name)
            return static_cast<Slave>(elem.second);
            
    }

    // If slave not found, return empty optional
    return std::optional<Slave>{};
}


std::vector<Slave> Configuration::get_slaves() const {

    // Preapre return vector
    std::vector<Slave> ret;

    // Iterate over configruation nodes
    for (const auto &elem : *this) {

        // Check if <Slave> tag retrived
        if (elem.first != "Slave")
            continue;

        // Add slave's description to the output
        ret.push_back(elem.second);
    }

    return ret;
}


std::vector<std::string> Configuration::list_slaves() const {

    using namespace std::literals::string_literals;

    // Preapre return vector
    std::vector<std::string> ret;

    // Iterate over configruation nodes
    for (const auto &elem : *this) {

        // Check if <Slave> tag retrived
        if (elem.first != "Slave")
            continue;

        // Parse slave's name
        auto name = elem.second.get_child_value<std::string>("Info.Name");
        //  Add slave's name to the result list
        ret.push_back(name);
        
    }

    return ret;
}


std::optional<Slave> Configuration::are_slaves_unique() const {

    // Get list of slaves on the bus
    auto slave_eni_list = get_slaves();

    // Check if slaves have unique names
    if(auto elem = details::find_first_non_unique_transformed(slave_eni_list.begin(), slave_eni_list.end(), 
        [](const auto &slave_config) { return slave_config.get_name(); }
    ); elem != slave_eni_list.end())
        return *elem;

    // Check if slaves have unique physical adresses
    if(auto elem = details::find_first_non_unique_transformed(slave_eni_list.begin(), slave_eni_list.end(), 
        [](const auto &slave_config) { return slave_config.get_physical_addr(); }
    ); elem != slave_eni_list.end())
        return *elem;

    // Check if slaves have unique auto-increment adresses
    if(auto elem = details::find_first_non_unique_transformed(slave_eni_list.begin(), slave_eni_list.end(), 
        [](const auto &slave_config) { return slave_config.get_auto_increment_addr(); }
    ); elem != slave_eni_list.end())
        return *elem;

    return std::optional<Slave>{ };
}

/* ================================================================================================================================ */

} // End namespace ethercat::eni
