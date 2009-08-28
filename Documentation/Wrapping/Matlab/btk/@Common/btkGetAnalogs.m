function [analogs analogsInfo] = btkGetAnalogs(h) %#ok
%BTKGETANALOGS Extract analogs from a biomechanical acquisition
% 
%  ANALOGS = BTKGETANALOGS(H) returns a structure of analogs.  Each 
%  fieldname in ANALOGS corresponds to a analog channel's label.  The biomechanical 
%  acquisition handle H is represented as a 32-bit integer and can be only
%  used with the btk* functions.
%
%  [ANALOGS ANALOGSINFO] = BTKGETANALOGS(H) returns analogs' informations  
%  in ANALOGSINFO.  The structure ANALOGSINFO has the followig fields:
%    - ANALOGSINFO.FREQUENCY
%    - ANALOGSINFO.UNITS
%  ANALOGSINFO.FREQUENCY contains the analogs' frequency.  ANALOGSINFO.UNITS
%  contains the unit of each analog channel.  Each fieldname in ANALOGSINFO.
%  UNITS corresponds to analog channel's label. 

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANALOGS not found');

% [EOF] btkGetAnalogs.m

