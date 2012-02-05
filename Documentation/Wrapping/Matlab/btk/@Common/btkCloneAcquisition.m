function hc = btkCloneAcquisition(ho) %#ok
%BTKCLONEACQUISITION Clone the given acquisition.
% 
%  HC = BTKCLONEACQUISITION(HO) creates a deep copy (HC) of the acquisition (HO).
%
%  The original acquisition is represented by the handle HO. 
%  The cloned acquisition is returned as a handle (HC).

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCLONEACQUISITION not found');

% [EOF] btkCloneAcquisition.m