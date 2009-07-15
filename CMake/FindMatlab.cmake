# - Try to find a version of Matlab and headers/library required by the 
#   used compiler. It determines the right MEX-File extension and add 
#   a macro to help the build of MEX-functions.
#
# For Windows, this module detects a Matlab's version between Matlab 7.0 
# (R14) and Matlab 7.9 (r2009b). 
# The Unix part of this module doesn't detect the Matlab version. To use it,
# it is necessary to set the MATLAB_ROOT with the path of the Matlab
# installation.
#
# This module defines: 
#  MATLAB_ROOT: Matlab installation path
#  MATLAB_INCLUDE_DIR: include path for mex.h, engine.h
#  MATLAB_MEX_LIBRARY: path to libmex.lib
#  MATLAB_MX_LIBRARY:  path to libmx.lib
#  MATLAB_ENG_LIBRARY: path to libeng.lib
#  MATLAB_LIBRARIES:   required libraries: libmex, libmx, libeng
#  MATLAB_MEXFILE_EXT: MEX extension required for the current plateform
#  MATLAB_CREATE_MEX: macro to build a MEX-file
#
# The macro MATLAB_CREATE_MEX requires in this order:
#  - function's name which will be called in Matlab;
#  - C/C++ source files;
#  - third libraries required.

# Copyright (c) 2009 Arnaud Barré <arnaud.barre@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF(MATLAB_ROOT AND MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
   # in cache already
   SET(Matlab_FIND_QUIETLY TRUE)
ENDIF(MATLAB_ROOT AND MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)

IF(WIN32)
  SET(MATLAB_PATHS 
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
  FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS})
  IF (${MATLAB_ROOT} STREQUAL "")
    FIND_PATH(MATLAB_ROOT "license.txt" 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.0;MATLABROOT]")
    IF (${MATLAB_ROOT}  NOT STREQUAL "")
      SET(OLD_WIN_MEXFILE_EXT 1)
    ENDIF (${MATLAB_ROOT} NOT STREQUAL "")
  ENDIF (${MATLAB_ROOT} STREQUAL "")
  #MESSAGE(STATUS "MATLAB_ROOT: ${MATLAB_ROOT}")
  
  SET(MATLAB_LIBRARIES_PATHS
    "${MATLAB_ROOT}/extern/lib/win32/microsoft"
    "${MATLAB_ROOT}/extern/lib/win32/microsoft/msvc71")
  SET(MATLAB_INCLUDE_PATHS 
    "${MATLAB_ROOT}/extern/include")

  # MEX files extension
  IF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$")
    IF(OLD_WIN_MEXFILE_EXT)
      SET(MATLAB_MEXFILE_EXT dll)
    ELSE(OLD_WIN_MEXFILE_EXT)
      SET(MATLAB_MEXFILE_EXT mexw32)
    ENDIF (OLD_WIN_MEXFILE_EXT)
  ELSE(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$")
    MESSAGE("Matlab Mex files are only supported by MS Visual Studio")
  ENDIF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$")

  # LIBMEX, LIBMX, LIBENG names
  SET(LIBMEX "libmex")
  SET(LIBMX "libmx")
  SET(LIBENG "libeng")
ELSE(WIN32)
  # MEX files extension
  IF(APPLE)
    IF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
      SET(MATLAB_MEXFILE_EXT mexmaci)
    ELSE(CMAKE_OSX_ARCHITECTURES MATCHES i386)
      SET(MATLAB_MEXFILE_EXT mexmac)
    ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
    SET(LIBMEX "libmex.dylib")
    SET(LIBMX "libmx.dylib")
    SET(LIBENG "libeng.dylib")
   ELSE(APPLE)
    SET(MATLAB_MEXFILE_EXT mexglx)
    SET(LIBMEX "libmex.so")
    SET(LIBMX "libmx.so")
    SET(LIBENG "libeng.so")
  ENDIF(APPLE)

  FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS})
  IF("${MATLAB_ROOT}" STREQUAL "MATLAB_ROOT-NOTFOUND")
    MESSAGE(FATAL_ERROR "On Linux/MacOS system, it is required to set the Matlab installation path in the MATLAB_ROOT variable")
  ENDIF("${MATLAB_ROOT}" STREQUAL "MATLAB_ROOT-NOTFOUND")
  IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # Regular x86
    SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/glnx86")
  ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # AMD64:
    SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/glnxa64")
  ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)

  SET(MATLAB_INCLUDE_PATHS "${MATLAB_ROOT}/extern/include/")
ENDIF(WIN32)

FIND_LIBRARY(MATLAB_MEX_LIBRARY
    ${LIBMEX}
    ${MATLAB_LIBRARIES_PATHS}
    )
FIND_LIBRARY(MATLAB_MX_LIBRARY
    ${LIBMX}
    ${MATLAB_LIBRARIES_PATHS}
    )
FIND_LIBRARY(MATLAB_ENG_LIBRARY
    ${LIBENG}
    ${MATLAB_LIBRARIES_PATHS}
    )
FIND_PATH(MATLAB_INCLUDE_DIR
    "mex.h"
    ${MATLAB_INCLUDE_PATHS}
    )

# This is common to UNIX and Win32:
SET(MATLAB_LIBRARIES
  ${MATLAB_MEX_LIBRARY}
  ${MATLAB_MX_LIBRARY}
  ${MATLAB_ENG_LIBRARY}
)

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

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Matlab DEFAULT_MSG MATLAB_ROOT MATLAB_INCLUDE_DIR MATLAB_MEX_LIBRARY MATLAB_MX_LIBRARY MATLAB_ENG_LIBRARY)

MARK_AS_ADVANCED(
  MATLAB_ROOT
  MATLAB_MEX_LIBRARY
  MATLAB_MX_LIBRARY
  MATLAB_ENG_LIBRARY
  MATLAB_LIBRARIES
  MATLAB_INCLUDE_DIR
  MATLAB_MEXFILE_EXT
)
