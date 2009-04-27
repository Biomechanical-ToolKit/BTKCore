# - Try to find a version of Matlab and headers/library required by the 
#   used compiler. It determines the right MEX-File extension.
# This module detects a Matlab's version between Matlab 7.0 (R14) and 
# Matlab 7.9 (r2009b).
#
# This module defines: 
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
#
# This module was tested with Matlab 7.0, 7.4 and 7.7 on a win32 system 
# with the compiler cl 9.0 (Microsoft Visual C++ Express edition 2008).

# Copyright (c) 2009 Arnaud Barré <arnaud.barre@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
   # in cache already
   SET(Matlab_FIND_QUIETLY TRUE)
ENDIF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)

IF(WIN32)
  SET(MATLAB_PATHS 
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.5;MATLABROOT]"
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

	MESSAGE(STATUS "FindMatlab.cmake needs a way to determine the Matlab version on Linux/Apple.\nYou should need to set MATLAB_ENG_LIBRARY, MATLAB_MX_LIBRARY, MATLAB_MEX_LIBRARY\n and MATLAB_INCLUDE_DIR manualy.")
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

	SET(MATLAB_LIBRARIES_PATHS
    ${MATLAB_ROOT})
	SET(MATLAB_INCLUDE_PATHS 
		"/usr/local/${MATLAB_VERSION}/extern/include/"
    "/opt/${MATLAB_VERSION}/extern/include/"
    "$ENV{HOME}/${MATLAB_VERSION}/extern/include/"
    "$ENV{HOME}/redhat-matlab/extern/include/")
ENDIF(WIN32)

FIND_LIBRARY(MATLAB_MEX_LIBRARY
    libmex
    ${MATLAB_LIBRARIES_PATHS}
    )
FIND_LIBRARY(MATLAB_MX_LIBRARY
    libmx
    ${MATLAB_LIBRARIES_PATHS}
    )
FIND_LIBRARY(MATLAB_ENG_LIBRARY
    libeng
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
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Matlab DEFAULT_MSG MATLAB_INCLUDE_DIR MATLAB_MEX_LIBRARY MATLAB_MX_LIBRARY MATLAB_ENG_LIBRARY)

MARK_AS_ADVANCED(
	MATLAB_ROOT
	MATLAB_MEX_LIBRARY
	MATLAB_MX_LIBRARY
  MATLAB_ENG_LIBRARY
	MATLAB_LIBRARIES
  MATLAB_INCLUDE_DIR
	MATLAB_MEXFILE_EXT:
)
