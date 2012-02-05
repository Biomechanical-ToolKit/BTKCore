function [analysis analysisInfo] = btkGetAnalysis(h) %#ok
%BTKGETANALYSIS Extract data contained in the metadata ANALYSIS.
% 
%  ANALYSIS = BTKGETANALYSIS(H) returns a structure of parameters related 
%  to the metadata ANALYSIS in the acquisition represented by the handle H.
%  Each fieldname in ANALYSIS corresponds to a parameter's label.  
%  The biomechanical acquisition handle H is represented as a double and 
%  can be only used with the btk* functions.
%
%  [ANALYSIS ANALYSISINFO] = BTKGETANALYSIS(H) returns analysis' informations  
%  in ANALYSISINFO.  The structure ANALYSISINFO has the followig fields:
%    - ANALYSISINFO.SUBJECTS
%    - ANALYSISINFO.UNITS
%  ANALYSISINFO.SUBJECTS contains the name of the subject related to each parameter.  
%  ANALYSISINFO.UNITS contains the unit of each parameter. 

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANALYSIS not found');

% [EOF] btkGetAnalysis.m

