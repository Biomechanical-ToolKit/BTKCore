#
# This file sets up include directories, link directories, and
# compiler settings for a project to use BTK.  It should not be
# included directly, but rather through the BTK_USE_FILE setting
# obtained from BTKConfig.cmake.
#

IF(BTK_BUILD_SETTINGS_FILE AND NOT SKIP_BTK_BUILD_SETTINGS_FILE)
  INCLUDE(${CMAKE_ROOT}/Modules/CMakeImportBuildSettings.cmake)
  CMAKE_IMPORT_BUILD_SETTINGS(${BTK_BUILD_SETTINGS_FILE})
ENDIF(BTK_BUILD_SETTINGS_FILE AND NOT SKIP_BTK_BUILD_SETTINGS_FILE)

# Add compiler flags needed to use BTK.
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BTK_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BTK_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BTK_REQUIRED_LINK_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${BTK_REQUIRED_LINK_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${BTK_REQUIRED_LINK_FLAGS}")

# Add include directories needed to use BTK.
INCLUDE_DIRECTORIES(BEFORE ${BTK_INCLUDE_DIRS})

# Add link directories needed to use BTK.
LINK_DIRECTORIES(${BTK_LIBRARY_DIRS})

# Load settings from the system eigen2 with which BTK was built, if any.
IF(BTK_USE_SYSTEM_EIGEN2)
  # If eigen2 has already been found, make sure it is the one used to
  # build BTK.
  IF(WIN32 OR APPLE)
    STRING(TOLOWER "${EIGEN2_INCLUDE_DIR}" BTK_CHECK_EIGEN2_INCLUDE_DIR)
    STRING(TOLOWER "${BTK_EIGEN2_INCLUDE_DIR}" BTK_CHECK_BTK_EIGEN2_INCLUDE_DIR)
  ELSE(WIN32 OR APPLE)
    SET(BTK_CHECK_EIGEN2_INCLUDE_DIR "${EIGEN2_INCLUDE_DIR}")
    SET(BTK_CHECK_BTK_EIGEN2_INCLUDE_DIR "${BTK_EIGEN2_INCLUDE_DIR}")
  ENDIF(WIN32 OR APPLE)
  STRING(COMPARE EQUAL "${BTK_CHECK_EIGEN2_INCLUDE_DIR}" "${BTK_CHECK_BTK_EIGEN2_INCLUDE_DIR}"
         BTK_EIGEN2_INCLUDE_DIR_MATCH)
  IF(EIGEN2_FOUND)
    IF(NOT BTK_EIGEN2_INCLUDE_DIR_MATCH)
      MESSAGE(FATAL_ERROR
        "BTK was built with EIGEN2 from \"${BTK_EIGEN2_INCLUDE_DIR}\", "
        "but this project is using EIGEN2 from \"${EIGEN2_INCLUDE_DIR}\".  "
        "Please set EIGEN2_INCLUDE_DIR to match the one used to build BTK."
        )
    ENDIF(NOT BTK_EIGEN2_INCLUDE_DIR_MATCH)
  ELSE(EIGEN2_FOUND)
    IF(EIGEN2_INCLUDE_DIR)
      IF(NOT BTK_EIGEN2_INCLUDE_DIR_MATCH)
        MESSAGE(
          "Warning: BTK was built with EIGEN2 from \"${BTK_EIGEN2_INCLUDE_DIR}\", "
          "but this project has set EIGEN2_INCLUDE_DIR to \"${EIGEN2_INCLUDE_DIR}\".  "
          "BTK is changing EIGEN2_INCLUDE_DIR to match the EIGEN2 with which it was built."
          )
      ENDIF(NOT BTK_EIGEN2_INCLUDE_DIR_MATCH)
    ENDIF(EIGEN2_INCLUDE_DIR)
    SET(EIGEN2_INCLUDE_DIR ${BTK_EIGEN2_INCLUDE_DIR})
    FIND_PACKAGE(Eigen2)
    IF(NOT EIGEN2_FOUND)
      MESSAGE(FATAL_ERROR
        "UseBTK could not load EIGEN2 settings from \"${EIGEN2_INCLUDE_DIR}\" even through "
        "BTK was built using EIGEN2 from this location."
        )
    ENDIF(NOT EIGEN2_FOUND)
  ENDIF(EIGEN2_FOUND)
ENDIF(BTK_USE_SYSTEM_EIGEN2)
