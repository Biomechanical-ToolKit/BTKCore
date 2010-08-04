function btkSetPointResiduals(h, r) %#ok
%BTKSETPOINTRESIDUALS Sets point's residuals from a row vector
% 
%  BTKSETPOINTVALUES(H, R) uses the matrix R to set point's residuals.
%  The number of rows of the matrix corresponds to the number of frames 
%  in the acquisition.  The number of columns must be equal to 1.
%  The biomechanical acquisition handle H is represented as a 32-bit integer
%  and can be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTVALUES not found');

% [EOF] btkSetPointResiduals.m
