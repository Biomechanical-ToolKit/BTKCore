function btkSetAnalogScale(h, idx_or_label, newscale) %#ok
%BTKSETANALOGSCALE Modify analog's scale and return updated analogs.
% 
%  BTKSETANALOGSCALE(H, INDEX, NEWSCALE) modifies analog's scale by NEWSCALE for 
%  the analog at the index INDEX. This scale is used to convert numeric data into analog 
%  data or vice versa.
%
%  The analog to modify can also be selected by its LABEL.
%  BTKSETANALOGSCALE(H, LABEL, NEWSCALE)
%
%  This function can also returns an updated list of analogs.
%  ANALOGS = BTKSETANALOGSCALE(H, INDEX, NEWSCALE)
%  ANALOGS = BTKSETANALOGSCALE(H, LABEL, NEWSCALE)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of analogs' informations.
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGSCALE(H, INDEX, NEWSCALE)
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGSCALE(H, LABEL, NEWSCALE)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANALOGSCALE not found');

% [EOF] btkSetAnalogScale.m
