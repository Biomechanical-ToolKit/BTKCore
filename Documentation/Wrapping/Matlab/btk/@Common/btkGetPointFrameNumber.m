function pfn = btkGetPointFrameNumber(h) %#ok
%BTKGETPOINTFRAMENUMBER Returns the number of frames for the points.
% 
%  PFN = BTKGETPOINTFRAMENUMBER(H) returns the number of frames  used in the point's data.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETPOINTFRAMENUMBER not found');

% [EOF] btkGetPointFrameNumber.m
