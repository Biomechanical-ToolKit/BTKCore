function [events eventsInfo] = btkGetEvents(h) %#ok
%BTKGETEVENTS Extract events from a biomechanical acquisition
% 
%  EVENTS = BTKGETEVENTS(H) returns a structure of events.  Each 
%  fieldname in EVENTS corresponds to a event's label. The value of each 
%  field corresponds to the time(s) where this event occured.  If the events%  represent actions realized by more than one subject, each event starts 
%  by the name of the corresponding subject.
%  biomechanical acquisition handle H is represented as a 32-bit integer 
%  and can be only used with the btk* functions.
%
%  [EVENTS EVENTSINFO] = BTKGETEVENTS(H) returns events' informations  
%  in EVENTSINFO.  The structure EVENTSINFO has the followig fields:
%    - EVENTSINFO.SUBJECTS
%    - EVENTSINFO.UNITS
%  ANALYSISINFO.SUBJECTS contains the name of the subject related to each 
%  event.  EVENTSINFO.UNITS contains the unit of each event.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETEVENTS not found');

% [EOF] btkGetEvents.m


