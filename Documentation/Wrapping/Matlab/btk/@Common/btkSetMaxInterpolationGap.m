function btkSetMaxInterpolationGap(h, gap) %#ok
%BTKSETMAXINTERPOLATIONGAP Set acquisition's maximum interpolation gap.
% 
%  BTKSETMAXINTERPOLATIONGAP(H, GAP) replace the maximum interpolation gap
%  for a given biomechanical acquistion represented by the handle H.  
%  This handle must be obtained by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETMAXINTERPOLATIONGAP not found');

% [EOF] btkSetMaxInterpolationGap.m
