CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/_TDDConfigure.h.in ${CMAKE_CURRENT_BINARY_DIR}/_TDDConfigure.h)

SET(TDD_SRCS
  _TDD.cpp
  _TDDBase.cpp
  _TDDExtras.cpp
  _TDDIO.cpp
  _TDDModel.cpp
  _TDDTrial.cpp
  )

# The CxxTest library included with BTK is modified.
# This version uses C/C++ Macros instead of Python/Perl test generator.
INCLUDE(${BTK_SOURCE_DIR}/Utilities/CxxTest/CxxTest.cmake)

ADD_EXECUTABLE(TDD ${TDD_SRCS})

TARGET_LINK_LIBRARIES(TDD Base Trial IO Model)

TARGET_INCLUDE_DIRECTORIES(TDD PRIVATE
  ${BTK_INCLUDE_DIRS_BUILD_TREE}
)

ADD_TEST(TDD_C++ ${BTK_EXECUTABLE_PATH}/TDD)

