# pugixml build file

cmake_minimum_required(VERSION 2.6)

project (pugixml)

# Inlclude path
include_directories (${PROJECT_SOURCE_DIR}/src)

# Build the library
file(GLOB_RECURSE Source_Files ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(pugixml ${Source_Files})

# Place in output path so other projects can also link
install(TARGETS pugixml LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
