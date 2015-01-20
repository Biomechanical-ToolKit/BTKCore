# Open3DMotion settings for Codamotion file formats (MDF, XMOVE)
# SET(USE_O3DM_NO_FILE_FORMAT_REGISTRATION 1)
# SET(USE_O3DM_MDF_FILE_FORMAT 1)
# SET(USE_O3DM_XMOVE_FILE_FORMAT 1)
# INCLUDE(${BTK_CMAKE_MODULE_PATH}/btkOpen3DMotionSources.cmake)

SET(BTKIO_SRCS
  btkIODevice.cpp
  btkEndianConverter.cpp
  btkBinaryStream.cpp
  btkFile.cpp
  # btkAcquisitionFileIO.cpp
  # btkAcquisitionFileIOFactory.cpp
  # btkAcquisitionFileIOFactory_registration.cpp
  # btkAcquisitionFileReader.cpp
  # btkAcquisitionFileWriter.cpp
  # btkASCIIFileWriter.cpp
  # btkBinaryFileStream.cpp
  # btkMultiSTLFileWriter.cpp
  # # File formats
  # btkANBFileIO.cpp
  # btkANCFileIO.cpp
  # btkANGFileIO.cpp
  # btkBSFFileIO.cpp
  # btkC3DFileIO.cpp
  # btkCALForcePlateFileIO.cpp
  # btkCLBFileIO.cpp
  # btkDelsysEMGFileIO.cpp
  # btkEMFFileIO.cpp
  # btkEMxFileIO.cpp
  # btkGRxFileIO.cpp
  # btkHPFFileIO.cpp
  # btkKistlerDATFileIO.cpp
  # btkPWRFileIO.cpp
  # btkMDFFileIO.cpp
  # btkMOMFileIO.cpp
  # btkRAxFileIO.cpp
  # btkRICFileIO.cpp
  # btkTDFFileIO.cpp
  # btkTRBFileIO.cpp
  # btkTRCFileIO.cpp
  # btkXLSOrthoTrakFileIO.cpp
  # btkXMOVEFileIO.cpp
  # # Utils & Others
  # btkCodamotionFileIOUtils_p.cpp
  # btkEliteFileIOUtils_p.cpp
  # btkMotionAnalysisFileIOUtils.cpp
  # btkMotionAnalysisFileIOUtils_p.cpp
  # # Needed by Open3DMotion for the Codamotion file formats
  # ${BTK_O3DM_SRCS}
  # "${BTK_SOURCE_DIR}/Utilities/pugixml/src/pugixml.cpp"
  # "${BTK_SOURCE_DIR}/Utilities/libb64/src/cdecode.c"
  # "${BTK_SOURCE_DIR}/Utilities/libb64/src/cencode.c"
)

# INCLUDE_DIRECTORIES(${BTK_O3DM_INCLUDE_DIR})
# INCLUDE_DIRECTORIES("${BTK_SOURCE_DIR}/Utilities/pugixml/src")
# INCLUDE_DIRECTORIES("${BTK_SOURCE_DIR}/Utilities/libb64/include")

ADD_LIBRARY(IO ${BTK_LIBS_BUILD_TYPE} ${BTKIO_SRCS})
TARGET_LINK_LIBRARIES(IO Common)
TARGET_INCLUDE_DIRECTORIES(Common PUBLIC
  $<BUILD_INTERFACE:${BTK_BINARY_DIR}>
  $<BUILD_INTERFACE:${BTK_SOURCE_DIR}/ThirdParties/eigen3>
  $<INSTALL_INTERFACE:include/btk>
  $<INSTALL_INTERFACE:include/btk/3rdparty/eigen3>
)
GENERATE_EXPORT_HEADER(IO
                       BASE_NAME BTK_IO
                       EXPORT_FILE_NAME btkIOExport.h)

SET_TARGET_PROPERTIES(IO PROPERTIES ${BTK_LIBRARY_PROPERTIES})
IF (NOT ${BTK_BUILD_SHARED_LIBS})
  SET_TARGET_PROPERTIES(IO PROPERTIES COMPILE_FLAGS -DBTK_IO_STATIC_DEFINE)
ENDIF()

INSTALL(TARGETS IO EXPORT BTKTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB __private "${CMAKE_CURRENT_SOURCE_DIR}/*_p.h")
IF (__private) 
  LIST(REMOVE_ITEM __files ${__private})
ENDIF ()
INSTALL(
  FILES 
    ${__files}
    ${CMAKE_CURRENT_BINARY_DIR}/btkIOExport.h
  DESTINATION include
  COMPONENT dev
)
