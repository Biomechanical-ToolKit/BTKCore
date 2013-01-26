function btkAppendEvent(h, label, time, context, subject, description, id) %#ok
%BTKAPPENDEVENT Append a new event and return an updated list of events.
% 
%  BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT) append an event with the label LABEL,
%  the time TIME and the context CONTEXT.  Even if there is no rule for the context,
%  these 3 contexts are oftenly used: General, Left, Right.  Moreover, as an event is
%  defined by also a subject, description and id and they are not provided by this 
%  function, they are automaticaly set.  If the metadata exists, the first value
%  in the metadata SUBJECTS:NAMES is used to set event's subject.  The description and 
%  id members are set with informations found in the first event with the same label.
%  
%  BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT) set also the event's 
%  subject.  The description and id are always automaticaly set.
%
%  BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT, DESCRIPTION) set also the 
%  description of the event.
%
%  BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT, DESCRIPTION, ID)  modifies also
%  the id of the event.  Even if there is no rule for the id value, these 3 next values 
%  are used in Vicon Workstation and Nexus to distinguish events' type.  The value 0 is for
%  general event. Value 1 corresponds to the foot strike event and value 2 for the foot off.
%
%  This function can also returns an updated list of events.
%  EVENTS = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT)
%  EVENTS = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT)
%  EVENTS = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT, DESCRIPTION)
%  EVENTS = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT, DESCRIPTION, ID)
%  The format of EVENTS is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  This function can also returns an updated list of events' informations.
%  [EVENTS, EVENTSINFO] = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT)
%  [EVENTS, EVENTSINFO] = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT)
%  [EVENTS, EVENTSINFO] = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT, DESCRIPTION)
%  [EVENTS, EVENTSINFO] = BTKAPPENDEVENT(H, LABEL, TIME, CONTEXT, SUBJECT, DESCRIPTION, ID)
%  The format of EVENTSINFO is the same than using the function <a href="matlab:help btkGetEvents">btkGetEvents</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKAPPENDEVENT not found');

% [EOF] btkAppendEvent.m
