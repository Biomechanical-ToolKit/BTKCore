function btkSetAnglesValues(h, v) %#ok
%BTKSETANGLESVALUES Sets angles' components from a matrix
% 
%  BTKSETANGLESVALUES(H, V) uses the matrix V to sets angles' components.
%  The number of rows of the matrix corresponds to the number of frames in the acquisition. 
%  The number of  columns of this matrix corresponds to the number of extracted 
%  angles multiplied by 3 (their components X,Y,Z).
%  The biomechanical acquisition handle H is represented as a double and can be 
%  only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANGLESVALUES not found');

% [EOF] btkSetAnglesValues.m
