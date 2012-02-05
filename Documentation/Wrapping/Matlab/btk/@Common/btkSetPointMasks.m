function btkSetPointMasks(h, idx_or_label, m) %#ok
%BTKSETPOINTMASKS Sets point's masks from a row vector
% 
%  BTKSETPOINTMASKS(H, INDEX, M) uses the matrix M to set point's masks.
%  The number of rows of the matrix corresponds to the number of frames 
%  in the acquisition.  The number of columns must be equal to 1. Each value
%  must be between 0 and 127 and corresponds to the conversion of a binary
%  number (coded on 7 bits) to a integer number.
%  The biomechanical acquisition handle H is represented as a double and
%  can be only used with the btk* functions.
%
%  BTKSETPOINTMASKS(H, LABEL, M) uses the point's label to extract the point 
%  to modify.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTVALUES not found');

% [EOF] btkSetPointMasks.m
