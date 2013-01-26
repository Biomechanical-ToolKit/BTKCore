function ar = btkGetAnalogResolution(h) %#ok
%BTKGETANALOGRESOLUTION Returns the ADC resolution.
% 
%  AR = BTKGETANALOGRESOLUTION(H) returns the resolution of the analog to
%  digital converter. The acquisition is represented by the handle H.  
%  This handle is obtained by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for function BTKGETANALOGRESOLUTION not found');

% [EOF] btkGetAnalogResolution.m
