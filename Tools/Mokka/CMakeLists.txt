FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_PHONON TRUE)
INCLUDE(${QT_USE_FILE})

SET(MOKKA_VERSION_MAJOR 0)
SET(MOKKA_VERSION_MINOR 6a4)
SET(MOKKA_VERSION_PATCH 0)
IF (MOKKA_VERSION_PATCH EQUAL 0)
  SET(MOKKA_VERSION_STRING "${MOKKA_VERSION_MAJOR}.${MOKKA_VERSION_MINOR}")
ELSE (MOKKA_VERSION_PATCH EQUAL 0)
  SET(MOKKA_VERSION_STRING "${MOKKA_VERSION_MAJOR}.${MOKKA_VERSION_MINOR}.${MOKKA_VERSION_PATCH}")
ENDIF (MOKKA_VERSION_PATCH EQUAL 0)

INCLUDE(${BTK_CMAKE_MODULE_PATH}/btkMinizipSources.cmake)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/mokkaConfigure.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/mokkaConfigure.h @ONLY IMMEDIATE)

SET(Mokka_CPP
  Acquisition.cpp
  About.cpp
  AbstractMultiView.cpp
  AbstractView.cpp
  ChartCycleSettingDialog.cpp
  ChartCycleSettingsManager.cpp
  ChartDialog.cpp
  ChartExportDialog.cpp
  ChartOptionsWidget.cpp
  ChartImagePreview.cpp
  ChartWidget.cpp
  CompositeView.cpp
  ExportASCIIDialog.cpp
  ExportSTLDialog.cpp
	ExportImageSeriesDialog.cpp
  FileInfoDockWidget.cpp
  ImportAssistantDialog.cpp
  LoggerWidget.cpp
  LoggerVTKOutput.cpp
  MainWindow.cpp
  Metadata.cpp
  MetadataView.cpp
  Model.cpp
  ModelDockWidget.cpp
  ModelTreeWidget.cpp
  MultiViewWidget.cpp
  NewEventDialog.cpp
  NewItemDialog.cpp
  NewLayoutDialog.cpp
  NewModelDialog.cpp
  NewSegmentDialog.cpp
  Preferences_Layouts.cpp
  ProgressWidget.cpp
  TimeEventBarWidget.cpp
  TimeEventControlerWidget.cpp
  TimeEventFunctors.cpp
  ToolsData.cpp
  ToolsManager.cpp
  Tools/AnalogToolOptionDialog.cpp
  Tools/FilterAnalog.cpp
  Tools/GaitEventDetection.cpp
  Tools/RectifyAnalog.cpp
  Tools/RemoveAnalogOffset.cpp
  Tools/ScaleAnalog.cpp
  Tools/SmoothAnalog.cpp
  UndoCommands.cpp
  UpdateManager.cpp
  UpdateManager_controller.cpp
  UpdateManager_installer.cpp
  UpdateManager_version.cpp
  Viz3DWidget.cpp
  VideoWidget.cpp
  VizRendererWidget.cpp
  main.cpp
  ${BTK_MINIZIP_SRCS}
  )

SET(Mokka_H
  Acquisition.h
  AbstractMultiView.h
  AbstractView.h
  ChartCycleSettingDialog.h
  ChartCycleSettingsManager.h
  ChartDialog.h
  ChartExportDialog.h
  ChartOptionsWidget.h
  ChartImagePreview.h
  ChartWidget.h
  CompositeView.h
  ExportASCIIDialog.h
  ExportSTLDialog.h
	ExportImageSeriesDialog.h
  FileInfoDockWidget.h
  ImportAssistantDialog.h
  LoggerWidget.h
  MainWindow.h
  MetadataView.h
  Metadata.h
  Model.h
  ModelDockWidget.h
  ModelTreeWidget.h
  MultiViewWidget.h
  NewEventDialog.h
  NewItemDialog.h
  NewSegmentDialog.h
  Preferences_Layouts.h
  ProgressWidget.h
  TimeEventBarWidget.h
  TimeEventControlerWidget.h
  ToolsManager.h
  Tools/AnalogToolOptionDialog.h
  Tools/FilterAnalogDialog.h
  Tools/GaitEventDetectionDialog.h
  Tools/RemoveAnalogOffsetDialog.h
  Tools/ScaleAnalogDialog.h
  Tools/SmoothAnalogDialog.h
  UpdateManager.h
  UpdateManager_p.h
  Viz3DWidget.h
  VideoWidget.h
  )

SET(Mokka_UI
  About.ui
  AbstractView.ui
  ChartCycleSettingDialog.ui
  ChartExportDialog.ui
  ChartOptionsWidget.ui
  ExportASCIIDialog.ui
  ExportSTLDialog.ui
	ExportImageSeriesDialog.ui
  FileInfoDockWidget.ui
  ImportAssistantDialog.ui
  MainWindow.ui
  Metadata.ui
  ModelDockWidget.ui
  NewEventDialog.ui
  NewItemDialog.ui
  NewSegmentDialog.ui
  ProgressWidget.ui
  TimeEventControlerWidget.ui
  Tools/AnalogToolOptionDialog.ui
  Tools/GaitEventDetectionDialog.ui
  UpdateManagerNewVersionDialog.ui
  UpdateManagerInstallerDialog.ui
  )

SET(Mokka_QRC
  images.qrc)

IF(APPLE)
  SET(Mokka_UI ${Mokka_UI} Preferences_mac.ui)
  SET(Mokka_H ${Mokka_H} Preferences_mac.h)
  SET(Mokka_CPP ${Mokka_CPP} Preferences_mac.mm)
ELSE(APPLE)
  SET(Mokka_UI ${Mokka_UI} Preferences.ui)
  SET(Mokka_H ${Mokka_H} Preferences.h)
  SET(Mokka_CPP ${Mokka_CPP} Preferences.cpp)
ENDIF(APPLE)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${QT_INCLUDE_DIR}
                    ${BTK_SOURCE_DIR}/Utilities/ParserCmdLine
                    ${BTK_MINIZIP_INCLUDE_DIR})

QT4_WRAP_UI(Mokka_UI_H ${Mokka_UI})
QT4_WRAP_CPP(Mokka_MOC ${Mokka_H})
QT4_ADD_RESOURCES(Mokka_RCC ${Mokka_QRC})

IF(WIN32)
  FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Icon.rc" "IDI_ICON1 ICON DISCARDABLE \"Resources\\Mokka.ico\"")
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Resources/Mokka.rc.in
                 ${CMAKE_CURRENT_BINARY_DIR}/InfoVersion.rc @ONLY IMMEDIATE)
  SET(Mokka_CPP ${Mokka_CPP} InfoVersion.rc)
  SET(GUI WIN32)
  SET(MOKKA_ICON "${CMAKE_CURRENT_BINARY_DIR}/Icon.rc")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
ELSEIF(APPLE)
  SET(GUI MACOSX_BUNDLE)
  SET(MACOSX_BUNDLE_ICON_FILE "Mokka.icns")
  SET(MACOSX_BUNDLE_INFO_STRING "${MOKKA_VERSION_STRING}, Copyright 2009-2012 Arnaud Barré.")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "org.btk.Mokka")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "Mokka")
  SET(MACOSX_BUNDLE_EXECUTABLE_NAME "Mokka")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${MOKKA_VERSION_STRING}")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${MOKKA_VERSION_STRING}")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "1")
  SET(MACOSX_BUNDLE_COPYRIGHT "© 2009-2012 Arnaud Barré")
  SET(MOKKA_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Resources/${MACOSX_BUNDLE_ICON_FILE}")
  SET_SOURCE_FILES_PROPERTIES("${MOKKA_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  
  # -------- Function to build OS X Stand Alone Bundles -----------------
  FUNCTION(MakeOSXBundleApp target binary_dir install_dir)
    SET (OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.sh")
    SET (OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake")
    CONFIGURE_FILE("${BTK_CMAKE_MODULE_PATH}/OSXBundle/CompleteBundle.cmake.in" "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}" @ONLY IMMEDIATE)
    CONFIGURE_FILE("${BTK_CMAKE_MODULE_PATH}/OSXBundle/CreateBundle.sh.in" "${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)
    INSTALL(SCRIPT "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}")
  ENDFUNCTION(MakeOSXBundleApp)
ENDIF(WIN32)

ADD_EXECUTABLE(Mokka ${GUI} ${MOKKA_ICON} ${Mokka_UI_H} ${Mokka_MOC} ${Mokka_RCC} ${Mokka_CPP})
TARGET_LINK_LIBRARIES(Mokka BTKCommon BTKIO BTKBasicFilters BTKVTK QVTK vtkCharts ${QT_LIBRARIES})

SET(MOKKA_HELP_DIR "${BTK_EXECUTABLE_PATH}/MokkaHelp")
IF(WIN32)
  TARGET_LINK_LIBRARIES(Mokka ${QT_QTMAIN_LIBRARY})
ELSEIF(APPLE)
  CONFIGURE_FILE("${BTK_CMAKE_MODULE_PATH}/OSXBundle/ApplicationInfo.plist.in" "${CMAKE_CURRENT_BINARY_DIR}/MokkaInfo.plist")
  SET_TARGET_PROPERTIES(Mokka PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/MokkaInfo.plist")
  SET(MOKKA_HELP_DIR "${BTK_EXECUTABLE_PATH}/Mokka.app/Contents/Resources/MokkaHelp")
ENDIF(WIN32)

ADD_CUSTOM_COMMAND(TARGET Mokka PRE_LINK
                   COMMAND ${CMAKE_COMMAND} -E copy_directory "${BTK_BINARY_DIR}/Documentation/MokkaHelp" "${MOKKA_HELP_DIR}")

# Installation
IF(NOT BTK_INSTALL_NO_RUNTIME)
  IF(APPLE) # MacOS
    MakeOSXBundleApp(Mokka ${EXECUTABLE_OUTPUT_PATH} "/Applications")
    SET(CMAKE_INSTALL_PREFIX "/Applications")
    INCLUDE(DeployQt4)
    INSTALL_QT4_EXECUTABLE("Mokka.app" "phonon_qt7")    
  ELSE(APPLE)
    INSTALL(TARGETS Mokka RUNTIME DESTINATION ${BTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries)
    IF(WIN32) # Windows
      IF(EXISTS "${BTK_BINARY_DIR}/Documentation/MokkaHelp")
        INSTALL(DIRECTORY "${BTK_BINARY_DIR}/Documentation/MokkaHelp" DESTINATION "${BTK_INSTALL_BIN_DIR_CM24}")
      ENDIF(EXISTS "${BTK_BINARY_DIR}/Documentation/MokkaHelp")
    ELSE(WIN32) # Linux
      IF(EXISTS "${BTK_BINARY_DIR}/Documentation/MokkaHelp")
        INSTALL(DIRECTORY "${BTK_BINARY_DIR}/Documentation/MokkaHelp" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/btk/mokka")
      ENDIF(EXISTS "${BTK_BINARY_DIR}/Documentation/MokkaHelp")
    ENDIF(WIN32)
  ENDIF(APPLE)
ENDIF(NOT BTK_INSTALL_NO_RUNTIME)

# -----------------------------------------------------------------------------

# Mokka Updater (for Windows Vista/7 only)
IF(WIN32)
  SET(MokkaUpdater_SRC
      Updater/main.cpp
      Updater/InfoVersion.rc
      Updater/Icon.rc)
  
  ADD_EXECUTABLE(MokkaUpdater WIN32 ${MokkaUpdater_SRC})
  TARGET_LINK_LIBRARIES(MokkaUpdater ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
ENDIF(WIN32)
