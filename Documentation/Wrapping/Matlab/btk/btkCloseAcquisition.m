function btkCloseAcquisition(h) %#ok
%BTKCLOSEACQUISTION Release the memory associated with a C++ btk::Acquisition object
%
%  BTKCLOSEACQUISTION(H) is only an alias for the function <a href="matlab:help btkDeleteAcquisition">btkDeleteAcquisition</a>.
%
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

btkDeleteAcquisition(h)

% [EOF] btkCloseAcquisition.m