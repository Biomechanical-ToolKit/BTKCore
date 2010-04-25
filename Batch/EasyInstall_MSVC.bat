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
IF (%3) == () (
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
  SET MSVS=%3
  IF NOT EXIST !MSVS!\VC\vcvarsall.bat GOTO missing_MSVS
)

SET CMAKE=""
IF (%4) == () (
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
  SET CMAKE=%4
  IF NOT EXIST !CMAKE!\bin\cmake.exe GOTO missing_CMAKE
)

SET setEnvCmd="%ProgramFiles%\Microsoft SDKs\Windows\v7.0\Bin\SetEnv.Cmd"

CD .
IF EXIST %setEnvCmd% (
  CALL %setEnvCmd%
) ELSE (
  :: 32 or 64 bits?
  set ARCH=
  IF EXIST %SYSTEMROOT%\SysWOW64 SET ARCH=x64
  call !MSVS!\VC\vcvarsall.bat !ARCH!
)
IF EXIST %2 RMDIR /S /Q %2
MKDIR %2
CD %2
!CMAKE!\bin\cmake.exe %1 -DCMAKE_BUILD_TYPE:CHAR=Release -G "NMake Makefiles" ..
IF ERRORLEVEL 1 GOTO error_CMAKE
nmake
IF ERRORLEVEL 1 GOTO error_MSVS_COMPILE
if !REQUIRE_PRIVILEGES! == 1 (
  ..\Batch\elevate nmake install
) ELSE (
  nmake install
)
IF ERRORLEVEL 1 GOTO error_MSVS_INSTALL
CD ..
GOTO :eof

:missing_MSVS
ECHO:
ECHO Impossible to find the file VC\vcvarsall.bat
ECHO:
ECHO Do you have Microsoft Visual Studio installed?
GOTO usage

:missing_CMAKE
ECHO:
ECHO Impossible to find the file bin\cmake.exe
ECHO:
ECHO Do you have CMake installed?
GOTO usage

:usage
ECHO:
ECHO Error in script usage. The correct usage is:
ECHO     %0 [CMAKE options] [MSVS installation path] [CMAKE installation path]
ECHO:
ECHO For example:
ECHO     %0 "-DBTK_WRAP_MATLAB:BOOL=1 -DBTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES:BOOL=1" "C:\Program Files\Microsoft Visual Studio 9.0" "C:\Program Files\CMake 2.6"
GOTO wait

:error_CMAKE
ECHO:
ECHO An error occurred during the configuration of the project
ECHO Report this to the project maintainer
GOTO wait

:error_MSVS_COMPILE
ECHO:
ECHO An error occurred during the compilation of the project
ECHO Report this to the project maintainer
GOTO wait

:error_MSVS_INSTALL
ECHO:
ECHO An error occurred during the installation of the project
ECHO Report this to the project maintainer
GOTO wait

:wait
ECHO:
ECHO Press a key to exit the program
SET /p Input=
EXIT /b 1
GOTO :eof