function h = btkNewAcquisition(pn, fn, an, r) %#ok
%BTKNEWACQUISITION Create an initialized acquisition
% 
%  H = BTKNEWACQUISITION(PN, FN) returns the handle H of an initialized 
%  biomechanical acquisition with PN points and FN frames. There is no analog
%  channels.
%
%  H = BTKNEWACQUISITION(PN, FN, AN) returns the handle H of an initialized 
%  biomechanical acquisition with PN points and AN analog channels. The number of frames 
%  corresponds to FN.
%
%  H = BTKNEWACQUISITION(PN, FN, AN, R) returns the handle H of an initialized 
%  biomechanical acquisition where the number of frame for points and analog channels
%  are differents. The number of frames for analog channels is set by mutiplying FN by R.
%  R corresponds to the analog samples number by point frame.
%
%  This handle is returned as a 32-bit and can be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKNEWACQUISITION not found');

% [EOF] btkNewAcquisition.m
