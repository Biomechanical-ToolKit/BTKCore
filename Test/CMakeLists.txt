SET(TDD_SRCS
  main.cpp
  )

LINK_DIRECTORIES(${BTK_LIBRARY_PATH})

# The CxxTest library included with BTK is modified.
# This version uses C/C++ Macros instead of Python/Perl test generator.
INCLUDE_DIRECTORIES(
  ${BTK_SOURCE_DIR}/Utilities/CxxTest)

ADD_EXECUTABLE(TDD ${TDD_SRCS})

TARGET_LINK_LIBRARIES(TDD BTKCommon BTKIO)

ADD_TEST(TDD ${BTK_EXECUTABLE_PATH}/TDD)

