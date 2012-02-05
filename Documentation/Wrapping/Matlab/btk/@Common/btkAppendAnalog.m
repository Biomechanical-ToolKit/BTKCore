function btkAppendAnalog(h, label, valuesdesc) %#ok
%BTKAPPENDANALOG Append a new analog channel and return an updated list of analog channels.
% 
%  BTKAPPENDANALOG(H, LABEL, VALUES) append an analog channel with the label LABEL
%  and the values VALUES.  VALUES must be a matrix (N,1) where N corresponds to the 
%  number of frames (for the analog part) in the acquisition.
%
%  BTKAPPENDANALOG(H, LABEL, VALUES, DESCRIPTION) modifies also the description of 
%  the analog channel.
%
%  This function can also returns an updated list of analog channels.
%  ANALOGS = BTKAPPENDANALOG(H, LABEL, VALUES)
%  ANALOGS = BTKAPPENDANALOG(H, LABEL, VALUES, DESCRIPTION)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>.
%
%  This function can also returns an updated list of analog channels informations.
%  [ANALOGS, ANALOGSINFO] = BTKAPPENDANALOG(H, LABEL, VALUES)
%  [ANALOGS, ANALOGSINFO] = BTKAPPENDANALOG(H, LABEL, VALUES, DESCRIPTION)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKAPPENDANALOG not found');

% [EOF] btkAppendAnalog.m
