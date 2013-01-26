function btkSetFrameNumber(h, num) %#ok
%BTKSETFRAMENUMBER Set points and analogs frames number.
% 
%  BTKSETFRAMENUMBER(H, NUM) resizes the frame number with 
%  the value NUM for the given acquisition's handle H. Points' frames 
%  number corresponds to the value NUM. Analogs' frames number corresponds
%  to R * NUM, where R is the number of analog samples per point frame 
%  This handle is obtained by using a btk* function.
%
%  BTKSETFRAMENUMBER(H, NUM, R) resizes the frame number of the analog
%  by the product R * NUM. R must be an integer greater or equal than 1.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETFRAMENUMBER not found');

% [EOF] btkSetFrameNumber.m
