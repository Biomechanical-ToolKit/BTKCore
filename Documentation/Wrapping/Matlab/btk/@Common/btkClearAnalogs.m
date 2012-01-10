function btkClearAnalogs(h) %#ok
%BTKCLEARANALOGS Remove all analog channels in the given acquisition.
% 
%  BTKCLEARANALOGS(H) removes the analog channels of the acquisition.
%  Use MEX functions <a href="matlab:help btkAppendAnalog">btkAppendAnalog</a>  and <a href="matlab:help btkRemoveAnalog">btkRemoveAnalog</a> to set an analog channel.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCLEARANALOGS not found');

% [EOF] btkClearAnalogs.m