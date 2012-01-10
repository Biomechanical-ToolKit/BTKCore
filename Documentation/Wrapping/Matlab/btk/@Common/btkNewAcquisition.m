function h = btkNewAcquisition(pn, fn, an, r) %#ok
%BTKNEWACQUISITION Create a new and initialized acquisition
% 
%  H = BTKNEWACQUISITION(PN, FN) returns the handle H of an initialized 
%  biomechanical acquisition with PN points and FN frames. There is no analog
%  channels. The points created are tagged with the type 'Marker'. To change the type
%  of theses points, use <a href="matlab:help btkSetPointType">btkSetPointType</a> or <a href="matlab:help btkSetPoint">btkSetPoint</a>.
%  Other parameters of the acquisition are set to:
%  - First frame equals to 1: use <a href="matlab:help btkSetFirstFrame">btkSetFirstFrame</a> to modify it.
%  - Points' frequency equals to 0: use <a href="matlab:help btkSetFrequency">btkSetFrequency</a> to modify it.
%  - Points' units set to "mm", "deg", "N", "Nmm", "W", "" for markers, angles, forces, 
%    moments, powers and scalars respectively: use <a href="matlab:help btkSetPointUnits">btkSetPointUnits</a> to modify them.
%  - Analogs' frequency equals to 0. Updated when the points' frequency is. 
%  - Analog channel resolution sets to 12 bits: use <a href="matlab:help btkSetAnalogResolution">btkSetAnalogResolution</a> to modify it.
%
%  H = BTKNEWACQUISITION(PN, FN, AN) returns the handle H of an initialized 
%  biomechanical acquisition with PN points and AN analog channels. The number of frames 
%  corresponds to FN.
%
%  H = BTKNEWACQUISITION(PN, FN, AN, R) returns the handle H of an initialized 
%  biomechanical acquisition where the number of frames  for points and analog channels
%  are differents. The number of frames for analog channels is set by mutiplying FN by R.
%  R corresponds to the analog samples number by point frame.
%
%  This handle is returned as a double and can be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKNEWACQUISITION not found');

% [EOF] btkNewAcquisition.m
