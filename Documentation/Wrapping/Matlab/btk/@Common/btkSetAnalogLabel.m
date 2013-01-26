function btkSetAnalogLabel(h, idx_or_label, new_label) %#ok
%BTKSETANALOGLABEL Modify analog's label and return updated analogs.
% 
%  BTKSETANALOGLABEL(H, INDEX, NEWLABEL) modifies analog's label by NEWLABEL for 
%  the analog at the index INDEX.  NEWLABEL must be a non-empty string.
%
%  The analog to modify can also be selected by its LABEL.
%  BTKSETANALOGLABEL(H, LABEL, NEWLABEL)
%
%  This function can also returns an updated list of analogs.
%  ANALOGS = BTKSETANALOGLABEL(H, INDEX, NEWLABEL)
%  ANALOGS = BTKSETANALOGLABEL(H, LABEL, NEWLABEL)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of analogs' informations.
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGLABEL(H, INDEX, NEWLABEL)
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGLABEL(H, LABEL, NEWLABEL)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANALOGLABEL not found');

% [EOF] btkSetAnalogLabel.m
