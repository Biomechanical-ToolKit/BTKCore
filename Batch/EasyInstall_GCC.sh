#!/bin/bash
# FUNCTION DECLARATION
function missing_cmake()
{
  echo ""
  echo "Impossible to find the file bin\cmake"
  echo ""
  echo "Do you have CMake installed?"
  wait_user
}

function error_config()
{
  echo ""
  echo "An error occurred during the configuration of the project."
  echo "Report this to the project maintainer."
  wait_user
}

function error_compile()
{
  echo ""
  echo "An error occurred during the compilation of the project."
  echo "Report this to the project maintainer."
  wait_user
}

function error_install()
{
  echo ""
  echo "An error occurred during the installation of the project."
  echo "Report this to the project maintainer."
  wait_user
}

function wait_user
{
  echo ""
  echo "Press a key to exit the program"
  read key
  exit 1
}
# If necessary, go back to the root of BTK
pwd | grep "/Batch" 1>/dev/null && cd ..
# Erase the folder where BTK is built
rm -Rf build-Matlab-Release-Redistribuable
mkdir build-Matlab-Release-Redistribuable
cd build-Matlab-Release-Redistribuable
# Detect CMake
CMAKE_PATHS[1]=""
CMAKE_PATHS[2]="/opt"
CMAKE_PATHS[3]="/usr"
CMAKE_PATHS[4]="/usr/local"
CMAKE_PATHS[5]="/Applications/CMake/Contents"
CMAKE_PATHS[6]="/Applications/CMake 2.6-2/Contents"
CMAKE_PATHS[7]="/Applications/CMake 2.6-3/Contents"
CMAKE_PATHS[8]="/Applications/CMake 2.6-4/Contents"
CMAKE_PATHS[9]="/Applications/CMake 2.8-1/Contents"
CMAKE_PATHS[10]="/Applications/CMake 2.8-2/Contents"
CMAKE=""
for f in 1 2 3 4 5 6 7 8 9 10
do
  if [ -e "${CMAKE_PATHS[$f]}/bin/cmake" ]
  then
    CMAKE="${CMAKE_PATHS[$f]}/bin/cmake"
    break
  fi
done
if [ ! -e $CMAKE ]
then
  missing_cmake
fi
# Configure
$CMAKE $1 -DCMAKE_BUILD_TYPE:CHAR=Release $2 -G "Unix Makefiles" ..
if [ $? -ne 0 ]
then
  error_config
fi
# Compilation
make
if [ $? -ne 0 ]
then
  error_compile
fi
# Installation
make install
if [ $? -ne 0 ]
then
  error_install
fi
exit 0
