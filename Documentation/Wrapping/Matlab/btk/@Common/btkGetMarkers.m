function [markers markersInfo] = btkGetMarkers(h) %#ok
%BTKGETMARKERS Extract markers from a biomechanical acquisition
% 
%  MARKERS = BTKGETMARKERS(H) returns a structure of markers.  Each 
%  fieldname in MARKERS corresponds to a marker's label.  The biomechanical 
%  acquisition handle H is represented as a double and can be only
%  used with the btk* functions.
%
%  [MARKERS MARKERSINFO] = BTKGETMARKERS(H) returns markers' informations  
%  in MARKERSINFO.  The structure MARKERSINFO has the followig fields:
%    - MARKERSINFO.FREQUENCY
%    - MARKERSINFO.UNITS
%  MARKERSINFO.FREQUENCY contains the markers' frequency.  MARKERSINFO.UNITS
%  contains the unit of each marker.  Each fieldname in MARKERSINFO.UNITS
%  corresponds to marker's label.
%
%  [MARKERS MARKERSINFO MARKERSRESIDUAL] = BTKGETMARKERS(H) returns markers'
%  residual related to their 3D reconstruction error.  Each fieldname in 
%  MARKERSRESIDUAL corresponds to a marker's label.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETMARKERS not found');

% [EOF] btkGetMarkers.m
