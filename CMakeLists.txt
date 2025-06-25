# ====================================================================================================================================
# @file       CMakeLists.txt
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 13th April 2021 12:43:42 am
# @modified   Wednesday, 25th June 2025 3:23:26 pm
# @project    ethercat-lib
# @brief      Build rules for C++ EtherCAT library
#
#
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ============================================================= Project ============================================================ #

# CMake version
cmake_minimum_required(VERSION 3.5)
# Project header
project(ethercat-lib VERSION 1.1.0)

# ========================================================== Configuration ========================================================= #

# Establish build type
set(MASTER_PROJECT OFF)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(MASTER_PROJECT ON)
endif()

# Compilation proprties
if(${MASTER_PROJECT})
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_EXTENSIONS ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_EXTENSIONS ON)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

# ROS environment
if(DEFINED ENV{ROS_DISTRO})
    set(ROS_PRESENT ON)
else()
    set(ROS_PRESENT OFF)
endif()


# Options
option(BUILD_TESTS "If TRUE tests will be built"                                    ON )
option(BUILD_DOC   "If TRUE documentation will be built"                            ON )
option(ROS_BUILD   "If TRUE the library will be built as a ROS2 package" ${ROS_PRESENT})

# Compilation options
add_compile_options(
    -Wpedantic
    -Wextra
    -Wall
    -Wno-missing-field-initializers
)

# ========================================================== Dependencies ========================================================== #

# Common dependencies
find_package(Boost REQUIRED)

# ============================================================= Sources ============================================================ #

# Source files
set(SRC_FILES

    # Types sources
    src/ethercat/types/builtin.cpp
    src/ethercat/types/type.cpp
    # ENI sources
    src/ethercat/eni/configuration/configuration.cpp
    src/ethercat/eni/process_image/process_image.cpp
    src/ethercat/eni/process_image/variable.cpp
    src/ethercat/eni/slave/slave.cpp
    src/ethercat/eni/slave/pdo.cpp
)

# Dependencies
set(LIB_DEPENDENCIES
    boost::boost
)

# ============================================================= Targets ============================================================ #

# Define helper macro
include(${CMAKE_SOURCE_DIR}/cmake/common/add_library_target.cmake)

# Add target
add_library_target(

    # Library sources
    SRC_FILES ${SRC_FILES}

    # Dependencies
    DEPENDENCIES ${LIB_DEPENDENCIES}

)

# ============================================================= Package ============================================================ #

# For non-ROS build create package manually
if(NOT ${ROS_BUILD})

    # Import package helpers
    include(CMakePackageConfigHelpers)

    # Generate package version file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ethercat-lib-${PROJECT_VERSION}/EthercatLibConfigVersion.cmake"
        COMPATIBILITY AnyNewerVersion
    )

    # Generate package config file
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/ethercat-lib-${PROJECT_VERSION}/EthercatLibConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ethercat-lib-${PROJECT_VERSION}
    )

    # Install config files
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ethercat-lib-${PROJECT_VERSION}/EthercatLibConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ethercat-lib-${PROJECT_VERSION}/EthercatLibConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ethercat-lib-${PROJECT_VERSION}
    )

endif()

# ============================================================== Tests ============================================================= #

# Test builds
if(${MASTER_PROJECT} AND BUILD_TESTS)
    add_subdirectory(tests)
endif()

# =============================================================== Docs ============================================================= #

# Build doc
if(${MASTER_PROJECT} AND BUILD_DOC)
    add_subdirectory(doc)
endif()

# ============================================================== Build ============================================================= #

if(${ROS_BUILD})
    ament_package()
endif()
