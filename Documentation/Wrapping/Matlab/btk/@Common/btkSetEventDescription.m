function btkSetEventDescription(h, label, new_description) %#ok
%BTKSETEVENTDESCRIPTION Modify the description of all events which have the required label.
% 
%  BTKSETEVENTDESCRIPTION(H, LABEL, NEWDESCRIPTION) modifies event's description by NEWDESCRIPTION
%  for all events with the label LABEL. LABEL must be a non-empty string.
%
%  This function can also returns an updated list of events.
%  EVENTS = BTKSETEVENTDESCRIPTION(H, LABEL, NEWDESCRIPTION)
%  The format of EVENTS is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  This function can also returns an updated list of events' informations.
%  [EVENTS, EVENTSINFO] = BTKSETEVENTDESCRIPTION(H, LABEL, NEWDESCRIPTION)
%  The format of BTKSETEVENTDESCRIPTION is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETEVENTDESCRIPTION not found');

% [EOF] btkSetEventDescription.m
