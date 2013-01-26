function btkRemoveEvent(h, options) %#ok
%BTKREMOVEEVENT Delete events corresponding to the given criterias.
% 
%  BTKREMOVEEVENT(H) is equivalent to the function <a href="matlab:help btkClearEvents">btkClearEvents</a>.
%
%  BTKREMOVEEVENT(H, INDEX) removes the events at the index INDEX. 
%
%  BTKREMOVEEVENT(H, OPTION, VALUE) removes only event(s) described by the options.
%  There are four options which can combined to refine the request.
%   - 'Time' followed by a single real value delete all events with this time +/- 0.00005 seconds
%   - 'Label' followed by a string delete all events which have this label
%   - 'Context' followed by a string delete all events which contains this context's name
%   - 'Subject' followed by a string delete all events which contains this subject's name
%  The combination of several options only delete events which match all criterias.
%
%  This function can also returns an updated list of events.
%  EVENTS = BTKREMOVEEVENT(H)
%  EVENTS = BTKREMOVEEVENT(H, INDEX)
%  EVENTS = BTKREMOVEEVENT(H, OPTION, VALUE)
%  The format of EVENTS is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  This function can also returns an updated list of events' informations.
%  [EVENTS, EVENTSINFO] = BTKREMOVEEVENT(H)
%  [EVENTS, EVENTSINFO] = BTKREMOVEEVENT(H, INDEX)
%  [EVENTS, EVENTSINFO] = BTKREMOVEEVENT(H, OPTION, VALUE)
%  The format of EVENTSINFO is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREMOVEEVENT not found');

% [EOF] btkRemovePoint.m
