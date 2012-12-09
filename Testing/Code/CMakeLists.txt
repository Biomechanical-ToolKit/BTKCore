INCLUDE(${BTK_CMAKE_MODULE_PATH}/btkGetC3DSamples.cmake)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/_TDDConfigure.h.in ${CMAKE_CURRENT_BINARY_DIR}/_TDDConfigure.h)
INCLUDE_DIRECTORIES(${BTK_BINARY_DIR}/Testing/Code)

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
INCLUDE_DIRECTORIES(${BTK_SOURCE_DIR}/Utilities/CxxTest)

ADD_EXECUTABLE(TDD ${TDD_SRCS})

TARGET_LINK_LIBRARIES(TDD BTKCommon BTKBasicFilters BTKIO ${BTKVTKAdapter} ${VTK_LIBRARIES_REQUIRED})

ADD_TEST(TDD ${BTK_EXECUTABLE_PATH}/TDD)

