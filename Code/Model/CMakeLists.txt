SET(BTKModel_SRCS
  btkJoint.cpp
  btkLandmarksTranslator.cpp
  btkModel.cpp
  btkPluginGait.cpp
  btkRelativePosition.cpp
  btkRelativeReferenceFrame.cpp
  btkRig.cpp
  btkSegment.cpp
)

ADD_LIBRARY(Model ${BTK_LIBS_BUILD_TYPE} ${BTKModel_SRCS})
TARGET_LINK_LIBRARIES(Model Trial)
TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
  $<BUILD_INTERFACE:${BTK_BINARY_DIR}>
  $<BUILD_INTERFACE:${BTK_SOURCE_DIR}/Code/Base>
  $<BUILD_INTERFACE:${BTK_SOURCE_DIR}/Code/Trial>
  $<BUILD_INTERFACE:${BTK_SOURCE_DIR}/ThirdParties/eigen3>
  $<INSTALL_INTERFACE:include/btk>
  $<INSTALL_INTERFACE:include/btk/3rdparty/eigen3>
)
GENERATE_EXPORT_HEADER(Model
                       BASE_NAME BTK_MODEL
                       EXPORT_FILE_NAME btkModelExport.h)

SET_TARGET_PROPERTIES(Model PROPERTIES ${BTK_LIBRARY_PROPERTIES})
IF (NOT ${BTK_BUILD_SHARED_LIBS})
  SET_TARGET_PROPERTIES(Model PROPERTIES COMPILE_FLAGS -DBTK_MODEL_STATIC_DEFINE)
ENDIF()

INSTALL(TARGETS Model EXPORT BTKTargets
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
    ${CMAKE_CURRENT_BINARY_DIR}/btkModelExport.h
  DESTINATION include
  COMPONENT dev
)
