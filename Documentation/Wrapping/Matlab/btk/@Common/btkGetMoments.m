function [moments momentsInfo] = btkGetMoments(h) %#ok
%BTKGETMOMENTS Extract moments from a biomechanical acquisition
% 
%  MOMENTS = BTKGETMOMENTS(H) returns a structure of moments.  Each 
%  fieldname in MOMENTS corresponds to a moment's label.  The biomechanical 
%  acquisition handle H is represented as a double and can be only used 
%  with the btk* functions.
%
%  [MOMENTS MOMENTSINFO] = BTKGETMOMENTS(H) returns moments' informations  
%  in MOMENTSINFO.  The structure MOMENTSINFO has the followig fields:
%    - MOMENTSINFO.FREQUENCY
%    - MOMENTSINFO.UNITS
%  MOMENTSINFO.FREQUENCY contains the moments' frequency.  MOMENTSINFO.UNITS
%  contains the unit of each moment.  Each fieldname in MOMENTSINFO.UNITS
%  corresponds to moment's label. 

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETMOMENTS not found');

% [EOF] btkGetMoments.m


