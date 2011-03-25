FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

SET(MOKKA_VERSION_MAJOR 0)
SET(MOKKA_VERSION_MINOR 2)
SET(MOKKA_VERSION_PATCH 0)
IF (MOKKA_VERSION_PATCH EQUAL 0)
  SET(MOKKA_VERSION_STRING "${MOKKA_VERSION_MAJOR}.${MOKKA_VERSION_MINOR}")
ELSE (MOKKA_VERSION_PATCH EQUAL 0)
  SET(MOKKA_VERSION_STRING "${MOKKA_VERSION_MAJOR}.${MOKKA_VERSION_MINOR}.${MOKKA_VERSION_PATCH}")
ENDIF (MOKKA_VERSION_PATCH EQUAL 0)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/mokkaConfigure.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/mokkaConfigure.h @ONLY IMMEDIATE)

SET(Mokka_CPP
  Acquisition.cpp
  About.cpp
  AbstractMultiView.cpp
  AbstractView.cpp
  CompositeView.cpp
  FileInfoDockWidget.cpp
  ImportAssistantDialog.cpp
  MainWindow.cpp
  Metadata.cpp
  MetadataView.cpp
  ModelDockWidget.cpp
  MultiViewWidget.cpp
  NewEventDialog.cpp
  NewModelDialog.cpp
  ProgressWidget.cpp
  TimeEventBarWidget.cpp
  TimeEventControlerWidget.cpp
  UndoCommands.cpp
  UpdateChecker.cpp
  Viz3DWidget.cpp
  main.cpp
  )

SET(Mokka_H
  Acquisition.h
  AbstractMultiView.h
  AbstractView.h
  CompositeView.h
  FileInfoDockWidget.h
  ImportAssistantDialog.h
  MainWindow.h
  MetadataView.h
  Metadata.h
  ModelDockWidget.h
  MultiViewWidget.h
  NewEventDialog.h
  NewModelDialog.h
  ProgressWidget.h
  TimeEventBarWidget.h
  TimeEventControlerWidget.h
  UpdateChecker.h
  Viz3DWidget.h)

SET(Mokka_UI
  About.ui
  AbstractView.ui
  FileInfoDockWidget.ui
  ImportAssistantDialog.ui
  MainWindow.ui
  Metadata.ui
  ModelDockWidget.ui
  NewEventDialog.ui
  NewModelDialog.ui
  ProgressWidget.ui
  TimeEventControlerWidget.ui
  UpdateChecker.ui)

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
  SET(MACOSX_BUNDLE_INFO_STRING "${MOKKA_VERSION_STRING}, Copyright 2009-2011 Arnaud Barré.")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "org.btk.Mokka")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "Mokka")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${MOKKA_VERSION_STRING}")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${MOKKA_VERSION_STRING}")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "1")
  SET(MACOSX_BUNDLE_COPYRIGHT "© 2009-2011 Arnaud Barré")
  SET(MOKKA_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Resources/${MACOSX_BUNDLE_ICON_FILE}")
  SET_SOURCE_FILES_PROPERTIES("${MOKKA_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  
  # -------- Function to build OS X Stand Alone Bundles -----------------
  FUNCTION(MakeOSXBundleApp target binary_dir install_dir)
    SET (OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.sh")
    CONFIGURE_FILE("${CMAKE_MODULE_PATH}/OSXBundle/CompleteBundle.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake" @ONLY IMMEDIATE)
    CONFIGURE_FILE("${CMAKE_MODULE_PATH}/OSXBundle/CreateBundle.sh.in" "${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)
    INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake")
  ENDFUNCTION(MakeOSXBundleApp)
ENDIF(WIN32)

ADD_EXECUTABLE(Mokka ${GUI} ${MOKKA_ICON} ${Mokka_UI_H} ${Mokka_MOC} ${Mokka_RCC} ${Mokka_CPP})
TARGET_LINK_LIBRARIES(Mokka BTKCommon BTKIO BTKBasicFilters BTKVTK QVTK ${QT_LIBRARIES})

SET(MOKKA_HELP_DIR "${BTK_EXECUTABLE_PATH}/MokkaHelp")
IF(WIN32)
  TARGET_LINK_LIBRARIES(Mokka ${QT_QTMAIN_LIBRARY})
ELSEIF(APPLE)
  SET_TARGET_PROPERTIES(Mokka PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_MODULE_PATH}/OSXBundle/ApplicationInfo.plist.in")
  SET(MOKKA_HELP_DIR "${BTK_EXECUTABLE_PATH}/Mokka.app/Contents/Resources/MokkaHelp")
ENDIF(WIN32)

ADD_CUSTOM_TARGET(MokkaHelp ALL
                  COMMAND ${CMAKE_COMMAND} -E copy_directory "${BTK_BINARY_DIR}/Documentation/MokkaHelp" "${MOKKA_HELP_DIR}")

# Installation
IF(NOT BTK_INSTALL_NO_RUNTIME)
  IF(APPLE)
    MakeOSXBundleApp(Mokka ${EXECUTABLE_OUTPUT_PATH} "/Applications")
  ELSE(APPLE)
    INSTALL(TARGETS Mokka RUNTIME DESTINATION ${BTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries)
    IF(WIN32) # Windows
      IF(EXISTS "${BTK_BINARY_DIR}/Documentation/MokkaHelp")
        INSTALL(DIRECTORY "${BTK_BINARY_DIR}/Documentation/MokkaHelp" DESTINATION "${BTK_INSTALL_BIN_DIR_CM24}")
      ENDIF(EXISTS "${BTK_BINARY_DIR}/Documentation/MokkaHelp")
    ELSE(WIN32) # Linux
      MESSAGE(STATUS "Where to install the Mokka documentation under linux? Contact the developers if you have any hint.")
    ENDIF(WIN32)
  ENDIF(APPLE)
  
ENDIF(NOT BTK_INSTALL_NO_RUNTIME)
