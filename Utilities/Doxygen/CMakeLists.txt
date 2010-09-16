CMAKE_DEPENDENT_OPTION(BUILD_DOCUMENTATION_API 
                       "Build API Documentation with Doxygen" ON
                       BUILD_DOCUMENTATION OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DOCUMENTATION_API_UNSELECTED_MODULES
                       "Build all the possible documentation even if the module is not selected (VizSupport, Matlab, ...)" OFF
                       BUILD_DOCUMENTATION_API OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DOCUMENTATION_MATLAB_FOR_WEB 
                       "Build the Matlab documentation for the web with Doxygen" OFF
                       BUILD_DOCUMENTATION OFF)
CMAKE_DEPENDENT_OPTION(BUILD_DOCUMENTATION_README 
                       "Build the Readme.html found in BTK root with Doxygen" OFF
                       BUILD_DOCUMENTATION OFF)
  
# Add all the known path for the extra documentation
IF (BUILD_DOCUMENTATION_API_UNSELECTED_MODULES)
  SET(BTK_WRAPPING_DOCUMENTATION "${BTK_SOURCE_DIR}/Wrapping")
  SET(BTK_WRAPPING_DOCUMENTATION_MATLAB "${BTK_SOURCE_DIR}/Wrapping/Matlab")
  SET(BTK_WRAPPING_DOCUMENTATION_SCILAB "${BTK_SOURCE_DIR}/Wrapping/Scilab")
  UNSET(BTK_VISSUPPORT_NO_DOCUMENTATION)
  UNSET(BTK_VTKSUPPORT_NO_DOCUMENTATION)
ENDIF(BUILD_DOCUMENTATION_API_UNSELECTED_MODULES)

# If choose to build documentation, then search for Doxygen executables.
IF(BUILD_DOCUMENTATION_API OR BUILD_DOCUMENTATION_README)
  FIND_PACKAGE(Doxygen REQUIRED)

  OPTION(DOXYGEN_SHORT_NAMES "Build BTK documentations with short files name" OFF)
  MARK_AS_ADVANCED(DOXYGEN_SHORT_NAMES)
  IF(DOXYGEN_SHORT_NAMES)
    SET(DOXYGEN_SHORT_NAMES YES)
  ELSE(DOXYGEN_SHORT_NAMES)
    SET(DOXYGEN_SHORT_NAMES NO)
  ENDIF(DOXYGEN_SHORT_NAMES)

  #
  # Configure the script and the doxyfile, then add target
  #
  SET(HAVE_DOT_YESNO NO)
  IF(DOT)
    SET(HAVE_DOT_YESNO YES)
    IF(NOT DOT_PATH)
      GET_FILENAME_COMPONENT(DOT_PATH ${DOT} PATH)
    ENDIF(NOT DOT_PATH)
  ENDIF(DOT)
ENDIF(BUILD_DOCUMENTATION_API OR BUILD_DOCUMENTATION_README)

# README target
IF(BUILD_DOCUMENTATION_README)
  FILE(READ ${BTK_SOURCE_DIR}/Documentation/Readme/License.dox.part LICENSE_DOX_PART)
  FILE(READ ${BTK_SOURCE_DIR}/Documentation/Readme/BuildInstructions.dox.part BUILD_INSTRUCTIONS_DOX_PART)

  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Readme/Readme.dox.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/Readme/Readme.dox)
    
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Utilities/Doxygen/Doxyfile_Readme.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_Readme)

  ADD_CUSTOM_TARGET(Doxygen_Readme ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BTK_BINARY_DIR}/Documentation/Readme
    COMMAND ${DOXYGEN_EXECUTABLE} ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_Readme
    MAIN_DEPENDENCY ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_Readme
    WORKING_DIRECTORY ${BTK_BINARY_DIR}/Utilities/Doxygen
    COMMAND ${CMAKE_COMMAND} -D BTK_BINARY_DIR:STRING=${BTK_BINARY_DIR} -D BTK_SOURCE_DIR:STRING=${BTK_SOURCE_DIR} -P "${BTK_SOURCE_DIR}/Documentation/Readme/replace_link.cmake"
    )
ENDIF(BUILD_DOCUMENTATION_README)

# API target
IF(BUILD_DOCUMENTATION_API)
  FILE(READ ${BTK_SOURCE_DIR}/Documentation/Readme/License.dox.part LICENSE_DOX_PART)
  FILE(READ ${BTK_SOURCE_DIR}/Documentation/Readme/BuildInstructions.dox.part BUILD_INSTRUCTIONS_DOX_PART)
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/API/License.dox.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/API/License.dox)
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/API/BuildInstructions.dox.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/API/BuildInstructions.dox)
  
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Utilities/Doxygen/Doxyfile_API.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_API)
    
  ADD_CUSTOM_TARGET(Doxygen_API ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BTK_BINARY_DIR}/Documentation/API 
    COMMAND ${DOXYGEN_EXECUTABLE} ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_API
    MAIN_DEPENDENCY ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_API
    WORKING_DIRECTORY ${BTK_BINARY_DIR}/Utilities/Doxygen)
ENDIF(BUILD_DOCUMENTATION_API)

# MATLAB_FOR_WEB
IF(BUILD_DOCUMENTATION_MATLAB_FOR_WEB)
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Utilities/Doxygen/Doxyfile_Wrapping_Matlab.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_Wrapping_Matlab)
    
  ADD_CUSTOM_TARGET(Doxyfile_Wrapping_Matlab ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab 
    COMMAND ${DOXYGEN_EXECUTABLE} ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_Wrapping_Matlab
    MAIN_DEPENDENCY ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile_Wrapping_Matlab
    WORKING_DIRECTORY ${BTK_BINARY_DIR}/Utilities/Doxygen
    COMMAND ${CMAKE_COMMAND} -D BTK_BINARY_DIR:STRING=${BTK_BINARY_DIR} -D BTK_SOURCE_DIR:STRING=${BTK_SOURCE_DIR} -P "${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/Web/finalize.cmake")
ENDIF(BUILD_DOCUMENTATION_MATLAB_FOR_WEB)

# API installation
IF(NOT BTK_INSTALL_NO_DOCUMENTATION)
  INSTALL(DIRECTORY ${BTK_BINARY_DIR}/Documentation/API
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Documentation
          COMPONENT Development)
ENDIF(NOT BTK_INSTALL_NO_DOCUMENTATION)

