CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

# BTK: Biomechanical ToolKit
PROJECT(BTK)

# BTK version number.
SET(BTK_VERSION_MAJOR 1)
SET(BTK_VERSION_MINOR 0)
SET(BTK_VERSION_PATCH 0)
SET(BTK_VERSION "${BTK_VERSION_MAJOR}.${BTK_VERSION_MINOR}.${BTK_VERSION_PATCH}")
SET(BTK_VERSION_STRING ${BTK_VERSION})
# The major.minor is enough to distinguish available features of the toolkit. Moreover, 
# this variable is used to create lib/share/include folders where the patch number does 
# not need to be included.
SET(BTK_LIBRARY_VERSION_STRING "${BTK_VERSION_MAJOR}.${BTK_VERSION_MINOR}")

# ------------------------------------------------------------------------
# CMAKE GENERAL CONFIGURATION
# ------------------------------------------------------------------------

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS "-stdlib=libc++ -fno-rtti")
  # CMake 3.1 complains it does not know how to set the compile flag to enable C++11 with (Apple) Clang
  SET(CMAKE_CXX11_STANDARD_COMPILE_OPTION "-std=c++11")
  SET(CMAKE_CXX11_EXTENSION_COMPILE_OPTION "-std=gnu++11")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows" AND MSVC)
  # CMake 3.1 complains it does not know how to set the compile flag to enable C++11 with MSVC
  SET(CMAKE_CXX11_STANDARD_COMPILE_OPTION "")
  SET(CMAKE_CXX11_EXTENSION_COMPILE_OPTION "")
  # CXX Flag details
  #  - GR-: disable RTTI
  #  - EHa: enable exception handling for C++ object
  #  - D_SCL_SECURE_NO_WARNINGS: disable function call with unsafe parameters warning
  SET(CMAKE_CXX_FLAGS "/GR- /EHa  -D_SCL_SECURE_NO_WARNINGS")
ELSE()
  SET(CMAKE_CXX_FLAGS "-fno-rtti")
ENDIF()

# ------------------------------------------------------------------------
# COMPILATION INSTRUCTIONS
# ------------------------------------------------------------------------

# Using the configuration "Always full RPATH"
# from http://www.cmake.org/Wiki/CMake_RPATH_handling
#             --------------------------
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/btk-${BTK_LIBRARY_VERSION_STRING}")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Extra CMake scripts
SET(BTK_CMAKE_MODULE_PATH "${BTK_SOURCE_DIR}/CMake")
SET(CMAKE_MODULE_PATH "${BTK_CMAKE_MODULE_PATH}")

# Output directories.
SET(LIBRARY_OUTPUT_PATH ${BTK_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
SET(BTK_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(EXECUTABLE_OUTPUT_PATH ${BTK_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
SET(BTK_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")

# Load some CMake macros.
INCLUDE(CheckIncludeFile)
INCLUDE(GenerateExportHeader)
INCLUDE(CMakePackageConfigHelpers)

# By default BTK is compiled in Release mode
IF(NOT CMAKE_BUILD_TYPE)
  MESSAGE(STATUS "Setting build type to 'Release' as none was specified.")
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF()

# 64-bit OS detection
IF((CMAKE_SIZEOF_VOID_P EQUAL 8) OR CMAKE_CL_64)
  SET(HAVE_64_BIT_COMPILER 1)
ENDIF()

# Try to find mmap
CHECK_INCLUDE_FILE(sys/mman.h HAVE_SYS_MMAP)

# Extra compiler flags for the debug mode
INCLUDE(${BTK_CMAKE_MODULE_PATH}/btkExtraCompilerFlags.cmake)

# BTK build configuration options.
OPTION(BUILD_SHARED_LIBS "Build BTK with shared libraries." OFF)
SET(BTK_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
IF(WIN32)
   IF(BUILD_SHARED_LIBS)
     SET(BTK_LIBS_BUILD_TYPE "SHARED")
   ELSE(BUILD_SHARED_LIBS)
     SET(BTK_LIBS_BUILD_TYPE "STATIC")
   ENDIF(BUILD_SHARED_LIBS)
ENDIF()

# Configure files with settings for use by the build.
CONFIGURE_FILE(${BTK_SOURCE_DIR}/btkConfigure.h.in
               ${BTK_BINARY_DIR}/btkConfigure.h @ONLY IMMEDIATE)

# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of BTK.
IF(WIN32)
  SET(BTK_LIBRARY_PREFIX "btk")
ELSE()
  SET(BTK_LIBRARY_PREFIX "libbtk")
ENDIF()
SET(BTK_LIBRARY_PROPERTIES 
  VERSION   "${BTK_VERSION_MAJOR}.${BTK_VERSION_MINOR}.${BTK_VERSION_PATCH}"
  SOVERSION "${BTK_VERSION_MAJOR}.${BTK_VERSION_MINOR}"
  PREFIX    "${BTK_LIBRARY_PREFIX}"
  DEBUG_POSTFIX _debug)

# ------------------------------------------------------------------------
# INSTALLATION VARIABLES
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# PROJECT'S SUBDIRECTORIES
# ------------------------------------------------------------------------

# Could be use by internal targets like TDD
SET(BTK_INCLUDE_DIRS_BUILD_TREE
  "${BTK_SOURCE_DIR}/Code"
  "${BTK_SOURCE_DIR}/Code/Base"
  "${BTK_SOURCE_DIR}/Code/IO"
  "${BTK_SOURCE_DIR}/Code/Model"
  "${BTK_SOURCE_DIR}/Code/Trial"
  "${BTK_SOURCE_DIR}/ThirdParties/eigen3"
  "${BTK_SOURCE_DIR}/Utilities")

# Code
ADD_SUBDIRECTORY(Code)

# TDD (Test-driven development) code
OPTION(BUILD_TESTING "Build BTK unit and regression tests." OFF)
IF(BUILD_TESTING)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)

# Extra library required (e.g. eigen3, CxxTest, Open3DMotion)
ADD_SUBDIRECTORY(ThirdParties)

# ------------------------------------------------------------------------
# INSTALLATION INSTRUCTIONS
# ------------------------------------------------------------------------

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
EXPORT(PACKAGE BTK)
SET(BTK_CMAKE_CONFIG_PACKAGE_LOCATION lib/cmake/BTK CACHE STRING "CMake scripts installation path")
SET(BTK_INCLUDE_INSTALL_DIR include/ CACHE STRING "BTK development headers inclusion path")

# Create the BTKConfig.cmake and BTKConfigVersion.cmake files
SET(CONF_INCLUDE_DIRS "${BTK_INCLUDE_DIRS_BUILD_TREE}")
CONFIGURE_PACKAGE_CONFIG_FILE(CMake/BTKConfig.cmake.in "${PROJECT_BINARY_DIR}/BTKConfig.cmake"
  INSTALL_DESTINATION ${BTK_CMAKE_CONFIG_PACKAGE_LOCATION}
  PATH_VARS BTK_INCLUDE_INSTALL_DIR)
WRITE_BASIC_PACKAGE_VERSION_FILE(
  "${PROJECT_BINARY_DIR}/BTKConfigVersion.cmake"
  VERSION ${BTK_VERSION}
  COMPATIBILITY SameMajorVersion
)
EXPORT(EXPORT BTKTargets
  FILE "${PROJECT_BINARY_DIR}/BTKTargets.cmake"
  NAMESPACE BTK::
)

INSTALL(EXPORT BTKTargets
  FILE
    BTKTargets.cmake
  NAMESPACE
    BTK::
  DESTINATION
    ${BTK_CMAKE_CONFIG_PACKAGE_LOCATION}
)
INSTALL(
  FILES
    "${PROJECT_BINARY_DIR}/BTKConfig.cmake"
    "${PROJECT_BINARY_DIR}/BTKConfigVersion.cmake"
  DESTINATION
    ${BTK_CMAKE_CONFIG_PACKAGE_LOCATION}
  COMPONENT
    dev
)