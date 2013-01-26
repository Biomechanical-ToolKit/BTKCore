function lf = btkGetLastFrame(h) %#ok
%BTKGETLASTFRAME Returns acquisition's last frame index.
% 
%  LF = BTKGETLASTFRAME(H) returns the last frame index of the acquisition.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

lf = btkGetFirstFrame(h) + btkGetPointFrameNumber(h) - 1;

% [EOF] btkGetFirstFrame.m
