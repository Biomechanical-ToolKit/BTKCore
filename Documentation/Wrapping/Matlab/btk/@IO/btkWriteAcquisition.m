function h = btkWriteAcquisition(filename) %#ok
%BTKWRITEACQUISITION Write acquisition's data into file (C3D, TRC, ...)
% 
%  BTKWRITEACQUISITION(FILENAME, H) export acquisition's data represented 
%  as the handle H into the file FILENAME.  This handle H must be obtained 
%  by the use of a btk* function.  Supported file formats are for the moment
%  TRC.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKWRITEACQUISITION not found');

% [EOF] btkWriteAcquisition.m
