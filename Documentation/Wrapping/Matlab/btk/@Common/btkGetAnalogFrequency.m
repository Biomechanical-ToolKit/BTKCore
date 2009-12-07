function af = btkGetAnalogFrequency(h) %#ok
%BTKGETANALOGFREQUENCY Returns the analog frequency.
% 
%  AF = BTKGETANALOGFREQUENCY(H) returns the acquisition's' frequency used for the analog channels.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009- Biomechafical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANALOGFREQUENCY not found');

% [EOF] btkGetAnalogFrequency.m
