function pn = btkGetAnalogNumber(h) %#ok
%BTKGETANALOGNUMBER Returns number of channels.
% 
%  AN = BTKGETANALOGNUMBER(H) returns the number of analog channels in one acquisition.
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANALOGNUMBER not found');

% [EOF] btkGetAnalogNumber.m
