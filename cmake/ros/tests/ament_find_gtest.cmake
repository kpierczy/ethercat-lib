# ====================================================================================================================================
# @file       ament_find_gtest.cmake
# @author     Dirk Thomas
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 27th April 2022 6:50:23 pm
# @modified   Monday, 11th July 2022 9:46:37 pm
# @project    ethercat-lib
# @brief      Modified version of the `ament_find_gtest` macro
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
# @brief Find gtest.
# @details Set the variables:
#
#   - ``GTEST_FOUND``
#   - ``GTEST_INCLUDE_DIRS``
#   - ``GTEST_LIBRARIES``
#   - ``GTEST_MAIN_LIBRARIES``
#   - ``GTEST_BOTH_LIBRARIES``
#
# @note You should always link against ``GTEST_LIBRARIES`` and optionally additionally 
#    against ``GTEST_MAIN_LIBRARIES``.
# ---------------------------------------------------------------------------------------
macro(ament_find_gtest)
    
    set(_ARGN "${ARGN}")

    # Check if unused parameters given
    if(_ARGN)
        message(FATAL_ERROR "ament_find_gtest() called with unused arguments: ${_ARGN}")
    endif()

    # Find the GTest
    _ament_cmake_gtest_find_gtest()
    
endmacro()
