FIND_PACKAGE(Matlab REQUIRED)

CMAKE_DEPENDENT_OPTION(BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES
                       "Compiler option to be able to redistribute BTK MEX files on machine without the same developper environment (builds BTK in Release mode and links libraries in static mode (if MSVC is the compiler)." OFF
                       BTK_WRAP_MATLAB OFF)

IF (BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES)
  SET(CMAKE_BUILD_MRD 1 CACHE STRING "State of building BTK with the Matlab Wrapping in redistribuable mode" FORCE)
  SET(CMAKE_OLD_BUILD_TYPE 0 CACHE STRING "CMAKE_BUILD_TYPE old value")
  SET(CMAKE_OLD_CXX_FLAGS_RELEASE 0 CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value")
  MARK_AS_ADVANCED(CMAKE_BUILD_MRD CMAKE_OLD_BUILD_TYPE CMAKE_OLD_CXX_FLAGS_RELEASE)
  IF(BTK_BUILD_SHARED_LIBS AND MSVC)
    MESSAGE(FATAL_ERROR "The use of options BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES and BUILD_SHARED_LIBS together are not compatible. The first wants to create a static library and the second a dynamic library. By using BUILD_SHARED_LIBS in Release mode, you can redistribute the MEX-Files, but the target machines need a \"Microsoft Visual C++ Redistributable Package\" corresponding to the used compiler.")
  ENDIF(BTK_BUILD_SHARED_LIBS AND MSVC)

  IF(NOT CMAKE_OLD_BUILD_TYPE)
    SET(CMAKE_OLD_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "CMAKE_BUILD_TYPE old value" FORCE)
  ENDIF(NOT CMAKE_OLD_BUILD_TYPE)
  IF(NOT CMAKE_OLD_CXX_FLAGS_RELEASE)
    SET(CMAKE_OLD_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value" FORCE)
  ENDIF(NOT CMAKE_OLD_CXX_FLAGS_RELEASE)

  SET(CMAKE_BUILD_TYPE Release CACHE STRING "The build type is fixed due to the activation of BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES, Uncheck it to go to previous options." FORCE)
  IF (MSVC)
    IF (${CMAKE_SYSTEM_VERSION} LESS 6.1) # Windows XP, VISTA
      SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "The CXX flags are fixed due to the activation of BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES, Uncheck it to go to previous flags." FORCE)
      MESSAGE(STATUS "WARNING: Due to a different mechanism introduced in Windows 7, it is not possible to redistribute the MEX files compiled for Windows XP or Vista into Windows 7.")
    ELSE (${CMAKE_SYSTEM_VERSION} LESS 6.1) # Windows 7 or upper.
      MESSAGE(STATUS "WARNING: Due to a different mechanism introduced in Windows 7, the redistribution of the MEX files into Windows XP or Vista compiled from Windows 7 requires to install the 'Visual C++ Redistributable Package'.")
    ENDIF (${CMAKE_SYSTEM_VERSION} LESS 6.1)
  ENDIF(MSVC)  
ENDIF(BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES)

IF(MATLAB_ROOT)
  INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})
  
  # Common: accessor
  MATLAB_MEX_CREATE(btkGetAnalogFrameNumber btkGetAnalogFrameNumber.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetAnalogFrequency btkGetAnalogFrequency.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetAnalogNumber btkGetAnalogNumber.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetAnalogsResolution btkGetAnalogsResolution.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetAnalogs btkGetAnalogs.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetAnalogsValues btkGetAnalogsValues.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetAnalysis btkGetAnalysis.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetEvents btkGetEvents.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetEventsValues btkGetEventsValues.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetFirstFrame btkGetFirstFrame.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetMaxInterpolationGap btkGetMaxInterpolationGap.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetMetaData btkGetMetaData.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetPointFrameNumber btkGetPointFrameNumber.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetPointFrequency btkGetPointFrequency.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetPointNumber btkGetPointNumber.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkGetPoints btkGetPoints.cpp BTKCommon)
  # Common: modifier
  MATLAB_MEX_CREATE(btkAppendAnalysisParameter btkAppendAnalysisParameter.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkAppendAnalog btkAppendAnalog.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkAppendEvent btkAppendEvent.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkAppendMetaData btkAppendMetaData.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkAppendPoint btkAppendPoint.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkClearAnalogs btkClearAnalogs.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkClearAnalysis btkClearAnalysis.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkClearEvents btkClearEvents.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkClearMetaData btkClearMetaData.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkClearPoints btkClearPoints.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkCloneAcquisition btkCloneAcquisition.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkDeleteAcquisition btkDeleteAcquisition.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkNewAcquisition btkNewAcquisition.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkRemoveAnalog btkRemoveAnalog.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkRemoveAnalysisParameter btkRemoveAnalysisParameter.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkRemoveEvent btkRemoveEvent.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkRemoveMetaData btkRemoveMetaData.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkRemovePoint btkRemovePoint.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogLabel btkSetAnalogLabel.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogDescription btkSetAnalogDescription.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogGain btkSetAnalogGain.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogScale btkSetAnalogScale.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogOffset btkSetAnalogOffset.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogsResolution btkSetAnalogsResolution.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogSampleNumberPerFrame btkSetAnalogSampleNumberPerFrame.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogValues btkSetAnalogValues.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetAnalogsValues btkSetAnalogsValues.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetEventDescription btkSetEventDescription.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetEventLabel btkSetEventLabel.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetEventId btkSetEventId.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetEventTime btkSetEventTime.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetFirstFrame btkSetFirstFrame.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetFrameNumber btkSetFrameNumber.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetFrequency btkSetFrequency.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMaxInterpolationGap btkSetMaxInterpolationGap.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMetaDataLabel btkSetMetaDataLabel.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMetaDataDescription btkSetMetaDataDescription.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMetaDataFormat btkSetMetaDataFormat.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMetaDataDimensions btkSetMetaDataDimensions.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMetaDataUnlock btkSetMetaDataUnlock.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetMetaDataValue btkSetMetaDataValue.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPoint btkSetPoint.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointDescription btkSetPointDescription.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointLabel btkSetPointLabel.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointMasks btkSetPointMasks.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointNumber btkSetPointNumber.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointResiduals btkSetPointResiduals.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointType btkSetPointType.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointUnit btkSetPointUnit.cpp BTKCommon)
  MATLAB_MEX_CREATE(btkSetPointValues btkSetPointValues.cpp BTKCommon)
  # BTKBasicFilters
  MATLAB_MEX_CREATE(btkGetAngles btkGetAngles.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetAnglesValues btkGetAnglesValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkSetAnglesValues btkSetAnglesValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetForcePlatforms btkGetForcePlatforms.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetForces btkGetForces.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetForcesValues btkGetForcesValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkSetForcesValues btkSetForcesValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetForcePlatformWrenches btkGetForcePlatformWrenches.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetGroundReactionWrenches btkGetGroundReactionWrenches.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetMarkers btkGetMarkers.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetMarkersValues btkGetMarkersValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkSetMarkersValues btkSetMarkersValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetMoments btkGetMoments.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetMomentsValues btkGetMomentsValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkSetMomentsValues btkSetMomentsValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetPowers btkGetPowers.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetPowersValues btkGetPowersValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkMergeAcquisitions btkMergeAcquisitions.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkSetPowersValues btkSetPowersValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetScalars btkGetScalars.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkGetScalarsValues btkGetScalarsValues.cpp BTKBasicFilters)
  MATLAB_MEX_CREATE(btkSetScalarsValues btkSetScalarsValues.cpp BTKBasicFilters)
  # IO
  MATLAB_MEX_CREATE(btkReadAcquisition btkReadAcquisition.cpp BTKIO)
  MATLAB_MEX_CREATE(btkWriteAcquisition btkWriteAcquisition.cpp BTKIO)
ENDIF(MATLAB_ROOT)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  # Matlab M file to configure
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m.in
    ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m)
  INSTALL(FILES ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk
          COMPONENT Development)
  # Documentation install
  # Main document file with BTK version
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m.in
    ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m)
  INSTALL(FILES ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk
          COMPONENT Development)
  # Whole documentation
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab  
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN "*.in" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE)

  #  MEX Files install
  # Fix a problem with this installation method and the use of the MS Visual Studio generators.
  IF(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    INSTALL(DIRECTORY "${BTK_LIBRARY_PATH}/@BUILD_TYPE@/"
            DESTINATION "${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk"
            COMPONENT Development
            FILES_MATCHING PATTERN "*.${MATLAB_MEXFILE_EXT}")
  ELSE(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    INSTALL(DIRECTORY "${BTK_LIBRARY_PATH}/"
            DESTINATION "${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk"
            COMPONENT Development
            FILES_MATCHING PATTERN "*.${MATLAB_MEXFILE_EXT}")
  ENDIF(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
ENDIF(NOT BTK_INSTALL_NO_LIBRARIES)

