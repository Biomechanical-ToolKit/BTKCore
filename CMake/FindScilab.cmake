# - Try to find a version of Scilab and headers/library required by the 
#   used compiler.
#
# For Windows, this module detects a Matlab's version between Matlab 7.0 
# (R14) and Matlab 7.9 (r2009b). 
# The MacOS X part of this module detect if a Matlab version exist based on the 
# name of the applications in /Applications.
# The Linux part of this module doesn't detect the Matlab version. To use it,
# it is necessary to set the SCILAB_ROOT with the path of the Matlab
# installation.
#
# This module defines: 
#  SCILAB_ROOT: Scilab installation path
#  SCILAB_MEX_INCLUDE_DIR: include path for mex.h (used for libmex)
#  SCILAB_CORE_INCLUDE_DIR: main headers
#  SCILAB_MEX_LIBRARY: path to libmex.lib
#  SCILAB_MX_LIBRARY:  path to libmx.lib
#  SCILAB_LIBRARIES:   required libraries: libmex, libmx

# Copyright (c) 2009 Arnaud Barré <arnaud.barre@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF(SCILAB_ROOT AND SCILAB_LIBRARIES)
   # in cache already
   SET(Scilab_FIND_QUIETLY TRUE)
ENDIF(SCILAB_ROOT AND SCILAB_LIBRARIES)

IF(WIN32)
  GET_FILENAME_COMPONENT(SCILAB_VER  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Scilab;LASTINSTALL]" NAME)
  FIND_PATH(SCILAB_ROOT "etc/scilab.start" "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Scilab\\${SCILAB_VER};SCIPATH]")
  # LIBMEX, LIBMX names
  SET(LIBMEX "libmex.dll")
  SET(LIBMX "libmx.dll")
ELSEIF(APPLE)
  FILE(GLOB SCILAB_PATHS "/Applications/Scilab*")
  SET(LIBMEX "libmex.dylib")
  SET(LIBMX "libmx.dylib")
ELSE(WIN32)
  SET(SCILAB_PATHS 
    "/usr/share/scilab"
    "/usr/local/share/scilab"
    "/opt/local/share/scilab")
  SET(SCILAB_MODULE_PATH 
    "/usr/include/scilab"
    "/usr/local/include/scilab"
    "/opt/local/include/scilab")
  SET(SCILAB_LIBRARIES_PATH 
    "/usr/lib/scilab"
    "/usr/local/lib/scilab"
    "/opt/local/lib/scilab")
  SET(LIBMEX "libmex.so")
  SET(LIBMX "libmx.so")
ENDIF(WIN32)

FIND_PATH(SCILAB_ROOT "etc/scilab.start" ${SCILAB_PATHS})

IF(WIN32 OR APPLE)
  FIND_PATH(SCILAB_ROOT "etc/scilab.start" ${SCILAB_PATHS})
  SET(SCILAB_MODULE_PATH "${SCILAB_ROOT}/modules")
  SET(SCILAB_LIBRARIES_PATH "${SCILAB_ROOT}/bin")
ENDIF(WIN32 OR APPLE)

FIND_LIBRARY(SCILAB_IMPL_LIBRARY
    "LibScilab"
    ${SCILAB_LIBRARIES_PATH} NO_DEFAULT_PATH
    )
FIND_FILE(SCILAB_MEX_LIBRARY
    ${LIBMEX}
    ${SCILAB_LIBRARIES_PATH} NO_DEFAULT_PATH
    )
FIND_FILE(SCILAB_MX_LIBRARY
    ${LIBMX}
    ${SCILAB_LIBRARIES_PATH} NO_DEFAULT_PATH
    )
FIND_PATH(SCILAB_MEX_INCLUDE_DIR
    "mex.h"
    "${SCILAB_MODULE_PATH}/mexlib/includes" NO_DEFAULT_PATH
    )
FIND_PATH(SCILAB_CORE_INCLUDE_DIR
    "core_math.h"
    "${SCILAB_MODULE_PATH}/core/includes" NO_DEFAULT_PATH
    )

SET(SCILAB_LIBRARIES
  ${SCILAB_IMPL_LIBRARY}
  ${SCILAB_MEX_LIBRARY}
  ${SCILAB_MX_LIBRARY}
)

IF(SCILAB_ROOT)
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Scilab DEFAULT_MSG SCILAB_ROOT SCILAB_MEX_INCLUDE_DIR SCILAB_CORE_INCLUDE_DIR SCILAB_IMPL_LIBRARY SCILAB_MEX_LIBRARY SCILAB_MX_LIBRARY)
ENDIF(SCILAB_ROOT)

MARK_AS_ADVANCED(
  SCILAB_IMPL_LIBRARY
  SCILAB_MEX_LIBRARY
  SCILAB_MX_LIBRARY
  SCILAB_LIBRARIES
  SCILAB_MEX_INCLUDE_DIR
  SCILAB_CORE_INCLUDE_DIR
)