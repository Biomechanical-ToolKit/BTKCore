INCLUDE(${CMAKE_MODULE_PATH}/btkGetC3DSamples.cmake)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/tddDefinitions.h.in ${CMAKE_CURRENT_BINARY_DIR}/tddDefinitions.h)
INCLUDE_DIRECTORIES(${BTK_BINARY_DIR}/Testing/Code)

SET(TDD_SRCS
  main.cpp
  )

LINK_DIRECTORIES(${BTK_LIBRARY_PATH})

# The CxxTest library included with BTK is modified.
# This version uses C/C++ Macros instead of Python/Perl test generator.
INCLUDE_DIRECTORIES(${BTK_SOURCE_DIR}/Utilities/CxxTest)

ADD_EXECUTABLE(TDD ${TDD_SRCS})

TARGET_LINK_LIBRARIES(TDD BTKCommon BTKIO)

ADD_TEST(TDD ${BTK_EXECUTABLE_PATH}/TDD)

