function btkSetAnalogSampleNumberPerFrame(h, ratio) %#ok
%BTKSETANALOGSAMPLENUMBERPERFRAME Set the number of analog sample per 3D frame
% 
%  BTKSETANALOGSAMPLENUMBERPERFRAME(H, ratio) sets the number of analog samples 
%  per 3D frame.  This function only works if your acquisition doesn't contains
%  any analog channel. In the other case, an error will be thrown.  The input 
%  argument RATIO must be a single integer value starting from 1.  The acquisition 
%  is represented by the handle H. This handle is obtained by the use of 
%  a btk* function.   

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for function BTKSETANALOGSAMPLENUMBERPERFRAME not found');

% [EOF] btkSetAnalogSampleNumberPerFrame.m
