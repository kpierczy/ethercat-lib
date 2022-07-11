# ====================================================================================================================================
# @file       add_standard_test.cmake
# @author     Dirk Thomas
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 27th April 2022 6:50:23 pm
# @modified   Monday, 11th July 2022 9:46:37 pm
# @project    ethercat-lib
# @brief      Defines `add_standard_test` macro
# ====================================================================================================================================

# ========================================================== Dependencies ========================================================== #

find_package(GTest REQUIRED)

# ============================================================= Macros ============================================================= #

# ---------------------------------------------------------------------------------------
# @brief Adds test target using GTest library
#
# @param target [NAME]
#    target name
# @param src [NAMES...]
#    list of source files
# @param COMMAND_SUFFIX [STR] 
#    test run command suffix
# ---------------------------------------------------------------------------------------
macro(add_standard_test target)

    enable_testing()
    
    # -------------------------- Parse arguments -------------------------

    # Single-value arguments
    set(SINGLE_ARGUMENTS
        COMMAND_SUFFIX
    )

    # Set arg prefix
    set(ARG_PREFIX "ARG")
    # Parse arguments
    cmake_parse_arguments(${ARG_PREFIX}
        ""
        "${SINGLE_ARGUMENTS}"
        ""
        ${ARGN}
    )

    # --------------------------- Define targets -------------------------

    # Add test executable
    add_executable(${target} ${ARG_UNPARSED_ARGUMENTS})

    # Add GTest include directories
    target_include_directories("${target}" BEFORE PUBLIC "${GTEST_INCLUDE_DIRS}")
    # Link GTest Main
    target_link_libraries("${target}" ${GTEST_MAIN_LIBRARIES})
    # Link GTest libraries
    target_link_libraries("${target}" ${GTEST_LIBRARIES})

    # Prepare path to the executable
    set(executable "$<TARGET_FILE:${target}>")
    # Prepare path to the result file
    set(result_file "${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}/${target}.gtest.xml")
    # Prepare command to run test
    set(cmd
        "${executable}"
        "--gtest_output=xml:${result_file}"
        "${ARG_COMMAND_SUFFIX}"
    )

    # Add test target
    add_test(
        NAME ${target}
        COMMAND ${cmd}
    )

endmacro()