function btkSetPointResiduals(h, idx_or_abel, r) %#ok
%BTKSETPOINTRESIDUALS Sets point's residuals from a row vector
% 
%  BTKSETPOINTRESIDUALS(H, INDEX, R) uses the matrix R to set point's residuals.
%  The number of rows of the matrix corresponds to the number of frames 
%  in the acquisition.  The number of columns must be equal to 1.
%  The biomechanical acquisition handle H is represented as a double and can
%  be only used with the btk* functions.
%
%  BTKSETPOINTRESIDUALS(H, LABEL, R) uses the point's label to extract the point 
%  to modify.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTVALUES not found');

% [EOF] btkSetPointResiduals.m
