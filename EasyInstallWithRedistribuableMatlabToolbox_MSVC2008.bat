@echo off
if (%1) == () (
  set MSVS="C:\Program Files\Microsoft Visual Studio 9.0"
) else ( 
  set MSVS=%1
)
if (%2) == () (
  set CMAKE="C:\Program Files\CMake 2.6"
) else (
  set CMAKE=%2
)

if NOT exist %MSVS%\VC\vcvarsall.bat goto missing_MSVS
if NOT exist %CMAKE%\bin\cmake.exe goto missing_CMAKE

cd .
call %MSVS%\VC\vcvarsall.bat
mkdir build
cd build
%CMAKE%\bin\cmake.exe -DBTK_WRAP_MATLAB:BOOL=1 -DBTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES:BOOL=1 -G "NMake Makefiles" ..
nmake
nmake install
cd ..
goto :eof

:missing_MSVS
echo:
echo Impossible to find the file VC\vcvarsall.bat in the folder %MSVS%
echo:
echo Is it the correct path? 
echo Do you have Microsoft Visual Studio installed?
goto usage

:missing_CMAKE
echo:
echo Impossible to find the file bin\cmake.exe in the folder %CMAKE%
echo:
echo Is it the correct path? 
echo Do you have CMake 2.6 or above installed?
goto usage

:usage
echo:
echo Error in script usage. The correct usage is:
echo     %0 [MSVS installation path] [CMAKE installation path]
echo:
echo By default, this script uses the paths "C:\Program Files\Microsoft Visual Studio 9.0" and "C:\Program Files\CMake 2.6" to find the files vcvarsall.bat and cmake.exe
echo:
echo For example:
echo     %0 "C:\Program Files\Microsoft Visual Studio 9.0" "C:\Program Files\CMake 2.6"
goto :eof