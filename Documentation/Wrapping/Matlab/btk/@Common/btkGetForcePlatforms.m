function [forceplates forceplatesInfo] = btkGetForcePlatforms(h) %#ok
%BTKGETFORCEPLATFORMS Extract force platforms' informations from a biomechanical acquisition
% 
%  FORCEPLATES = BTKGETFORCEPLATFORMS(H) returns a structure of force platforms. 
%  The structure FORCEPLATFORMS is a row structure where each row contains 
%  data for one force platform. Fieldnames for each row are:
%   - FORCEPLATES(i).CHANNELS
%   - FORCEPLATES(i).CORNERS
%   - FORCEPLATES(i).ORIGIN
%   - FORCEPLATES(i).TYPE
%  The field CHANNELS is also a structure with field corresponding to analog 
%  channel.  Their fieldname's label is the label of the analog channel.
%  Field CORNERS is a matrix (3,4) of double with the 3D coordinates 
%  of the four corners of the force platform (expressed in the global frame).
%  Field ORIGIN is a matrix (3,1) of double with the 3D coordinates 
%  of the origin the force platform (expressed in the global frame).
%  Field TYPE is an integer which indicate the force platform type.  For
%  much information on this type, see the C3D documentation (www.c3d.org).
%  The biomechanical acquisition handle H is represented as a double and can
%  be only used with the btk* functions.
%
%  [FORCEPLATES FORCEPLATESINFO] = BTKGETFORCEPLATFORMS(H) returns 
%  force platform's channel' informations in FORCEPLATESINFO.  The structure 
%  FORCEPLATESINFO has the following fields:
%    - FORCEPLATESINFO(i).FREQUENCY
%    - FORCEPLATESINFO(i).UNITS
%    - FORCEPLATESINFO(i).CAL_MATRIX
%  Field FREQUENCY contains the force platform's channel' frequency.  
%  Field UNITS contains the unit of each analog channel.  Each fieldname in
%  UNITS corresponds to analog channel's label.  Field CAL_MATRIX contains a
%  matrix which is used to correct the cross talk between channel.  This 
%  matrix is only used with supported force platform type.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETFORCEPLATFORMS not found');

% [EOF] btkGetForcePlatforms.m

