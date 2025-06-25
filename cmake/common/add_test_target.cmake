# ====================================================================================================================================
# @file       add_test_target.cmake
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Monday, 11th July 2022 3:00:31 pm
# @modified   Wednesday, 25th June 2025 3:21:02 pm
# @project    ethercat-lib
# @brief      Definition of the add_test_target() macro
#
#
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ========================================================== Dependencies ========================================================== #

# For standard CMake build
if(NOT ${ROS_BUILD})
    include(${CMAKE_SOURCE_DIR}/cmake/standard/add_standard_test.cmake)
# For ROS2 package build
else()
    include(${CMAKE_SOURCE_DIR}/cmake/ros/add_ament_test.cmake)
endif()

# ========================================================== Configuration ========================================================= #

if(NOT ${ROS_BUILD})
    enable_testing()
endif()

# =========================================================== Definitions ========================================================== #

# ----------------------------------------------------------------------------------
# @brief Adds shared library target providing proper exports and installs for the
#    declared target
#
# @param target [NAME]
#    target name
# @param SRC_FILES [PATHS...]
#    list of source files
# @param COMMAND_SUFFIX [STR]
#    test run command suffix
# @param DEPENDENCIES [NAMES...]
#    list of dependencies
# @param ADDITIONAL_DEFINES [NAMES...]
#    additional compile-time defines for the test
# @param ADDITIONAL_OPTIONS [NAMES...]
#    additional compile-time options for the test
# ----------------------------------------------------------------------------------
macro(add_test_target target)

    # -------------------------- Parse arguments -------------------------

    # Single-value arguments
    set(SINGLE_ARGUMENTS
        COMMAND_SUFFIX
    )

    # Multi-value arguments
    set(MULTI_ARGUMENTS
        SRC_FILES
        DEPENDENCIES
        ADDITIONAL_DEFINES
        ADDITIONAL_OPTIONS
    )

    # Set arg prefix
    set(ARG_PREFIX "ARG")
    # Parse arguments
    cmake_parse_arguments(${ARG_PREFIX}
        ""
        "${SINGLE_ARGUMENTS}"
        "${MULTI_ARGUMENTS}"
        ${ARGN}
    )

    # -------------------------- Evaluate arguments ----------------------

    if(ARG_COMMAND_SUFFIX)
        set(ARG_COMMAND_SUFFIX "COMMAND_SUFFIX" ${ARG_COMMAND_SUFFIX})
    endif()

    # --------------------------- Define targets -------------------------

    # For standard CMake build
    if(NOT ${ROS_BUILD})

        add_standard_test(${target}
            ${ARG_SRC_FILES}
        )

    # For ROS2 package build
    else()

        ament_add_gtest(${target}

            # Test sources
            ${ARG_SRC_FILES}

            # Test-runner suffix (stop test-case after first failure)
            ${ARG_COMMAND_SUFFIX}
        )

    endif()

    # --------------------------- Define targets -------------------------

    # Link dependencies to the test executable [targets]
    if(ARG_DEPENDENCIES)
        target_link_libraries(${target} ${ARG_DEPENDENCIES})
    endif()
    # Additional compilation definitions for the test
    if(ARG_ADDITIONAL_DEFINES)
        target_compile_definitions(${target} PRIVATE ${ARG_ADDITIONAL_DEFINES})
    endif()
    # Additional compilation flags for the test
    if(ARG_ADDITIONAL_OPTIONS)
        target_compile_options(${target} PRIVATE ${ARG_ADDITIONAL_OPTIONS})
    endif()

endmacro()
