function h = btkReadC3D(filename) %#ok
%BTKREADC3D Read a C3D file
% 
%  H = BTKREADC3D(FILENAME) returns the handle H of a biomechanical 
%  acquisition stored in the C3D file FILENAME.  This handle is returned 
%  as a 32-bit and can be only used with the btk* function.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREADC3D not found');

% [EOF] btkReadC3D.m
