function FPW = btkGetForcePlatformWrenches(h, threshold) %#ok
%BTKGETFORCEPLATFORMWRENCHES Extract force platform wrenches (FPW) from a biomechanical acquisition in the global frame.
% 
%  FPW = BTKGETFORCEPLATFORMWRENCHES(H) returns a structure of wrenches 
%  computed from force platform data and expressed in the global frame.
%  The structure FPW is a row structure where each row contains 
%  data for one wrench.  Fieldnames for each row are:
%   - FPW(i).P
%   - FPW(i).F
%   - FPW(i).M
%  The field P is a matrix (n,3) where n is the number of frames and contains the
%  positions of the wrench in the global frame (force platform orgin).  The field F 
%  is a matrix (n,3) which contains wrench's forces in the global frame.  The field M 
%  is a matrix (n,3) which contains wrench's moments in the global frame.
%
%  FPW = BTKGETFORCEPLATFORMWRENCHES(H, INGLOBALFRAME) gives the possibility to compute 
%  the force platform in the local frame when INGLOBALFRAME is equal to 0. Other values
%  for INGLOBALFRAME compute the force platform wrenches in the global frame.
%
%  The biomechanical acquisition handle H is represented as a double and can be 
%  only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETFORCEPLATFORMWRENCHES not found');

% [EOF] btkGetForcePlatformWrenches.m

