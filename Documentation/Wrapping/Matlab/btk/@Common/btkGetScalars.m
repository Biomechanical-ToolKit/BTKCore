function [scalars scalarsInfo] = btkGetScalars(h) %#ok
%BTKGETSCALARS Extract scalars from a biomechanical acquisition
% 
%  SCALARS = BTKGETSCALARS(H) returns a structure of scalars.  Each 
%  fieldname in SCALARS corresponds to a scalar's label.  The biomechanical 
%  acquisition handle H is represented as a double and can be only
%  used with the btk* functions.
%
%  [SCALARS SCALARSINFO] = BTKGETSCALARS(H) returns scalars' informations  
%  in SCALARSINFO.  The structure SCALARSINFO has the followig fields:
%    - SCALARSINFO.FREQUENCY
%    - SCALARSINFO.UNITS
%  SCALARSINFO.FREQUENCY contains the scalars' frequency.  SCALARSINFO.UNITS
%  contains the unit of each scalar.  Each fieldname in SCALARSINFO.UNITS
%  corresponds to scalar's label. 

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETSCALARS not found');

% [EOF] btkGetScalars.m



