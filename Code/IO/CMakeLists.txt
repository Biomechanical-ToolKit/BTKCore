SET(BTKIO_SRCS
  btkAcquisitionFileIO.cpp
  btkAcquisitionFileIOFactory.cpp
  btkAcquisitionFileReader.cpp
  btkAcquisitionFileWriter.cpp
  btkBinaryFileStream.cpp
  # File formats
  btkAMTIForcePlatformFileIO.cpp
  btkANBFileIO.cpp
  btkANCFileIO.cpp
  btkANGFileIO.cpp
  btkC3DFileIO.cpp
  btkCALForcePlateFileIO.cpp
  btkEMFFileIO.cpp
  btkEMxFileIO.cpp
  btkGRxFileIO.cpp
  btkPWRFileIO.cpp
  btkRAxFileIO.cpp
  btkRICFileIO.cpp
  btkMOMFileIO.cpp
  btkTDFFileIO.cpp
  btkTRBFileIO.cpp
  btkTRCFileIO.cpp
  btkXLSOrthoTrakFileIO.cpp
  # Private methods
  btkMotionAnalysisFileIOUtils_p.cpp
  btkEliteFileIOUtils_p.cpp
)

ADD_LIBRARY(BTKIO ${BTK_LIBS_BUILD_TYPE} ${BTKIO_SRCS})

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
  INSTALL(FILES ${__files}
    DESTINATION ${BTK_INSTALL_INCLUDE_DIR_CM24}/IO
    COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)
