@ECHO OFF
:: Global options used into EasyInstall_MSVC
:: Limited to 5 options
SET options.1=-DBTK_WRAP_SCILAB:BOOL=ON
SET options.2=-DBTK_WRAP_SCILAB_REDISTRIBUABLE_MEX_FILES:BOOL=ON
CALL .\EasyInstall_MSVC.bat build-Scilab-Release-Redistribuable %1 %2