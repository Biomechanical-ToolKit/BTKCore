# Open3DMotion settings for Codamotion file formats (MDF, XMOVE)
SET(USE_O3DM_NO_FILE_FORMAT_REGISTRATION 1)
SET(USE_O3DM_MDF_FILE_FORMAT 1)
SET(USE_O3DM_XMOVE_FILE_FORMAT 1)
INCLUDE(${BTK_CMAKE_MODULE_PATH}/btkOpen3DMotionSources.cmake)

SET(BTKIO_SRCS
  btkAcquisitionFileIO.cpp
  btkAcquisitionFileIOFactory.cpp
  btkAcquisitionFileIOFactory_registration.cpp
  btkAcquisitionFileReader.cpp
  btkAcquisitionFileWriter.cpp
  btkASCIIFileWriter.cpp
  btkBinaryFileStream.cpp
  btkMultiSTLFileWriter.cpp
  # File formats
  btkAMTIForcePlatformFileIO.cpp
  btkANBFileIO.cpp
  btkANCFileIO.cpp
  btkANGFileIO.cpp
  btkBSFFileIO.cpp
  btkC3DFileIO.cpp
  btkCALForcePlateFileIO.cpp
  btkCLBFileIO.cpp
  btkDelsysEMGFileIO.cpp
  btkEMFFileIO.cpp
  btkEMxFileIO.cpp
  btkGRxFileIO.cpp
  btkPWRFileIO.cpp
  btkRAxFileIO.cpp
  btkRICFileIO.cpp
  btkMDFFileIO.cpp
  btkMOMFileIO.cpp
  btkTDFFileIO.cpp
  btkTRBFileIO.cpp
  btkTRCFileIO.cpp
  btkXLSOrthoTrakFileIO.cpp
  btkXMOVEFileIO.cpp
  # Utils & Others
  btkCodamotionFileIOUtils_p.cpp
  btkEliteFileIOUtils_p.cpp
  btkMotionAnalysisFileIOUtils.cpp
  btkMotionAnalysisFileIOUtils_p.cpp
  # Needed by Open3DMotion for the Codamotion file formats
  ${BTK_O3DM_SRCS}
  "${BTK_SOURCE_DIR}/Utilities/pugixml/src/pugixml.cpp"
  "${BTK_SOURCE_DIR}/Utilities/libb64/src/cdecode.c"
  "${BTK_SOURCE_DIR}/Utilities/libb64/src/cencode.c"
)

INCLUDE_DIRECTORIES(${BTK_O3DM_INCLUDE_DIR})
INCLUDE_DIRECTORIES("${BTK_SOURCE_DIR}/Utilities/pugixml/src")
INCLUDE_DIRECTORIES("${BTK_SOURCE_DIR}/Utilities/libb64/include")

ADD_LIBRARY(BTKIO ${BTK_LIBS_BUILD_TYPE} ${BTKIO_SRCS})
SET(BTK_LIBRARIES ${BTK_LIBRARIES} "BTKIO" CACHE INTERNAL "BTK modules compiled")

TARGET_LINK_LIBRARIES(BTKIO BTKCommon)

IF(BTK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(BTKIO PROPERTIES ${BTK_LIBRARY_PROPERTIES})
ENDIF(BTK_LIBRARY_PROPERTIES)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS BTKIO
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
    DESTINATION ${BTK_INSTALL_INCLUDE_DIR_CM24}/IO
    COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)
