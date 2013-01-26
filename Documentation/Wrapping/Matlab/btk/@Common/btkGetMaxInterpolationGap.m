function gap = btkGetMaxInterpolationGap(h) %#ok
%BTKGETMAXINTERPOLATIONGAP Returns acquisition's maximum interpolation gap.
% 
%  GAP = BTKGETMAXINTERPOLATIONGAP(H) returns the maximum interpolation gap
%  for the acquisition. The acquistion is represented by the handle H, 
%  obtained by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETMAXINTERPOLATIONGAP not found');

% [EOF] btkGetMaxInterpolationGap.m
