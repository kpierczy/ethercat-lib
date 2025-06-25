# ====================================================================================================================================
# @file       add_standard_library.cmake
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Thursday, 12th May 2022 11:27:46 pm
# @modified   Wednesday, 25th June 2025 3:08:25 pm
# @project    ethercat-lib
# @brief      Definition of the add_standard_library() macro
#
#
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ============================================================ Includes ============================================================ #

# Include common utilities
include(${CMAKE_SOURCE_DIR}/cmake/common/arguments.cmake)

# =========================================================== Definitions ========================================================== #

# ----------------------------------------------------------------------------------
# @brief Adds shared library target providing proper exports and installs for the
#    declared target
#
# @param SKIP_INCLUDES_INSTALL
#    if given, include directory will not be installed
# @param SKIP_EXPORT
#    if given, target will not be exported and just installed
# @param LIBRARY_NAME [NAME]
#    name of the library target
# @param LIBRARY_TYPE [TYPE] (optional, default: SHARED)
#    type of the library target
# @param TARGET_DEPENDENCIES_TYPE [TYPE] (optional, default: )
#    type of the dependencies linking
# @param SOURCES [NAMES...]
#    list of source files
# @param ADDITIONAL_INCLUDES [NAMES...] (optional)
#    list of additional include interfaces
# @param OVERRIDE_INCLUDES [NAMES...] (optional)
#    list of include interfaces to be registered instead of default ones
# @param DEPENDENCIES [NAMES...] (optional)
#    list of dependencies to be passed to `target_link_libraries()`
# ----------------------------------------------------------------------------------
macro(add_standard_library)

    # -------------------------- Parse arguments -------------------------

    # Flag arguments
    set(FLAG_ARGUMENTS
        SKIP_INCLUDES_INSTALL
        SKIP_EXPORT
    )

    # Single-value arguments
    set(SINGLE_ARGUMENTS
        LIBRARY_NAME
        LIBRARY_TYPE
        TARGET_DEPENDENCIES_TYPE
    )

    # Multi-value arguments
    set(MULTI_ARGUMENTS
        SOURCES
        ADDITIONAL_INCLUDES
        OVERRIDE_INCLUDES
        DEPENDENCIES
    )

    # Set arg prefix
    set(ARG_PREFIX "ARG")
    # Parse arguments
    cmake_parse_arguments(${ARG_PREFIX}
        "${FLAG_ARGUMENTS}"
        "${SINGLE_ARGUMENTS}"
        "${MULTI_ARGUMENTS}"
        ${ARGN}
    )

    # ----------------------- Set default arguments ----------------------

    # Set default library type
    parse_arg(LIBRARY_TYPE "SHARED")
    # Set default dependencies type
    strip_arg(TARGET_DEPENDENCIES_TYPE)

    # Set includes type
    if(LIBRARY_TYPE STREQUAL "INTERFACE")
        set(INCLUDE_TYPE "INTERFACE")
    else()
        set(INCLUDE_TYPE "PUBLIC")
    endif()

    # Optionally override dependencies type
    if(LIBRARY_TYPE STREQUAL "INTERFACE")
        set(TARGET_DEPENDENCIES_TYPE "INTERFACE")
    endif()

    # ------------------------- Validate arguments -----------------------

    # Check if target name has been given
    if(NOT ARG_LIBRARY_NAME)
        message(FATAL_ERROR "add_standard_library() must be invoked with shared library name")
    endif()

    # Check if source files has been given
    if(NOT ARG_SOURCES AND NOT (LIBRARY_TYPE STREQUAL "INTERFACE"))
        message(FATAL_ERROR "add_standard_library() must be invoked with at least one source file if non-interface library is requested")
    endif()

    # --------------------------- Define targets -------------------------

    # Add target
    add_library(${ARG_LIBRARY_NAME} ${LIBRARY_TYPE} ${ARG_SOURCES})
    # Add include directories to the target
    if(NOT ARG_OVERRIDE_INCLUDES)
        target_include_directories(${ARG_LIBRARY_NAME}
            ${INCLUDE_TYPE}
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:include>"
                ${ARG_ADDITIONAL_INCLUDES}
        )
    else()
        target_include_directories(${ARG_LIBRARY_NAME} ${ARG_OVERRIDE_INCLUDES})
    endif()
    # Link target dependencies
    if(ARG_DEPENDENCIES)
        target_link_libraries(${ARG_LIBRARY_NAME} ${AMENT_TARGET_DEPENDENCIES_TYPE} ${ARG_DEPENDENCIES})
    endif()

    # Install includes
    if(NOT ARG_SKIP_INCLUDES_INSTALL)
        install(
            DIRECTORY
                include/
            DESTINATION
                include
        )
    endif()

    # Prepare export string
    if(NOT ARG_SKIP_EXPORT)
        set(EXPORT_NAME "export_${ARG_LIBRARY_NAME}")
        set(EXPORT_STRING EXPORT "${EXPORT_NAME}")
    endif()
    # Install targets into the exporting set
    install(TARGETS ${ARG_LIBRARY_NAME} ${EXPORT_STRING}
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin
    )

    # Export targets
    if(NOT ARG_SKIP_EXPORT)
        install(${EXPORT_STRING}
            FILE
                ${EXPORT_NAME}Targets.cmake
            DESTINATION
                lib/cmake/ethercat-lib-${PROJECT_VERSION}
        )
    endif()

endmacro()
