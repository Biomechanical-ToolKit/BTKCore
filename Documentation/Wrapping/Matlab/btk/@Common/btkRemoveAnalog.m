function btkRemoveAnalog(h, idx_or_label) %#ok
%BTKREMOVEANALOG Delete the analog channel with the given index or label. 
%
%  Warning: Removing an analog channel might alter the acquisition. If the analog channel
%  is one of them used in a force platform channel, then it cannot be possible to extract
%  force platform data nor ground reaction wrenches for example. 
% 
%  BTKREMOVEANALOG(H, INDEX) remove the analog channel at the given index.
%
%  The analog channel to remove can also be selected by its LABEL.
%  BTKREMOVEANALOG(H, LABEL)
%
%  This function can also returns an updated list of analog channels.
%  ANALOGS = BTKREMOVEANALOG(H, INDEX)
%  ANALOGS = BTKREMOVEANALOG(H, LABEL)
%  The format of ANALOGS is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  This function can also returns an updated list of points' informations.
%  [ANALOGS, ANALOGSINFO] = BTKREMOVEANALOG(H, INDEX)
%  [ANALOGS, ANALOGSINFO] = BTKREMOVEANALOG(H, LABEL)
%  The format of ANALOGSINFO is the same than using the function <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for function BTKREMOVEANALOG not found');

% [EOF] btkRemoveAnalog.m
