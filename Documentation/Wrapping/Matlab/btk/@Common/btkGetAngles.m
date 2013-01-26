function [angles anglesInfo] = btkGetAngles(h) %#ok
%BTKGETANGLES Extract angles from a biomechanical acquisition
% 
%  ANGLES = BTKGETANGLES(H) returns a structure of angles.  Each 
%  fieldname in ANGLES corresponds to an angle's label.  The biomechanical 
%  acquisition handle H is represented as a double and can be only
%  used with the btk* functions.
%
%  [ANGLES ANGLESINFO] = BTKGETANGLES(H) returns angles' informations  
%  in ANGLESINFO.  The structure ANGLESINFO has the followig fields:
%    - ANGLESINFO.FREQUENCY
%    - ANGLESINFO.UNITS
%  ANGLESINFO.FREQUENCY contains the angles' frequency.  ANGLESINFO.UNITS
%  contains the unit of each angle.  Each fieldname in ANGLESINFO.UNITS
%  corresponds to angle's label. 

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANGLES not found');

% [EOF] btkGetAngles.m



