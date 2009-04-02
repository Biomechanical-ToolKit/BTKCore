# If choose to build documentation, then search for Doxygen executables.
CMAKE_DEPENDENT_OPTION(BUILD_DOXYGEN "Build Doxygen Documentation" OFF
                       "BUILD_DOCUMENTATION" OFF)

IF(BUILD_DOXYGEN)
  FIND_PACKAGE(Doxygen REQUIRED)

  OPTION(DOXYGEN_SHORT_NAMES "Build Doxygen of BTK using short file names" OFF)
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

  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Utilities/Doxygen/Doxyfile.in
    ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile)

  ADD_CUSTOM_TARGET(Doxygen ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BTK_BINARY_DIR}/Documentation/Doxygen 
    COMMAND ${DOXYGEN_EXECUTABLE} ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile
    MAIN_DEPENDENCY ${BTK_BINARY_DIR}/Utilities/Doxygen/Doxyfile
    WORKING_DIRECTORY ${BTK_BINARY_DIR}/Utilities/Doxygen
    )

ENDIF(BUILD_DOXYGEN)
