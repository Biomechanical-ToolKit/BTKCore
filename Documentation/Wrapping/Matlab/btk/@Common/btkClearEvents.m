function btkClearEvents(h) %#ok
%BTKCLEAREVENTS Clear acquisition's events.
% 
%  BTKCLEAREVENTS(H) removes the events of the acquisition.
%  Use MEX functions <a href="matlab:help btkAppendEvent">btkAppendEvent</a>  and <a href="matlab:help btkRemoveEvent">btkRemoveEvent</a> to set acquisition's events.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCLEAREVENTS not found');

% [EOF] btkClearEvents.m