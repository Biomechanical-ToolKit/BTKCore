# NOTE: The original idea came from Joakim Söderberg [1]. This version is a 
# refactoring of the code with several improvements:
# - Directory inclusion (i.e. No more need to list the files to coverage).
# - File/pattern exclusion
# - Merging of GCDA file results (very important when the files to cover are 
#   used in different libraries/binaries)
#
# [1]see https://github.com/JoakimSoderberg/coveralls-cmake)
#
# This is intended to be run by a custom target in a CMake project like this.
# 0. Compile program with coverage support.
# 1. Clear coverage data. (Recursively delete *.gcda in build dir)
# 2. Run the unit tests.
# 3. Run this script specifying the source folder and optionnaly some files
#   or patterns to exclude.
#
# This script will then use gcov to generate .gcov files in the directory specified
# via the COV_PATH var. This should probably be the same as your cmake build dir.
# It then parses the .gcov files to convert them into the Coveralls JSON format:
# https://coveralls.io/docs/api
#
# WARNING: This script is intentionnaly limited to be used with Travis-CI.
#
# Example for running this script as a custom target in your project:
# SET(COVERALLS_FILE "${PROJECT_BINARY_DIR}/coveralls.json")
# ADD_CUSTOM_TARGET(coveralls
#   # Generate the output
#   COMMAND ${CMAKE_COMMAND}
#     -DCOVERAGE_SRCS_PATH="${PROJECT_SOURCE_DIR}/Code"
#     -DCOVERAGE_SRCS_EXCLUDED=""
#     -DCOVERALLS_OUTPUT_FILE="${COVERALLS_FILE}"
#     -DCOV_PATH="${PROJECT_BINARY_DIR}"
#     -DPROJECT_ROOT="${PROJECT_SOURCE_DIR}"
#     -P "${CMAKE_MODULE_PATH}/CoverallsGenerateGcov.cmake"
#   # Upload the result
#   COMMAND curl -S -F json_file=@${COVERALLS_FILE} https://coveralls.io/api/v1/jobs && echo
#   COMMENT "Generating and uploading coveralls output...")

#
# Make sure we have the needed arguments.
#
IF(NOT COVERALLS_OUTPUT_FILE)
  message(FATAL_ERROR "Coveralls: No coveralls output file specified. Please set COVERALLS_OUTPUT_FILE")
ENDIF()

IF(NOT COV_PATH)
  message(FATAL_ERROR "Coveralls: Missing coverage directory path where gcov files will be generated. Please set COV_PATH")
ENDIF()

IF(NOT COVERAGE_SRCS_PATH)
  message(FATAL_ERROR "Coveralls: Missing the path of source files that we should get the coverage data for COVERAGE_SRCS_PATH")
ENDIF()

if (NOT PROJECT_ROOT)
  message(FATAL_ERROR "Coveralls: Missing PROJECT_ROOT.")
endif()

find_program(GCOV_EXECUTABLE gcov)

IF(NOT GCOV_EXECUTABLE)
    message(FATAL_ERROR "gcov not found! Aborting...")
ENDIF()

############################# Macros #########################################

#
# This macro converts from the full path format gcov outputs:
#
#    /path/to/project/root/build/#path#to#gcda###path#to#project#root#subdir#the_file.c.gcov
#
# to the original source file path the .gcov is for:
#
#   /path/to/project/root/subdir/the_file.c
#
MACRO(GET_SOURCE_PATH_FROM_GCOV_FILENAME _SRC_FILENAME _GCOV_FILENAME)

  # /path/to/project/root/build/#path#to#project#root#subdir#the_file.c.gcov 
  # -> 
  # #path#to#project#root#subdir#the_file.c.gcov   
  GET_FILENAME_COMPONENT(_GCOV_FILENAME_WEXT ${_GCOV_FILENAME} NAME)

  # #path#to#project#root#subdir#the_file.c.gcov -> /path/to/project/root/subdir/the_file.c
  STRING(REGEX REPLACE "\\.gcov$" "" SRC_FILENAME_TMP ${_GCOV_FILENAME_WEXT})
  STRING(REGEX REPLACE ".*\#\#\#" "/" SRC_FILENAME_TMP ${SRC_FILENAME_TMP})
  STRING(REGEX REPLACE "\#" "/" SRC_FILENAME_TMP ${SRC_FILENAME_TMP})
  SET(${_SRC_FILENAME} "${SRC_FILENAME_TMP}")
ENDMACRO()

##############################################################################

SET(JSON_SERVICE_NAME "travis-ci")
SET(JSON_SERVICE_JOB_ID $ENV{TRAVIS_JOB_ID})

# Reset the output file
FILE(WRITE "${COVERALLS_OUTPUT_FILE}"
"{
  \"service_name\": \"${JSON_SERVICE_NAME}\",
  \"service_job_id\": \"${JSON_SERVICE_JOB_ID}\",
  \"source_files\": ["
)

# Get the files to cover
FOREACH(_EXT h hh hpp hxx c cc cpp cxx tpp txx)
  FILE(GLOB_RECURSE _COVERAGE_SRCS RELATIVE "${COVERAGE_SRCS_PATH}" "${COVERAGE_SRCS_PATH}/*.${_EXT}")
  LIST(APPEND COVERAGE_SRCS ${_COVERAGE_SRCS})
ENDFOREACH()
LIST(SORT COVERAGE_SRCS)

# Transform the optional string arguement COVERAGE_SRCS_EXCLUDED into a list
string(REGEX REPLACE "\\::" ";" COVERAGE_SRCS_EXCLUDED "${COVERAGE_SRCS_EXCLUDED}")

# Get the coverage data.
FILE(GLOB_RECURSE GCDA_FILES RELATIVE "${COV_PATH}" "${COV_PATH}/*.gcda")
# Get a list of all the object directories needed by gcov
# (The directories the .gcda files and .o files are found in)
# and run gcov on those.
file(MAKE_DIRECTORY ${COV_PATH}/_temp_gcov)
FOREACH(GCDA ${GCDA_FILES})
  GET_FILENAME_COMPONENT(GCDA_DIR ${GCDA} PATH)
  EXECUTE_PROCESS(
    COMMAND ${GCOV_EXECUTABLE} -pl -o "${COV_PATH}/${GCDA_DIR}" "${COV_PATH}/${GCDA}"
    WORKING_DIRECTORY ${COV_PATH}/_temp_gcov
    OUTPUT_QUIET
  )
ENDFOREACH()

# Filter out the .gcov files not in the source path
FILE(GLOB _GCOV_FILES RELATIVE "${COV_PATH}/_temp_gcov" "${COV_PATH}/_temp_gcov/*.gcov")
SET(INC 0)
FOREACH(_GCOV_FILE ${_GCOV_FILES})
  GET_SOURCE_PATH_FROM_GCOV_FILENAME(_GCOV_SRC_FILE ${_GCOV_FILE})
  STRING(FIND "${_GCOV_SRC_FILE}" "${COVERAGE_SRCS_PATH}" FOUND)
  IF(NOT FOUND EQUAL -1)
    FILE(RELATIVE_PATH _GCOV_SRC_REL_VAR "${COVERAGE_SRCS_PATH}" "${_GCOV_SRC_FILE}")
    STRING(REGEX REPLACE "/" "_" _GCOV_SRC_REL_VAR ${_GCOV_SRC_REL_VAR})
    STRING(REGEX REPLACE "\\." "_" _GCOV_SRC_REL_VAR ${_GCOV_SRC_REL_VAR})
    LIST(APPEND "${_GCOV_SRC_REL_VAR}" ${INC})
  ENDIF()
  MATH(EXPR INC "${INC}+1")
ENDFOREACH()

LIST(LENGTH COVERAGE_SRCS COVERAGE_SRCS_LENGTH)
SET(INC 0)
FOREACH(_COVERAGE_SRC ${COVERAGE_SRCS})
  MATH(EXPR INC "${INC}+1")
  # Check if the file is not excluded from the coverage
  LIST(FIND COVERAGE_SRCS_EXCLUDED "${_COVERAGE_SRC}" FOUND)
  IF(NOT FOUND EQUAL -1)
    MESSAGE(STATUS "EXCLUDED: ${_COVERAGE_SRC}")
  ELSE()
    FILE(RELATIVE_PATH GCOV_SRC_REL_PATH "${PROJECT_ROOT}" "${COVERAGE_SRCS_PATH}/${_COVERAGE_SRC}")
    FILE(MD5 "${COVERAGE_SRCS_PATH}/${_COVERAGE_SRC}" GCOV_CONTENTS_MD5)
    # Check if the file is covered or not
    SET(_GCOV_SRC_REL_VAR "${_COVERAGE_SRC}")
    STRING(REGEX REPLACE "/" "_" _GCOV_SRC_REL_VAR ${_GCOV_SRC_REL_VAR})
    STRING(REGEX REPLACE "\\." "_" _GCOV_SRC_REL_VAR ${_GCOV_SRC_REL_VAR})
    IF(DEFINED ${_GCOV_SRC_REL_VAR})
      MESSAGE(STATUS "COVERED: ${_COVERAGE_SRC}")
      SET(GCOV_FILE_COVERAGE "[")
      # The file is covered but might be used by different GCDA files. We need to merge their results together
      LIST(GET _GCOV_FILES ${${_GCOV_SRC_REL_VAR}} _SRC_GCOV_FILES)
      # Reset the total number of calls by line
      SET(INC_LINE 1)
      FILE(STRINGS "${COVERAGE_SRCS_PATH}/${_COVERAGE_SRC}" SRC_LINES)
      FOREACH(_SRC_LINE ${SRC_LINES})
        UNSET(_GCOV_LINE_CALL_${INC_LINE})
        MATH(EXPR INC_LINE "${INC_LINE}+1")
      ENDFOREACH()
      FOREACH(_SRC_GCOV_FILE ${_SRC_GCOV_FILES})
        FILE(STRINGS "${COV_PATH}/_temp_gcov/${_SRC_GCOV_FILE}" GCOV_LINES)
        SET(GCOV_LINE_COUNT 1) # Line number for the .gcov.
        SET(DO_SKIP 0)
        FOREACH(GCOV_LINE ${GCOV_LINES})
          # Example of what we're parsing:
          # Hitcount  |Line | Source
          # "        8:   26:        IF(!allowed || (strlen(allowed) == 0))"
          STRING(REGEX REPLACE "^([^:]*):([^:]*):.*$" "\\1;\\2" RES "${GCOV_LINE}")
          # Check if we should exclude lines using the Lcov syntax.
          STRING(REGEX MATCH "LCOV_EXCL_START" START_SKIP "${GCOV_LINE}")
          STRING(REGEX MATCH "LCOV_EXCL_END" END_SKIP "${GCOV_LINE}")
          STRING(REGEX MATCH "LCOV_EXCL_LINE" LINE_SKIP "${GCOV_LINE}")

          SET(RESET_SKIP 0)
          IF(LINE_SKIP AND NOT DO_SKIP)
            SET(DO_SKIP 1)
            SET(RESET_SKIP 1)
          ENDIF()
          IF(START_SKIP)
            SET(DO_SKIP 1)
          ENDIF()
          IF(END_SKIP)
            SET(DO_SKIP 0)
          ENDIF()
          
          LIST(LENGTH RES RES_COUNT)
          IF(RES_COUNT EQUAL 2)
            LIST(GET RES 0 HITCOUNT)
            LIST(GET RES 1 LINE)
            STRING(STRIP ${HITCOUNT} HITCOUNT)
            STRING(STRIP ${LINE} LINE)
            # Lines with 0 line numbers are metadata and can be ignored.
            IF( (NOT ${LINE} EQUAL 0) AND (NOT DO_SKIP) AND (NOT ${HITCOUNT} STREQUAL "-") )
              # Translate the hitcount.
              IF (NOT DEFINED _GCOV_LINE_CALL_${LINE})
                SET(_GCOV_LINE_CALL_${LINE} 0)
              ENDIF()
              IF(${HITCOUNT} STREQUAL "#####")
                MATH(EXPR _GCOV_LINE_CALL_${LINE} "${_GCOV_LINE_CALL_${LINE}}+0")
              ELSE()
                MATH(EXPR _GCOV_LINE_CALL_${LINE} "${_GCOV_LINE_CALL_${LINE}}+${HITCOUNT}")
              ENDIF()
            ENDIF()
          ELSE()
            MESSAGE(WARNING "Failed to properly parse line (RES_COUNT = ${RES_COUNT}) ${GCOV_FILE}:${GCOV_LINE_COUNT}\n-->${GCOV_LINE}")
          ENDIF()

          IF(RESET_SKIP)
            SET(DO_SKIP 0)
          ENDIF()
          MATH(EXPR GCOV_LINE_COUNT "${GCOV_LINE_COUNT}+1")
        ENDFOREACH()
      ENDFOREACH()
      # Generate the content of the variable GCOV_FILE_COVERAGE
      SET(INC_LINE 1)
      FOREACH(_SRC_LINE ${SRC_LINES})
        IF (NOT DEFINED _GCOV_LINE_CALL_${INC_LINE})
          SET(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}null, ")
        ELSE()
          SET(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}${_GCOV_LINE_CALL_${INC_LINE}}, ")
        ENDIF()
        MATH(EXPR INC_LINE "${INC_LINE}+1")
      ENDFOREACH()
    ELSE()
      MESSAGE(STATUS "NOT COVERED: ${_COVERAGE_SRC}")
      # The file is not covered and all these lines are set to 0 (no call)
      FILE(STRINGS "${COVERAGE_SRCS_PATH}/${_COVERAGE_SRC}" SRC_LINES)
      SET(GCOV_FILE_COVERAGE "[")
      FOREACH(SOURCE ${SRC_LINES})
        SET(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}0, ")
      ENDFOREACH()
    ENDIF()
    STRING(REGEX REPLACE ",[ ]*$" "]" GCOV_FILE_COVERAGE ${GCOV_FILE_COVERAGE})
    FILE(APPEND "${COVERALLS_OUTPUT_FILE}"
      "{\n      \"name\": \"${GCOV_SRC_REL_PATH}\",\n"
         "      \"source_digest\": \"${GCOV_CONTENTS_MD5}\",\n"
         "      \"coverage\": ${GCOV_FILE_COVERAGE}\n"
      "  }")
    IF(INC LESS COVERAGE_SRCS_LENGTH)
      FILE(APPEND "${COVERALLS_OUTPUT_FILE}" ", ")
    ENDIF()
  ENDIF()
ENDFOREACH()

FILE(APPEND "${COVERALLS_OUTPUT_FILE}" "]\n}")
