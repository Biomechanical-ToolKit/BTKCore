# As with the source code for the MEX functions, the code sued for the unit test is the Matlab one.

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../Matlab/_TDD.m.in ${BTK_EXECUTABLE_PATH}/TDD.m)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../Matlab/_TDDConfigure.m.in ${CMAKE_CURRENT_BINARY_DIR}/TDDConfigure.m)

ADD_TEST(NAME "TDD_Octave" WORKING_DIRECTORY ${BTK_EXECUTABLE_PATH} COMMAND ${OCTAVE_EXECUTABLE} "--eval \"out = TDD; if (out) exit(0); else exit(1); end\"")