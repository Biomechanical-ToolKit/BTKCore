function btkGetPoint(h, idx_or_label) %#ok
%BTKGETPOINT Get point's data.
% 
%  VALUES = BTKGETPOINT(H, INDEX) returns the point's component as a matrix (N,3) 
%  where N corresponds to the number of frames in the acquisition.
%
%  [VALUES, RESIDUALS] = BTKGETPOINT(H, INDEX) returns the point's residuals in a matrix (N,1) 
%
%  You can get the point's data by using its LABEL instead of its index.
%  VALUES = BTKGETPOINT(H, LABEL)
%  [VALUES, RESIDUALS] = BTKGETPOINT(H, LABEL)
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETPOINT not found');

% [EOF] btkGetPoint.m
