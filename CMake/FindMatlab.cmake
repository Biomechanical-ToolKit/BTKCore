# - Try to find a version of Matlab
# This module defines:
#  MATLAB_INCLUDE_DIR: include path for mex.h, engine.h
#  MATLAB_LIBRARIES:   required libraries: libmex, etc
#  MATLAB_MEX_LIBRARY: path to libmex.lib
#  MATLAB_MX_LIBRARY:  path to libmx.lib
#  MATLAB_ENG_LIBRARY: path to libeng.lib
#  MATLAB_CREATE_MEX: macro to build a MEX-file

# Copyright (c) 2009 Arnaud Barré <arnaud.barre@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

SET(MATLAB_FOUND 0)
IF(WIN32)
  SET(MATLAB_PATHS 
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.5;MATLABROOT]"
		)
  FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS})
	#MESSAGE(STATUS "MATLAB_ROOT: ${MATLAB_ROOT}")

  FIND_LIBRARY(MATLAB_MEX_LIBRARY
    libmex
    "${MATLAB_ROOT}/extern/lib/win32/microsoft"
    )
  FIND_LIBRARY(MATLAB_MX_LIBRARY
    libmx
    "${MATLAB_ROOT}/extern/lib/win32/microsoft"
    )
  FIND_LIBRARY(MATLAB_ENG_LIBRARY
    libeng
    "${MATLAB_ROOT}/extern/lib/win32/microsoft"
    )

  FIND_PATH(MATLAB_INCLUDE_DIR
    "mex.h"
    "${MATLAB_ROOT}/extern/include"
    )
	#MESSAGE(STATUS "MATLAB_INCLUDE_DIR ${MATLAB_INCLUDE_DIR}")

	# MEX files extension
	IF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$")
		SET(MATLAB_MEXFILE_EXT mexw32)
	ELSE(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$")
		MESSAGE("Matlab Mex files are only supported by MS Visual Studio")
	ENDIF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$")
ELSE(WIN32)
	# MEX files extension
  IF(APPLE)
		IF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
			SET(MATLAB_MEXFILE_EXT mexmaci)
		ELSE(CMAKE_OSX_ARCHITECTURES MATCHES i386)
			SET(MATLAB_MEXFILE_EXT mexmac)
		ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
 	ELSE(APPLE)
		SET(MATLAB_MEXFILE_EXT mexglx)
	ENDIF(APPLE)

	MESSAGE(FATAL_ERROR "Need a way to determine the Matlab version on Linux/Apple")
  SET(MATLAB_VERSION "Matlab-7.8")
  IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # Regular x86
    SET(MATLAB_ROOT
      /usr/local/${MATLAB_VERSION}/bin/glnx86/
      /opt/${MATLAB_VERSION}/bin/glnx86/
      $ENV{HOME}/${MATLAB_VERSION}/bin/glnx86/
      $ENV{HOME}/redhat-matlab/bin/glnx86/
      )
  ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # AMD64:
    SET(MATLAB_ROOT
      /usr/local/${MATLAB_VERSION}/bin/glnxa64/
      /opt/${MATLAB_VERSION}/bin/glnxa64/
      $ENV{HOME}/matlab7_64/bin/glnxa64/
      $ENV{HOME}/${MATLAB_VERSION}/bin/glnxa64/
      $ENV{HOME}/redhat-matlab/bin/glnxa64/
      )
  ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  FIND_LIBRARY(MATLAB_MEX_LIBRARY	q
    mex
    ${MATLAB_ROOT}
    )
  FIND_LIBRARY(MATLAB_MX_LIBRARY
    mx
    ${MATLAB_ROOT}
    )
  FIND_LIBRARY(MATLAB_ENG_LIBRARY
    eng
    ${MATLAB_ROOT}
    )
  FIND_PATH(MATLAB_INCLUDE_DIR
    "mex.h"
    "/usr/local/${MATLAB_VERSION}/extern/include/"
    "/opt/${MATLAB_VERSION}/extern/include/"
    "$ENV{HOME}/${MATLAB_VERSION}/extern/include/"
    "$ENV{HOME}/redhat-matlab/extern/include/"
    )
ENDIF(WIN32)

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

IF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
  SET(MATLAB_FOUND 1)
ENDIF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)

MARK_AS_ADVANCED(
  MATLAB_LIBRARIES
  MATLAB_MEX_LIBRARY
  MATLAB_MX_LIBRARY
  MATLAB_ENG_LIBRARY
  MATLAB_INCLUDE_DIR
	MATLAB_MEXFILE_EXT
  MATLAB_FOUND
  MATLAB_ROOT
)

