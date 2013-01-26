function pf = btkGetPointFrequency(h) %#ok
%BTKGETPOINTFREQUENCY Returns the point's frequency.
% 
%  PF = BTKGETPOINTFREQUENCY(H) returns the acquisition frequency used for the point's data.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechafical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETPOINTFREQUENCY not found');

% [EOF] btkGetPointFrequency.m
