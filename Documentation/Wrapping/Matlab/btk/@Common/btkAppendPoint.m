function btkAppendPoint(h, type, label, values, residuals desc) %#ok
%BTKAPPENDPOINT Append a new point and return an updated list of points.
% 
%  BTKAPPENDPOINT(H, TYPE, LABEL, VALUES) append a point with the type TYPE,
%  the label LABEL and the values VALUES.  TYPE must be a string corresponding 
%  to one of the following type:
%   - marker
%   - angle
%   - force
%   - moment
%   - power
%   - scalar
%   - reaction
%  The input VALUES must be a matrix (N,3) where N corresponds to the number 
%  of frames in the acquisition.
%
%  BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS) set also the point's 
%  residuals. RESIDUALS is matrix (N,1) and corresponds to the residuals of
%  the 3D reconstruction of markers. 
%  NOTE: The residual is an informational parameter and is not available 
%  in every file format.
%
%  BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, DESCRIPTION) 
%  modifies also the description of the point.
%
%  This function can also returns an updated list of points.
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES)
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS)
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, DESCRIPTION)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>.
%
%  This function can also returns an updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES)
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS)
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, DESCRIPTION)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKAPPENDPOINT not found');

% [EOF] btkAppendPoint.m
