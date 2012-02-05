function btkSetPointType(h, idx_or_label, new_type) %#ok
%BTKSETPOINTTYPE Modify point's type and return updated points.
% 
%  BTKSETPOINTTYPE(H, INDEX, NEWTYPE) modifies point's type by NEWTYPE for 
%  the point at the index INDEX.  NEWTYPE must be a string corresponding to
%  one of the following type:
%   - marker
%   - angle
%   - force
%   - moment
%   - power
%   - scalar
%   - reaction
%
%  The point to modify can also be selected by its LABEL.
%  BTKSETPOINTTYPE(H, LABEL, NEWTYPE)
%
%  This function can also returns an updated list of points.
%  POINTS = BTKSETPOINTTYPE(H, INDEX, NEWTYPE)
%  POINTS = BTKSETPOINTTYPE(H, LABEL, NEWTYPE)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  This function can also returns an updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKSETPOINTTYPE(H, INDEX, NEWTYPE)
%  [POINTS, POINTSINFO] = BTKSETPOINTTYPE(H, LABEL, NEWTYPE)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTTYPE not found');

% [EOF] btkSetPointType.m
