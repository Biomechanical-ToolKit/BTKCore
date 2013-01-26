function btkSetFrequency(h, freq) %#ok
%BTKSETFREQUENCY Set the main frequency.
% 
%  BTKSETFREQUENCY(H, FREQ) set the main frequency of the acquisition
%  by the value FREQ. The acquisitions is represented by the handle H.  
%  This handle is obtained by using a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETFREQUENCY not found');

% [EOF] btkSetFrequency.m
