function [powers powersInfo] = btkGetPowers(h) %#ok
%BTKGETPOWERS Extract powers from a biomechanical acquisition
% 
%  POWERS = BTKGETPOWERS(H) returns a structure of powers.  Each 
%  fieldname in POWERS corresponds to a power's label.  The biomechanical 
%  acquisition handle H is represented as a double and can be only
%  used with the btk* functions.
%
%  [POWERS POWERSINFO] = BTKGETPOWERS(H) returns powers' informations  
%  in POWERSINFO.  The structure POWERSINFO has the followig fields:
%    - POWERSINFO.FREQUENCY
%    - POWERSINFO.UNITS
%  POWERSINFO.FREQUENCY contains the powers' frequency.  POWERSINFO.UNITS
%  contains the unit of each power.  Each fieldname in POWERSINFO.UNITS
%  corresponds to power's label. 

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETPOWERS not found');

% [EOF] btkGetPowers.m

