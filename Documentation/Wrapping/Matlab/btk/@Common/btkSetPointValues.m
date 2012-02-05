function btkSetPointValues(h, idx_or_label, v) %#ok
%BTKSETPOINTVALUES Sets point's coordinates (values) from a matrix
% 
%  BTKSETPOINTVALUES(H, INDEX, V) uses the matrix V to set point's coordinates.
%  The number of rows of the matrix corresponds to the number of frames 
%  in the acquisition.  The number of columns of this matrix corresponds 
%  to the coordinates of the point.
%  The biomechanical acquisition handle H is represented as a double and
%  can be only used with the btk* functions.
%
%  BTKSETPOINTVALUES(H, LABEL, V) uses the point's label to extract the point 
%  to modify.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTVALUES not found');

% [EOF] btkSetPointValues.m
