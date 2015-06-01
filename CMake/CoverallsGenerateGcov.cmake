#
# The MIT License (MIT)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Copyright (C) 2014 Joakim Söderberg <joakim.soderberg@gmail.com>
#
# This is intended to be run by a custom target in a CMake project like this.
# 0. Compile program with coverage support.
# 1. Clear coverage data. (Recursively delete *.gcda in build dir)
# 2. Run the unit tests.
# 3. Run this script specifying which source files the coverage should be performed on.
#
# This script will then use gcov to generate .gcov files in the directory specified
# via the COV_PATH var. This should probably be the same as your cmake build dir.
#
# It then parses the .gcov files to convert them into the Coveralls JSON format:
# https://coveralls.io/docs/api
#
# Example for running as standalone CMake script from the command line:
# (Note it is important the -P is at the end...)
# $ cmake -DCOV_PATH=$(pwd) 
#         -DCOVERAGE_SRCS="catcierge_rfid.c;catcierge_timer.c" 
#         -P ../cmake/CoverallsGcovUpload.cmake
#


# ================================ MODIFICATION ============================= #
#
#  - 2015/05/29 - Arnaud Barré
#    * The CMake variable 'COVERAGE_SRCS' was replaced by 'COVERAGE_SRCS_PATH'
#      It is no more necessary to give all the files to cover. Instead a path
#      is given and all the files with one of the following extensions '.h',
#      '.hh', '.hpp', '.hxx', '.c', '.cc', '.cpp', '.cxx', '.tpp', '.txx' will
#      be retrieved and covered.
#    * All the logs 'MESSAGE(...)' were commented to not spam Travis-CI
#    * The finding of Git was muted
#    * gcov was muted to not spam Travis-CI
#
#  - 2015/05/30 - Arnaud Barré
#    * The Git part (detection, extraction, display) was removed as Coveralls
#      retrieved these information directly from Travis-CI. It is then no more
#      necessary to finish this part (see TODOs of the original author)
#
#  - 2015/06/01 - Arnaud Barré
#    * Code cleanup to remove the parts related to the parsing of source file
#      All the code using the variable GCOV_FILE_SOURCE is not necessary.
#
# =========================================================================== #

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#
# Make sure we have the needed arguments.
#
if (NOT COVERALLS_OUTPUT_FILE)
	message(FATAL_ERROR "Coveralls: No coveralls output file specified. Please set COVERALLS_OUTPUT_FILE")
endif()

if (NOT COV_PATH)
	message(FATAL_ERROR "Coveralls: Missing coverage directory path where gcov files will be generated. Please set COV_PATH")
endif()

if (NOT COVERAGE_SRCS_PATH)
	message(FATAL_ERROR "Coveralls: Missing the path of source files that we should get the coverage data for COVERAGE_SRCS_PATH")
endif()

if (NOT PROJECT_ROOT)
	message(FATAL_ERROR "Coveralls: Missing PROJECT_ROOT.")
endif()

SET(COVERAGE_SRCS, "")
FOREACH(_EXT h hh hpp hxx c cc cpp cxx tpp txx)
  FILE(GLOB_RECURSE _COVERAGE_SRCS "${COVERAGE_SRCS_PATH}/*.${_EXT}")
  LIST(APPEND COVERAGE_SRCS ${_COVERAGE_SRCS})
ENDFOREACH()

find_program(GCOV_EXECUTABLE gcov)

if (NOT GCOV_EXECUTABLE)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

############################# Macros #########################################

#
# This macro converts from the full path format gcov outputs:
#
#    /path/to/project/root/build/#path#to#project#root#subdir#the_file.c.gcov
#
# to the original source file path the .gcov is for:
#
#   /path/to/project/root/subdir/the_file.c
#
macro(get_source_path_from_gcov_filename _SRC_FILENAME _GCOV_FILENAME)

	# /path/to/project/root/build/#path#to#project#root#subdir#the_file.c.gcov 
	# -> 
	# #path#to#project#root#subdir#the_file.c.gcov   
	get_filename_component(_GCOV_FILENAME_WEXT ${_GCOV_FILENAME} NAME)

	# #path#to#project#root#subdir#the_file.c.gcov -> /path/to/project/root/subdir/the_file.c
	string(REGEX REPLACE "\\.gcov$" "" SRC_FILENAME_TMP ${_GCOV_FILENAME_WEXT})
	string(REGEX REPLACE "\#" "/" SRC_FILENAME_TMP ${SRC_FILENAME_TMP})
	set(${_SRC_FILENAME} "${SRC_FILENAME_TMP}")
endmacro()

##############################################################################

# Get the coverage data.
file(GLOB_RECURSE GCDA_FILES "${COV_PATH}/*.gcda")
# message("GCDA files:")

# Get a list of all the object directories needed by gcov
# (The directories the .gcda files and .o files are found in)
# and run gcov on those.
foreach(GCDA ${GCDA_FILES})
  # message("Process: ${GCDA}")
  # message("------------------------------------------------------------------------------")
	get_filename_component(GCDA_DIR ${GCDA} PATH)

	#
	# The -p below refers to "Preserve path components",
	# This means that the generated gcov filename of a source file will
	# keep the original files entire filepath, but / is replaced with #.
	# Example:
	#
	# /path/to/project/root/build/CMakeFiles/the_file.dir/subdir/the_file.c.gcda
	# ------------------------------------------------------------------------------
	# File '/path/to/project/root/subdir/the_file.c'
	# Lines executed:68.34% of 199
	# /path/to/project/root/subdir/the_file.c:creating '#path#to#project#root#subdir#the_file.c.gcov'
	#
	# If -p is not specified then the file is named only "the_file.c.gcov"
	#
	execute_process(
		COMMAND ${GCOV_EXECUTABLE} -p -o ${GCDA_DIR} ${GCDA}
		WORKING_DIRECTORY ${COV_PATH}
    OUTPUT_QUIET
	)
endforeach()

# TODO: Make these be absolute path
file(GLOB ALL_GCOV_FILES ${COV_PATH}/*.gcov)

# Get only the filenames to use for filtering.
#set(COVERAGE_SRCS_NAMES "")
#foreach (COVSRC ${COVERAGE_SRCS})
#	get_filename_component(COVSRC_NAME ${COVSRC} NAME)
#	message("${COVSRC} -> ${COVSRC_NAME}")
#	list(APPEND COVERAGE_SRCS_NAMES "${COVSRC_NAME}")
#endforeach()

#
# Filter out all but the gcov files we want.
#
# We do this by comparing the list of COVERAGE_SRCS filepaths that the
# user wants the coverage data for with the paths of the generated .gcov files,
# so that we only keep the relevant gcov files.
#
# Example:
# COVERAGE_SRCS =
#				/path/to/project/root/subdir/the_file.c
#
# ALL_GCOV_FILES =
#				/path/to/project/root/build/#path#to#project#root#subdir#the_file.c.gcov
#				/path/to/project/root/build/#path#to#project#root#subdir#other_file.c.gcov
# 
# Result should be:
# GCOV_FILES = 
#				/path/to/project/root/build/#path#to#project#root#subdir#the_file.c.gcov
#
set(GCOV_FILES "")
#message("Look in coverage sources: ${COVERAGE_SRCS}")
# message("\nFilter out unwanted GCOV files:")
# message("===============================")

set(COVERAGE_SRCS_REMAINING ${COVERAGE_SRCS})

foreach (GCOV_FILE ${ALL_GCOV_FILES})

	#
	# /path/to/project/root/build/#path#to#project#root#subdir#the_file.c.gcov 
	# -> 
	# /path/to/project/root/subdir/the_file.c 
	get_source_path_from_gcov_filename(GCOV_SRC_PATH ${GCOV_FILE})

	# Is this in the list of source files?
	# TODO: We want to match against relative path filenames from the source file root...
	list(FIND COVERAGE_SRCS ${GCOV_SRC_PATH} WAS_FOUND)

	if (NOT WAS_FOUND EQUAL -1)
    # message("YES: ${GCOV_FILE}")
		list(APPEND GCOV_FILES ${GCOV_FILE})

		# We remove it from the list, so we don't bother searching for it again.
		# Also files left in COVERAGE_SRCS_REMAINING after this loop ends should
		# have coverage data generated from them (no lines are covered).
		list(REMOVE_ITEM COVERAGE_SRCS_REMAINING ${GCOV_SRC_PATH})
	else()
    # message("NO:  ${GCOV_FILE}")
	endif()
endforeach()

# TODO: Enable setting these
set(JSON_SERVICE_NAME "travis-ci")
set(JSON_SERVICE_JOB_ID $ENV{TRAVIS_JOB_ID})

set(JSON_TEMPLATE
"{
  \"service_name\": \"\@JSON_SERVICE_NAME\@\",
  \"service_job_id\": \"\@JSON_SERVICE_JOB_ID\@\",
  \"source_files\": \@JSON_GCOV_FILES\@
}"
)

set(SRC_FILE_TEMPLATE
"{
      \"name\": \"\@GCOV_SRC_REL_PATH\@\",
      \"source_digest\": \"\@GCOV_CONTENTS_MD5\@\",
      \"coverage\": \@GCOV_FILE_COVERAGE\@
  }"
)

# message("\nGenerate JSON for files:")
# message("=========================")

set(JSON_GCOV_FILES "[")

# Read the GCOV files line by line and get the coverage data.
foreach (GCOV_FILE ${GCOV_FILES})

	get_source_path_from_gcov_filename(GCOV_SRC_PATH ${GCOV_FILE})
	file(RELATIVE_PATH GCOV_SRC_REL_PATH "${PROJECT_ROOT}" "${GCOV_SRC_PATH}")

	# The new coveralls API doesn't need the entire source (Yay!)
	# However, still keeping that part for now. Will cleanup in the future.
	file(MD5 "${GCOV_SRC_PATH}" GCOV_CONTENTS_MD5)
  # message("MD5: ${GCOV_SRC_PATH} = ${GCOV_CONTENTS_MD5}")

	# Loads the gcov file as a list of lines.
	# (We first open the file and replace all occurences of [] with _
	#  because CMake will fail to parse a line containing unmatched brackets...
	#  also the \ to escaped \n in macros screws up things.)
	# https://public.kitware.com/Bug/view.php?id=15369
	file(READ ${GCOV_FILE} GCOV_CONTENTS)
	string(REPLACE "[" "_" GCOV_CONTENTS "${GCOV_CONTENTS}")
	string(REPLACE "]" "_" GCOV_CONTENTS "${GCOV_CONTENTS}")
	string(REPLACE "\\" "_" GCOV_CONTENTS "${GCOV_CONTENTS}")
	file(WRITE ${GCOV_FILE}_tmp "${GCOV_CONTENTS}")

	file(STRINGS ${GCOV_FILE}_tmp GCOV_LINES)
	list(LENGTH GCOV_LINES LINE_COUNT)

	# We want a json array of coverage data as a single string
	# start building them from the contents of the .gcov
	set(GCOV_FILE_COVERAGE "[")

	set(GCOV_LINE_COUNT 1) # Line number for the .gcov.
	set(DO_SKIP 0)
	foreach (GCOV_LINE ${GCOV_LINES})
		#message("${GCOV_LINE}")
		# Example of what we're parsing:
		# Hitcount  |Line | Source
		# "        8:   26:        if (!allowed || (strlen(allowed) == 0))"
		string(REGEX REPLACE 
			"^([^:]*):([^:]*):(.*)$" 
			"\\1;\\2;\\3"
			RES
			"${GCOV_LINE}")

		# Check if we should exclude lines using the Lcov syntax.
		string(REGEX MATCH "LCOV_EXCL_START" START_SKIP "${GCOV_LINE}")
		string(REGEX MATCH "LCOV_EXCL_END" END_SKIP "${GCOV_LINE}")
		string(REGEX MATCH "LCOV_EXCL_LINE" LINE_SKIP "${GCOV_LINE}")

		set(RESET_SKIP 0)
		if (LINE_SKIP AND NOT DO_SKIP)
			set(DO_SKIP 1)
			set(RESET_SKIP 1)
		endif()

		if (START_SKIP)
			set(DO_SKIP 1)
      # message("${GCOV_LINE_COUNT}: Start skip")
		endif()

		if (END_SKIP)
			set(DO_SKIP 0)
		endif()

		list(LENGTH RES RES_COUNT)

		if (RES_COUNT GREATER 2)
			list(GET RES 0 HITCOUNT)
			list(GET RES 1 LINE)
			list(GET RES 2 SOURCE)

			string(STRIP ${HITCOUNT} HITCOUNT)
			string(STRIP ${LINE} LINE)

			# Lines with 0 line numbers are metadata and can be ignored.
			if (NOT ${LINE} EQUAL 0)
				
				if (DO_SKIP)
					set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}null, ")
				else()
					# Translate the hitcount into valid JSON values.
					if (${HITCOUNT} STREQUAL "#####")
						set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}0, ")
					elseif (${HITCOUNT} STREQUAL "-")
						set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}null, ")
					else()
						set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}${HITCOUNT}, ")
					endif()
				endif()
			endif()
		else()
			message(WARNING "Failed to properly parse line (RES_COUNT = ${RES_COUNT}) ${GCOV_FILE}:${GCOV_LINE_COUNT}\n-->${GCOV_LINE}")
		endif()

		if (RESET_SKIP)
			set(DO_SKIP 0)
		endif()
		math(EXPR GCOV_LINE_COUNT "${GCOV_LINE_COUNT}+1")
	endforeach()

  # message("${GCOV_LINE_COUNT} of ${LINE_COUNT} lines read!")

	# Advanced way of removing the trailing comma in the JSON array.
	# "[1, 2, 3, " -> "[1, 2, 3"
	string(REGEX REPLACE ",[ ]*$" "" GCOV_FILE_COVERAGE ${GCOV_FILE_COVERAGE})

	# Append the trailing ] to complete the JSON array.
	set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}]")

	# Generate the final JSON for this file.
  # message("Generate JSON for file: ${GCOV_SRC_REL_PATH}...")
	string(CONFIGURE ${SRC_FILE_TEMPLATE} FILE_JSON)

	set(JSON_GCOV_FILES "${JSON_GCOV_FILES}${FILE_JSON}, ")
  
  message(STATUS " - COVERED: ${GCOV_SRC_REL_PATH}")
endforeach()

# Loop through all files we couldn't find any coverage for
# as well, and generate JSON for those as well with 0% coverage.
foreach(NOT_COVERED_SRC ${COVERAGE_SRCS_REMAINING})

	file(RELATIVE_PATH GCOV_SRC_REL_PATH "${PROJECT_ROOT}" "${NOT_COVERED_SRC}")
	file(MD5 "${NOT_COVERED_SRC}" GCOV_CONTENTS_MD5)

	# Loads the source file as a list of lines.
	file(STRINGS ${NOT_COVERED_SRC} SRC_LINES)

	set(GCOV_FILE_COVERAGE "[")

	foreach (SOURCE ${SRC_LINES})
		set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}0, ")
	endforeach()

	# Remove trailing comma, and complete JSON array with ]
	string(REGEX REPLACE ",[ ]*$" "" GCOV_FILE_COVERAGE ${GCOV_FILE_COVERAGE})
	set(GCOV_FILE_COVERAGE "${GCOV_FILE_COVERAGE}]")

	# Generate the final JSON for this file.
	# message("Generate JSON for non-gcov file: ${NOT_COVERED_SRC}...")
	string(CONFIGURE ${SRC_FILE_TEMPLATE} FILE_JSON)
	set(JSON_GCOV_FILES "${JSON_GCOV_FILES}${FILE_JSON}, ")
  
  message(STATUS " - NOT COVERED: ${GCOV_SRC_REL_PATH}")
endforeach()

# Get rid of trailing comma.
string(REGEX REPLACE ",[ ]*$" "" JSON_GCOV_FILES ${JSON_GCOV_FILES})
set(JSON_GCOV_FILES "${JSON_GCOV_FILES}]")

# Generate the final complete JSON!
# message("Generate final JSON...")
string(CONFIGURE ${JSON_TEMPLATE} JSON)

file(WRITE "${COVERALLS_OUTPUT_FILE}" "${JSON}")
# message("###########################################################################")
# message("Generated coveralls JSON containing coverage data:")
# message("${COVERALLS_OUTPUT_FILE}")
# message("###########################################################################")

