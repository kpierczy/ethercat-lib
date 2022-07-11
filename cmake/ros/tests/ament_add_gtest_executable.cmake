# ====================================================================================================================================
# @file       ament_add_gtest.cmake
# @author     Dirk Thomas
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 27th April 2022 6:50:23 pm
# @modified   Monday, 11th July 2022 9:46:37 pm
# @project    ethercat-lib
# @brief      Modified version of the `ament_add_gtest_executable` function
#    
# Copyright 2014-2015 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ====================================================================================================================================

# ---------------------------------------------------------------------------------------
# @brief Add an executable using gtest.
#
# @details Call add_executable(target ARGN) and link it against the gtest libraries.
#    It does not register the executable as a test.
#
#    If gtest is not available the specified target is not being created and
#    therefore the target existence should be checked before being used.
#
# @param target [NAME]
#    the target name which will also be used as the test name
# @param ARGN [NAMES...]
#    the list of source files
# @param SKIP_LINKING_MAIN_LIBRARIES
#    if set skip linking against the gtest
# ---------------------------------------------------------------------------------------
macro(ament_add_gtest_executable target)

    # Find GTest
    _ament_cmake_gtest_find_gtest()

    # On success add test target
    if(GTEST_FOUND)
        _ament_add_gtest_executable("${target}" ${ARGN})
    endif()
    
endmacro()

# ---------------------------------------------------------------------------------------
# @brief Imlementation of the `ament_add_gtest_executable` routine
# ---------------------------------------------------------------------------------------
function(_ament_add_gtest_executable target)

    # Parse arguments
    cmake_parse_arguments(ARG
        "SKIP_LINKING_MAIN_LIBRARIES"
        ""
        ""
        ${ARGN}
    )
    
    # Check if unused arguments given
    if(NOT ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ament_add_gtest_executable() must be invoked with at least one source file")
    endif()

    # Add executable target (should be EXCLUDE_FROM_ALL if it would be possible to add this target as a dependency to the "test" target)
    add_executable("${target}" ${ARG_UNPARSED_ARGUMENTS})
    # Add linclude directories
    target_include_directories("${target}" BEFORE PUBLIC "${GTEST_INCLUDE_DIRS}")

    # Link GTest mainlibraries
    if(NOT ARG_SKIP_LINKING_MAIN_LIBRARIES)
        target_link_libraries("${target}" ${GTEST_MAIN_LIBRARIES})
    endif()
    # Link GTest libraries
    target_link_libraries("${target}" ${GTEST_LIBRARIES})

    # Set additional properties for non-Windows build
    if(NOT WIN32)
        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)
        target_link_libraries("${target}" Threads::Threads)
    endif()
    
endfunction()
