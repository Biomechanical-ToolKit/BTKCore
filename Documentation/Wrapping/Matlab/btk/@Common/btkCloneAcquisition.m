function hc = btkCloneAcquisition(ho) %#ok
%BTKCLONEACQUISITION Clone the given acquisition.
% 
%  HC = BTKCLONEACQUISITION(HO) creates a deep copy (HC) of the acquisition (HO).
%
%  The original acquisition is represented by the handle HO. 
%  The cloned acquisition is returned as a handle (HC).
%
%  The release of the memory associated with the handle HC can be done automatically
%  by Matlab when you use the command 'clear all', or you can use the function
%  BTKDELETEACQUISITION.  The use of the function BTKDELETEACQUISITION is greatly
%  advised when you are doing batch processing as Matlab does not manage the C++
%  memory and an "Out of memory" error could be thrown.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCLONEACQUISITION not found');

% [EOF] btkCloneAcquisition.m