# ====================================================================================================================================
# @file       FindSphinx.cmake
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Monday, 23rd May 2022 10:31:26 pm
# @modified   Monday, 11th July 2022 2:07:23 pm
# @project    ethercat-lib
# @brief      Helper CMake script finding Sphinx executable
# 
# 
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

#Look for an executable called sphinx-build
find_program(SPHINX_EXECUTABLE
    NAMES
        sphinx-build
    DOC
        "Sphinx build utility"
)

# Add standard helper
include(FindPackageHandleStandardArgs)

# Handle standard arguments to find_package like REQUIRED and QUIET
find_package_handle_standard_args(
    Sphinx
    "Failed to find sphinx-build executable"
    SPHINX_EXECUTABLE
)
