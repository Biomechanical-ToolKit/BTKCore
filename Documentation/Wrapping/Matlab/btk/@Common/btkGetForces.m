function [forces forcesInfo] = btkGetForces(h) %#ok
%BTKGETFORCES Extract forces from a biomechanical acquisition
% 
%  FORCES = BTKGETFORCES(H) returns a structure of forces.  Each 
%  fieldname in FORCES corresponds to a force's label.  The biomechanical 
%  acquisition handle H is represented as a double and can be only
%  used with the btk* functions.
%
%  [FORCES FORCESINFO] = BTKGETFORCES(H) returns forces' informations  
%  in FORCESINFO.  The structure FORCESINFO has the followig fields:
%    - FORCESINFO.FREQUENCY
%    - FORCESINFO.UNITS
%  FORCESINFO.FREQUENCY contains the forces' frequency.  FORCESINFO.UNITS
%  contains the unit of each force.  Each fieldname in FORCESINFO.UNITS
%  corresponds to force's label. 

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETFORCES not found');

% [EOF] btkGetForces.m

