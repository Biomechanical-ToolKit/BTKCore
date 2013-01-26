function btkSetPointDescription(h, idx_or_label, new_desc) %#ok
%BTKSETPOINTDESCRIPTION Modify point's description and return updated points.
% 
%  BTKSETPOINTDESCRIPTION(H, INDEX, NEWDESC) modifies point's description by
%  NEWDESC for the point at the index INDEX.  NEWDESC must be a string.
%
%  The point to modify can also be selected by its LABEL.
%  BTKSETPOINTDESCRIPTION(H, LABEL, NEWDESC)
%
%  This function can also returns an updated list of points.
%  POINTS = BTKSETPOINTDESCRIPTION(H, INDEX, NEWDESC)
%  POINTS = BTKSETPOINTDESCRIPTION(H, LABEL, NEWDESC)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  This function can also returns an updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKSETPOINTDESCRIPTION(H, INDEX, NEWDESC)
%  [POINTS, POINTSINFO] = BTKSETPOINTDESCRIPTION(H, LABEL, NEWDESC)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTDESCRIPTION not found');

% [EOF] btkSetPointDescription.m
