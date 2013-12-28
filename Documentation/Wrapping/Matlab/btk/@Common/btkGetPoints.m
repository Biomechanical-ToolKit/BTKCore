function [points pointsInfo] = btkGetPoints(h) %#ok
%BTKGETPOINTS Extract points from a biomechanical acquisition
% 
%  POINTS = BTKGETPOINTS(H) returns a structure of points.  Each 
%  fieldname in POINTS corresponds to an adapted point's label compatible
%  with Matlab fieldname's rules (no accent, first character must be a letter,
%  only [A-Z,a-z,_] characters accepted).
%
%  The biomechanical acquisition handle H is represented as a double 
%  and can be only used with the btk* functions.
%
%  [POINTS POINTSINFO] = BTKGETPOINTS(H) returns points' informations  
%  in POINTSINFO.  The structure POINTSINFO has the followig fields:
%    - POINTSINFO.FIRSTFRAME
%    - POINTSINFO.FREQUENCY
%    - POINTSINFO.UNITS
%    - POINTSINFO.LABEL
%    - POINTSINFO.DESCRIPTION
%    - POINTSINFO.RESIDUALS
%  POINTSINFO.FIRSTFRAME contains the index of the first frame of the
%  acquisition. POINTSINFO.FREQUENCY contains the points' frequency.
%  POINTSINFO.UNITS contains the unit of each point.  POINTSINFO.LABEL
%  contains the true label of each point. POINTSINFO.DESCRIPTION contains
%  the description of each point. POINTSINFO.RESIDUALS contains the point's
%  residuals.  Each fieldname in POINTSINFO.LABEL, POINTSINFO.DESCRIPTION,
%  POINTSINFO.UNITS, and POINTSINFO.RESIDUALS corresponds to point's label.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETPOINTS not found');

% [EOF] btkGetPoints.m

