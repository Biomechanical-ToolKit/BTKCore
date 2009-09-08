FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(Mokka_CPP
  About.cpp
  MainWindow.cpp
  Metadata.cpp
  MetadataView.cpp
  ComboBoxDelegate.cpp
  main.cpp
  )

SET(Mokka_H
  About.h
  MainWindow.h
  MetadataView.h
  Metadata.h
  ComboBoxDelegate.h)

SET(Mokka_UI
  About.ui
  MainWindow.ui
  Metadata.ui)

SET(Mokka_QRC
  images.qrc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

QT4_WRAP_UI(Mokka_UI_H ${Mokka_UI})
QT4_WRAP_CPP(Mokka_MOC ${Mokka_H})
QT4_ADD_RESOURCES(Mokka_RCC ${Mokka_QRC})

ADD_EXECUTABLE(Mokka ${Mokka_UI_H} ${Mokka_MOC} ${Mokka_RCC} ${Mokka_CPP})
TARGET_LINK_LIBRARIES(Mokka BTKCommon BTKIO BTKBasicFilters BTKVTK QVTK ${QT_LIBRARIES})

# Installation
IF(NOT BTK_INSTALL_NO_RUNTIME)
  INSTALL(TARGETS Mokka
    RUNTIME DESTINATION ${BTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries)
ENDIF(NOT BTK_INSTALL_NO_RUNTIME)
