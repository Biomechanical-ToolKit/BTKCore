@ECHO OFF
:: Global options used into EasyInstall_MSVC
:: Limited to 5 options
SET options.1=-DBTK_WRAP_MATLAB:BOOL=ON
SET options.2=-DBTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES:BOOL=ON
CALL .\EasyInstall_MSVC.bat build-Matlab-Release-Redistribuable %1 %2