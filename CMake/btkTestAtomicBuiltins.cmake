# This script is based on the VTK script "vtkTestBuiltins.cmake"

# check for gcc/clang atomic builtins like __sync_add_and_fetch
IF(NOT WIN32)
  IF("HAVE_ATOMIC_BUILTINS" MATCHES "^HAVE_ATOMIC_BUILTINS$")
    MESSAGE(STATUS "Checking for builtin __sync_add_and_fetch")
    TRY_COMPILE(BTK_TEST_ATOMIC_BUILTINS_COMPILED 
      ${BTK_BINARY_DIR} ${CMAKE_SOURCE_DIR}/CMake/Code/btkTestAtomicBuiltins.cpp OUTPUT_VARIABLE OUTPUT)
    IF(BTK_TEST_ATOMIC_BUILTINS_COMPILED)
      MESSAGE(STATUS "Checking for builtin __sync_add_and_fetch -- success")
      SET(HAVE_ATOMIC_BUILTINS 1 CACHE INTERNAL "For __sync atomic builtins.")
      FILE(APPEND "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log"
        "Determining if the C++ compiler supports __sync_add_and_fetch builtin passed with the following output:\n ${OUTPUT}\n")
    ELSE(BTK_TEST_ATOMIC_BUILTINS_COMPILED)
      MESSAGE(STATUS "Checking for builtin __sync_add_and_fetch -- failed")
      SET(HAVE_ATOMIC_BUILTINS 0 CACHE INTERNAL "For __sync atomic builtins.")
      FILE(APPEND "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log"
        "Determining if the C++ compiler supports __sync_add_and_fetch builtin failed with the following output:\n ${OUTPUT}\n")
    ENDIF(BTK_TEST_ATOMIC_BUILTINS_COMPILED)
  ENDIF("HAVE_ATOMIC_BUILTINS" MATCHES "^HAVE_ATOMIC_BUILTINS$")
ENDIF(NOT WIN32)