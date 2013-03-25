# - Try to find a version of Matlab and headers/library required by the 
#   used compiler. It determines the right MEX-File extension and add 
#   a macro to help the build of MEX-functions.
#
# This module detects a Matlab's version between Matlab 7.0 (R14)
# and Matlab 8.0 (r2012b).
#
# This module defines: 
#  MATLAB_ROOT:               Matlab installation path
#  MATLAB_INCLUDE_DIR:        Include path for mex.h, engine.h
#  MATLAB_MEX_LIBRARY:        Path to libmex.lib
#  MATLAB_MX_LIBRARY:         Path to libmx.lib
#  MATLAB_ENG_LIBRARY:        Path to libeng.lib
#  MATLAB_LIBRARIES:          Required libraries: libmex, libmx, libeng
#  MATLAB_MEXFILE_EXT:        MEX extension required for the current platform
#  MATLAB_CREATE_MEX:         Macro to build a MEX-file
#  MATLAB_MATLABR2010B_FOUND: Variable only available under Windows (used to fix compilation issue with MSVC 2010)
#
# The macro MATLAB_CREATE_MEX requires in this order:
#  - function's name which will be called in Matlab;
#  - C/C++ source files;
#  - third libraries required.

# Copyright (c) 2009-2013 Arnaud Barré <arnaud.barre@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF(MATLAB_ROOT AND MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
   # in cache already
   SET(Matlab_FIND_QUIETLY TRUE)
ENDIF(MATLAB_ROOT AND MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)

IF(WIN32)
  # Default paths
  SET(MATLAB_PATHS 
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.0;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.15;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.14;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.13;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.12;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.11;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.10;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.9;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.8;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.5;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.4;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.3;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.2;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.1;MATLABROOT]"
  )
  FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS} NO_DEFAULT_PATH)
  # Paths for Matlab 32-bit under Windows 64-bit
  IF(NOT MATLAB_ROOT)
    SET(MATLAB_PATHS 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.15;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.14;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.13;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.12;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.11;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.10;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.9;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.8;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.7;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.6;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.5;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.4;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.3;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.2;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.1;MATLABROOT]"
    )
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS} NO_DEFAULT_PATH)
  ENDIF(NOT MATLAB_ROOT)
  IF(NOT MATLAB_ROOT)
    SET(MATLAB_PATHS 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\6.5;MATLABROOT]"
    )
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS} NO_DEFAULT_PATH)
    IF(MATLAB_ROOT)
      SET(MATLAB_OLD_WIN_MEXFILE_EXT 1 CACHE STRING "Old MEX extension for Windows")
    ENDIF(MATLAB_ROOT)
  ENDIF(NOT MATLAB_ROOT)
  
  FIND_PATH(MATLABR2010B_TEMP "license.txt" "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.11;MATLABROOT]" NO_DEFAULT_PATH)
  IF(MATLABR2010B_TEMP)
    SET(MATLAB_MATLABR2010B_FOUND 1)
  ENDIF(MATLABR2010B_TEMP)
  
  SET(MATLAB_LIBRARIES_PATHS
      "${MATLAB_ROOT}/extern/lib/win64/microsoft"
      "${MATLAB_ROOT}/extern/lib/win32/microsoft"
      "${MATLAB_ROOT}/extern/lib/win32/microsoft/msvc70"
      "${MATLAB_ROOT}/extern/lib/win32/microsoft/msvc71")
  SET(MATLAB_INCLUDE_PATHS "${MATLAB_ROOT}/extern/include")

  # MEX files extension
  IF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$" OR CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    IF(MATLAB_OLD_WIN_MEXFILE_EXT)
      SET(MATLAB_MEXFILE_EXT dll)
    ELSE(MATLAB_OLD_WIN_MEXFILE_EXT)
      IF(CMAKE_CL_64)
        SET(MATLAB_MEXFILE_EXT mexw64)
      ELSE(CMAKE_CL_64)
        SET(MATLAB_MEXFILE_EXT mexw32)
      ENDIF(CMAKE_CL_64)
    ENDIF (MATLAB_OLD_WIN_MEXFILE_EXT)
  ELSE(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$" OR CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    MESSAGE(FATAL_ERROR "Matlab Mex files are only supported by MS Visual Studio")
  ENDIF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$" OR CMAKE_CXX_COMPILER MATCHES "^.*cl$")
  #MESSAGE(STATUS "MATLAB_MEXFILE_EXT: ${MATLAB_MEXFILE_EXT}")

  # LIBMEX, LIBMX, LIBENG names
  SET(LIBMEX "libmex")
  SET(LIBMX "libmx")
  SET(LIBENG "libeng")
ELSE(WIN32)
  # MEX files extension
  IF(APPLE)
    FILE(GLOB MATLAB_PATHS "/Applications/MATLAB_*")
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS})
    IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
      IF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
        SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/mac")
          SET(MATLAB_MEXFILE_EXT mexmac)
        ELSE(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
        SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/maci")
          SET(MATLAB_MEXFILE_EXT mexmaci)
        ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
    ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
      # To force the compilation in 32-bit
      IF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
        MESSAGE("The compilation of the Matlab MEX functions is forced to 32-bit mode as the architecture is set to i386.")
        SET(FORCE_COMPILE_32BIT_MODE 1)
      ELSE(CMAKE_OSX_ARCHITECTURES MATCHES i386)
        IF(EXISTS "${MATLAB_ROOT}/bin/maci64/")
          SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/maci64")
          SET(MATLAB_MEXFILE_EXT mexmaci64)
          SET(FORCE_COMPILE_32BIT_MODE 0)
        ELSE(EXISTS "${MATLAB_ROOT}/bin/maci64/")
          MESSAGE("You use a Matlab 32-bit version on a 64-bit OS. BTK will be compiled in 32-bit to be useable with your Matlab version.")
          SET(FORCE_COMPILE_32BIT_MODE 1)
        ENDIF(EXISTS "${MATLAB_ROOT}/bin/maci64/")
      ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
      IF(FORCE_COMPILE_32BIT_MODE)
        SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/maci")
        SET(MATLAB_MEXFILE_EXT mexmaci)
        SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG -m32" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        SET(CMAKE_C_FLAGS "-O3 -DNDEBUG -m32" CACHE STRING "Flags used by the compiler during all build types." FORCE)
      ENDIF(FORCE_COMPILE_32BIT_MODE)
    ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  
    SET(LIBMEX "libmex.dylib")
    SET(LIBMX "libmx.dylib")
    SET(LIBENG "libeng.dylib")
  ELSE(APPLE)
    FILE(GLOB MATLAB_LOCAL_PATHS "/usr/local/[Mm][Aa][Tt][Ll][Aa][Bb]*")
    FILE(GLOB MATLAB_LOCAL_PATHS_BIS "/usr/local/[Mm][Aa][Tt][Ll][Aa][Bb]/R*")
    FILE(GLOB MATLAB_OPT_PATHS "/opt/[Mm][Aa][Tt][Ll][Aa][Bb]*")
    FILE(GLOB MATLAB_USR_PATHS "/usr/[Mm][Aa][Tt][Ll][Aa][Bb]*")
    SET(MATLAB_PATHS ${MATLAB_LOCAL_PATHS} ${MATLAB_LOCAL_PATHS_BIS} ${MATLAB_OPT_PATHS} ${MATLAB_USR_PATHS})
  
    SET(LIBMEX "libmex.so")
    SET(LIBMX "libmx.so")
    SET(LIBENG "libeng.so")
  
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS})
    IF(NOT MATLAB_ROOT)
      MESSAGE("Linux users have to set the Matlab installation path into the MATLAB_ROOT variable.")
    ENDIF(NOT MATLAB_ROOT)
  
    IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
      # Regular x86
      SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/glnx86")
      SET(MATLAB_MEXFILE_EXT mexglx)
    ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
      # AMD64:
      SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/glnxa64")
      SET(MATLAB_MEXFILE_EXT mexa64)
    ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  ENDIF(APPLE)
  
  # Common for UNIX
  SET(MATLAB_INCLUDE_PATHS "${MATLAB_ROOT}/extern/include")
ENDIF(WIN32)

FIND_LIBRARY(MATLAB_MEX_LIBRARY
    ${LIBMEX}
    ${MATLAB_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_LIBRARY(MATLAB_MX_LIBRARY
    ${LIBMX}
    ${MATLAB_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_LIBRARY(MATLAB_ENG_LIBRARY
    ${LIBENG}
    ${MATLAB_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_PATH(MATLAB_INCLUDE_DIR
    "mex.h"
    ${MATLAB_INCLUDE_PATHS} NO_DEFAULT_PATH
    )

# This is common to UNIX and Win32:
SET(MATLAB_LIBRARIES
  ${MATLAB_MEX_LIBRARY}
  ${MATLAB_MX_LIBRARY}
  ${MATLAB_ENG_LIBRARY}  
)

# Fix for Matlab 2007b under MacOS X
IF(APPLE)
  FIND_FILE(MATLAB_BOOST_FILESYSTEM_LIBRARY "libboost_filesystem-1_33_1.dylib" "${MATLAB_ROOT}/bin/maci/")
  IF(MATLAB_BOOST_FILESYSTEM_LIBRARY)
    SET(MATLAB_LIBRARIES
      ${MATLAB_LIBRARIES}
      ${MATLAB_BOOST_FILESYSTEM_LIBRARY}
    )
  ENDIF(MATLAB_BOOST_FILESYSTEM_LIBRARY)
ENDIF(APPLE)

# Macros for building MEX-files
MACRO(MATLAB_EXTRACT_SOURCES_LIBRARIES sources thirdlibraries)
  SET(${sources})
  SET(${thirdlibraries})
  FOREACH(_arg ${ARGN})
    GET_FILENAME_COMPONENT(_ext ${_arg} EXT)
    IF("${_ext}" STREQUAL "")
      LIST(APPEND ${thirdlibraries} "${_arg}")
    ELSE("${_ext}" STREQUAL "")
      LIST(APPEND ${sources} "${_arg}")
    ENDIF ("${_ext}" STREQUAL "")
  ENDFOREACH(_arg)
ENDMACRO(MATLAB_EXTRACT_SOURCES_LIBRARIES)

# MATLAB_MEX_CREATE(functionname inputfiles thridlibraries)
MACRO(MATLAB_MEX_CREATE functionname)
  MATLAB_EXTRACT_SOURCES_LIBRARIES(sources thirdlibraries ${ARGN})
  ADD_LIBRARY(${functionname} SHARED ${sources} mexFunction.def)
  TARGET_LINK_LIBRARIES(${functionname} ${MATLAB_LIBRARIES} ${thirdlibraries})
  SET_TARGET_PROPERTIES(${functionname} PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
  )
ENDMACRO(MATLAB_MEX_CREATE)

IF(MATLAB_ROOT)
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Matlab DEFAULT_MSG MATLAB_ROOT MATLAB_INCLUDE_DIR MATLAB_MEX_LIBRARY MATLAB_MX_LIBRARY MATLAB_ENG_LIBRARY)
ENDIF(MATLAB_ROOT)

MARK_AS_ADVANCED(
  MATLAB_MEX_LIBRARY
  MATLAB_MX_LIBRARY
  MATLAB_ENG_LIBRARY
  MATLAB_LIBRARIES
  MATLAB_INCLUDE_DIR
  MATLAB_MEXFILE_EXT
  MATLAB_OLD_WIN_MEXFILE_EXT
)

IF(WIN32)
  MARK_AS_ADVANCED(MATLAB_MATLABR2010B_FOUND)
ENDIF(WIN32)
