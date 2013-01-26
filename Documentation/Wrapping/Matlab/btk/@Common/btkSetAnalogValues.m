function btkSetAnalogValues(h, idx_or_label, values) %#ok
%BTKSETANALOGVALUES Modify analog's value and return updated analogs.
% 
%  BTKSETANALOGVALUES(H, INDEX, VALUES) modifies analog's values by VALUES for 
%  the analog at the index INDEX. VALUES is a column vector of real data (i.e scaled)
%  and must be the same size than the number of analog frames.
%
%  The analog to modify can also be selected by its LABEL.
%  BTKSETANALOGVALUES(H, LABEL, VALUES)
%
%  This function can also returns an updated list of analogs.
%  ANALOGS = BTKSETANALOGVALUES(H, INDEX, VALUES)
%  ANALOGS = BTKSETANALOGVALUES(H, LABEL, VALUES)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of analogs' informations.
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGVALUES(H, INDEX, VALUES)
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGVALUES(H, LABEL, VALUES)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANALOGVALUES not found');

% [EOF] btkSetAnalogValues.m
