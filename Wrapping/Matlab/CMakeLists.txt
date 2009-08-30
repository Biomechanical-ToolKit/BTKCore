FIND_PACKAGE(Matlab REQUIRED)

CMAKE_DEPENDENT_OPTION(BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES
                       "Compiler option to be able to redistribute BTK MEX files on machine without the same developper environment (builds BTK in Release mode and links libraries in static mode (if MSVC is the compiler)." OFF
                       BTK_WRAP_MATLAB OFF)

IF (BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES)
  SET(CMAKE_OLD_BUILD_TYPE 0 CACHE STRING "CMAKE_BUILD_TYPE old value")
  SET(CMAKE_OLD_CXX_FLAGS_RELEASE 0 CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value")
  MARK_AS_ADVANCED(CMAKE_OLD_BUILD_TYPE CMAKE_OLD_CXX_FLAGS_RELEASE)
  IF(BTK_BUILD_SHARED_LIBS AND MSVC)
    MESSAGE(FATAL_ERROR "The use of options BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES and BUILD_SHARED_LIBS together are not compatible. The first wants to create a static library and the second a dynamic library. By using BUILD_SHARED_LIBS in Release mode, you can redistribute the MEX-Files, but the target machines need a \"Microsoft Visual C++ Redistributable Package\" corresponding to the used compiler.")
  ENDIF(BTK_BUILD_SHARED_LIBS AND MSVC)

  IF(NOT CMAKE_OLD_BUILD_TYPE)
    SET(CMAKE_OLD_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "CMAKE_BUILD_TYPE old value" FORCE)
    SET(CMAKE_OLD_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value" FORCE)
  ENDIF(NOT CMAKE_OLD_BUILD_TYPE)

  SET(CMAKE_BUILD_TYPE Release CACHE STRING "The build type is fixed due to the activation of BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES, Uncheck it to go to previous options." FORCE)
  IF (MSVC)
    SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "The CXX flags are fixed due to the activation of BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES, Uncheck it to go to previous flags." FORCE)
  ENDIF(MSVC)
ENDIF(BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES)

IF(MATLAB_ROOT)
INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})

# Common: accessor
MATLAB_MEX_CREATE(btkGetAnalogFrameNumber btkGetAnalogFrameNumber.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetAnalogFrequency btkGetAnalogFrequency.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetAnalogNumber btkGetAnalogNumber.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetAnalogs btkGetAnalogs.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetAnalogsValues btkGetAnalogsValues.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetAnalysis btkGetAnalysis.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetEvents btkGetEvents.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetFirstFrame btkGetFirstFrame.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetMetaData btkGetMetaData.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetPointFrameNumber btkGetPointFrameNumber.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetPointFrequency btkGetPointFrequency.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetPointNumber btkGetPointNumber.cpp BTKCommon)
MATLAB_MEX_CREATE(btkGetPoints btkGetPoints.cpp BTKCommon)
# Common: modifier
MATLAB_MEX_CREATE(btkAppendAnalysisParameter btkAppendAnalysisParameter.cpp BTKCommon)
MATLAB_MEX_CREATE(btkAppendPoint btkAppendPoint.cpp BTKCommon)
MATLAB_MEX_CREATE(btkClearAnalysis btkClearAnalysis.cpp BTKCommon)
MATLAB_MEX_CREATE(btkRemoveAnalysisParameter btkRemoveAnalysisParameter.cpp BTKCommon)
MATLAB_MEX_CREATE(btkRemovePoint btkRemovePoint.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetAnalogsValues btkSetAnalogsValues.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetFirstFrame btkSetFirstFrame.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetFrameNumber btkSetFrameNumber.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetPoint btkSetPoint.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetPointDescription btkSetPointDescription.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetPointLabel btkSetPointLabel.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetPointNumber btkSetPointNumber.cpp BTKCommon)
MATLAB_MEX_CREATE(btkSetPointType btkSetPointType.cpp BTKCommon)
# BTKBasicFilters
MATLAB_MEX_CREATE(btkGetAngles btkGetAngles.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetAnglesValues btkGetAnglesValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkSetAnglesValues btkSetAnglesValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetForcePlatforms btkGetForcePlatforms.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetForces btkGetForces.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetForcesValues btkGetForcesValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkSetForcesValues btkSetForcesValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetGroundReactionWrenches btkGetGroundReactionWrenches.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetMarkers btkGetMarkers.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetMarkersValues btkGetMarkersValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkSetMarkersValues btkSetMarkersValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetMoments btkGetMoments.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetMomentsValues btkGetMomentsValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkSetMomentsValues btkSetMomentsValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetPowers btkGetPowers.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetPowersValues btkGetPowersValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkSetPowersValues btkSetPowersValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetScalars btkGetScalars.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkGetScalarsValues btkGetScalarsValues.cpp BTKBasicFilters)
MATLAB_MEX_CREATE(btkSetScalarsValues btkSetScalarsValues.cpp BTKBasicFilters)
# IO
MATLAB_MEX_CREATE(btkReadAcquisition btkReadAcquisition.cpp BTKIO)
MATLAB_MEX_CREATE(btkWriteAcquisition btkWriteAcquisition.cpp BTKIO)
ENDIF(MATLAB_ROOT)

IF(NOT BTK_INSTALL_NO_DEVELOPMENT)
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab  
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE)

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
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)

