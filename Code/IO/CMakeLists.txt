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
  btkC3DFileIO.cpp
  btkCALForcePlateFileIO.cpp
  btkDelsysEMGFileIO.cpp
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
  # Utils & Others
  btkMotionAnalysisFileIOUtils.cpp
  btkMotionAnalysisFileIOUtils_p.cpp
  btkEliteFileIOUtils_p.cpp
)

ADD_LIBRARY(BTKIO ${BTK_LIBS_BUILD_TYPE} ${BTKIO_SRCS})
SET(BTK_LIBRARIES "${BTK_LIBRARIES} BTKIO" CACHE INTERNAL "BTK modules compiled")

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
