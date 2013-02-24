Project(EasyInstall Java)

MACRO(MAKE_JAR_EXECUTABLE VAR_TARGET VAR_ENTRY)
  GET_TARGET_PROPERTY(VAR_LOCATION "${VAR_TARGET}" LOCATION)
  ADD_CUSTOM_COMMAND(TARGET ${VAR_TARGET} POST_BUILD COMMAND ${CMAKE_Java_ARCHIVE} ufe ${VAR_LOCATION} ${VAR_ENTRY} COMMENT "Creating JAR entry point.")
  UNSET(VAR_LOCATION)
ENDMACRO()

SET(EASYINSTALL_SRC 
    EasyInstall.java)
 
SET(CMAKE_Java_FLAGS "-source 1.5 -target 1.5")
    
SET (EASYINSTALL_ENTRY "EasyInstall") 

ADD_LIBRARY(EasyInstall ${EASYINSTALL_SRC})
MAKE_JAR_EXECUTABLE(EasyInstall "${EASYINSTALL_ENTRY}")

ADD_CUSTOM_COMMAND(TARGET EasyInstall POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${BTK_BINARY_DIR}/bin/EasyInstall.jar ${BTK_SOURCE_DIR}/EasyInstallWithRedistributableMatlabToolbox.jar
  COMMENT "Copying to EasyInstallWithRedistributableMatlabToolbox.jar in the root."
  )
