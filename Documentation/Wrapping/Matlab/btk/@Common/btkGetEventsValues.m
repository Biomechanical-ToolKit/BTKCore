function [times, labels, descriptions, ids] = btkGetEventsValuesValues(h) %#ok
%BTKGETEVENTSVALUES Extract events' values from a biomechanical acquisition
% 
%  Events' time are computed using the following formula:
%   t = (event_frame - first_frame) / sample_frequency
%  Hence, if an event is set at the first frame of the acquisition, the (relative)
%  time is then set to 0.0 second.
%
%  TIMES = BTKGETEVENTSVALUES(H) returns a matrix (n,1) of real values containing the 
%  time of each event. These times are not sorted.
%
%  [TIMES, LABELS] = BTKGETEVENTSVALUES(H) returns also an array (n,1) of cells  containing
%  the label of each event.
%
%  [TIMES, LABELS, DESCRIPTIONS] = BTKGETEVENTSVALUES(H) returns also an array (n,1) of cells containing
%  the description of each event.
%
%  [TIMES, LABELS, DESCRIPTIONS, IDS] = BTKGETEVENTSVALUES(H) returns also a matrix (n,1) containing
%  the id associated with each event.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETEVENTSVALUES not found');

% [EOF] btkGetEventsValues.m