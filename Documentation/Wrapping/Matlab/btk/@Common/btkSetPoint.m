function btkSetPoint(h, idx_or_label, values, residuals, masks) %#ok
%BTKSETPOINT Modify point's data and return updated points.
% 
%  BTKSETPOINT(H, INDEX, VALUES) modifies point's values by VALUES for the 
%  point at the index INDEX.  VALUES must be a matrix (N,3) where N 
%  corresponds to the number of frames in the acquisition.
%
%  BTKSETPOINT(H, INDEX, VALUES, RESIDUALS) modifies also the point's 
%  residuals. RESIDUALS is matrix (N,1) and corresponds to the residuals of
%  the 3D reconstruction of markers. 
%  NOTE: This parameter is an informational parameter and seems not used in 
%  some commercial softwares.
%
%  BTKSETPOINT(H, INDEX, VALUES, RESIDUALS, MASKS) modifies also the 
%  masks of the camera used to reconstruct markers' positions. MASKS is a 
%  matrix (N,1) of integer corresponding to a conversion of a binary number.
%  For example, the value 14 correspond to the mask 0b0001110 meaning that
%  cameras #2, #3 and #4 was used to reconstruct the point. You can use the
%  function <a href="matlab:help btkConvertBinaryMasks2Decimals">btkConvertBinaryMasks2Decimals</a> to convert the binary masks into 
%  decimal values.
%  NOTE: This parameter is an informational parameter and seems not used in 
%  some commercial softwares.
%
%  BTKSETPOINT(H, INDEX, VALUES, RESIDUALS, MASKS, DESCRIPTION) modifies also 
%  the description of the point.
%
%  The point to modify can also be selected by its LABEL.
%  BTKSETPOINT(H, LABEL, VALUES)
%  BTKSETPOINT(H, LABEL, VALUES, RESIDUALS)
%  BTKSETPOINT(H, LABEL, VALUES, RESIDUALS, MASKS)
%  BTKSETPOINT(H, LABEL, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%
%  This function can also returns an updated list of points.
%  POINTS = BTKSETPOINT(H, INDEX, VALUES)
%  POINTS = BTKSETPOINT(H, INDEX, VALUES, RESIDUALS)
%  POINTS = BTKSETPOINT(H, INDEX, VALUES, RESIDUALS, MASKS)
%  POINTS = BTKSETPOINT(H, INDEX, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%  POINTS = BTKSETPOINT(H, LABEL, VALUES)
%  POINTS = BTKSETPOINT(H, LABEL, VALUES, RESIDUALS)
%  POINTS = BTKSETPOINT(H, LABEL, VALUES, RESIDUALS, MASKS)
%  POINTS = BTKSETPOINT(H, LABEL, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%  The format of POINTS is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  This function can also returns an updated list of points' informations.
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, INDEX, VALUES)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, INDEX, VALUES, RESIDUALS)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, INDEX, VALUES, RESIDUALS, MASKS)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, INDEX, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, LABEL, VALUES)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, LABEL, VALUES, RESIDUALS)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, LABEL, VALUES, RESIDUALS, MASKS)
%  [POINTS, POINTSINFO] = BTKSETPOINT(H, LABEL, VALUES, RESIDUALS, MASKS, DESCRIPTION)
%  The format of POINTSINFO is the same than using the function <a href="matlab:help btkGetPoints">btkGetPoints</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINT not found');

% [EOF] btkSetPoint.m
