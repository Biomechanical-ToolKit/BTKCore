@ECHO OFF
call .\Batch\EasyInstall_MSVC.bat "-DBTK_WRAP_MATLAB:BOOL=1 -DBTK_WRAP_MATLAB_REDISTRIBUABLE_MEX_FILES:BOOL=1" build-Matlab-Release-Redistribuable %1 %2