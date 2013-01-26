function btkSetAnalogGain(h, idx_or_label, new_gain) %#ok
%BTKSETANALOGGAIN Modify analog's gain and return updated analogs.
% 
%  BTKSETANALOGGAIN(H, INDEX, NEWGAIN) modifies analog's gain by NEWGAIN for 
%  the analog at the index INDEX.  NEWGAIN must be a value between 0 and 5 to represent:
%    - 0 : unknown gain
%    - 1 : +/- 10 volts
%    - 2 : +/- 5 volts
%    - 3 : +/- 2.5 volts
%    - 4 : +/- 1.25 volts
%    - 5 : +/- 1 volt
%
%  The analog to modify can also be selected by its LABEL.
%  BTKSETANALOGGAIN(H, LABEL, NEWGAIN)
%
%  This function can also returns an updated list of analogs.
%  ANALOGS = BTKSETANALOGGAIN(H, INDEX, NEWGAIN)
%  ANALOGS = BTKSETANALOGGAIN(H, LABEL, NEWGAIN)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of analogs' informations.
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGGAIN(H, INDEX, NEWGAIN)
%  [ANALOGS, ANALOGSINFO] = BTKSETANALOGGAIN(H, LABEL, NEWGAIN)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETANALOGGAIN not found');

% [EOF] btkSetAnalogGain.m
