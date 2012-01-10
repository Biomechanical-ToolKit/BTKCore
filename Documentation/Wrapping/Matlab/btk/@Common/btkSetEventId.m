function btkSetEventId(h, label, newid) %#ok
%BTKSETEVENTID Modify the id of all events which have the required label.
% 
%  BTKSETEVENTID(H, LABEL, NEWID) modifies event's id by NEWID
%  for all events with the label LABEL. LABEL must be a non-empty string.
%
%  Even if there is no rule for the id value, these 3 next values 
%  are used in Vicon Workstation and Nexus to distinguish events' type.
%  The value 0 is for general event. Value 1 corresponds to the foot strike 
%  event and value 2 for the foot off.  Without the use of these values, it 
%  is  known that Workstation and Nexus put all events in general (default 
%  value is 0).  For Polygon, one user reported that some points had their
%  values set to 0.
%
%  This function can also returns an updated list of events.
%  EVENTS = BTKSETEVENTID(H, LABEL, NEWID)
%  The format of EVENTS is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  This function can also returns an updated list of events' informations.
%  [EVENTS, EVENTSINFO] = BTKSETEVENTID(H, LABEL, NEWID)
%  The format of EVENTSINFO is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETEVENTID not found');

% [EOF] btkSetEventId.m
