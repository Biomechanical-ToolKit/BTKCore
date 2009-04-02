# First release build with CMAKE 2.6.3.
# The prior versions of CMAKE have not been tested
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3)
# BTK: Biomechanical ToolKit
PROJECT(BTK)

# BTK version number. An even minor number corresponds to releases.
SET(BTK_VERSION_MAJOR 0)
SET(BTK_VERSION_MINOR 1)
SET(BTK_VERSION_PATCH 0)

# Load some macros.
#INCLUDE(${CMAKE_ROOT}/Modules/CMakeDependentOption.cmake)
INCLUDE(CMakeDependentOption)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)

# Output directories.
IF(NOT LIBRARY_OUTPUT_PATH)
  SET (LIBRARY_OUTPUT_PATH ${BTK_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET (EXECUTABLE_OUTPUT_PATH ${BTK_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
SET(BTK_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(BTK_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

SET(CMAKE_MODULE_PATH ${BTK_SOURCE_DIR}/CMake)

# BTK_USE_SYSTEM_EIGEN2 is provided so that you can use an installed or external
# version of eigen2. If its on, you must specify the variables 
OPTION(BTK_USE_SYSTEM_EIGEN2 "Use an outside build of Eigen2." OFF)
MARK_AS_ADVANCED(BTK_USE_SYSTEM_EIGEN2)
IF(BTK_USE_SYSTEM_EIGEN2)
  FIND_PACKAGE(Eigen2)
  IF(EIGEN2_FOUND)
    INCLUDE(${EIGEN2_INCLUDE_DIR})
    SET(BTK_EIGEN2_LIBRARIES eigen2)
  ELSE(EIGEN2_FOUND)
    MESSAGE(FATAL_ERROR "Must set EIGEN2_DIR for BTK_USE_SYSTEM_EIGEN2.")
  ENDIF(EIGEN2_FOUND)
ELSE(BTK_USE_SYSTEM_EIGEN2)
  SET(BTK_EIGEN2_LIBRARIES btkeigen2)
  SET(EIGEN2_INCLUDE_DIR
    ${BTK_SOURCE_DIR}/Utilities/eigen2
    )
ENDIF(BTK_USE_SYSTEM_EIGEN2)

# The entire BTK tree should use the same include path
# Create the list of include directories needed for BTK header files.
INCLUDE(${CMAKE_MODULE_PATH}/btkIncludeDirectories.cmake)
# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${BTK_INCLUDE_DIRS_BUILD_TREE}
  ${BTK_INCLUDE_DIRS_SYSTEM}
)

# Find the header memory.h in specific path.
# This header contians the share_ptr class.
CHECK_INCLUDE_FILE_CXX(memory HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CXX(tr1/memory HAVE_TR1_MEMORY_H)
CHECK_INCLUDE_FILE(boost/memory.hpp HAVE_BOOST_MEMORY_HPP)
CHECK_INCLUDE_FILE(boost/tr1/memory.hpp HAVE_BOOST_TR1_MEMORY_HPP)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)


ADD_SUBDIRECTORY(Code)

# BTK TDD (Test-driven development) code
OPTION(BUILD_TESTING "Build BTK Tests" OFF)
IF(BUILD_TESTING)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(Test)
ENDIF(BUILD_TESTING)

# Wrapping
ADD_SUBDIRECTORY(Wrapping)
OPTION(BTK_WRAP_MATLAB "Wrap BTK classes into Matlab functions." OFF)
IF(BTK_WRAP_MATLAB)
  ADD_SUBDIRECTORY(Wrapping/Matlab)
ENDIF(BTK_WRAP_MATLAB)

# BTK documentation
OPTION(BUILD_DOCUMENTATION "Build BTK Documentation." OFF)
IF(BUILD_DOCUMENTATION)
  ADD_SUBDIRECTORY(Utilities/Doxygen)
ENDIF(BUILD_DOCUMENTATION)
