# ====================================================================================================================================
# @file       add_suitable_library.cmake
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Monday, 11th July 2022 3:00:31 pm
# @modified   Monday, 11th July 2022 4:38:19 pm
# @project    ethercat-lib
# @brief      Definition of the add_library_target() macro
# 
# 
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ========================================================== Dependencies ========================================================== #

# For standard CMake build
if(NOT ${ROS_BUILD})
    include(${CMAKE_SOURCE_DIR}/cmake/standard/add_standard_library.cmake)
# For ROS2 package build
else()
    include(${CMAKE_SOURCE_DIR}/cmake/ros/add_ament_library.cmake)
endif()

# =========================================================== Definitions ========================================================== #

# ----------------------------------------------------------------------------------
# @brief Adds shared library target providing proper exports and installs for the
#    declared target
#
# @param SRC_FILES [PATHS...] 
#    list of source files
# @param DEPENDENCIES [NAMES...] 
#    list of dependencies
# ----------------------------------------------------------------------------------
macro(add_library_target)
    
    # -------------------------- Parse arguments -------------------------

    # Multi-value arguments
    set(MULTI_ARGUMENTS
        SRC_FILES
        DEPENDENCIES
    )

    # Set arg prefix
    set(ARG_PREFIX "ARG")
    # Parse arguments
    cmake_parse_arguments(${ARG_PREFIX}
        ""
        ""
        "${MULTI_ARGUMENTS}"
        ${ARGN}
    )

    # --------------------------- Define targets -------------------------


    # For standard CMake build
    if(NOT ${ROS_BUILD})

        # Add target
        add_standard_library(

            # Library name
            LIBRARY_NAME ${PROJECT_NAME}

            # Library sources
            SOURCES ${ARG_SRC_FILES}

            # Dependencies
            DEPENDENCIES ${ARG_DEPENDENCIES}
            
        )

    # For ROS2 package build
    else()

        # Add target
        add_ament_library(
        
            # Library name
            LIBRARY_NAME ${PROJECT_NAME}
        
            # Library sources
            SOURCES ${ARG_SRC_FILES}
        
            # Dependencies
            AMENT_DEPENDENCIES ${ARG_DEPENDENCIES}
            
        )

    endif()

endmacro()
