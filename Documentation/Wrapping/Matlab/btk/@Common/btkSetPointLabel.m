function btkSetPointLabel(h, idx_or_label, new_label) %#ok
%BTKSETPOINTLABEL Modify point's label and return updated points.
% 
%  BTKSETPOINTLABEL(H, INDEX, NEWLABEL) modifies point's label by NEWLABEL for 
%  the point at the index INDEX.  NEWLABEL must be a non-empty string.
%
%  The point to modify can also be selected by its LABEL.
%  BTKSETPOINTLABEL(H, LABEL, NEWLABEL)
%
%  This function can also returns an updated list of points.
%  POINTS = BTKSETPOINTLABEL(H, INDEX, NEWLABEL)
%  POINTS = BTKSETPOINTLABEL(H, LABEL, NEWLABEL)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  This function can also returns an updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKSETPOINTLABEL(H, INDEX, NEWLABEL)
%  [POINTS, POINTSINFO] = BTKSETPOINTLABEL(H, LABEL, NEWLABEL)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTLABEL not found');

% [EOF] btkSetPointLabel.m
