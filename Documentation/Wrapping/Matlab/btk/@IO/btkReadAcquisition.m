function h = btkReadAcquisition(filename) %#ok
%BTKREADACQUISITION Read an acquisition's file (C3D, TRC, ...)
% 
%  H = BTKREADACQUISITION(FILENAME) returns the handle H of a biomechanical 
%  acquisition stored in file FILENAME.  This handle is returned 
%  as a 32-bit and can be only used with the btk* function.  Supported 
%  file formats are C3D AND TRC.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREADACQUISITION not found');

% [EOF] btkReadAcquisition.m
