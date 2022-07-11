# ====================================================================================================================================
# @file       ament_add_gtest.cmake
# @author     Dirk Thomas
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 27th April 2022 6:50:23 pm
# @modified   Monday, 11th July 2022 9:46:37 pm
# @project    ethercat-lib
# @brief      Modified version of the `ament_add_gtest` function providing ability to add prefix and suffix strings to the test 
#             run command
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
# @brief Add a gtest.
#
# @details Call add_executable(target ARGN), link it against the gtest libraries
#    and register the executable as a test.
#
#    If gtest is not available the specified target is not being created and
#    therefore the target existence should be checked before being used.
#
# @param target [NAME]
#    the target name which will also be used as the test name
# @param ARGN [NAMES...]
#    the list of source files
# @param RUNNER [STR...]
#    the path to the test runner script (default: see ament_add_test).
# @param TIMEOUT [INT...]
#    the test timeout in seconds, default defined by ``ament_add_test()``
# @param WORKING_DIRECTORY [DIR]
#    the working directory for invoking the executable in, default defined by 
#    ``ament_add_test()``
# @param SKIP_LINKING_MAIN_LIBRARIES
#    if set skip linking against the gtest main libraries
# @param SKIP_TEST
#    if set mark the test as being skipped
# @param ENV [NAME=VAL...]
#    list of env vars to set; listed as ``VAR=value``
# @param APPEND_ENV [NAME=VAL...]
#    list of env vars to append if already set, otherwise set; listed as ``VAR=value``
# @param APPEND_LIBRARY_DIRS [DIRS...]
#    list of library dirs to append to the appropriate OS specific env var, a la
#    LD_LIBRARY_PATH
#
# @added
#
# @param COMMAND_PREFIX 
#    multielement keyword argument providing strings to prefix test command with
# @param COMMAND_SUFFIX 
#    multielement keyword argument providing strings to suffix test command with
#   
# @endadded
# ---------------------------------------------------------------------------------------
macro(ament_add_gtest target)
    
    # Parse arguments
    cmake_parse_arguments(_ARG
        "SKIP_LINKING_MAIN_LIBRARIES;SKIP_TEST"
        "RUNNER;TIMEOUT;WORKING_DIRECTORY"
        "APPEND_ENV;APPEND_LIBRARY_DIRS;ENV;COMMAND_PREFIX;COMMAND_SUFFIX"
        ${ARGN}
    )
    
    # Check if unused arguments given
    if(NOT _ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ament_add_gtest() must be invoked with at least one source file")
    endif()

    # Set soruce files
    set(_argn_executable ${_ARG_UNPARSED_ARGUMENTS})
    # Forward optional flag to the `ament_add_gtest_executable`
    if(_ARG_SKIP_LINKING_MAIN_LIBRARIES)
        list(APPEND _argn_executable "SKIP_LINKING_MAIN_LIBRARIES")
    endif()

    # Add test executable target
    ament_add_gtest_executable("${target}" ${_argn_executable})

    # Prepare list forwarding arguments to ament_add_gtest_test 
    set(_argn_test "")

    # Prepend keyword argument with keyword (if given)
    if(_ARG_RUNNER)
        list(APPEND _argn_test "RUNNER" "${_ARG_RUNNER}")
    endif()
    if(_ARG_TIMEOUT)
        list(APPEND _argn_test "TIMEOUT" "${_ARG_TIMEOUT}")
    endif()
    if(_ARG_WORKING_DIRECTORY)
        list(APPEND _argn_test "WORKING_DIRECTORY" "${_ARG_WORKING_DIRECTORY}")
    endif()
    if(_ARG_SKIP_TEST)
        list(APPEND _argn_test "SKIP_TEST")
    endif()
    if(_ARG_ENV)
        list(APPEND _argn_test "ENV" ${_ARG_ENV})
    endif()
    if(_ARG_APPEND_ENV)
        list(APPEND _argn_test "APPEND_ENV" ${_ARG_APPEND_ENV})
    endif()
    if(_ARG_APPEND_LIBRARY_DIRS)
        list(APPEND _argn_test "APPEND_LIBRARY_DIRS" ${_ARG_APPEND_LIBRARY_DIRS})
    endif()
    if(_ARG_COMMAND_PREFIX)
        list(APPEND _argn_test "COMMAND_PREFIX" ${_ARG_COMMAND_PREFIX})
    endif()
    if(_ARG_COMMAND_SUFFIX)
        list(APPEND _argn_test "COMMAND_SUFFIX" ${_ARG_COMMAND_SUFFIX})
    endif()
    
    # Add test target
    ament_add_gtest_test("${target}" ${_argn_test})
    
endmacro()
