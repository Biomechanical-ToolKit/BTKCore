function btkRemovePoint(h, idx_or_label) %#ok
%BTKREMOVEPOINT Delete the point with the given index or label. 
% 
%  BTKREMOVEPOINT(H, INDEX) remove the point at the given index.
%
%  The point to remove can also be selected by its LABEL.
%  BTKREMOVEPOINT(H, LABEL)
%
%  This function can also returns an updated list of points.
%  POINTS = BTKREMOVEPOINT(H, INDEX)
%  POINTS = BTKREMOVEPOINT(H, LABEL)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  This function can also returns an updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKREMOVEPOINT(H, INDEX)
%  [POINTS, POINTSINFO] = BTKREMOVEPOINT(H, LABEL)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for function BTKREMOVEPOINT not found');

% [EOF] btkRemovePoint.m
