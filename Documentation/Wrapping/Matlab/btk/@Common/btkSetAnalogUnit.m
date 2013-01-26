function btkSetAnalogUnit(h, idx_or_label, new_unit) %#ok
%BTKSETANALOGUNIT Modify the analog's unit and return a list of the updated analog channels.
% 
%  BTKSETANALOGUNIT(H, INDEX, NEWUNIT) modifies analog's unit by
%  NEWUNIT for the analog channel at the index INDEX.  NEWUNIT must be a string.
%
%  The point to modify can also be selected by its LABEL.
%  BTKSETANALOGUNIT(H, LABEL, NEWUNIT)
%
%  This function can also returns an updated list of points.
%  ANALOGS = BTKSETANALOGUNIT(H, INDEX, NEWUNIT)
%  ANALOGS = BTKSETANALOGUNIT(H, LABEL, NEWUNIT)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of points' informations.
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGUNIT(H, INDEX, NEWUNIT)
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGUNIT(H, LABEL, NEWUNIT)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANALOGUNIT not found');

% [EOF] btkSetAnalogUnit.m
