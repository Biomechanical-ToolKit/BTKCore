# When the code is compiled under Linux 32-bit from a 64-bit machine, some unit test fails
# due to some rounding errors. The most important issue is when the code is compiled in 
# Release mode. The number of frames computed for ANC files is sometimes wrong. This error
# does not happen when the code is compiled in Debug mode.
# Adding the flag -ffloat-store to CMAKE_CXX_FLAGS solves the problem. All the unit tests
# passed after that.
IF( (${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_SIZEOF_VOID_P EQUAL 4) )
  MESSAGE(STATUS "\nFlag -ffloat-store added to CMAKE_CXX_FLAGS to fix possible round off errors under Linux 32-bit.\n")
  SET(CMAKE_CXX_FLAGS "-ffloat-store")
  SET(BTK_REQUIRED_CXX_FLAGS ${CMAKE_CXX_FLAGS})
ENDIF( (${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_SIZEOF_VOID_P EQUAL 4) )