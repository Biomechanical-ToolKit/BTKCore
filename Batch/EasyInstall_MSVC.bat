@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

SET REQUIRE_PRIVILEGES=0;
:: Vista
VER | FIND /I "6.0" >NUL
IF NOT ERRORLEVEL 1 SET REQUIRE_PRIVILEGES=1
:: Windows 7
VER | FIND /I "6.1" >NUL
IF NOT ERRORLEVEL 1 SET REQUIRE_PRIVILEGES=1

SET MSVS=""
IF (%2) == () (
  :: Create a temporary file to list the known MSVC directories
  > %TEMP%.\BTK-MSVC.txt ECHO Known MSVC directories
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles%\Microsoft Visual Studio .NET 2003"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles%\Microsoft Visual Studio 8"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles%\Microsoft Visual Studio 9.0"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles%\Microsoft Visual Studio 10.0"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles% (x86)\Microsoft Visual Studio .NET 2003"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles% (x86)\Microsoft Visual Studio 8"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles% (x86)\Microsoft Visual Studio 9.0"
  >> %TEMP%.\BTK-MSVC.txt ECHO "%ProgramFiles% (x86)\Microsoft Visual Studio 10.0"
  :: Look for MSVC
  FOR	/F "eol=;delims=" %%i IN (%TEMP%.\BTK-MSVC.txt) DO IF EXIST %%i\VC\vcvarsall.bat SET MSVS=%%i
  DEL %TEMP%.\BTK-MSVC.txt
  IF !MSVS! == "" GOTO :missing_MSVS
) ELSE (
  SET MSVS=%2
  IF NOT EXIST !MSVS!\VC\vcvarsall.bat goto missing_MSVS
)

SET CMAKE=""
IF (%3) == () (
  :: Create a temporary file to list the known CMAKE directories
  > %TEMP%.\BTK-CMAKE.txt ECHO Known CMAKE directories
  >> %TEMP%.\BTK-CMAKE.txt ECHO "%ProgramFiles%\CMake 2.6"
  >> %TEMP%.\BTK-CMAKE.txt ECHO "%ProgramFiles%\CMake 2.8"
  >> %TEMP%.\BTK-CMAKE.txt ECHO "%ProgramFiles% (x86)\CMake 2.6"
  >> %TEMP%.\BTK-CMAKE.txt ECHO "%ProgramFiles% (x86)\CMake 2.8"
  :: Look for CMAKE
  FOR	/F "eol=;delims=," %%i IN (%TEMP%.\BTK-CMAKE.txt) DO IF EXIST %%i\bin\cmake.exe SET CMAKE=%%i
  DEL %TEMP%.\BTK-CMAKE.txt
  IF !CMAKE! == "" GOTO :missing_CMAKE
) ELSE (
  SET CMAKE=%3
  IF NOT EXIST !CMAKE!\bin\cmake.exe GOTO missing_CMAKE
)

SET setEnvCmd="%ProgramFiles%\Microsoft SDKs\Windows\v7.0\Bin\SetEnv.Cmd"

cd .
IF EXIST %setEnvCmd% (
  call %setEnvCmd%
) ELSE (
  :: 32 or 64 bits?
  set ARCH=
  IF EXIST %SYSTEMROOT%\SysWOW64 SET ARCH=x64
  call !MSVS!\VC\vcvarsall.bat !ARCH!
)
mkdir build-Matlab-Release-Redistribuable
cd build-Matlab-Release-Redistribuable
!CMAKE!\bin\cmake.exe %1 -DCMAKE_BUILD_TYPE:CHAR=Release -G "NMake Makefiles" ..
if errorlevel 1 goto error_CMAKE
::nmake
if errorlevel 1 goto error_MSVS_COMPILE
if !REQUIRE_PRIVILEGES! == 1 (
  ..\Batch\elevate nmake install
) else (
  nmake install
)
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
echo     %0 [CMAKE options] [MSVS installation path] [CMAKE installation path]
echo:
echo For example:
echo     %0 "-DBTK_WRAP_MATLAB:BOOL=1 -DBTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES:BOOL=1" "C:\Program Files\Microsoft Visual Studio 9.0" "C:\Program Files\CMake 2.6"
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