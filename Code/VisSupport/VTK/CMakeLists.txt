SET(BTKVTK_SRCS
  btkVTKAxesWidget.cpp
  btkVTKAxis.cpp
  btkVTKChartExtraAcquisition.cpp
  btkVTKChartLayout.cpp
  btkVTKChartTimeSeries.cpp
  btkVTKContextActor.cpp
  btkVTKDataObjectAdapter.cpp
  btkVTKForcePlatformsSource.cpp
  btkVTKFrustumFromTwoPoints.cpp
  btkVTKGRFsFramesSource.cpp
  btkVTKGroundSource.cpp
  btkVTKInteractorStyleTrackballFixedUpCamera.cpp
  btkVTKMarkersSource.cpp
  btkVTKMarkersFramesSource.cpp
  btkVTKOpenGLContextDevice2D.cpp
  btkVTKOpenGL2ContextDevice2D.cpp
  btkVTKPlotCycle.cpp
  btkVTKSegmentsFramesSource.cpp
  )

# For btk::btkVTKOpenGLContextDevice2D_p.h
IF(NOT ${VTK_FREETYPE_LIBRARIES} MATCHES "vtkfreetype")
  FIND_PACKAGE(Freetype REQUIRED)
  INCLUDE_DIRECTORIES("${FREETYPE_INCLUDE_DIR_freetype2}")
ELSE(NOT ${VTK_FREETYPE_LIBRARIES} MATCHES "vtkfreetype")
  # The following hack is needed when the VTK library is not installed
  # but used directly from its build folder.
  LIST(LENGTH VTK_INCLUDE_DIRS VTK_INCLUDE_DIRS_LIST_LEN)
  IF(VTK_INCLUDE_DIRS_LIST_LEN EQUAL 1)
    INCLUDE_DIRECTORIES("${VTK_INCLUDE_DIRS}/vtkfreetype/include")
  ENDIF(VTK_INCLUDE_DIRS_LIST_LEN EQUAL 1)
ENDIF(NOT ${VTK_FREETYPE_LIBRARIES} MATCHES "vtkfreetype")
  
IF(VTK_USE_QT)
  SET(BTKVTK_SRCS ${BTKVTK_SRCS} btkVTKQtStringToImage.cpp)
  SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
  SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
  SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
  FIND_PACKAGE(Qt4 REQUIRED)
  SET(QT_DONT_USE_QTGUI)
  INCLUDE(${QT_USE_FILE})
ENDIF(VTK_USE_QT)

ADD_LIBRARY(BTKVTK ${BTK_LIBS_BUILD_TYPE} ${BTKVTK_SRCS})
SET(BTK_LIBRARIES ${BTK_LIBRARIES} "BTKVTK" CACHE INTERNAL "BTK modules compiled")

TARGET_LINK_LIBRARIES(BTKVTK
                      BTKCommon
                      vtkCommon vtkFiltering vtkGraphics vtkRendering vtkHybrid vtkCharts vtkftgl
                      ${QT_LIBRARIES})

IF(BTK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(BTKVTK PROPERTIES ${BTK_LIBRARY_PROPERTIES})
ENDIF(BTK_LIBRARY_PROPERTIES)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS BTKVTK
    RUNTIME DESTINATION ${BTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${BTK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${BTK_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_LIBRARIES)

IF(NOT BTK_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __private "${CMAKE_CURRENT_SOURCE_DIR}/*_p.h")
  IF (__private) 
    LIST(REMOVE_ITEM __files ${__private})
  ENDIF (__private)
  INSTALL(FILES ${__files}
    DESTINATION ${BTK_INSTALL_INCLUDE_DIR_CM24}/VisSupport/VTK
    COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)