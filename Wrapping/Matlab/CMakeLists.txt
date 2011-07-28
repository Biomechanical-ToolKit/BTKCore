FIND_PACKAGE(Matlab REQUIRED)

CMAKE_DEPENDENT_OPTION(BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES
                       "Compiler option to be able to redistribute BTK MEX files on machine without the same developper environment (builds BTK in Release mode and links libraries in static mode (if MSVC is the compiler)." OFF
                       BTK_WRAP_MATLAB OFF)

IF (BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES)
  SET(CMAKE_BUILD_MRD 1 CACHE STRING "State of building BTK with the Matlab Wrapping in redistribuable mode" FORCE)
  SET(CMAKE_OLD_BUILD_TYPE 0 CACHE STRING "CMAKE_BUILD_TYPE old value")
  SET(CMAKE_OLD_CXX_FLAGS_RELEASE 0 CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value")
  MARK_AS_ADVANCED(CMAKE_BUILD_MRD CMAKE_OLD_BUILD_TYPE CMAKE_OLD_CXX_FLAGS_RELEASE)
  IF(BTK_BUILD_SHARED_LIBS AND MSVC)
    MESSAGE(FATAL_ERROR "The use of options BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES and BUILD_SHARED_LIBS together are not compatible. The first wants to create a static library and the second a dynamic library. By using BUILD_SHARED_LIBS in Release mode, you can redistribute the MEX-Files, but the target machines need a \"Microsoft Visual C++ Redistributable Package\" corresponding to the used compiler.")
  ENDIF(BTK_BUILD_SHARED_LIBS AND MSVC)

  IF(NOT CMAKE_OLD_BUILD_TYPE)
    SET(CMAKE_OLD_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "CMAKE_BUILD_TYPE old value" FORCE)
  ENDIF(NOT CMAKE_OLD_BUILD_TYPE)
  IF(NOT CMAKE_OLD_CXX_FLAGS_RELEASE)
    SET(CMAKE_OLD_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value" FORCE)
  ENDIF(NOT CMAKE_OLD_CXX_FLAGS_RELEASE)

  SET(CMAKE_BUILD_TYPE Release CACHE STRING "The build type is fixed due to the activation of BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES, Uncheck it to go to previous options." FORCE)
  IF (MSVC)
    IF (${CMAKE_SYSTEM_VERSION} LESS 6.1) # Windows XP, VISTA
      SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "The CXX flags are fixed due to the activation of BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES, Uncheck it to go to previous flags." FORCE)
      MESSAGE(STATUS "WARNING: Due to a different mechanism introduced in Windows 7, it is not possible to redistribute the MEX files compiled for Windows XP or Vista into Windows 7.")
    ELSE (${CMAKE_SYSTEM_VERSION} LESS 6.1) # Windows 7 or upper.
      MESSAGE(STATUS "WARNING: Due to a different mechanism introduced in Windows 7, the redistribution of the MEX files into Windows XP or Vista compiled from Windows 7 requires to install the 'Visual C++ Redistributable Package'.")
    ENDIF (${CMAKE_SYSTEM_VERSION} LESS 6.1)
  ENDIF(MSVC)  
ENDIF(BTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES)

IF(MATLAB_MEXFILE_EXT STREQUAL mexa64)
  # Force to build library (Common, IO, ...) as shared objects for linux 64-bit.
  # See issue 9: http://code.google.com/p/b-tk/issues/detail?id=9
  MESSAGE(STATUS "-------------------------------------------------------------------------")
  MESSAGE(STATUS "WARNING: Due to the impossibility to mix static and dynamic library under\nlinux 64-bit, BTK is set to build shared libraries.")
  SET(BUILD_SHARED_LIBS TRUE CACHE BOOL "This option is fixed because you cannot mix static and dynamic library under linux 64-bit." FORCE)
  # Message about the incompatibility with the library 'libstdc++' embedded into Matlab
  MESSAGE(STATUS "-------------------------------------------------------------------------")
  MESSAGE(STATUS "WARNING: Because Matlab embeds its own version of the library 'libstdc++'\nand then create incompatibility with MEX files, this library will be\nmoved during the installation step into {MATLAB_ROOT}/sys/os/glnxa64/_fix_btk_linux_64\ninstead of {MATLAB_ROOT}/sys/os/glnxa64")
  MESSAGE(STATUS "-------------------------------------------------------------------------")
ENDIF(MATLAB_MEXFILE_EXT STREQUAL mexa64)

IF(NOT MATLAB_ROOT)
  MESSAGE(STATUS "")
  MESSAGE(FATAL_ERROR "ERROR: Matlab was not found!")
ELSE(NOT MATLAB_ROOT)
  INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})
  
  # All the defintions to build the wrapping functions are in the file CMakeLists.part.in
  # We need to define the macro used to build the functions (MEX_CREATE_MACRO).
  # And if necessary the relative to the sources (MEX_PATH_PREFIX).
  SET(MEX_CREATE_MACRO "MATLAB_MEX_CREATE")
  SET(MEX_PATH_PREFIX "")
  CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.part.in
    ${CMAKE_CURRENT_BINARY_DIR}/WrappingFunctions.cmake @ONLY)
  INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/WrappingFunctions.cmake")
ENDIF(NOT MATLAB_ROOT)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  # Matlab M file to configure
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m.in
    ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m)
  INSTALL(FILES ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/btkGetVersion.m
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk
          COMPONENT Development)
  # Documentation install
  # Main document file with BTK version
  CONFIGURE_FILE(
    ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m.in
    ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m)
  INSTALL(FILES ${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/btk/Contents.m
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk
          COMPONENT Development)
  # Whole documentation
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Documentation/Wrapping/Matlab/btk
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab  
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN "*.in" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE)

  # MEX Files install
  # Fix a problem with this installation method and the use of the MS Visual Studio generators.
  IF(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    INSTALL(DIRECTORY "${BTK_LIBRARY_PATH}/@BUILD_TYPE@/"
            DESTINATION "${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk"
            COMPONENT Development
            FILES_MATCHING PATTERN "*.${MATLAB_MEXFILE_EXT}")
  ELSE(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    INSTALL(DIRECTORY "${BTK_LIBRARY_PATH}/"
            DESTINATION "${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Matlab/btk"
            COMPONENT Development
            FILES_MATCHING PATTERN "*.${MATLAB_MEXFILE_EXT}")
  ENDIF(CMAKE_CXX_COMPILER MATCHES "^.*cl$")
  
  IF(MATLAB_MEXFILE_EXT STREQUAL mexa64)
    # Move dynamic library libstdc++ embedded with Matlab
    # See issue 8: http://code.google.com/p/b-tk/issues/detail?id=8
    FILE(GLOB MATLAB_LIBSTDCXX_LIBRARY_FILES "${MATLAB_ROOT}/sys/os/glnxa64/libstdc++.so*")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory \"${MATLAB_ROOT}/sys/os/glnxa64/_fix_btk_linux_64\")")
    FOREACH(_file ${MATLAB_LIBSTDCXX_LIBRARY_FILES})
       INSTALL(CODE "EXECUTE_PROCESS(COMMAND sudo cp -R \"${_file}\" \"${MATLAB_ROOT}/sys/os/glnxa64/_fix_btk_linux_64/\"
                                     COMMAND sudo rm \"${_file}\")")
    ENDFOREACH(_file)
                                  
  ENDIF(MATLAB_MEXFILE_EXT STREQUAL mexa64)
ENDIF(NOT BTK_INSTALL_NO_LIBRARIES)

