function btkSetFirstFrame(h, ff, adaptEvents) %#ok
%BTKSETFIRSTFRAME Set acquisition's first frame index (and optionally adapt events' frame/time).
% 
%  BTKSETFIRSTFRAME(H, FF) replace the first frame index by the content of 
%  FF for a given biomechanical acquistion represented by the handle H.  
%  This handle must be obtained by the use of a btk* function.
%
%  BTKSETFIRSTFRAME(H, FF, ADAPTEVENTS) adapt also event's frame/time if 
%  the option ADAPTEVENTS is set to a non-null value.  The event's frame 
%  is shifted by the difference between the new first frame and the old 
%  one.  The event's time is recomputed using the new frame and the current
%  point's frequency.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETFIRSTFRAME not found');

% [EOF] btkSetFirstFrame.m
