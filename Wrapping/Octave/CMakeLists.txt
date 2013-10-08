FIND_PACKAGE(Octave REQUIRED)

IF (UNIX AND NOT APPLE AND HAVE_64_BIT AND NOT BUILD_SHARED_LIBS)
  # Force to build library (Common, IO, ...) as shared objects for Linux 64-bit.
  # See issue 9: http://code.google.com/p/b-tk/issues/detail?id=9
  MESSAGE(STATUS "\n-------------------------------------------------------------------------")
  MESSAGE(STATUS "WARNING: Due to the impossibility to mix static and dynamic library under\nLinux 64-bit, BTK is set to build shared libraries.")
  MESSAGE(STATUS "-------------------------------------------------------------------------")
  SET(BUILD_SHARED_LIBS TRUE CACHE BOOL "This option is fixed because you cannot mix static and dynamic library under Linux 64-bit." FORCE)
ENDIF (UNIX AND NOT APPLE AND HAVE_64_BIT AND NOT BUILD_SHARED_LIBS)

IF(NOT OCTAVE_FOUND)
  MESSAGE(STATUS "")
  MESSAGE(FATAL_ERROR "ERROR: Octave was not found!")
ELSE(NOT OCTAVE_FOUND)
  INCLUDE_DIRECTORIES(${OCTAVE_INCLUDE_DIR})
  
  # We basically compile all files that are compiled for the matlab wrapping.
  # All the defintions to build the wrapping functions are in the file CMakeLists.part.in
  # We need to define the macro used to build the functions
  SET(MEX_CREATE_MACRO "OCTAVE_MEX_CREATE")
  # The sources are in the folder Matlab as well as a part of the script to build them
  SET(MEX_PATH_PREFIX "../Matlab/") # Do not forget the slah at the end!
  # All the defintions to build the wrapping functions are in the file CMakeLists.part.in
  CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Matlab/CMakeLists.part.in
    ${CMAKE_CURRENT_BINARY_DIR}/WrappingFunctions.cmake @ONLY)
  INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/WrappingFunctions.cmake")
ENDIF(NOT OCTAVE_FOUND)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  # Octave M file to configure
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m.in
    ${BTK_BINARY_DIR}/Documentation/Wrapping/Octave/btk/btkGetVersion.m)
  INSTALL(FILES ${BTK_BINARY_DIR}/Documentation/Wrapping/Octave/btk/btkGetVersion.m
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Octave/btk
          COMPONENT Development)
  # Documentation install
  # Main document file with BTK version
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m.in
    ${BTK_BINARY_DIR}/Documentation/Wrapping/Octave/btk/Contents.m)
  INSTALL(FILES ${BTK_BINARY_DIR}/Documentation/Wrapping/Octave/btk/Contents.m
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Octave/btk
          COMPONENT Development)
  # Whole documentation
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Octave  
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN "*.in" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE)

  # MEX Files install
  # Fix a problem with this installation method and the use of the MS Visual Studio generators.
  IF(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    INSTALL(DIRECTORY "${BTK_LIBRARY_PATH}/@BUILD_TYPE@/"
            DESTINATION "${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Octave/btk"
            COMPONENT Development
            FILES_MATCHING PATTERN "*.mex")
  ELSE(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    INSTALL(DIRECTORY "${BTK_LIBRARY_PATH}/"
            DESTINATION "${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Octave/btk"
            COMPONENT Development
            FILES_MATCHING PATTERN "*.mex")
  ENDIF(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
  
ENDIF(NOT BTK_INSTALL_NO_LIBRARIES)

