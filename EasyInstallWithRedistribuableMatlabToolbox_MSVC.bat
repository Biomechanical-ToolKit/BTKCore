@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

set MSVS2003="C:\Program Files\Microsoft Visual Studio .NET 2003"
set MSVS2005="C:\Program Files\Microsoft Visual Studio 8"
set MSVS2008="C:\Program Files\Microsoft Visual Studio 9.0"
set MSVS2010="C:\Program Files\Microsoft Visual Studio 10.0"

set CMAKE26="C:\Program Files\CMake 2.6"
set CMAKE28="C:\Program Files\CMake 2.8"

set MSVS=
if (%1) == () (
  set MSVS=%MSVS2010%
  if NOT exist !MSVS!\VC\vcvarsall.bat (
    set MSVS=%MSVS2008%
    if NOT exist !MSVS!\VC\vcvarsall.bat (
      set MSVS=%MSVS2005%
      if NOT exist !MSVS!\VC\vcvarsall.bat (
        set MSVS=%MSVS2003%
        if NOT exist !MSVS!\VC\vcvarsall.bat goto missing_MSVS
      )
    )
  )
) else ( 
  set MSVS=%1
  if NOT exist !MSVS!\VC\vcvarsall.bat goto missing_MSVS
)

set CMAKE=
if (%2) == () (
  set CMAKE=%CMAKE28%
  if NOT exist !CMAKE!\bin\cmake.exe (
    set CMAKE=%CMAKE26%
    if NOT exist !CMAKE!\bin\cmake.exe goto missing_CMAKE
  )
) else (
  set CMAKE=%2
  if NOT exist !CMAKE!\bin\cmake.exe goto missing_CMAKE
)

cd .
call !MSVS!\VC\vcvarsall.bat
mkdir build
cd build
!CMAKE!\bin\cmake.exe -DBTK_WRAP_MATLAB:BOOL=1 -DBTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES:BOOL=1 -G "NMake Makefiles" ..
if errorlevel 1 goto error_CMAKE
nmake
if errorlevel 1 goto error_MSVS_COMPILE
nmake install
if errorlevel 1 goto error_MSVS_INSTALL
cd ..
goto :eof

:missing_MSVS
echo:
echo Impossible to find the file VC\vcvarsall.bat
echo:
echo Do you have Microsoft Visual Studio installed?
goto usage

:missing_CMAKE
echo:
echo Impossible to find the file bin\cmake.exe
echo:
echo Do you have CMake installed?
goto usage

:usage
echo:
echo Error in script usage. The correct usage is:
echo     %0 [MSVS installation path] [CMAKE installation path]
echo:
echo For example:
echo     %0 "C:\Program Files\Microsoft Visual Studio 9.0" "C:\Program Files\CMake 2.6"
goto wait

:error_CMAKE
echo:
echo An error occurred during the configuration of the project
echo Report this to the project maintainer
goto wait

:error_MSVS_COMPILE
echo:
echo An error occurred during the compilation of the project
echo Report this to the project maintainer
goto wait

:error_MSVS_INSTALL
echo:
echo An error occurred during the installation of the project
echo Report this to the project maintainer
goto wait

:wait
echo:
echo Press a key to exit the program
set /p Input=
exit /b 1
goto :eof