function btkClearPoints(h) %#ok
%BTKCLEARPOINTS Clear acquisition's events.
% 
%  BTKCLEARPOINTS(H) removes the points of the acquisition.
%  Use MEX functions <a href="matlab:help btkAppendPoint">btkAppendPoint</a>  and <a href="matlab:help btkRemovePoint">btkRemovePoint</a> to set acquisition's points.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCLEARPOINTS not found');

% [EOF] btkClearPoints.m