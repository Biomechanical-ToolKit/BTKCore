# libb64 build file

cmake_minimum_required(VERSION 2.6)

project (libb64)

# Inlclude path
include_directories (${PROJECT_SOURCE_DIR}/include)

# Build the library
file(GLOB_RECURSE Source_Files ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(libb64 ${Source_Files})

# Place in output path so other projects can also link
install(TARGETS libb64 LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
