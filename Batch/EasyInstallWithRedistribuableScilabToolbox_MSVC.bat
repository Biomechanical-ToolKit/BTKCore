@ECHO OFF
call .\EasyInstall_MSVC.bat "-DBTK_WRAP_SCILAB:BOOL=1 -DBTK_WRAP_SCILAB_REDISTRIBUABLE_MEX_FILES:BOOL=1" build-Scilab-Release-Redistribuable %1 %2