function btkSetFirstFrame(h, ff) %#ok
%BTKSETFIRSTFRAME Set acquisition's first frame index.
% 
%  BTKSETFIRSTFRAME(H, FF) replace the first frame index by the content of 
%  FF for a given biomechanical acquistion represented by the handle H.  
%  This handle must be obtained by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETFIRSTFRAME not found');

% [EOF] btkSetFirstFrame.m
