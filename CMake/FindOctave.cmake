# - Try to find a version of Octave and headers/library required by the 
#   used compiler. It determines the right MEX-File extension and add 
#   a macro to help the build of MEX-functions.
#
# This module defines: 
#  OCTAVE_EXECUTABLE:          Path to the Matlab executable
#  OCTAVE_INCLUDE_DIR:         Include path for mex.h, mexproto.h
#  OCTAVE_OCTINTERP_LIBRARY:   Path to the library octinterp
#  OCTAVE_OCTAVE_LIBRARY:      Path to the library octave
#  OCTAVE_CRUFT_LIBRARY:       Oath to the library cruft
#  OCTAVE_LIBRARIES:           Required libraries: octinterp, octave, cruft
#  OCTAVE_CREATE_MEX:          Macro to build a MEX-file
#
# The macro OCTAVE_CREATE_MEX requires in this order:
#  - function's name which will be called in Octave;
#  - C/C++ source files;
#  - third libraries required.

# Copyright (c) 2009-2015 Arnaud Barré <arnaud.barre@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF(OCTAVE_ROOT AND OCTAVE_EXECUTABLE AND OCTAVE_INCLUDE_DIR AND OCTAVE_LIBRARIES)
  STRING(COMPARE NOTEQUAL "${OCTAVE_ROOT}" "${OCTAVE_ROOT_LAST}" OCTAVE_CHANGED)
  IF(OCTAVE_CHANGED)
    SET(OCTAVE_USE_MINGW32 OCTAVE_USE_MINGW32-NOTFOUND CACHE INTERNAL "")
    SET(OCTAVE_EXECUTABLE OCTAVE_EXECUTABLE-NOTFOUND CACHE INTERNAL "")
    SET(OCTAVE_OCTINTERP_LIBRARY OCTAVE_OCTINTERP_LIBRARY-NOTFOUND CACHE INTERNAL "")
    SET(OCTAVE_OCTAVE_LIBRARY OCTAVE_OCTAVE_LIBRARY-NOTFOUND CACHE INTERNAL "")
    SET(OCTAVE_CRUFT_LIBRARY OCTAVE_CRUFT_LIBRARY-NOTFOUND CACHE INTERNAL "")
    SET(OCTAVE_INCLUDE_DIR OCTAVE_INCLUDE_DIR-NOTFOUND CACHE INTERNAL "")
  ELSE(OCTAVE_CHANGED)
    # in cache already
    SET(Octave_FIND_QUIETLY TRUE)
  ENDIF(OCTAVE_CHANGED)
ENDIF(OCTAVE_ROOT AND OCTAVE_EXECUTABLE AND OCTAVE_INCLUDE_DIR AND OCTAVE_LIBRARIES)

SET(OCTAVE_MEXFILE_EXT mex)

IF(WIN32)
  SET(OCTAVE_PATHS_L1 )
  SET(OCTAVE_PATHS_L2 )
  # Level 0
  FILE(GLOB OCTAVE_PATHS_L0A "c:/Octave*")
  FILE(GLOB OCTAVE_PATHS_L0B "c:/Software/Octave*")
  SET(OCTAVE_PATHS_L0 ${OCTAVE_PATHS_L0A} ${OCTAVE_PATHS_L0B})
  # Level 1
  FOREACH(_file_ ${OCTAVE_PATHS_L0})
    FILE(GLOB OCTAVE_PATHS_TEMP "${_file_}/*")
    SET(OCTAVE_PATHS_L1 ${OCTAVE_PATHS_L1};${OCTAVE_PATHS_TEMP})
  ENDFOREACH(_file_ OCTAVE_PATHS_L0)
  # Level 2
  FOREACH(_file_ ${OCTAVE_PATHS_L1})
    FILE(GLOB OCTAVE_PATHS_TEMP "${_file_}/*")
    SET(OCTAVE_PATHS_L2 ${OCTAVE_PATHS_L2};${OCTAVE_PATHS_TEMP})
  ENDFOREACH(_file_ OCTAVE_PATHS_L1)
  # Merge levels
  SET(OCTAVE_PATHS ${OCTAVE_PATHS_L0} ${OCTAVE_PATHS_L1} ${OCTAVE_PATHS_L2})
  
  FIND_PATH(OCTAVE_ROOT "bin/octave.exe" ${OCTAVE_PATHS})
  FIND_PATH(OCTAVE_USE_MINGW32 "bin/mingw32-make.exe" "${OCTAVE_ROOT}/mingw32")
  
  IF(MSVC AND OCTAVE_USE_MINGW32)
    MESSAGE(FATAL_ERROR 
      "You must use the generator \"MinGW Makefiles\" as the "
      "version of Octave installed on your computer was compiled "
      "with MinGW. You should also specify the native compiler "
      "(GCC, G++ and GFortan) and add the path of MinGW in the "
      "environment variable PATH. Contact the developers of the "
      "project for more details")
  ENDIF(MSVC AND OCTAVE_USE_MINGW32)
  
  FILE(GLOB OCTAVE_INCLUDE_PATHS "${OCTAVE_ROOT}/include/octave-*/octave")
  FILE(GLOB OCTAVE_LIBRARIES_PATHS "${OCTAVE_ROOT}/lib/octave-*")
  IF (NOT OCTAVE_LIBRARIES_PATHS)
    FILE(GLOB OCTAVE_LIBRARIES_PATHS "${OCTAVE_ROOT}/lib/octave/*")
  ENDIF (NOT OCTAVE_LIBRARIES_PATHS)
  
  # LIBOCTINTERP, LIBOCTAVE, LIBCRUFT names
  IF(OCTAVE_USE_MINGW32)
    SET(LIBOCTINTERP "liboctinterp")
    SET(LIBOCTAVE "liboctave")
    SET(LIBCRUFT "libcruft")
  ELSE(OCTAVE_USE_MINGW32)
    SET(LIBOCTINTERP "octinterp")
    SET(LIBOCTAVE "octave")
    SET(LIBCRUFT "cruft")
  ENDIF(OCTAVE_USE_MINGW32)
  
ELSE(WIN32)
  IF(APPLE)
    FILE(GLOB OCTAVE_PATHS "/Applications/Octave*")
    FIND_PATH(OCTAVE_ROOT "Contents/Resources/bin/octave" ${OCTAVE_PATHS})

    FILE(GLOB OCTAVE_INCLUDE_PATHS "${OCTAVE_ROOT}/Contents/Resources/include/octave-*/octave")
    FILE(GLOB OCTAVE_LIBRARIES_PATHS "${OCTAVE_ROOT}/Contents/Resources/lib/octave-*")

    SET(LIBOCTINTERP "liboctinterp.dylib")
    SET(LIBOCTAVE "liboctave.dylib")
    SET(LIBCRUFT "libcruft.dylib")
  ELSE(APPLE)
    FILE(GLOB OCTAVE_LIB_LOCAL_PATHS "/usr/local/lib/octave-*")
    FILE(GLOB OCTAVE_LIB_USR_PATHS "/usr/lib/octave-*")
    FILE(GLOB OCTAVE_INCLUDE_USR_PATHS "/usr/include/octave-*")

    SET(OCTAVE_INCLUDE_PATHS 
      "/usr/local/include"
      "/usr/local/include/octave"
      "/usr/include"
      "/usr/include/octave"
      ${OCTAVE_INCLUDE_USR_PATHS})
    SET(OCTAVE_LIBRARIES_PATHS
      "/usr/lib/x86_64-linux-gnu"
      "/usr/local/lib"
      "/usr/local/lib/octave"
      ${OCTAVE_LIB_LOCAL_PATHS}
      "/usr/lib"
      "/usr/lib/octave"
      ${OCTAVE_LIB_USR_PATHS})  
    SET(OCTAVE_ROOT
      "/usr/bin"
      "/usr/local/bin")
      
    SET(LIBOCTINTERP "octinterp")
    SET(LIBOCTAVE "octave")
    SET(LIBCRUFT "cruft")
  ENDIF(APPLE)
ENDIF(WIN32)

FIND_PROGRAM(OCTAVE_EXECUTABLE
    "octave"
    PATHS ${OCTAVE_ROOT}
    PATH_SUFFIXES "bin" NO_DEFAULT_PATH
    )
FIND_LIBRARY(OCTAVE_OCTINTERP_LIBRARY
    ${LIBOCTINTERP}
    ${OCTAVE_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_LIBRARY(OCTAVE_OCTAVE_LIBRARY
    ${LIBOCTAVE}
    ${OCTAVE_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_LIBRARY(OCTAVE_CRUFT_LIBRARY
    ${LIBCRUFT}
    ${OCTAVE_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_PATH(OCTAVE_INCLUDE_DIR
    "mex.h"
    PATHS ${OCTAVE_INCLUDE_PATHS}
    PATH_SUFFIXES "octave" NO_DEFAULT_PATH
    )

SET(OCTAVE_ROOT_LAST "${OCTAVE_ROOT}" CACHE INTERNAL "" FORCE)

# This is common to UNIX and Win32:
SET(OCTAVE_LIBRARIES
  ${OCTAVE_OCTINTERP_LIBRARY}
  ${OCTAVE_OCTAVE_LIBRARY}
  ${OCTAVE_CRUFT_LIBRARY}
  CACHE INTERNAL "Octave libraries" FORCE 
)

# Macros for building MEX-files
MACRO(OCTAVE_EXTRACT_SOURCES_LIBRARIES sources thirdlibraries)
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
ENDMACRO(OCTAVE_EXTRACT_SOURCES_LIBRARIES)

# OCTAVE_MEX_CREATE(functionname inputfiles thridlibraries)
MACRO(OCTAVE_MEX_CREATE functionname)
  OCTAVE_EXTRACT_SOURCES_LIBRARIES(sources thirdlibraries ${ARGN})
  ADD_LIBRARY(${functionname} SHARED ${sources} mexFunction.def)
  TARGET_LINK_LIBRARIES(${functionname} ${OCTAVE_LIBRARIES} ${thirdlibraries})
  SET_TARGET_PROPERTIES(${functionname} PROPERTIES
    PREFIX ""
    SUFFIX  ".${OCTAVE_MEXFILE_EXT}"
  )
ENDMACRO(OCTAVE_MEX_CREATE)

INCLUDE(FindPackageHandleStandardArgs)

# The variable OCTAVE_ROOT is only relevant for WIN32
IF(WIN32)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Octave DEFAULT_MSG OCTAVE_ROOT OCTAVE_EXECUTABLE OCTAVE_INCLUDE_DIR OCTAVE_OCTINTERP_LIBRARY OCTAVE_OCTAVE_LIBRARY OCTAVE_CRUFT_LIBRARY)
ELSE(WIN32)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Octave DEFAULT_MSG OCTAVE_EXECUTABLE OCTAVE_INCLUDE_DIR OCTAVE_OCTINTERP_LIBRARY OCTAVE_OCTAVE_LIBRARY OCTAVE_CRUFT_LIBRARY)
ENDIF(WIN32)

MARK_AS_ADVANCED(
  OCTAVE_EXECUTABLE
  OCTAVE_OCTINTERP_LIBRARY
  OCTAVE_OCTAVE_LIBRARY
  OCTAVE_CRUFT_LIBRARY
  OCTAVE_LIBRARIES
  OCTAVE_INCLUDE_DIR
)
