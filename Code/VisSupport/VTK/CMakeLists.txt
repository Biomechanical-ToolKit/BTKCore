#FIND_PACKAGE(VTK REQUIRED)

#IF(VTK_MAJOR_VERSION LESS 5)
#  MESSAGE(FATAL_ERROR "VTK 5.2 or greater required")
#ENDIF(VTK_MAJOR_VERSION LESS 5)

SET(BTKVTK_SRCS
  btkVTKAxesWidget.cpp
  btkVTKDataObjectAdapter.cpp
  btkVTKForcePlatformsSource.cpp
  btkVTKGRFsFramesSource.cpp
  btkVTKInteractorStyleTrackballCamera.cpp
  btkVTKMarkersSource.cpp
  btkVTKMarkersFramesSource.cpp
  )

#INCLUDE(${VTK_USE_FILE})
#INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
#LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})
ADD_LIBRARY(BTKVTK ${BTKVTK_SRCS})
