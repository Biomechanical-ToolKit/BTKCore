function btkSetEventSubject(h, index, newSubject) %#ok
%BTKSETEVENTSUBJECT Modify the subject's information of the selected event.
% 
%  BTKSETEVENTSUBJECT(H, INDEX, NEWSUBJECT) modifies the subject's information by NEWSUBJECT.
%  INDEX must be a single integer value.
%
%  This function can also returns an updated list of events.
%  EVENTS = BTKSETEVENTSUBJECT(H, INDEX, NEWSUBJECT)
%  The format of EVENTS is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  This function can also returns an updated list of events' informations.
%  [EVENTS, EVENTSINFO] = BTKSETEVENTSUBJECT(H, INDEX, NEWSUBJECT)
%  The format of EVENTSINFO is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETEVENTSUBJECT not found');

% [EOF] btkSetEventSubject.m
