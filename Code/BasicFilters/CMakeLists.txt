SET(BTKBasicFilters_SRCS
  btkAcquisitionUnitConverter.cpp
  btkForcePlatformsExtractor.cpp
  btkForcePlatformWrenchFilter.cpp
  btkGroundReactionWrenchFilter.cpp
  btkIMUsExtractor.cpp
  btkMergeAcquisitionFilter.cpp
  btkSeparateKnownVirtualMarkersFilter.cpp
  btkSpecializedPointsExtractor.cpp
  btkVerticalGroundReactionForceGaitEventDetector.cpp
  btkWrenchDirectionAngleFilter.cpp
)

ADD_LIBRARY(BTKBasicFilters ${BTK_LIBS_BUILD_TYPE} ${BTKBasicFilters_SRCS})

TARGET_LINK_LIBRARIES(BTKBasicFilters BTKCommon)

IF(BTK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(BTKBasicFilters PROPERTIES ${BTK_LIBRARY_PROPERTIES})
ENDIF(BTK_LIBRARY_PROPERTIES)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS BTKBasicFilters
    RUNTIME DESTINATION ${BTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${BTK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${BTK_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_LIBRARIES)

IF(NOT BTK_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  INSTALL(FILES ${__files}
    DESTINATION ${BTK_INSTALL_INCLUDE_DIR_CM24}/BasicFilters
    COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)
