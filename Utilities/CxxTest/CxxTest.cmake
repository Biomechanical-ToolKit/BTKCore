# Simple CMake script which include paths of CxxTest and additional content.
# The variable for the source files is not provided as they are all included in 
# one file 'cxxtest/Root.cpp'. This file is included at the end of the main.cpp
# file.

GET_FILENAME_COMPONENT(CXXTEST_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)

INCLUDE_DIRECTORIES("${CXXTEST_ROOT}"
                    "${CXXTEST_ROOT}/additional/generator/macro")
