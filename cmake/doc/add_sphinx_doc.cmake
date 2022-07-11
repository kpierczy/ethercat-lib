# ====================================================================================================================================
# @file       add_sphinx_doc.cmake
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Thursday, 12th May 2022 11:27:46 pm
# @modified   Monday, 11th July 2022 4:43:58 pm
# @project    ethercat-lib
# @brief      Definition of the add_sphinx_doc() function
#    
#    
# @copyright Krzysztof Pierczyk © 2022
# ====================================================================================================================================

# ============================================================ Includes ============================================================ #

# Include common utilities
include(${CMAKE_SOURCE_DIR}/cmake/common/arguments.cmake)

# =========================================================== Definitions ========================================================== #

# ----------------------------------------------------------------------------------
# @brief Creates preconfigrued build target for Sphinx Doxygen documentation
# @details Function assumes integration of the Sphinx documentation with Doxygen
#    parser based on the 'breathe' tool.Besides the main target it provides
#    an intermidiet one that builds Doxygen documentation in the XML format.
#    Next, it is passed as the source project to the breathe framework
#
# @param target_name [NAME]
#     basename for resulting targets; for each of @p BUILDERS the 
#     ${target_name}_${BUILDER} target is declared
#
# @param WORKING_DIR [DIR] (optional, default: ${CMAKE_CURRENT_SOURCE_DIR})
#     working dircetory to run Sphinx from
# @param SOURCE_DIR [DIR] (optional, default: ${CMAKE_CURRENT_SOURCE_DIR})
#     soruce dircetory for Sphinx; this needs to contain 'conf.py' file
# @param SOURCE_FILES [FILES...] (optional, default: ${SOURCE_DIR}/index.rst)
#     list of documentation source files
# @param OUTPUT_DIR [DIR] (optional, default: ${CMAKE_CURRENT_BINARY_DIR}/doc/sphinx)
#     output directory for Sphinx results
# @param OUTPUT_LOG_FILE [FILE] (optional, default: ${CMAKE_BINARY_DIR}/log/doc/${PROJECT_NAME}/sphinx)
#     base for the name of the Sphinx error log files; ${BUILDER}.log suffix is 
#     appended to the file name automatically; by default, the log will be put into 
#     the subdirectory of the default log directory used by colcon (for convinience);
#     if COLCON_PREFIX_PATH env variable is present, the default log dir is set to
#     $ENV{COLCON_PREFIX_PATH}/../log/doc/${PROJECT_NAME}/sphinx
# @param BUILDERS [NAMES...] (optional, default: "html")
#     list of builders to run Sphinx generator with
# @param INSTALL_DIR [DIR] (optional)
#     installation directory; if not given, doc will not be installed
#
# @param ENVS [ENV=VAL...] (optional)
#     additional environment variables to be exported when running Sphinx
#
# @param GENERATE_CPP_API_DOC
#     if given, function will add additional targets generating XML documentation
#     with Doxygen based on @p DOXYGEN_* parameters that can be used with the
#     Breathe plugin; as so, the function will also populate @a breathe_projects 
#     dictionary with the ${BREATHE_PROJECT_NAME}
# @param BREATHE_PROJECT_NAME [NAME] (optional, default: ${RPOJECT_NAME})
#     if @p GENERATE_CPP_API_DOC given, Breathe @a breathe_projects dictionary
#     will be populated with @p BREATHE_PROJECT_NAME record storing absolute
#     path to the Doxygen-gnerated XML API documentation
# @param NO_BREATHE_DEFAULT_PROJECT
#     if set, function will not populate @a breathe_default_project Breathe
#     variable with the @p BREATHE_PROJECT_NAME (takes effect only if)
#     GENERATE_CPP_API_DOC is given
#
# @param DOXYGEN_WORKING_DIR [DIR] (optional, default: ${WORKING_DIR})
#     working dircetory for Doxygen
# @param DOXYGEN_SOURCE_FILES [FILES...] (optional)
#     list of source files to be passed as dependencies of the Doxygen command target
#     to properly handle recompilation requirement by CMake
# @param DOXYGEN_OUTPUT_DIR [DIR] (optional, default: ${OUTPUT_DIR}/doxygen)
#     output directory for intermediate Doxygen results
# @param DOXYGEN_OUTPUT_LOG_FILE [FILE] (optional, default: basename(${OUTPUT_LOG_FILE}/sphinx.doxygen.log)
#     name of the Soxygen error log file; by default, the log will be put into 
#     the subdirectory of the default log directory used by colcon (for convinience)
# @param DOXYGEN_INSTALL_DIR [DIR] (optional)
#     Doxygen XML output installation directory; if not given, doc will not be installed
#
# @param DOXYGEN_DOXY_FILES [FILES...] (optional)
#     Doxygen configuration files (will be concatenated in the given order)
# @param DOXYGEN_DOXY_LINES [LINES...] (optional)
#     additional lines that should be concatenated to the doxyfile used to generate
#     doc
# @param DOXYGEN_DOXY_ENVS [ENV=VAL...] (optional, default: ${ENVS})
#     additional environment variables to be exported when running Doxygen
#
# @note Underthehood function uses @ref add_doxygen_doc to generate Doxygen 
#    documentation from C++ code . See it to learn about additional facilities
#    that are provided by this function for creating Doxygen config files.
# @note Populating of Breathe entities is performed by passing -D... arguments
#    to the Sphinx. As so, the 'breathe' extension must be defined in the conf.py
#    to let Breathe parse this variables into Python entities
#     additional environment variables to be exported when running Doxygen
#
# @note Function runs Sphinx target command with some additional environment
#     variables that can be utilized inside source conf.py file. These are:
#
#        SPHINX_WORKING_DIR
#           - workign directory of the Sphinx
#        SPHINX_SOURCE_DIR
#           - soruce directory of the Sphinx
#        SPHINX_OUTPUT_DIR
#           - output directory of the Sphinx
#        SPHINX_OUTPUT_LOG_DIR
#           - output directory of the Sphinx log
#        SPHINX_OUTPUT_LOG_FILE
#           - output file for the Sphinx log
#        SPHINX_INSTALL_DIR
#           - path to the install dir
#
# ----------------------------------------------------------------------------------
function(add_sphinx_doc target_name)
    
    # -------------------------- Parse arguments -------------------------

    # Flag arguments
    set(FLAG_ARGUMENTS

        # C++ config
        GENERATE_CPP_API_DOC
        # Breather config
        NO_BREATHE_DEFAULT_PROJECT

    )

    # Single value arguments
    set(SINGLE_ARGUMENTS

        # Sphinx files & directories
        WORKING_DIR
        SOURCE_DIR
        OUTPUT_DIR
        OUTPUT_LOG_FILE
        INSTALL_DIR
        
        # Breather config
        BREATHE_PROJECT_NAME
        
        # Doxygen files & directories
        DOXYGEN_WORKING_DIR
        DOXYGEN_OUTPUT_DIR
        DOXYGEN_OUTPUT_LOG_FILE
        DOXYGEN_INSTALL_DIR
        
    )
        
    # Multi-value arguments
    set(MULTI_ARGUMENTS
    
        # Sphinx sources
        SOURCE_FILES
        # Sphinx config
        BUILDERS
        ENVS
        # Doxygen sources
        DOXYGEN_SOURCE_FILES
        # Doxygen config
        DOXYGEN_DOXY_FILES
        DOXYGEN_DOXY_LINES
        DOXYGEN_DOXY_ENVS

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

    # -------------------------- Strip arguments -------------------------
    
    # Strip flag arguments
    strip_arg(GENERATE_CPP_API_DOC)
    strip_arg(NO_BREATHE_DEFAULT_PROJECT)
    # Strip single-value arguments
    strip_arg(INSTALL_DIR)
    strip_arg(DOXYGEN_INSTALL_DIR)
    # Strip multi-value arguments
    strip_arg(ENVS)
    strip_arg(DOXYGEN_SOURCE_FILES)
    strip_arg(DOXYGEN_DOXY_FILES)
    strip_arg(DOXYGEN_DOXY_LINES)
    
    # ----------------------- Set default arguments ----------------------

    # Path to the default log dir dir
    if(DEFINED ENV{COLCON_PREFIX_PATH})
        set(DEFAULT_LOG_DIR $ENV{COLCON_PREFIX_PATH}/../log)
    else()
        set(DEFAULT_LOG_DIR ${CMAKE_BINARY_DIR}/log)
    endif()

    # Set default working dircetory (Sphinx)
    parse_arg(WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    # Set default source dircetory (Sphinx)
    parse_arg(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    # Set default source files (Sphinx)
    parse_arg(SOURCE_FILES ${SOURCE_DIR}/index.rst)
    # Set default output dircetory (Sphinx)
    parse_arg(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc/sphinx)
    # Set default output log file (Sphinx)
    parse_arg(OUTPUT_LOG_FILE ${DEFAULT_LOG_DIR}/doc/${PROJECT_NAME}/sphinx)
    # Set default builder formats (Sphinx)
    parse_arg(BUILDERS "html")

    # Set default project name (Breathe)
    parse_arg(BREATHE_PROJECT_NAME ${PROJECT_NAME})

    # Set default working dircetory (Doxygen)
    parse_arg(DOXYGEN_WORKING_DIR ${WORKING_DIR})
    # Set default output dircetory (Doxygen)
    parse_arg(DOXYGEN_OUTPUT_DIR ${OUTPUT_DIR}/doxygen)
    # Set default output log file (Doxygen)
    get_filename_component(OUTPUT_LOG_DIR "${OUTPUT_LOG_FILE}" DIRECTORY)
    parse_arg(DOXYGEN_OUTPUT_LOG_FILE ${OUTPUT_LOG_DIR}/sphinx.doxygen.log)
    # Set default envs (Doxygen)
    if(ENVS)
        parse_arg(DOXYGEN_DOXY_ENVS "${ENVS}")
    else()
        strip_arg(DOXYGEN_DOXY_ENVS)
    endif()

    # ------------------------- Validate arguments -----------------------

    # Check if output format given
    if(NOT BUILDERS)
        message(FATAL_ERROR "[add_sphinx_doc] add_sphinx_target() must be invoked with at least one builder")
    endif()


    # ---------------------------- System check --------------------------

    # Check if Sphinx has been found
    if(NOT SPHINX_FOUND)
        message(FATAL_ERROR "Sphinx need to be installed to generate the doxygen documentation")
    endif()

    # -------------------------- Build doxygen doc -----------------------


    # Build C++ API documentation with Doxygen
    if(GENERATE_CPP_API_DOC)
        
        # Doxygen target name
        set(DOXYGEN_TARGET_NAME ${target_name}_doxygen_doc)
        
        # Add Doxygen target
        add_doxygen_doc(${DOXYGEN_TARGET_NAME}

            # Working directory
            WORKING_DIR ${DOXYGEN_WORKING_DIR}
            # Source files
            SOURCE_FILES ${DOXYGEN_SOURCE_FILES}
            # Output directory
            OUTPUT_DIR ${DOXYGEN_OUTPUT_DIR}
            # Output log file
            OUTPUT_LOG_FILE ${DOXYGEN_OUTPUT_LOG_FILE}
            # Target output file
            OUTPUT_FILE "xml/index.xml"
            # Target install dir
            INSTALL_DIR ${DOXYGEN_INSTALL_DIR}

            # Configuration files for Doxygen
            DOXY_FILES ${DOXYGEN_DOXY_FILES}
            # Additional envs for Doxygen
            DOXY_ENVS ${DOXYGEN_DOXY_ENVS}
            # Additional configuration lines for Doxygen
            DOXY_LINES 
            
                # User-defined
                ${DOXYGEN_DOXY_LINES}
                # Overwrites
                "GENERATE_XML = YES"
                
        )

        # Calculate doxygen-related paths
        set(DOXYGEN_OUTPUT_DIR  ${DOXYGEN_OUTPUT_DIR}/xml)

        # ----------------------------------------------------
        # Set additional variables to be passed to the Sphinx 
        # ----------------------------------------------------

        # Add argument telling Breathe where to look for Doxygen data
        list(APPEND SPHINX_ARGS -Dbreathe_projects.${BREATHE_PROJECT_NAME}=${DOXYGEN_OUTPUT_DIR})
        # Add argument telling Breathe what is the default project to look API definitions in
        if(NOT NO_BREATHE_DEFAULT_PROJECT)
            list(APPEND SPHINX_ARGS -Dbreathe_default_project=${BREATHE_PROJECT_NAME})
        endif()
        
    endif()

    # -------------------------- Define doc targets ----------------------
    
    # Iterate over all builder to be run
    foreach(builder IN LISTS BUILDERS)
        
        # Compute target name
        set(sphinx_target_name ${target_name}_${builder})
        # Calculate sphinx directories
        set(SPHINX_OUTPUT_DIR      ${OUTPUT_DIR}/${builder})
        set(SPHINX_OUTPUT_FILE     ${SPHINX_OUTPUT_DIR}/index.${builder})
        set(SPHINX_OUTPUT_LOG_FILE ${OUTPUT_LOG_FILE}.${builder}.log)

        # Build command
        add_custom_command(
            
            # Target file
            OUTPUT ${SPHINX_OUTPUT_FILE}
            # Environment
            COMMAND ${CMAKE_COMMAND} -E env 
            
                # User-provided envs
                ${ENVS}
                # Predefined envs
                SPHINX_WORKING_DIR=${WORKING_DIR}
                SPHINX_SOURCE_DIR=${SOURCE_DIR}
                SPHINX_OUTPUT_DIR=${SPHINX_OUTPUT_DIR}
                SPHINX_OUTPUT_LOG_DIR=${OUTPUT_LOG_DIR}
                SPHINX_OUTPUT_LOG_FILE=${SPHINX_OUTPUT_LOG_FILE}
                SPHINX_INSTALL_DIR=${INSTALL_DIR}

            # Command
            ${SPHINX_EXECUTABLE} 

                # Output config
                -b ${builder}
                # Output log
                -w ${SPHINX_OUTPUT_LOG_FILE}
                # Additional args
                ${SPHINX_ARGS}
                # Input dir
                ${SOURCE_DIR} 
                # Output dir
                ${SPHINX_OUTPUT_DIR}

            # Working dir
            WORKING_DIRECTORY ${WORKING_DIR}
            # Set dependencies
            DEPENDS
                
                # Depend on Doxygen output
                ${DOXYGEN_TARGET_NAME}
                # Depend on Sphinx files
                ${SOURCE_FILES}

            # Main depdnency
            MAIN_DEPENDENCY ${SOURCE_DIR}/conf.py
            
            # Run text
            COMMENT "Generating Sphinx documentation"
            # Verbatim command
            VERBATIM
       
        )

        # Nice named target so we can run the job easily
        add_custom_target(${sphinx_target_name} ALL DEPENDS ${SPHINX_OUTPUT_FILE})
        
    endforeach()
    
    # --------------------------- Define installs ------------------------
    
    # Install output
    if(INSTALL_DIR)
        install(

            # Install all generated subdirectories
            DIRECTORY ${OUTPUT_DIR}/
            # To the requested directory
            DESTINATION ${INSTALL_DIR}
            # Except doxygen output (it is potentially installed by add_doxygen_doc())
            PATTERN "doxygen" EXCLUDE

        )
    endif()

endfunction()
