function h = btkNewAcquisition(pn, fn, an, r) %#ok
%BTKNEWACQUISITION Create a new and initialized acquisition
% 
%  H = BTKNEWACQUISITION() returns an uninitialized and empty acquisition.  The
%  use of the function <a href="matlab:help btkSetFrameNumber">btkSetFrameNumber</a>, <a href="matlab:help btkSetPointNumber">btkSetPointNumber</a>, <a href="matlab:help btkSetAnalogNumber">btkSetAnalogNumber</a> is
%  then required to set properly the information of the acquistion. It is impossible
%  to set directly data if the acquisition is uninitialized.
%  The parameters of the acquisition are set to:
%  - First frame equals to 1: use <a href="matlab:help btkSetFirstFrame">btkSetFirstFrame</a> to modify it.
%  - Points' frequency equals to 0: use <a href="matlab:help btkSetFrequency">btkSetFrequency</a> to modify it.
%  - Points' units set to "mm", "deg", "N", "Nmm", "W", "" for markers, angles, forces, 
%    moments, powers and scalars respectively: use <a href="matlab:help btkSetPointsUnit">btkSetPointsUnit</a> to modify them.
%  - Analogs' frequency equals to 0. Updated when the points' frequency is. 
%  - Analog channel resolution sets to 12 bits: use <a href="matlab:help btkSetAnalogResolution">btkSetAnalogResolution</a> to modify it.  
%
%  H = BTKNEWACQUISITION(PN) returns an uninitialized acquisition with @c pn
%  points and 0 frame. The use of the function <a href="matlab:help btkSetFrameNumber">btkSetFrameNumber</a> is then required to
%  set properly the number of frames.  It is impossible to set directly data if the
%  acquisition is uninitialized.  There is no analog channels.  The points created
%  are tagged with the type 'Marker'.  To change the type of theses points, use <a href="matlab:help btkSetPointType">btkSetPointType</a>
%  or <a href="matlab:help btkSetPoint">btkSetPoint</a>.
%
%  H = BTKNEWACQUISITION(PN, FN) returns the handle H of an initialized 
%  biomechanical acquisition with PN points and FN frames. 
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
%  The release of the memory associated with the handle H can be done automatically
%  by Matlab when you use the command 'clear all', or you can use the function
%  BTKDELETEACQUISITION.  The use of the function BTKDELETEACQUISITION is greatly
%  advised when you are doing batch processing as Matlab does not manage the C++
%  memory and an "Out of memory" error could be thrown.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKNEWACQUISITION not found');

% [EOF] btkNewAcquisition.m
