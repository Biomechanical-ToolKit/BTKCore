FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(MOKKA_VERSION "0.2a1")

IF(VTK_MAJOR_VERSION LESS 5)
  IF(VTK_MINOR_VERSION LESS 6)
    MESSAGE(FATAL_ERROR "Mokka requires now VTK 5.6 or greater for the charts")
  ENDIF(VTK_MINOR_VERSION LESS 6)
ENDIF(VTK_MAJOR_VERSION LESS 5)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/About.cpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/About.cpp @ONLY IMMEDIATE)

SET(Mokka_CPP
  Acquisition.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/About.cpp
  AbstractMultiView.cpp
  AbstractView.cpp
  CompositeView.cpp
  FileInfoDockWidget.cpp
  MainWindow.cpp
  Metadata.cpp
  MetadataView.cpp
  ModelDockWidget.cpp
  MultiViewWidget.cpp
  NewEventDialog.cpp
  NewModelDialog.cpp
  ProgressWidget.cpp
  TableWidget.cpp
  TimeEventBarWidget.cpp
  TimeEventControlerWidget.cpp
  UndoCommands.cpp
  Viz3DWidget.cpp
  main.cpp
  )

SET(Mokka_H
  Acquisition.h
  AbstractMultiView.h
  AbstractView.h
  CompositeView.h
  FileInfoDockWidget.h
  MainWindow.h
  MetadataView.h
  Metadata.h
  ModelDockWidget.h
  MultiViewWidget.h
  NewEventDialog.h
  NewModelDialog.h
  ProgressWidget.h
  TableWidget.h
  TimeEventBarWidget.h
  TimeEventControlerWidget.h
  Viz3DWidget.h)

SET(Mokka_UI
  About.ui
  AbstractView.ui
  FileInfoDockWidget.ui
  MainWindow.ui
  Metadata.ui
  ModelDockWidget.ui
  NewEventDialog.ui
  NewModelDialog.ui
  ProgressWidget.ui
  TimeEventControlerWidget.ui)

SET(Mokka_QRC
  images.qrc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${QT_INCLUDE_DIR}
                    ${BTK_SOURCE_DIR}/Utilities/ParserCmdLine)

QT4_WRAP_UI(Mokka_UI_H ${Mokka_UI})
QT4_WRAP_CPP(Mokka_MOC ${Mokka_H})
QT4_ADD_RESOURCES(Mokka_RCC ${Mokka_QRC})

IF(WIN32)
  SET(GUI WIN32)
  SET(MOKKA_ICON "Mokka.rc")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
ELSEIF(APPLE)
  SET(GUI MACOSX_BUNDLE)
  SET(MACOSX_BUNDLE_ICON_FILE "Mokka.icns")
  SET(MACOSX_BUNDLE_INFO_STRING "${MOKKA_VERSION}, Copyright 2009-2010 Arnaud Barré.")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "org.btk.Mokka")
  #SET(MACOSX_BUNDLE_LONG_VERSION_STRING "")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "Mokka")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${MOKKA_VERSION}")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "1")
  SET(MACOSX_BUNDLE_COPYRIGHT "© 2009-2010 Arnaud Barré")
  SET(MOKKA_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Resources/${MACOSX_BUNDLE_ICON_FILE}")
  SET_SOURCE_FILES_PROPERTIES("${MOKKA_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  
  # -------- Function to build OS X Stand Alone Bundles -----------------
  FUNCTION(MakeOSXBundleApp target binary_dir install_dir)
    SET (OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.sh")
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Resources/OSXBundle/CompleteBundle.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake" @ONLY IMMEDIATE)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Resources/OSXBundle/CreateBundle.sh.in" "${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)
    INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake")
  ENDFUNCTION(MakeOSXBundleApp)
ENDIF(WIN32)

ADD_EXECUTABLE(Mokka ${GUI} ${MOKKA_ICON} ${Mokka_UI_H} ${Mokka_MOC} ${Mokka_RCC} ${Mokka_CPP})
TARGET_LINK_LIBRARIES(Mokka BTKCommon BTKIO BTKBasicFilters BTKVTK QVTK ${QT_LIBRARIES})

IF(WIN32)
	TARGET_LINK_LIBRARIES(Mokka ${QT_QTMAIN_LIBRARY})
ENDIF(WIN32)

# Installation
IF(NOT BTK_INSTALL_NO_RUNTIME)
  IF(APPLE)
    MakeOSXBundleApp(Mokka ${EXECUTABLE_OUTPUT_PATH} "/Applications")
    #INSTALL(TARGETS Mokka RUNTIME BUNDLE DESTINATION "/Applications" COMPONENT RuntimeLibraries)
  ELSE(APPLE)
    INSTALL(TARGETS Mokka RUNTIME DESTINATION ${BTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries)
  ENDIF(APPLE)
ENDIF(NOT BTK_INSTALL_NO_RUNTIME)
