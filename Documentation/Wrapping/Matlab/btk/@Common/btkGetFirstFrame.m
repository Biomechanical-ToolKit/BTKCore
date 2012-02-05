function ff = btkGetFirstFrame(h) %#ok
%BTKGETFIRSTFRAME Returns acquisition's first frame index.
% 
%  FF = BTKGETFIRSTFRAME(H) returns the first frame index of the acquisition.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETFIRSTFRAME not found');

% [EOF] btkGetFirstFrame.m
