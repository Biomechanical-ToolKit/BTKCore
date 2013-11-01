# Include directories for other projects installed on the system.
SET(BTK_INCLUDE_DIRS_SYSTEM "")

# Include directories from the build tree.
SET(BTK_INCLUDE_DIRS_BUILD_TREE ${BTK_INCLUDE_DIRS_BUILD_TREE} ${BTK_BINARY_DIR})

# These directories are always needed.
SET(BTK_INCLUDE_DIRS_BUILD_TREE ${BTK_INCLUDE_DIRS_BUILD_TREE}
  ${BTK_SOURCE_DIR}/Code/BasicFilters
  ${BTK_SOURCE_DIR}/Code/Common
  ${BTK_SOURCE_DIR}/Code/IO
  ${BTK_SOURCE_DIR}
)

IF(BTK_USE_VTK)
  SET(BTK_INCLUDE_DIRS_BUILD_TREE ${BTK_INCLUDE_DIRS_BUILD_TREE}
    ${BTK_SOURCE_DIR}/Code/VisSupport/VTK)
ENDIF(BTK_USE_VTK)

# Include directories from the install tree.
SET(BTK_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}${BTK_INSTALL_INCLUDE_DIR}")
SET(BTK_INCLUDE_RELATIVE_DIRS ${BTK_INCLUDE_RELATIVE_DIRS}
  BasicFilters
  Common
  IO
  Utilities
)
IF(BTK_USE_VTK)
  SET(BTK_INCLUDE_RELATIVE_DIRS ${BTK_INCLUDE_RELATIVE_DIRS}
    VisSupport/VTK)
ENDIF(BTK_USE_VTK)

# Because embedded Eigen3 library is required in lots of header of BTK.
SET(BTK_INCLUDE_DIRS_BUILD_TREE ${BTK_INCLUDE_DIRS_BUILD_TREE}
  ${BTK_SOURCE_DIR}/Utilities/eigen3)
SET(BTK_INCLUDE_RELATIVE_DIRS ${BTK_INCLUDE_RELATIVE_DIRS}
  Utilities/eigen3)

