function lf = btkGetLastFrame(h) %#ok
%BTKGETLASTFRAME Returns acquisition's last frame index.
% 
%  LF = BTKGETLASTFRAME(H) returns the last frame index of the acquisition.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

lf = btkGetFirstFrame(h) + btkGetPointFrameNumber(h) - 1;

% [EOF] btkGetFirstFrame.m
