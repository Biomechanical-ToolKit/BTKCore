# Path for the files used in the unit/regression tests
IF(BUILD_TESTING AND NOT BTK_TESTING_DATA_PATH)
  SET(BTK_TESTING_DATA_PATH "${BTK_TESTING_DATA_PATH}" CACHE PATH "Path for the data used by the unit tests." FORCE)
ENDIF(BUILD_TESTING AND NOT BTK_TESTING_DATA_PATH)

# Build the directories used to write files in some unit/regression tests
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${BTK_BINARY_DIR}/Testing/Data/Output/ANBSamples")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${BTK_BINARY_DIR}/Testing/Data/Output/ANCSamples")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${BTK_BINARY_DIR}/Testing/Data/Output/C3DSamples")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${BTK_BINARY_DIR}/Testing/Data/Output/CALForcePlateSamples")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${BTK_BINARY_DIR}/Testing/Data/Output/STLSamples")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${BTK_BINARY_DIR}/Testing/Data/Output/TRCSamples")

# C++
ADD_SUBDIRECTORY(C++)
# Python
IF(BTK_WRAP_PYTHON)
  ADD_SUBDIRECTORY(Python)
ENDIF(BTK_WRAP_PYTHON)
# Matlab
IF(BTK_WRAP_MATLAB)
  ADD_SUBDIRECTORY(Matlab)
ENDIF(BTK_WRAP_MATLAB)
# Octave
IF(BTK_WRAP_OCTAVE)
  ADD_SUBDIRECTORY(Octave)
ENDIF(BTK_WRAP_OCTAVE)