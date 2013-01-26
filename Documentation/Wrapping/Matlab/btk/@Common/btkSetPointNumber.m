function btkSetPointNumber(h, pn) %#ok
%BTKSETPOINTNUMBER Sets the number of points.
% 
%  BTKSETPOINTNUMBER(H, PN) resizes the number of points by PN for the 
%  acquisition represented by the handle H.  This handle is obtained by the 
%  use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTNUMBER not found');

% [EOF] btkSetPointNumber.m
