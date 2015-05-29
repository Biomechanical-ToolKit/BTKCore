# Based on the file vtkCompilerExtras.cmake
IF(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
  INCLUDE(CheckCXXCompilerFlag)

  # Compatible flags between GCC and Clang
  # -Wshadow has been removed due to eigen
  SET(CMAKE_CXX_FLAGS_WARN "-Wnon-virtual-dtor -Wno-long-long -Wcast-align -Wchar-subscripts -Wall -Wextra -Wpointer-arith -Wformat-security -Woverloaded-virtual -Wunused-parameter -fno-common")
  # Addtional warnings for GCC
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS_WARN "${CMAKE_CXX_FLAGS_WARN} -fno-check-new -ansi")
  ENDIF()  

  # This flag is useful as not returning from a non-void function is an error
  # with MSVC, but it is not supported on all GCC compiler versions
  CHECK_CXX_COMPILER_FLAG(-Werror=return-type HAVE_COMPILER_ERROR_RETURN_TYPE)
  IF(HAVE_COMPILER_ERROR_RETURN_TYPE)
    SET(CMAKE_CXX_FLAGS_ERROR "-Werror=return-type")
  ENDIF()

  # Set up the debug CXX_FLAGS for extra warnings
  OPTION(USE_COMPILER_EXTRA_FLAGS_DEBUG "Add compiler flags to do stricter checking when building debug." OFF)
  IF(USE_COMPILER_EXTRA_FLAGS_DEBUG)
    SET(CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO 0 CACHE STRING "CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO old value")
    SET(CMAKE_OLD_CXX_FLAGS_DEBUG 0 CACHE STRING "CMAKE_OLD_CXX_FLAGS_DEBUG old value")
    MARK_AS_ADVANCED(CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO CMAKE_OLD_CXX_FLAGS_DEBUG)
    IF(NOT CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO)
      SET(CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} CACHE STRING "CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO old value" FORCE)
    ENDIF()
    IF(NOT CMAKE_OLD_CXX_FLAGS_DEBUG)
      SET(CMAKE_OLD_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} CACHE STRING "CMAKE_OLD_CXX_FLAGS_DEBUG old value" FORCE)
    ENDIF()
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_WARN}" CACHE STRING "Flags used by the compiler during Release with Debug builds." FORCE)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_OLD_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_WARN} ${CMAKE_CXX_FLAGS_ERROR}" CACHE STRING "Flags used by the compiler during debug builds." FORCE)
  ELSE()
    IF(CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO)
      SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO} CACHE STRING "Flags used by the compiler during Release with Debug builds." FORCE)
      SET(CMAKE_OLD_CXX_FLAGS_RELWITHDEBINFO 0 CACHE STRING "" FORCE)
    ENDIF()
    IF(CMAKE_OLD_CXX_FLAGS_DEBUG)
      SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_OLD_CXX_FLAGS_DEBUG} CACHE STRING "Flags used by the compiler during debug builds." FORCE)
      SET(CMAKE_OLD_CXX_FLAGS_DEBUG 0 CACHE STRING "" FORCE)
    ENDIF()
  ENDIF()
ENDIF()
