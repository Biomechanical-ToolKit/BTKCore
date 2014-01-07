INCLUDE(${BTK_CMAKE_MODULE_PATH}/btkGetC3DSamples.cmake)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/_TDDConfigure.h.in ${CMAKE_CURRENT_BINARY_DIR}/_TDDConfigure.h)
INCLUDE_DIRECTORIES(${BTK_BINARY_DIR}/Testing/C++)

SET(TDD_SRCS
  _TDD.cpp
  _TDDBasicFilters.cpp
  _TDDCommon.cpp
  _TDDExtras.cpp
  _TDDIO.cpp
  )

LINK_DIRECTORIES(${BTK_LIBRARY_PATH})

# The CxxTest library included with BTK is modified.
# This version uses C/C++ Macros instead of Python/Perl test generator.
INCLUDE(${BTK_SOURCE_DIR}/Utilities/CxxTest/CxxTest.cmake)

ADD_EXECUTABLE(TDD ${TDD_SRCS})

TARGET_LINK_LIBRARIES(TDD BTKCommon BTKBasicFilters BTKIO)

ADD_TEST(TDD_C++ ${BTK_EXECUTABLE_PATH}/TDD)

