/* ============================================================================================================================ *//**
 * @file       eni_parsing_test.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 18th May 2021 1:54:23 am
 * @modified   Monday, 11th July 2022 3:59:07 pm
 * @project    ethercat-lib
 * @brief      Unit tests for the ENI (EtherCAT Network Informations) files parser.
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

// System includes
#include <memory>
#include <filesystem>
// Tetsing includes
#include "gtest/gtest.h"
// Private includes
#include "ethercat/eni.hpp"

/* ======================================================= Common functions ======================================================= */

/// Log start string
constexpr auto log_start = "[    LOG   ] ";

/* =========================================================== Fixtures =========================================================== */

class CifxEthercatENIParserTest : public testing::Test {
protected:

    // Skip setup of common resources
    static void SetUpTestSuite() { }
    // Skip teardown of common resources
    static void TearDownTestSuite() { }

    // Path to the exmpla ENI file
    static std::filesystem::path eni_path;

    // Parse ENI Configuration
    static std::unique_ptr<ethercat::eni::Configuration> eni_config;

};

// Initiali common configuration data
std::filesystem::path CifxEthercatENIParserTest::eni_path { std::filesystem::path{ ETHERCAT_LIB_TEST_ENI_PATH } };
// Initiali common data
std::unique_ptr<ethercat::eni::Configuration> CifxEthercatENIParserTest::eni_config{ };

/* ==================================================== Toolkit initialization ==================================================== */

TEST_F(CifxEthercatENIParserTest, XMLParsing) {

    // Print ENI path
    std::cout << "[    LOG   ] ENI file used: " << eni_path << std::endl;

    // Parse ENI file
    try { eni_config = std::make_unique<ethercat::eni::Configuration>( ethercat::eni::configruation_from_file(eni_path) ); }
    // If thrown, report
    catch(std::exception &e) { FAIL() << e.what(); }
    
}


TEST_F(CifxEthercatENIParserTest, MasterParsing) {

    std::optional<ethercat::eni::Master> master;

    // Get <Master> tag parser
    try { master = eni_config->get_master(); }
    // Fail on error
    catch(std::exception &e) { FAIL() << e.what(); }

    // Validate master's name
    ASSERT_EQ(master->get_name(), "Master");

}


TEST_F(CifxEthercatENIParserTest, SlavesListing) {

    std::optional<ethercat::eni::Slave> imu_slave;
    std::optional<ethercat::eni::Slave> wheel_rl_slave;
    std::optional<ethercat::eni::Slave> wheel_rr_slave;
    std::optional<ethercat::eni::Slave> wheel_fl_slave;
    std::optional<ethercat::eni::Slave> wheel_fr_slave;

    // Get <Slave> tags parsers
    try { 
        
        imu_slave      = eni_config->get_slave("Imu");
        wheel_rl_slave = eni_config->get_slave("WheelRearLeft");
        wheel_rr_slave = eni_config->get_slave("WheelRearRight");
        wheel_fl_slave = eni_config->get_slave("WheelFrontLeft");
        wheel_fr_slave = eni_config->get_slave("WheelFrontRight");

    // Fail on error
    } catch(std::exception &e) { FAIL() << e.what(); }

    // Check if all slaves has been parsed
    ASSERT_TRUE(imu_slave.has_value());
    ASSERT_TRUE(wheel_rl_slave.has_value());
    ASSERT_TRUE(wheel_rr_slave.has_value());
    ASSERT_TRUE(wheel_fl_slave.has_value());
    ASSERT_TRUE(wheel_fr_slave.has_value());
    
    // List all slaves
    auto slaves = eni_config->get_slaves();

    auto slave_in_list = [&slaves](std::string_view name) {
        return std::find_if(slaves.begin(), slaves.end(),
            [&name](ethercat::eni::Slave &s){ return s.get_name() == name; }
        ) != slaves.end();
    };

    // Check if all slaves are listed by the configuration
    ASSERT_EQ  (slaves.size(), 5UL              );
    ASSERT_TRUE(slave_in_list("Imu")            );
    ASSERT_TRUE(slave_in_list("WheelRearLeft")  );
    ASSERT_TRUE(slave_in_list("WheelRearRight") );
    ASSERT_TRUE(slave_in_list("WheelFrontLeft") );
    ASSERT_TRUE(slave_in_list("WheelFrontRight"));

}


TEST_F(CifxEthercatENIParserTest, SlaveParsing) {

    // Get the slave
    auto slave = eni_config->get_slave("WheelRearLeft");
    // Check if name matches
    ASSERT_EQ(slave->get_name(), "WheelRearLeft");
    // Check if addresses match
    ASSERT_EQ(slave->get_physical_addr(),       1002U );
    ASSERT_EQ(slave->get_auto_increment_addr(), 65535U);

    auto pdos = slave->get_pdos();
    // Check if PDOs are properly parsed
    ASSERT_EQ(pdos.inputs.size(),  35U);
    ASSERT_EQ(pdos.outputs.size(), 31U);

    auto assigned_pdos = slave->get_assigned_pdos();
    // Check if PDOs are properly parsed
    ASSERT_EQ(assigned_pdos.inputs.size(),  1U);
    ASSERT_EQ(assigned_pdos.outputs.size(), 1U);

}


TEST_F(CifxEthercatENIParserTest, SlavePdoParsing) {

    // Get assigned PDO
    auto unassigned_pdo = eni_config->get_slave("WheelRearLeft")->get_pdos().outputs[0];
    auto assigned_pdo   = eni_config->get_slave("WheelRearLeft")->get_pdos().inputs[3];

    // Assert valid direction
    ASSERT_EQ(unassigned_pdo.get_direction(), ethercat::eni::Slave::Pdo::Direction::Outputs);
    ASSERT_EQ(assigned_pdo.get_direction(),   ethercat::eni::Slave::Pdo::Direction::Inputs);
    // Assert valid asignment
    ASSERT_TRUE(not unassigned_pdo.get_sync_manager().has_value());
    ASSERT_TRUE(assigned_pdo.get_sync_manager().has_value());
    ASSERT_EQ  (*assigned_pdo.get_sync_manager(), 3U);
    // Assert asignment
    ASSERT_FALSE(unassigned_pdo.is_assigned());
    ASSERT_TRUE (assigned_pdo.is_assigned());
    // Assert 'Fixed' configruation
    ASSERT_TRUE(unassigned_pdo.is_fixed());
    ASSERT_TRUE (assigned_pdo.is_fixed());
    // Assert valid indeces
    ASSERT_EQ(unassigned_pdo.get_index(), 0x1600U);
    ASSERT_EQ(assigned_pdo.get_index(),   0x1a03U);
    // Assert valid names
    ASSERT_EQ(unassigned_pdo.get_name(), "Outputs");
    ASSERT_EQ(assigned_pdo.get_name(),   "Inputs");

    // Assert valid excludes
    ASSERT_EQ(unassigned_pdo.get_excludes().size(), 6U);
    ASSERT_EQ(unassigned_pdo.get_excludes()[0], 0x1601U);
    ASSERT_EQ(unassigned_pdo.get_excludes()[1], 0x1602U);
    ASSERT_EQ(unassigned_pdo.get_excludes()[2], 0x1603U);
    ASSERT_EQ(unassigned_pdo.get_excludes()[3], 0x1604U);
    ASSERT_EQ(unassigned_pdo.get_excludes()[4], 0x1605U);
    ASSERT_EQ(unassigned_pdo.get_excludes()[5], 0x1606U);
    ASSERT_EQ(assigned_pdo.get_excludes().size(),   4U);
    ASSERT_EQ(assigned_pdo.get_excludes()[0], 0x1a00U);
    ASSERT_EQ(assigned_pdo.get_excludes()[1], 0x1a01U);
    ASSERT_EQ(assigned_pdo.get_excludes()[2], 0x1a02U);
    ASSERT_EQ(assigned_pdo.get_excludes()[3], 0x1a04U);
    
    // Assert valid entries
    ASSERT_EQ(unassigned_pdo.get_entries().size(), 3U);
    ASSERT_EQ(unassigned_pdo.get_entries()[0].get_index(), 0x607aU);
    ASSERT_EQ(unassigned_pdo.get_entries()[1].get_index(), 0x60feU);
    ASSERT_EQ(unassigned_pdo.get_entries()[2].get_index(), 0x6040U);
    ASSERT_EQ(assigned_pdo.get_entries().size(),   4U);
    ASSERT_EQ(assigned_pdo.get_entries()[0].get_index(), 0x6064U);
    ASSERT_EQ(assigned_pdo.get_entries()[1].get_index(), 0x60fdU);
    ASSERT_EQ(assigned_pdo.get_entries()[2].get_index(), 0x606cU);
    ASSERT_EQ(assigned_pdo.get_entries()[3].get_index(), 0x6041U);

}


TEST_F(CifxEthercatENIParserTest, SlavePdoEntryParsing) {

    // Get assigned PDOs' entries
    auto unassigned_pdo_entry = eni_config->get_slave("WheelRearLeft")->get_pdos().outputs[0].get_entries()[0];
    auto assigned_pdo_entry   = eni_config->get_slave("WheelRearLeft")->get_pdos().inputs[3].get_entries()[3];

    // Assert valid indeces
    ASSERT_EQ(unassigned_pdo_entry.get_index(), 0x607aU);
    ASSERT_EQ(assigned_pdo_entry.get_index(),   0x6041U);
    // Assert valid subindeces
    ASSERT_EQ(unassigned_pdo_entry.get_subindex(), 0U);
    ASSERT_EQ(assigned_pdo_entry.get_subindex(),   0U);
    // Assert valid length
    ASSERT_EQ(unassigned_pdo_entry.get_bit_len(),  32U);
    ASSERT_EQ(unassigned_pdo_entry.get_byte_len(), 4U);
    ASSERT_EQ(assigned_pdo_entry.get_bit_len(),    16U);
    ASSERT_EQ(assigned_pdo_entry.get_byte_len(),   2U);
    // Assert valid names
    ASSERT_EQ(unassigned_pdo_entry.get_name(), "Target Position");
    ASSERT_EQ(assigned_pdo_entry.get_name(),   "Status word");
    // Assert valid data types
    ASSERT_EQ(unassigned_pdo_entry.get_data_type().get_id(), ethercat::types::Type::ID::DoubleInt);
    ASSERT_EQ(assigned_pdo_entry.get_data_type().get_id(),   ethercat::types::Type::ID::UnsignedInt);

}


TEST_F(CifxEthercatENIParserTest, CyclicParsing) {

    // Get assigned PDO
    auto cyclic = eni_config->get_cyclic();

    using namespace std::literals::chrono_literals;

    // Assert valid bus cycle
    ASSERT_EQ(cyclic.get_cycle_time(), 10ms);

}


TEST_F(CifxEthercatENIParserTest, ProcessImageParsing) {

    // Get assigned PDO
    auto process_image = eni_config->get_process_image();

    // Assert valid PDI size
    ASSERT_EQ(process_image.get_size(ethercat::eni::ProcessImage::Direction::Inputs),  1536U);
    ASSERT_EQ(process_image.get_size(ethercat::eni::ProcessImage::Direction::Outputs), 1536U);
    // Assert valid mapped variables number
    ASSERT_EQ(process_image.get_variables().inputs.size(),  34U);
    ASSERT_EQ(process_image.get_variables().outputs.size(), 11U);
    // Assert valid mapped slave's variables number ( @note +1 because of WcState variable)
    ASSERT_EQ(process_image.get_slave_variables("WheelRearLeft").inputs.size(),  4U + 1U);
    ASSERT_EQ(process_image.get_slave_variables("WheelRearLeft").outputs.size(), 2U);

}

/* ================================================================================================================================ */