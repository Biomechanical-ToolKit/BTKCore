 # SWIG 1.3 is not enough because some unit tests failed with this version
FIND_PACKAGE(SWIG 2.0 REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(NumPy REQUIRED)

IF (UNIX AND NOT APPLE AND HAVE_64_BIT AND NOT BUILD_SHARED_LIBS)
  # Force to build library (Common, IO, ...) as shared objects for Linux 64-bit.
  # See issue 9: http://code.google.com/p/b-tk/issues/detail?id=9
  MESSAGE(STATUS "\n-------------------------------------------------------------------------")
  MESSAGE(STATUS "WARNING: Due to the impossibility to mix static and dynamic library under\nLinux 64-bit, BTK is set to build shared libraries.")
  MESSAGE(STATUS "-------------------------------------------------------------------------")
  SET(BUILD_SHARED_LIBS TRUE CACHE BOOL "This option is fixed because you cannot mix static and dynamic library under Linux 64-bit." FORCE)
ENDIF (UNIX AND NOT APPLE AND HAVE_64_BIT AND NOT BUILD_SHARED_LIBS)

INCLUDE(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${NUMPY_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES("${BTK_SOURCE_DIR}/Utilities/SWIG")

#SWIG_EXPORT_ITERATOR_METHODS
SET(CMAKE_SWIG_FLAGS "")
SET(CMAKE_SWIG_OUTDIR "${BTK_EXECUTABLE_PATH}")

SET(BTKSWIG_CPP_SRCS 
    btkCommonSwig.cpp
    btkIOSwig.cpp
    btkBasicFiltersSwig.cpp)
    
SET_SOURCE_FILES_PROPERTIES(btkSwig.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(btkSwig.i PROPERTIES SWIG_FLAGS "-includeall")

SWIG_ADD_MODULE(btk python btkSwig.i ${BTKSWIG_CPP_SRCS})

# Python extension modules on Windows must have the extension ".pyd" instead of ".dll" since Python 2.5.
IF(WIN32 AND NOT CYGWIN)
  SET_TARGET_PROPERTIES(${SWIG_MODULE_btk_REAL_NAME} PROPERTIES SUFFIX ".pyd")
ENDIF(WIN32 AND NOT CYGWIN)

SET(BTK_SWIG_LINK_LIBRARIES
    BTKCommon
    BTKIO
    BTKBasicFilters)

SWIG_LINK_LIBRARIES(btk ${PYTHON_LIBRARIES} ${BTK_SWIG_LINK_LIBRARIES})