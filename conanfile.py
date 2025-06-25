# ====================================================================================================================================
# @file       conanfile.py
# @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
# @date       Wednesday, 25th June 2025 2:55:22 pm
# @modified   Wednesday, 25th June 2025 3:23:15 pm
# @project    ethercat-lib
# @brief
#
#
# @copyright Krzysztof Pierczyk © 2025
# ====================================================================================================================================

# ============================================================ Imports ============================================================= #

# Conan imports
from conan import ConanFile
from conan.tools.cmake.layout import cmake_layout
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

# ============================================================ Script ============================================================== #

class EthercatLibConan(ConanFile):

    name        = "ethercat-lib"
    version     = "1.1.0"
    license     = "MIT"
    author      = "Krzysztof Pierczyk"
    description = "Abstract C++ library for interfacing EtherCAT networks"

    # ---------------------------------------------------------------------------- #

    package_type    = 'static-library'
    settings        = [ 'os', 'arch', 'compiler', 'build_type' ]
    min_cppstd      = 20
    exports_sources = (
        'cmake/*',
        'doc/*',
        'include/*',
        'src/*',
        'tests/*',
        'CMakeLists.txt'
    )

    # ---------------------------------------------------------------------------- #

    def build_requirements(self):
        self.tool_requires('doxygen/[>=1.0.0 <2.0.0]')
        self.test_requires('gtest/[>=1.0.0 <1.13.0]')

    def requirements(self):
        self.requires('boost/[>=1.88.0 <2.0.0]',
            options = { 'header_only': True },
            transitive_headers = True)

    def layout(self):
        cmake_layout(self)

    def generate(self):

        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):

        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):

        cmake = CMake(self)
        cmake.install()

# ================================================================================================================================== #
