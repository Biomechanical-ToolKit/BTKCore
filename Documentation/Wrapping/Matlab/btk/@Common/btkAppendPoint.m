function btkAppendPoint(h, type, label, values, residuals, masks, desc) %#ok
%BTKAPPENDPOINT Append a new point and return an updated list of points.
% 
%  BTKAPPENDPOINT(H, TYPE, LABEL, VALUES) append a point with the type TYPE,
%  the label LABEL and the values VALUES.  VALUES must be a matrix (N,3) 
%  where N corresponds to the number of frames in the acquisition.
%
%  BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS) set also the point's 
%  residuals. RESIDUALS is matrix (N,1) and corresponds to the residuals of
%  the 3D reconstruction of markers. 
%  NOTE: This parameter is an informational parameter and seems not used in 
%  some commercial softwares.
%
%  BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, MASKS) set also the 
%  masks of the camera used to reconstruct markers' positions. MASKS is a 
%  matrix (N,1) of integer corresponding to a conversion of a binary number.
%  For example, the value 14 correspond to the mask 0b0001110 meaning that
%  cameras #2, #3 and #4 was used to reconstruct the point 
%  NOTE: This parameter is an informational parameter and seems not used in 
%  some commercial software.
%
%  BTKSETPBTKAPPENDPOINTOINT(H, TYPE, LABEL, VALUES, RESIDUALS, MASKS, DESCRIPTION) 
%  modifies also the description of the point.
%
%  This function can also returns updated list of points.
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES)
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS)
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, MASKS)
%  POINTS = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>.
%
%  This function can also returns updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES)
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS)
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, MASKS)
%  [POINTS, POINTSINFO] = BTKAPPENDPOINT(H, TYPE, LABEL, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for function BTKAPPENDPOINT not found');

% [EOF] btkAppendPoint.m
