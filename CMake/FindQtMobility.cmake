# - Try to find the QtMobility libraries
#
# This module will search for the QtMobility libraries.
#
# It supports both a minimum version and searching for individual
# components. For the minimum version, use
# find_package(QtMobility 1.2.0). For specific components, use
# find_package(QtMobility COMPONENTS ...). See below for a list of known
# components.
#
# Once done this will define
#  QTMOBILITY_FOUND - QtMobility and all specified components were found.
#  QTMOBILITY_INCLUDE_DIR - Include directory for global QtMobility files.
#  QTMOBILITY_INCLUDE_DIRS - All found QtMobility components' include dirs.
#  QTMOBILITY_LIBRARIES - All found QtMobility components' libraries.
#  QTMOBILITY_VERSION - The version of QtMobility that was found.
#
# For each found component the following will be defined:
#  QTMOBILITY_{COMPONENT}_INCLUDE_DIR - The include directory for the component.
#  QTMOBILITY_{COMPONENT}_LIBRARY - The location of the library for the component.
#
# Note that searching for components will be limited to the specified components
# if the components option is used.
#
# Copyright (c) 2011 Arjen Hiemstra <ahiemstra@heimr.nl>
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Modified by Arnaud Barré:
#  - New internal macro to merge release and debug libraries of the same component
#    (based on macro _QT4_ADJUST_LIB_VARS from FindQt4).
#  - MacOS X: Test if included into Qt (configured with the option -mac-deploy).
#  - Windows: Try to be more robust (look for more folder starting from c:\Qt*).

SET(QTMOBILITY_COMPONENTS
    Bearer
    Connectivity
    Contacts
    Feedback
    Gallery
    Location
    Messaging
    MultimediaKit
    Organizer
    PublishSubscribe
    Sensors
    ServiceFramework
    SystemInfo
    Versit
)

IF (QtMobility_FIND_COMPONENTS)
  FOREACH (component ${QtMobility_FIND_COMPONENTS})
    STRING(TOUPPER ${component} _COMPONENT)
    SET(QTMOBILITY_USE_${_COMPONENT} 1)
  ENDFOREACH (component)
ENDIF (QtMobility_FIND_COMPONENTS)

# convenience macro for dealing with debug/release library names
# Adapted from the macro _QT4_ADJUST_LIB_VARS found in FindQt4.cmake
# Updated to use the major revision number found in QtMobility
MACRO (_QTMOBILITY_ADJUST_LIB_VARS _camelCaseBasename _mr)

  STRING(TOUPPER "${_camelCaseBasename}" basename)

  IF (QTMOBILITY_${basename}_LIBRARY_RELEASE OR QTMOBILITY_${basename}_LIBRARY_DEBUG)

    IF (NOT TARGET QtMobility${_mr}::${_camelCaseBasename})
      ADD_LIBRARY(QtMobility${_mr}::${_camelCaseBasename} UNKNOWN IMPORTED )

      IF (QTMOBILITY_${basename}_LIBRARY_RELEASE)
        SET_PROPERTY(TARGET QtMobility${_mr}::${_camelCaseBasename} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
        IF (QTMOBILITY_USE_FRAMEWORKS)
          SET_PROPERTY(TARGET QtMobility${_mr}::${_camelCaseBasename}        PROPERTY IMPORTED_LOCATION_RELEASE "${QTMOBILITY_${basename}_LIBRARY_RELEASE}/${_camelCaseBasename}" )
        else()
          SET_PROPERTY(TARGET QtMobility${_mr}::${_camelCaseBasename}        PROPERTY IMPORTED_LOCATION_RELEASE "${QTMOBILITY_${basename}_LIBRARY_RELEASE}" )
        ENDIF ()
      ENDIF (QTMOBILITY_${basename}_LIBRARY_RELEASE)

      IF (QTMOBILITY_${basename}_LIBRARY_DEBUG)
        SET_PROPERTY(TARGET QtMobility${_mr}::${_camelCaseBasename} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
        IF (QTMOBILITY_USE_FRAMEWORKS)
          SET_PROPERTY(TARGET QtMobility${_mr}::${_camelCaseBasename}        PROPERTY IMPORTED_LOCATION_DEBUG "${QTMOBILITY_${basename}_LIBRARY_DEBUG}/${_camelCaseBasename}" )
        else()
          SET_PROPERTY(TARGET QtMobility${_mr}::${_camelCaseBasename}        PROPERTY IMPORTED_LOCATION_DEBUG "${QTMOBILITY_${basename}_LIBRARY_DEBUG}" )
        ENDIF ()
      ENDIF (QTMOBILITY_${basename}_LIBRARY_DEBUG)
    ENDIF (NOT TARGET QtMobility${_mr}::${_camelCaseBasename})

    IF (QTMOBILITY_USE_IMPORTED_TARGETS)
        SET(QTMOBILITY_${basename}_LIBRARY       QtMobility${_mr}::${_camelCaseBasename} )
        SET(QTMOBILITY_${basename}_LIBRARIES     QtMobility${_mr}::${_camelCaseBasename} )
    ELSE(QTMOBILITY_USE_IMPORTED_TARGETS)
      # If the release- as well as the debug-version of the library have been found:
      IF (QTMOBILITY_${basename}_LIBRARY_DEBUG AND QTMOBILITY_${basename}_LIBRARY_RELEASE)
        # If the generator supports configuration types then set
        # optimized and debug libraries, or IF the CMAKE_BUILD_TYPE has a value
        IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          SET(QTMOBILITY_${basename}_LIBRARY       optimized ${QTMOBILITY_${basename}_LIBRARY_RELEASE} debug ${QTMOBILITY_${basename}_LIBRARY_DEBUG})
        ELSE(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          # If there are no configuration types and CMAKE_BUILD_TYPE has no value
          # then just use the release libraries
          SET(QTMOBILITY_${basename}_LIBRARY       ${QTMOBILITY_${basename}_LIBRARY_RELEASE} )
        ENDIF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(QTMOBILITY_${basename}_LIBRARIES       optimized ${QTMOBILITY_${basename}_LIBRARY_RELEASE} debug ${QTMOBILITY_${basename}_LIBRARY_DEBUG})
      ENDIF (QTMOBILITY_${basename}_LIBRARY_DEBUG AND QTMOBILITY_${basename}_LIBRARY_RELEASE)

      # If only the release version was found, set the debug variable also to the release version
      IF (QTMOBILITY_${basename}_LIBRARY_RELEASE AND NOT QTMOBILITY_${basename}_LIBRARY_DEBUG)
        SET(QTMOBILITY_${basename}_LIBRARY_DEBUG ${QTMOBILITY_${basename}_LIBRARY_RELEASE})
        SET(QTMOBILITY_${basename}_LIBRARY       ${QTMOBILITY_${basename}_LIBRARY_RELEASE})
        SET(QTMOBILITY_${basename}_LIBRARIES     ${QTMOBILITY_${basename}_LIBRARY_RELEASE})
      ENDIF (QTMOBILITY_${basename}_LIBRARY_RELEASE AND NOT QTMOBILITY_${basename}_LIBRARY_DEBUG)

      # If only the debug version was found, set the release variable also to the debug version
      IF (QTMOBILITY_${basename}_LIBRARY_DEBUG AND NOT QTMOBILITY_${basename}_LIBRARY_RELEASE)
        SET(QTMOBILITY_${basename}_LIBRARY_RELEASE ${QTMOBILITY_${basename}_LIBRARY_DEBUG})
        SET(QTMOBILITY_${basename}_LIBRARY         ${QTMOBILITY_${basename}_LIBRARY_DEBUG})
        SET(QTMOBILITY_${basename}_LIBRARIES       ${QTMOBILITY_${basename}_LIBRARY_DEBUG})
      ENDIF (QTMOBILITY_${basename}_LIBRARY_DEBUG AND NOT QTMOBILITY_${basename}_LIBRARY_RELEASE)

      # put the value in the cache:
      SET(QTMOBILITY_${basename}_LIBRARY ${QTMOBILITY_${basename}_LIBRARY} CACHE STRING "The Qt${_camelCaseBasename} library" FORCE)

    ENDIF (QTMOBILITY_USE_IMPORTED_TARGETS)

    SET(QTMOBILITY_${basename}_FOUND 1)

  ELSE (QTMOBILITY_${basename}_LIBRARY_RELEASE OR QTMOBILITY_${basename}_LIBRARY_DEBUG)

    SET(QTMOBILITY_${basename}_LIBRARY "" CACHE STRING "The Qt${_camelCaseBasename} library" FORCE)

  ENDIF (QTMOBILITY_${basename}_LIBRARY_RELEASE OR QTMOBILITY_${basename}_LIBRARY_DEBUG)

  IF (QTMOBILITY_${basename}_INCLUDE_DIR)
    #add the include directory to QTMOBILITY_INCLUDE_DIRS
    SET(QTMOBILITY_INCLUDE_DIRS "${QTMOBILITY_${basename}_INCLUDE_DIR}" ${QTMOBILITY_INCLUDE_DIRS})
  ENDIF (QTMOBILITY_${basename}_INCLUDE_DIR)

  # Make variables changeble to the advanced user
  MARK_AS_ADVANCED(QTMOBILITY_${basename}_LIBRARY QTMOBILITY_${basename}_LIBRARY_RELEASE QTMOBILITY_${basename}_LIBRARY_DEBUG QTMOBILITY_${basename}_INCLUDE_DIR)
ENDMACRO (_QTMOBILITY_ADJUST_LIB_VARS)

# Give some possible paths under Windows
IF (WIN32)
  SET(QTMOBILITY_PATHS_L1 )
  SET(QTMOBILITY_PATHS_L2 )
  # Level 0
  FILE(GLOB QTMOBILITY_PATHS_L0 "c:/Qt*")
  # Level 1
  FOREACH(_file_ ${QTMOBILITY_PATHS_L0})
    FILE(GLOB QTMOBILITY_PATHS_TEMP "${_file_}/*")
    SET(QTMOBILITY_PATHS_L1 ${QTMOBILITY_PATHS_L1} ${QTMOBILITY_PATHS_TEMP})
  ENDFOREACH(_file_ QTMOBILITY_PATHS_L0)
  # Level 2
  FOREACH(_file_ ${QTMOBILITY_PATHS_L1})
    FILE(GLOB QTMOBILITY_PATHS_TEMP "${_file_}/*")
    SET(QTMOBILITY_PATHS_L2 ${QTMOBILITY_PATHS_L2} ${QTMOBILITY_PATHS_TEMP})
  ENDFOREACH(_file_ QTMOBILITY_PATHS_L1)
  # Merge levels
  SET(QTMOBILITY_PATHS ${QTMOBILITY_PATHS_L0} ${QTMOBILITY_PATHS_L1} ${QTMOBILITY_PATHS_L2})
ENDIF (WIN32)

FIND_PATH(QTMOBILITY_ROOT "include/QtMobility/qmobilityglobal.h" PATHS ${QTMOBILITY_PATHS})
IF (NOT QTMOBILITY_ROOT AND QT4_FOUND)
  FIND_PATH(QTMOBILITY_ROOT "include/QtMobility/qmobilityglobal.h" PATH_SUFFIXES ".." PATHS ${QT_INCLUDE_DIR})
ENDIF (NOT QTMOBILITY_ROOT AND QT4_FOUND)

FIND_PATH(QTMOBILITY_INCLUDE_DIR qmobilityglobal.h PATH_SUFFIXES "include/QtMobility" PATHS ${QTMOBILITY_ROOT})

#Find the mobility version
IF (QTMOBILITY_INCLUDE_DIR)
    FILE(READ "${QTMOBILITY_INCLUDE_DIR}/qmobilityglobal.h" _qtmobility_global_header LIMIT 5000)
    STRING(REGEX MATCH "#define QTM_VERSION_STR \"([0-9.]*)\"" _qtmobility_version_match  "${_qtmobility_global_header}")
    SET(QTMOBILITY_VERSION "${CMAKE_MATCH_1}")
    STRING(SUBSTRING ${QTMOBILITY_VERSION} 0 1 QTMOBILITY_VERSION_MAJOR)
ENDIF (QTMOBILITY_INCLUDE_DIR)

SET(QTMOBILITY_VARIABLES "QTMOBILITY_ROOT;QTMOBILITY_INCLUDE_DIR")
set(QTMOBILITY_INCLUDE_DIRS ${QTMOBILITY_INCLUDE_DIR})

#A list of files to find for specific components
SET(QTMOBILITY_FIND_FILES
    QNetworkConfiguration #Bearer
    QBluetoothSocket #Connectivity
    QContact #Contacts
    QFeedbackInterface #Feedback
    QAbstractGallery #Gallery
    QLandmark #Location
    QMessage #Messaging
    QMediaPlayer #MultimediaKit
    QOrganizerItem #Organizer
    QValueSpace #PublishSubscribe
    QSensor #Sensors
    QService #ServiceFramework
    QSystemInfo #SystemInfo
    QVersitDocument #Versit
)

LIST(LENGTH QTMOBILITY_COMPONENTS _component_count)
MATH(EXPR _component_count "${_component_count} - 1")
FOREACH (index RANGE ${_component_count})
    LIST(GET QTMOBILITY_COMPONENTS ${index} component)
    LIST(GET QTMOBILITY_FIND_FILES ${index} file)
    STRING(TOUPPER ${component} _COMPONENT)

    IF (NOT QtMobility_FIND_COMPONENTS OR QTMOBILITY_USE_${_COMPONENT})
        FIND_PATH(QTMOBILITY_${_COMPONENT}_INCLUDE_DIR ${file} PATH_SUFFIXES Qt${component} qt4/Qt${component} PATHS "${QTMOBILITY_ROOT}/include")
        FIND_LIBRARY(QTMOBILITY_${_COMPONENT}_LIBRARY_RELEASE NAMES Qt${component} Qt${component}${QTMOBILITY_VERSION_MAJOR} PATHS "${QTMOBILITY_ROOT}/lib")
        FIND_LIBRARY(QTMOBILITY_${_COMPONENT}_LIBRARY_DEBUG NAMES Qt${component} Qt${component}d${QTMOBILITY_VERSION_MAJOR} PATHS "${QTMOBILITY_ROOT}/lib")
        _QTMOBILITY_ADJUST_LIB_VARS(${component} ${QTMOBILITY_VERSION_MAJOR})
        LIST(APPEND QTMOBILITY_VARIABLES "QTMOBILITY_${_COMPONENT}_INCLUDE_DIR" "QTMOBILITY_${_COMPONENT}_LIBRARY")
        LIST(APPEND QTMOBILITY_LIBRARIES ${QTMOBILITY_${_COMPONENT}_LIBRARY})
    ENDIF (NOT QtMobility_FIND_COMPONENTS OR QTMOBILITY_USE_${_COMPONENT})
ENDFOREACH (index)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtMobility REQUIRED_VARS ${QTMOBILITY_VARIABLES} VERSION_VAR QTMOBILITY_VERSION)

MARK_AS_ADVANCED(${QTMOBILITY_VARIABLES})