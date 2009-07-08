function btkSetPointFrameNumber(h, pfn) %#ok
%BTKSETPOINTFRAMENUMBER Set the points's frame number.
% 
%  BTKSETPOINTFRAMENUMBER(H, PFN) resizes the points's frame number with 
%  the value PFN for the given acquisition's handle H.  This handle is 
%  obtained by using a btk* function.
%
%  This function can also returns updated list of points.
%  POINTS = BTKSETPOINTFRAMENUMBER(H, PFN)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  This function can also returns updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKSETPOINTFRAMENUMBER(H, PFN)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTFRAMENUMBER not found');

% [EOF] btkSetPointFrameNumber.m
