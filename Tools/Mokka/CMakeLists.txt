FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(Mokka_CPP
  About.cpp
  ComboBoxDelegate.cpp
  MainWindow.cpp
  Metadata.cpp
  MetadataView.cpp
  PointsEditor.cpp
  TableWidget.cpp
  UndoCommands.cpp
  main.cpp
  )

SET(Mokka_H
  About.h
  ComboBoxDelegate.h
  MainWindow.h
  MetadataView.h
  Metadata.h
  PointsEditor.h
  TableWidget.h)

SET(Mokka_UI
  About.ui
  MainWindow.ui
  Metadata.ui
  PointsEditor.ui)

SET(Mokka_QRC
  images.qrc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${QT_INCLUDE_DIR})

QT4_WRAP_UI(Mokka_UI_H ${Mokka_UI})
QT4_WRAP_CPP(Mokka_MOC ${Mokka_H})
QT4_ADD_RESOURCES(Mokka_RCC ${Mokka_QRC})

IF(WIN32)
  #SET(GUI WIN32)
  #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
ELSEIF(APPLE)
  SET(GUI MACOSX_BUNDLE)
  # -------- Function to build OS X Stand Alone Bundles -----------------
  FUNCTION(MakeOSXBundleApp target binary_dir install_dir)
    SET (OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.sh")
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Resources/OSXBundle/CompleteBundle.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake" @ONLY IMMEDIATE)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Resources/OSXBundle/CreateBundle.sh.in" "${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)
    INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/OSXBundle.cmake")
  ENDFUNCTION(MakeOSXBundleApp)
ENDIF(WIN32)

ADD_EXECUTABLE(Mokka ${GUI} ${Mokka_UI_H} ${Mokka_MOC} ${Mokka_RCC} ${Mokka_CPP})
TARGET_LINK_LIBRARIES(Mokka BTKCommon BTKIO BTKBasicFilters BTKVTK QVTK ${QT_LIBRARIES})

IF(WIN32)
	#TARGET_LINK_LIBRARIES(Mokka ${QT_QTMAIN_LIBRARY})
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
