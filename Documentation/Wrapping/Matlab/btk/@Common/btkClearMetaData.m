function btkClearMetaData(h) %#ok
%BTKCLEARMETADATA Clear acquisition's events.
% 
%  BTKCLEARMETADATA(H) removes the metadata in the acquisition.
%  Use MEX functions <a href="matlab:help btkAppendMetaData">btkAppendMetaData</a>  and <a href="matlab:help btkRemoveMetaData">btkRemoveMetaData</a> to set acquisition's metadata.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCLEARMETADATA not found');

% [EOF] btkClearMetaData.m