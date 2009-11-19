# - Try to find a version of Scilab and headers/library required by the 
#   used compiler.
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
  SET(LIBSCILAB "LibScilab")
ELSEIF(APPLE)
  FILE(GLOB SCILAB_PATHS "/Applications/Scilab*")
  SET(LIBMEX "libmex.dylib")
  SET(LIBMX "libmx.dylib")
  SET(LIBSCILAB "scilab")
ELSE(WIN32)
  SET(SCILAB_PATHS 
    "/usr/share/scilab"
    "/usr/local/share/scilab"
    "/opt/local/share/scilab")
  SET(SCILAB_MODULE_PATHS 
    "/usr/include/scilab"
    "/usr/local/include/scilab"
    "/opt/local/include/scilab")
  SET(SCILAB_LIBRARIES_PATHS
    "/usr/lib/scilab"
    "/usr/local/lib/scilab"
    "/opt/local/lib/scilab")
  SET(SCILAB_MODULES_MEXLIB_PATHS 
    "/usr/include/scilab/mexlib"
    "/usr/local/include/scilab/mexlib"
    "/opt/local/include/scilab/mexlib")
  SET(SCILAB_MODULES_CORE_PATHS
    "/usr/include/scilab/core"
    "/usr/local/include/scilab/core"
    "/opt/local/include/scilab/core")
  SET(LIBMEX "libmex.so")
  SET(LIBMX "libmx.so")
  SET(LIBSCILAB "scilab")
ENDIF(WIN32)

FIND_PATH(SCILAB_ROOT "etc/scilab.start" ${SCILAB_PATHS})

IF(WIN32 OR APPLE)
  FIND_PATH(SCILAB_ROOT "etc/scilab.start" ${SCILAB_PATHS})
  SET(SCILAB_MODULE_PATHS "${SCILAB_ROOT}/modules")
  SET(SCILAB_LIBRARIES_PATHS "${SCILAB_ROOT}/bin")
  SET(SCILAB_MODULES_MEXLIB_PATHS "${SCILAB_MODULE_PATHS}/mexlib/includes")
  SET(SCILAB_MODULES_CORE_PATHS "${SCILAB_MODULE_PATHS}/core/includes")
ENDIF(WIN32 OR APPLE)

FIND_LIBRARY(SCILAB_IMPL_LIBRARY
    ${LIBSCILAB}
    ${SCILAB_LIBRARIES_PATHS} NO_DEFAULT_PATH)
FIND_FILE(SCILAB_MEX_LIBRARY
    ${LIBMEX}
    ${SCILAB_LIBRARIES_PATHS} NO_DEFAULT_PATH)
FIND_FILE(SCILAB_MX_LIBRARY
    ${LIBMX}
    ${SCILAB_LIBRARIES_PATHS} NO_DEFAULT_PATH)
FIND_PATH(SCILAB_MEX_INCLUDE_DIR
    "mex.h"
    ${SCILAB_MODULES_MEXLIB_PATHS} NO_DEFAULT_PATH)
FIND_PATH(SCILAB_CORE_INCLUDE_DIR
    "core_math.h"
    ${SCILAB_MODULES_CORE_PATHS} NO_DEFAULT_PATH
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
