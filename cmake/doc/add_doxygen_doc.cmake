# ====================================================================================================================================
# @file       add_doxygen_doc.cmake
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Thursday, 12th May 2022 11:27:46 pm
# @modified   Monday, 11th July 2022 4:44:24 pm
# @project    ethercat-lib
# @brief      Definition of the add_doxygen_doc() function
#    
#    
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ============================================================ Includes ============================================================ #

# Include common utilities
include(${CMAKE_SOURCE_DIR}/cmake/common/arguments.cmake)

# ============================================================= Helpers ============================================================ #

# ----------------------------------------------------------------------------------
# @brief Helper macro adding custom command compiling set of @p DOXY_FILES into
#    an @p OUTPUT_FILE
#
# @param OUTPUT_FILE [FILE]
#    name of the output files
# @param DOXY_FILES [FILES...]
#    names of files to be concatenated
# ----------------------------------------------------------------------------------
function(compile_doxyfiles)

    # Parse arguments
    cmake_parse_arguments(ARG
        ""
        "OUTPUT_FILE"
        "DOXY_FILES;ENV"
        ${ARGN}
    )

    # Add command
    add_custom_command(
            
        # Target file
        OUTPUT ${ARG_OUTPUT_FILE}
        # Command
        COMMAND cat ${ARG_DOXY_FILES} > ${ARG_OUTPUT_FILE}
        # Files dependencies
        DEPENDS ${ARG_DOXY_FILES}
        
    )

    # Append empty line to the file
    append_doxyfile_line(${ARG_OUTPUT_FILE} "")

endfunction()

# ----------------------------------------------------------------------------------
# @brief Helper macro adding custom command (with APPEND option) appending string 
#   line to the compile doxyfile
#
# @param output_file
#    name of the target doxyfile
# @param str
#    string to be appended
# ----------------------------------------------------------------------------------
function(append_doxyfile_line output_file str)

    # Add command
    add_custom_command(OUTPUT ${output_file} COMMAND echo "${str}" >> ${output_file} APPEND)

endfunction()

# =========================================================== Definitions ========================================================== #

# ----------------------------------------------------------------------------------
# @brief Helper macro adding custom target generating doxygen docs
#
# @param target_name [NAME]
#     target name
#
# @param WORKING_DIR [DIR] (optional, default: ${CMAKE_CURRENT_SOURCE_DIR})
#     working dircetory for Doxygen
# @param SOURCE_FILES [FILES...] (optional)
#     list of source files to be passed as dependencies of the Doxygen command target
#     to properly handle recompilation requirement by CMake
# @param OUTPUT_DIR [DIR] (optional, default: ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
#     output directory for Doxygen results; this will overwrite 'OUTPUT_DIRECTORY' 
#     in the original @p DOXYFILE
# @param OUTPUT_LOG_FILE [FILE] (optional, default: ${CMAKE_BINARY_DIR}/log/doc/${PROJECT_NAME}/doxygen.log)
#     name of the Doxygen output log file; this will overwrite 'WARN_LOGFILE' in the
#     original @p DOXYFILE; by default, the log will be put into the subdirectory
#     of the default log directory used by colcon (for convinience);
#     if COLCON_PREFIX_PATH env variable is present, the default log dir is set to
#     $ENV{COLCON_PREFIX_PATH}/../log/doc/${PROJECT_NAME}/sphinx
# @param OUTPUT_FILE [FILE] (optional, default: html/index.html)
#     output file produced by Doxygen relative to the @p OUTPUT_DIR; this is 
#     required to tell CMake what file is expected to be produced by the command
#     to properly handle dependencies tree
# @param TAG_FILE [FILE] (optional)
#     name of the tag file (relative to the ); if not given, tag file will not be 
#     generated
# @param INSTALL_DIR [DIR] (optional)
#     installation directory; if not given, doc will not be installed
#
# @param DOXY_FILES [FILES...] (required)
#     Doxygen configuration files (will be concatenated in the given order)
# @param DOXY_LINES [LINES...] (optional)
#     additional lines that should be concatenated to the doxyfile used to generate
#     doc
# @param DOXY_ENVS [ENV=VAL...] (optional)
#     additional environment variables to be exported when running Doxygen
#
# @note Function runs Doxygen target command with some additional environment
#     variables that can be utilized inside source @p DOXY_FILES and @p DOXY_LINES
#     for convinience. These are:
#
#        DOXYGEN_WORKING_DIR
#           - workign directory of the Doxygen
#        DOXYGEN_OUTPUT_DIR
#           - output directory of the Doxygen
#        DOXYGEN_OUTPUT_LOG_DIR
#           - output directory of the Doxygen log
#        DOXYGEN_OUTPUT_LOG_FILE
#           - output file for the Doxygen log
#        DOXYGEN_TAG_FILE
#           - path to the output tag file (if requested)
#        DOXYGEN_INSTALL_DIR
#           - path to the install dir
#    
# ----------------------------------------------------------------------------------
function(add_doxygen_doc target_name)
    
    # -------------------------- Parse arguments -------------------------

    # Single-value arguments
    set(SINGLE_ARGUMENTS

        # Directories & files
        WORKING_DIR
        OUTPUT_DIR
        OUTPUT_LOG_FILE
        OUTPUT_FILE
        TAG_FILE
        INSTALL_DIR
        
    )

    # Multi-value arguments
    set(MULTI_ARGUMENTS

        # Directories & files
        SOURCE_FILES
        DOXY_FILES
        # Additional config
        DOXY_LINES
        # Additional envs
        DOXY_ENVS
        
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

    # -------------------------- Strip arguments -------------------------

    # Strip single-value arguments
    strip_arg(TAG_FILE)
    strip_arg(INSTALL_DIR)
    # Strip multi-value arguments
    strip_arg(SOURCE_FILES)
    strip_arg(DOXY_FILES)
    strip_arg(DOXY_LINES)
    strip_arg(DOXY_ENVS)

    # ----------------------- Set default arguments ----------------------

    # Path to the default log dir dir
    if(DEFINED ENV{COLCON_PREFIX_PATH})
        set(DEFAULT_LOG_DIR $ENV{COLCON_PREFIX_PATH}/../log)
    else()
        set(DEFAULT_LOG_DIR ${CMAKE_BINARY_DIR}/log)
    endif()

    # Set default working dircetory
    parse_arg(WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    # Set default output directory
    parse_arg(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
    # Set default log file
    parse_arg(OUTPUT_LOG_FILE ${DEFAULT_LOG_DIR}/doc/${PROJECT_NAME}/doxygen.log)
    # Set default output file
    parse_arg(OUTPUT_FILE html/index.html)

    # ------------------------- Validate arguments -----------------------

    # Check if doxyfile given
    if(NOT DOXY_FILES)
        message(FATAL_ERROR "[add_doxygen_doc] add_doxygen_doc() must be invoked with at least one doxyfile")
    endif()

    # ---------------------------- System check --------------------------

    # Check if Doxygen has been found
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "[add_doxygen_doc] Doxygen need to be installed to generate the doxygen documentation")
    endif()

    # ------------------ Prepare output files/directories ----------------

    # Create output directory
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    
    # Compute log directory path
    get_filename_component(OUTPUT_LOG_DIR "${OUTPUT_LOG_FILE}" DIRECTORY)
    # Create log file
    file(MAKE_DIRECTORY ${OUTPUT_LOG_DIR})
    file(TOUCH          ${OUTPUT_LOG_FILE})

    # ------------------------ Prepare doxygen config --------------------

    # Set absolute path tot the target file
    set(OUTPUT_FILE ${OUTPUT_DIR}/${OUTPUT_FILE})
    # Name of the compiled doxygen config file
    set(DOXY_FILE ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.doxyfile)

    # Create temporary doxyfile from the source configs
    compile_doxyfiles(OUTPUT_FILE ${DOXY_FILE} DOXY_FILES ${ARG_DOXY_FILES})
    # Overrite output dir and log file configuration
    append_doxyfile_line(${DOXY_FILE} "OUTPUT_DIRECTORY = ${OUTPUT_DIR}"     )
    append_doxyfile_line(${DOXY_FILE} "WARN_LOGFILE     = ${OUTPUT_LOG_FILE}")

    # Add tagfile config
    if(TAG_FILE)

        # Compute absolute path to the tag file
        set(TAG_FILE "${OUTPUT_DIR}/${TAG_FILE}")
        # Append tag configuration to the doxygen config
        append_doxyfile_line(${DOXY_FILE} "GENERATE_TAGFILE = ${TAG_FILE}")
        
    endif()

    # Add additional config
    foreach(line IN LISTS ARG_DOXY_LINES)
        append_doxyfile_line(${DOXY_FILE} "${line}")
    endforeach()
    
    # Compute name for Doxygen config file target
    set(doxyfile_target_name ${target_name}_doxyfile)
    # Add target building the Doxygen configuration file
    add_custom_target(${doxyfile_target_name} ALL DEPENDS ${DOXY_FILE})

    # -------------------------- Define doc target -----------------------
    
    # Add doxygen command
    add_custom_command(

        # Target file
        OUTPUT ${OUTPUT_FILE}
        # Environment
        COMMAND ${CMAKE_COMMAND} -E env 

            # Additional environment
            ${DOXY_ENVS}
            # Predefines environment
            DOXYGEN_WORKING_DIR=${WORKING_DIR}
            DOXYGEN_OUTPUT_DIR=${OUTPUT_DIR}
            DOXYGEN_OUTPUT_LOG_DIR=${OUTPUT_LOG_DIR}
            DOXYGEN_OUTPUT_LOG_FILE=${OUTPUT_LOG_FILE}
            DOXYGEN_TAG_FILE=${TAG_FILE}
            DOXYGEN_INSTALL_DIR=${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}

        # Comand
        ${DOXYGEN_EXECUTABLE} ${DOXY_FILE}

        # Working directory
        WORKING_DIRECTORY ${WORKING_DIR}
        # Dependencies
        DEPENDS 

            # Doxyfile
            ${doxyfile_target_name}
            # Custom sources
            ${SOURCE_FILES}
            
        # Run text
        COMMENT "Generating Doxgen docs"
        # Verbatim command
        VERBATIM
        
    )

    # Add target to the command
    add_custom_target(${target_name} ALL DEPENDS ${OUTPUT_FILE})

    # # --------------------------- Define installs ------------------------

    # Install output
    if(INSTALL_DIR)
        install(DIRECTORY ${OUTPUT_DIR}/ DESTINATION ${INSTALL_DIR})
    endif()

endfunction()
