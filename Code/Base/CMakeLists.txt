SET(BTKBase_SRCS
  btkAlgorithm.cpp
  btkAny.cpp
  btkDataFactory.cpp
  btkExecutive.cpp
  btkInformation.cpp
  btkLogger.cpp
  btkNode.cpp
  btkObject.cpp
  btkPort.cpp
)

ADD_LIBRARY(Base ${BTK_LIBS_BUILD_TYPE} ${BTKBase_SRCS})
TARGET_INCLUDE_DIRECTORIES(Base PUBLIC
  $<BUILD_INTERFACE:${BTK_BINARY_DIR}>
  $<BUILD_INTERFACE:${BTK_SOURCE_DIR}/ThirdParties/eigen3>
  $<INSTALL_INTERFACE:include/btk>
  $<INSTALL_INTERFACE:include/btk/3rdparty/eigen3>
)
GENERATE_EXPORT_HEADER(Base
                       BASE_NAME BTK_BASE
                       EXPORT_FILE_NAME btkBaseExport.h)

SET_TARGET_PROPERTIES(Base PROPERTIES ${BTK_LIBRARY_PROPERTIES})
IF (NOT ${BTK_BUILD_SHARED_LIBS})
  SET_TARGET_PROPERTIES(Base PROPERTIES COMPILE_FLAGS -DBTK_BASE_STATIC_DEFINE)
ENDIF()

INSTALL(TARGETS Base EXPORT BTKTargets
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
    ${CMAKE_CURRENT_BINARY_DIR}/btkBaseExport.h
  DESTINATION include
  COMPONENT dev
)
