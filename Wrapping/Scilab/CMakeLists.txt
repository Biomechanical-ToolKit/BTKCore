FIND_PACKAGE(Scilab REQUIRED)

CMAKE_DEPENDENT_OPTION(BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES
                       "Compiler option to be able to redistribute BTK SCILAB files on machine without the same developper environment (builds BTK in Release mode and links libraries in static mode (if MSVC is the compiler)." OFF
                       BTK_WRAP_SCILAB OFF)

IF (BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES)
  SET(CMAKE_OLD_BUILD_TYPE 0 CACHE STRING "CMAKE_BUILD_TYPE old value")
  SET(CMAKE_OLD_C_FLAGS_RELEASE 0 CACHE STRING "CMAKE_C_FLAGS_RELEASE old value")
  SET(CMAKE_OLD_CXX_FLAGS_RELEASE 0 CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value")
  MARK_AS_ADVANCED(CMAKE_OLD_BUILD_TYPE CMAKE_OLD_C_FLAGS_RELEASE CMAKE_OLD_CXX_FLAGS_RELEASE)
  IF(BTK_BUILD_SHARED_LIBS AND MSVC)
    MESSAGE(FATAL_ERROR "The use of options BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES and BUILD_SHARED_LIBS together are not compatible. The first wants to create a static library and the second a dynamic library. By using BUILD_SHARED_LIBS in Release mode, you can redistribute the MEX-Files, but the target machines need a \"Microsoft Visual C++ Redistributable Package\" corresponding to the used compiler.")
  ENDIF(BTK_BUILD_SHARED_LIBS AND MSVC)

  IF(NOT CMAKE_OLD_BUILD_TYPE)
    SET(CMAKE_OLD_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "CMAKE_BUILD_TYPE old value" FORCE)
    SET(CMAKE_OLD_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE} CACHE STRING "CMAKE_C_FLAGS_RELEASE old value" FORCE)
    SET(CMAKE_OLD_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "CMAKE_CXX_FLAGS_RELEASE old value" FORCE)
  ENDIF(NOT CMAKE_OLD_BUILD_TYPE)

  SET(CMAKE_BUILD_TYPE Release CACHE STRING "The build type is fixed due to the activation of BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES, Uncheck it to go to previous options." FORCE)
  IF (MSVC)
    SET(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "The C flags are fixed due to the activation of BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES, Uncheck it to go to previous flags." FORCE)
    SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "The CXX flags are fixed due to the activation of BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES, Uncheck it to go to previous flags." FORCE)
  ENDIF(MSVC)
ENDIF(BTK_WRAP_SCILAB_REDISTRIBUABLE_FILES)

SET(SCILAB_TOOLBOX_NAME "btk")

IF(SCILAB_ROOT)
  # Look for all MEX functions
  FILE(GLOB SCILAB_MEX_SRCS "${BTK_SOURCE_DIR}/Wrapping/Matlab/*.cpp")
  # And the the internal functions
  FILE(GLOB SCILAB_CXX_SRCS "${BTK_SOURCE_DIR}/Wrapping/Matlab/*.cxx")

  # Generate the gateway file: libBTK.c
  SET(LIBBTKC_PATH "${CMAKE_BINARY_DIR}/Wrapping/Scilab/lib${SCILAB_TOOLBOX_NAME}.c")
  FILE(WRITE ${LIBBTKC_PATH} "\#include <mex.h>\nstatic int direct_gateway(char *fname,void F(void)) { F();return 0;};\n")
  FOREACH(FUNCTION ${SCILAB_MEX_SRCS})
    GET_FILENAME_COMPONENT(FUNC ${FUNCTION} NAME_WE)
    FILE(APPEND ${LIBBTKC_PATH} "extern Gatefunc mex_${FUNC};\n")
  ENDFOREACH(FUNCTION)
  FILE(APPEND ${LIBBTKC_PATH} "static GenericTable Tab[]={\n")
  FOREACH(FUNCTION ${SCILAB_MEX_SRCS})
    GET_FILENAME_COMPONENT(FUNC ${FUNCTION} NAME_WE)
    FILE(APPEND ${LIBBTKC_PATH} "  {mex_gateway,mex_${FUNC},\"${FUNC}\"},\n")
  ENDFOREACH(FUNCTION)
  FILE(APPEND ${LIBBTKC_PATH} "};\n\nint C2F(lib${SCILAB_TOOLBOX_NAME})()\n{\n  Rhs = Max(0, Rhs);\n  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);\n  return 0;\n}")

  # Include
  INCLUDE_DIRECTORIES("${SCILAB_MEX_INCLUDE_DIR}")
  INCLUDE_DIRECTORIES("${SCILAB_CORE_INCLUDE_DIR}")
  INCLUDE_DIRECTORIES("${BTK_SOURCE_DIR}/Wrapping/Matlab/")

  # Build the library
  SET(SCILAB_SRCS "lib${SCILAB_TOOLBOX_NAME}.c" ${SCILAB_MEX_SRCS})
  ADD_LIBRARY(${SCILAB_TOOLBOX_NAME} SHARED ${SCILAB_SRCS} ${SCILAB_CXX_SRCS})
  FOREACH(FUNCTION ${SCILAB_SRCS})
    GET_FILENAME_COMPONENT(FUNC ${FUNCTION} NAME_WE)
    SET_SOURCE_FILES_PROPERTIES(${FUNCTION} PROPERTIES COMPILE_FLAGS "-DmexFunction=mex_${FUNC}")
  ENDFOREACH(FUNCTION)
  SET_TARGET_PROPERTIES(${SCILAB_TOOLBOX_NAME} PROPERTIES PREFIX "lib")
  TARGET_LINK_LIBRARIES(${SCILAB_TOOLBOX_NAME} ${SCILAB_IMPL_LIBRARY} BTKCommon BTKIO BTKBasicFilters)
  IF(MSVC)
    # Generate the DEF file (instead of the use of Dumpexts provided with Scilab)
    SET(LIBBTKDEF_PATH "${CMAKE_BINARY_DIR}/Wrapping/Scilab/lib${SCILAB_TOOLBOX_NAME}.def")
    FILE(WRITE ${LIBBTKDEF_PATH} "LIBRARY    lib${SCILAB_TOOLBOX_NAME}.dll\nEXPORTS\n")
    FILE(APPEND ${LIBBTKDEF_PATH} "\tlib${SCILAB_TOOLBOX_NAME}_\n")
    FOREACH(FUNCTION ${SCILAB_MEX_SRCS})
      GET_FILENAME_COMPONENT(FUNC ${FUNCTION} NAME_WE)
      FILE(APPEND ${LIBBTKDEF_PATH} "\tmex_${FUNC}\n")
    ENDFOREACH(FUNCTION)
    # Supplementary flags for MSVC
    SET_TARGET_PROPERTIES(${SCILAB_TOOLBOX_NAME} PROPERTIES COMPILE_FLAGS "/D__MSC__ /DSTRICT /D_CRT_SECURE_NO_DEPRECATE /D__MAKEFILEVC__ /DFORDLL")  
    SET_TARGET_PROPERTIES(${SCILAB_TOOLBOX_NAME} PROPERTIES LINK_FLAGS "/DEF:\"${LIBBTKDEF_PATH}\"")
  ENDIF(MSVC)

  # Generate the associated Loader.sce
  SET(GATEWAY_LOADER "${CMAKE_BINARY_DIR}/bin/loader_mex_gateway.sce")
  FILE(WRITE ${GATEWAY_LOADER} "lib${SCILAB_TOOLBOX_NAME}_path = get_file_path('loader_mex_gateway.sce');\nlist_functions = [\n")
  FOREACH(FUNCTION ${SCILAB_MEX_SRCS})
    GET_FILENAME_COMPONENT(FUNC ${FUNCTION} NAME_WE)
    FILE(APPEND ${GATEWAY_LOADER} "  '${FUNC}';\n")
  ENDFOREACH(FUNCTION)
  FILE(APPEND ${GATEWAY_LOADER} "];\naddinter(lib${SCILAB_TOOLBOX_NAME}_path+'/lib${SCILAB_TOOLBOX_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}','lib${SCILAB_TOOLBOX_NAME}',list_functions);\nclear lib${SCILAB_TOOLBOX_NAME}_path;\nclear list_functions;\nclear get_file_path;\n")
ENDIF(SCILAB_ROOT)

IF(NOT BTK_INSTALL_NO_LIBRARIES)
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Documentation/Wrapping/Scilab/btk
          DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Scilab  
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE)

  INSTALL(FILES ${GATEWAY_LOADER}
    DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Scilab/btk/sci_gateway
    COMPONENT Development)

# Because CMake copy btk.lib on windows and is not necessary
IF(WIN32)
  INSTALL(TARGETS ${SCILAB_TOOLBOX_NAME}
    RUNTIME DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Scilab/btk/sci_gateway COMPONENT RuntimeLibraries)
ELSE(WIN32)
  INSTALL(TARGETS ${SCILAB_TOOLBOX_NAME}
    LIBRARY DESTINATION ${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Scilab/btk/sci_gateway COMPONENT RuntimeLibraries)
ENDIF(WIN32)

  INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${SCILAB_BINARY} -nw -f \"${CMAKE_INSTALL_PREFIX}/${BTK_INSTALL_SHARE_DIR_CM24}/Wrapping/Scilab/btk/builder.sce\")")
ENDIF(NOT BTK_INSTALL_NO_LIBRARIES)

