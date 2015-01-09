SET(BTKCommon_SRCS
  # btkAcquisition.cpp
  # btkAnalog.cpp
  btkAny.cpp
  # btkDataObject.cpp
  btkEvent.cpp
  # btkForcePlatform.cpp
  # btkIMU.cpp
  btkLogger.cpp
  # btkMetaData.cpp
  # btkMetaDataInfo.cpp
  # btkMetaDataUtils.cpp
  btkNode.cpp
  btkObject.cpp
  # btkPoint.cpp
  # btkProcessObject.cpp
  # btkTimeSequence.cpp
  # btkTriangleMesh.cpp
  # btkWrench.cpp
)

ADD_LIBRARY(Common ${BTK_LIBS_BUILD_TYPE} ${BTKCommon_SRCS})
TARGET_INCLUDE_DIRECTORIES(Common PUBLIC
  $<BUILD_INTERFACE:${BTK_BINARY_DIR}>
  $<BUILD_INTERFACE:${BTK_SOURCE_DIR}/ThirdParties/eigen3>
  $<INSTALL_INTERFACE:include/btk>
  $<INSTALL_INTERFACE:include/btk/3rdparty/eigen3>
)
GENERATE_EXPORT_HEADER(Common
                       BASE_NAME BTK_COMMON
                       EXPORT_FILE_NAME btkCommonExport.h)

SET_TARGET_PROPERTIES(Common PROPERTIES ${BTK_LIBRARY_PROPERTIES})
IF (NOT ${BTK_BUILD_SHARED_LIBS})
  SET_TARGET_PROPERTIES(Common PROPERTIES COMPILE_FLAGS -DBTK_COMMON_STATIC_DEFINE)
ENDIF()

INSTALL(TARGETS Common EXPORT BTKTargets
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
    ${CMAKE_CURRENT_BINARY_DIR}/btkCommonExport.h
  DESTINATION include
  COMPONENT dev
)
