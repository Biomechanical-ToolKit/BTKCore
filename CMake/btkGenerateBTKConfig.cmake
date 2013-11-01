# Generate the BTKConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# BTK.

#---------------------------------------------------------------------------
# Settings specific to the build tree.

# Generate CMake lines that will define the BTK_SOURCE_DIR in the BTKConfig.cmake.
# We want this to happen only in the BTKConfig.cmake of the build dir, not in the
# installed or relocatable one.

# The library dependencies file.
SET(BTK_LIBRARY_DEPENDS_FILE  ${BTK_BINARY_DIR}/BTKLibraryDepends.cmake)

# The "use" file.
SET(BTK_USE_FILE ${BTK_BINARY_DIR}/UseBTK.cmake)

# The build settings file.
SET(BTK_BUILD_SETTINGS_FILE ${BTK_BINARY_DIR}/BTKBuildSettings.cmake)

# Library directory.
SET(BTK_LIBRARY_DIRS_CONFIG ${BTK_LIBRARY_PATH})

# Determine the include directories needed.
SET(BTK_INCLUDE_DIRS_CONFIG
  ${BTK_INCLUDE_DIRS_BUILD_TREE}
  ${BTK_INCLUDE_DIRS_SYSTEM}
)

#---------------------------------------------------------------------------
# Configure BTKConfig.cmake for the build tree.
CONFIGURE_FILE(${BTK_CMAKE_MODULE_PATH}/BTKConfig.cmake.in
               ${BTK_BINARY_DIR}/BTKConfig.cmake @ONLY IMMEDIATE)

#---------------------------------------------------------------------------
# Settings specific to the install tree.

# The library dependencies file.
SET(BTK_LIBRARY_DEPENDS_FILE "\${BTK_INSTALL_PREFIX}${BTK_INSTALL_SHARE_DIR}/BTKLibraryDepends.cmake")

# The "use" file.
SET(BTK_USE_FILE \${BTK_INSTALL_PREFIX}${BTK_INSTALL_SHARE_DIR}/UseBTK.cmake)

# The build settings file.
SET(BTK_BUILD_SETTINGS_FILE \${BTK_INSTALL_PREFIX}${BTK_INSTALL_SHARE_DIR}/BTKBuildSettings.cmake)

# Include directories.
SET(BTK_INCLUDE_DIRS_CONFIG \${BTK_INSTALL_PREFIX}${BTK_INSTALL_INCLUDE_DIR})
FOREACH(DIR ${BTK_INCLUDE_RELATIVE_DIRS})
  LIST(APPEND BTK_INCLUDE_DIRS_CONFIG \${BTK_INSTALL_PREFIX}${BTK_INSTALL_INCLUDE_DIR}/${DIR})
ENDFOREACH(DIR)
IF(BTK_INCLUDE_DIRS_SYSTEM)
  LIST(APPEND BTK_INCLUDE_DIRS_CONFIG ${BTK_INCLUDE_DIRS_SYSTEM})
ENDIF(BTK_INCLUDE_DIRS_SYSTEM)

# Link directories.
SET(BTK_LIBRARY_DIRS_CONFIG "\${BTK_INSTALL_PREFIX}${BTK_INSTALL_LIB_DIR}")

#-----------------------------------------------------------------------------
# Configure BTKConfig.cmake for the install tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix.
STRING(REGEX REPLACE "/" ";" BTK_INSTALL_SHARE_DIR_COUNT
  "${BTK_INSTALL_SHARE_DIR}")
SET(BTK_CONFIG_CODE "
# Compute the installation prefix from this BTKConfig.cmake file location.
GET_FILENAME_COMPONENT(BTK_INSTALL_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)")
FOREACH(p ${BTK_INSTALL_SHARE_DIR_COUNT})
  SET(BTK_CONFIG_CODE
    "${BTK_CONFIG_CODE}\nGET_FILENAME_COMPONENT(BTK_INSTALL_PREFIX \"\${BTK_INSTALL_PREFIX}\" PATH)"
    )
ENDFOREACH(p)

CONFIGURE_FILE(${BTK_CMAKE_MODULE_PATH}/BTKConfig.cmake.in
               ${BTK_BINARY_DIR}/Utilities/BTKConfig.cmake @ONLY IMMEDIATE)
               
# Setup an install rule for the config file.
IF(NOT BTK_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${BTK_BINARY_DIR}/Utilities/BTKConfig.cmake 
    DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}
    COMPONENT Development
    )
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)


