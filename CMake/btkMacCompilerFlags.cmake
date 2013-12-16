# Fix for MacOS 10.9 and the value of CMAKE_OSX_DEPLOYMENT_TARGET.
# Since MacOS 10.9, the default is to use libc++ and C++11.
# If the CMAKE_OSX_DEPLOYMENT_TARGET is set to 10.8 or less, the C++11 flag is not activated. In this case, we force to use the libstdc++ library
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  EXEC_PROGRAM(sw_vers ARGS -productVersion OUTPUT_VARIABLE MACOS_VERSION)
  IF(MACOS_VERSION VERSION_GREATER "10.8")
    IF((NOT "${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "") AND (CMAKE_OSX_DEPLOYMENT_TARGET VERSION_LESS "10.9"))
      MESSAGE(STATUS "\nForce to use libc++ under MacOS 10.9 and greater even if the\ndeployment target is set to 10.8 or less.\n")
      SET(CMAKE_CXX_FLAGS "-stdlib=libc++")
      SET(BTK_REQUIRED_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    ENDIF((NOT "${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "") AND (CMAKE_OSX_DEPLOYMENT_TARGET VERSION_LESS "10.9"))
  ENDIF(MACOS_VERSION VERSION_GREATER "10.8")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")