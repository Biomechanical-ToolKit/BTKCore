function btkSetAnalogOffset(h, idx_or_label, new_offset) %#ok
%BTKSETANALOGOFFSET Modify analog's offset and return updated analogs.
% 
%  BTKSETANALOGOFFSET(H, INDEX, NEWOFFSET) modifies analog's offset by NEWOFFSET for 
%  the analog at the index INDEX. This offset is used to convert numeric data into analog 
%  data or vice versa.
%
%  The analog to modify can also be selected by its LABEL.
%  BTKSETANALOGOFFSET(H, LABEL, NEWOFFSET)
%
%  This function can also returns an updated list of analogs.
%  ANALOGS = BTKSETANALOGOFFSET(H, INDEX, NEWOFFSET)
%  ANALOGS = BTKSETANALOGOFFSET(H, LABEL, NEWOFFSET)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of analogs' informations.
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGOFFSET(H, INDEX, NEWOFFSET)
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGOFFSET(H, LABEL, NEWOFFSET)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANALOGOFFSET not found');

% [EOF] btkSetAnalogOffset.m
