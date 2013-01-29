function btkSetPointsValues(h. v) %#ok
%BTKSETPOINTSVALUES Sets points' components from a matrix
% 
%  BTKSETPOINTSVALUES(H, V) uses the matrix V to sets points' components.
%  The number of rows of the matrix corresponds to the number of frames in the acquisition. 
%  The number of  columns of this matrix corresponds to the number of points 
%  multiplied by 3 (their components X,Y,Z).
%  The biomechanical acquisition handle H is represented as a double and can
%  be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTSVALUES not found');

% [EOF] btkSetPointsValues.m
