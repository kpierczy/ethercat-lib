# ====================================================================================================================================
# @file       CMakeLists.txt
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 13th April 2021 12:43:42 am
# @modified   Monday, 11th July 2022 4:45:08 pm
# @project    ethercat-lib
# @brief
#    
#    Build rules for Hilscher's CIFX/netX Toolkit-based CIFX EtherCAT tests
#
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# =========================================================== Dependencies ========================================================= #

include(${CMAKE_SOURCE_DIR}/cmake/common/add_test_target.cmake)

# ========================================================== Configuration ========================================================= #

# Common suffix for running all tests
list(APPEND COMMON_SUFFIX --gtest_break_on_failure)
# Common defines for compilling all tests
list(APPEND COMMON_DEFINES "ETHERCAT_LIB_TEST_ENI_PATH=\"${CMAKE_CURRENT_BINARY_DIR}/data/test_eni.xml\"")
# Common options for compilling all tests
list(APPEND COMMON_OPTIONS "-Wno-maybe-uninitialized")

# ======================================================== ENI parsing test ======================================================== #

set(TEST_NAME eni_parsing_test)

# Add test
add_test_target(${TEST_NAME}

    # Test sources
    SRC_FILES
        src/eni_parsing_test.cpp

    # Test-runner suffix (stop test-case after first failure)
    COMMAND_SUFFIX ${COMMON_SUFFIX}
    
    # Link dependencies
    DEPENDENCIES ${PROJECT_NAME}
    # Additional compilation definitions for the test            
    ADDITIONAL_DEFINES ${COMMON_DEFINES}   
    # Additional compilation flags for the test            
    ADDITIONAL_OPTIONS ${COMMON_OPTIONS}   
)

# ======================================================= DefaultParse tests ======================================================= #

set(TEST_NAME default_parser_test)

# Add test
add_test_target(${TEST_NAME}

    # Test sources
    SRC_FILES
        src/default_parser_test.cpp

    # Test-runner suffix (stop test-case after first failure)
    COMMAND_SUFFIX ${COMMON_SUFFIX}
    
    # Link dependencies
    DEPENDENCIES ${PROJECT_NAME}  
    # Additional compilation flags for the test            
    ADDITIONAL_OPTIONS ${COMMON_OPTIONS}   
)

# ============================================================ Resources =========================================================== #

# Copy test resources
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# ================================================================================================================================== #
