# - Find a BTK installation or build tree.
# The following variables are set if BTK is found.  If BTK is not
# found, BTK_FOUND is set to false.
#  BTK_FOUND         - Set to true when BTK is found.
#  BTK_USE_FILE      - CMake file to use BTK.
#  BTK_MAJOR_VERSION - The BTK major version number.
#  BTK_MINOR_VERSION - The BTK minor version number 
#                       (odd non-release).
#  BTK_BUILD_VERSION - The BTK patch level 
#                       (meaningless for odd minor).
#  BTK_INCLUDE_DIRS  - Include directories for BTK
#  BTK_LIBRARY_DIRS  - Link directories for BTK libraries
#  BTK_KITS          - List of BTK kits, in CAPS 
#                      (COMMON,IO,) etc.
#  BTK_LANGUAGES     - List of wrapped languages, in CAPS
#                      (MATLAB) etc.
# The following cache entries must be set by the user to locate BTK:
#  BTK_DIR  - The directory containing BTKConfig.cmake.  
#             This is either the root of the build tree,
#             or the lib/btk directory.  This is the 
#             only cache entry.
#

# Construct consitent error messages for use below.
SET(BTK_DIR_DESCRIPTION "directory containing BTKConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/btk for an installation.")
SET(BTK_DIR_MESSAGE "BTK not found.  Set the BTK_DIR cmake cache entry to the ${BTK_DIR_DESCRIPTION}")

# Search only if the location is not already known.
IF(NOT BTK_DIR)
  # Get the system search path as a list.
  IF(UNIX)
    STRING(REGEX MATCHALL "[^:]+" BTK_DIR_SEARCH1 "$ENV{PATH}")
  ELSE(UNIX)
    STRING(REGEX REPLACE "\\\\" "/" BTK_DIR_SEARCH1 "$ENV{PATH}")
  ENDIF(UNIX)
  STRING(REGEX REPLACE "/;" ";" BTK_DIR_SEARCH2 "${BTK_DIR_SEARCH1}")

  # Construct a set of paths relative to the system search path.
  SET(BTK_DIR_SEARCH "")
  FOREACH(dir ${BTK_DIR_SEARCH2})
    SET(BTK_DIR_SEARCH ${BTK_DIR_SEARCH}
      ${dir}/../lib/btk
      )
  ENDFOREACH(dir)

  #
  # Look for an installation or build tree.
  #
  FIND_PATH(BTK_DIR UseBTK.cmake
    # Support legacy cache files.
    ${BTK_DIR_SEARCH_LEGACY}

    # Look for an environment variable BTK_DIR.
    $ENV{BTK_DIR}

    # Look in places relative to the system executable search path.
    ${BTK_DIR_SEARCH}

    # Look in standard UNIX install locations.
    /usr/local/lib/btk
    /usr/lib/btk

    # Read from the CMakeSetup registry entries.  It is likely that
    # BTK will have been recently built.
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild1]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild2]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild3]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild4]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild5]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild6]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild7]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild8]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild9]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild10]

    # Help the user find it if we cannot.
    DOC "The ${BTK_DIR_DESCRIPTION}"
  )
ENDIF(NOT BTK_DIR)

# If BTK was found, load the configuration file to get the rest of the
# settings.
IF(BTK_DIR)
  # Make sure the BTKConfig.cmake file exists in the directory provided.
  IF(EXISTS ${BTK_DIR}/BTKConfig.cmake)

    # We found BTK.  Load the settings.
    SET(BTK_FOUND 1)
    INCLUDE(${BTK_DIR}/BTKConfig.cmake)

  ELSE(EXISTS ${BTK_DIR}/BTKConfig.cmake)
    IF(EXISTS ${BTK_DIR}/UseBTK.cmake)
      # We found BTK 4.0 (UseBTK.cmake exists, but not BTKConfig.cmake).
      SET(BTK_FOUND 1)
      # Load settings for BTK 4.0.
      INCLUDE(UseBTKConfig40)
    ELSE(EXISTS ${BTK_DIR}/UseBTK.cmake)
      # We did not find BTK.
      SET(BTK_FOUND 0)
    ENDIF(EXISTS ${BTK_DIR}/UseBTK.cmake)
  ENDIF(EXISTS ${BTK_DIR}/BTKConfig.cmake)
ELSE(BTK_DIR)
  # We did not find BTK.
  SET(BTK_FOUND 0)
ENDIF(BTK_DIR)

#-----------------------------------------------------------------------------
IF(NOT BTK_FOUND)
  # BTK not found, explain to the user how to specify its location.
  IF(BTK_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR ${BTK_DIR_MESSAGE})
  ELSE(BTK_FIND_REQUIRED)
    IF(NOT BTK_FIND_QUIETLY)
      MESSAGE(STATUS ${BTK_DIR_MESSAGE})
    ENDIF(NOT BTK_FIND_QUIETLY)
  ENDIF(BTK_FIND_REQUIRED)
ENDIF(BTK_FOUND)
