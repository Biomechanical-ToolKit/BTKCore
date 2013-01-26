function av = btkGetAnalogsValues(h) %#ok
%BTKGETANALOGSVALUES Extract analog channels' values and store them in a matrix
% 
%  AV = BTKGETANALOGSVALUES(H) returns a matrix of reals.  The number of rows
%  of this matrix corresponds to the number of analog's frame.  The number 
%  of columns of this matrix corresponds to the number of analog channels.
%  The biomechanical acquisition handle H is represented as a double and can 
%  be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANALOGSVALUES not found');

% [EOF] btkGetAnalogsValues.m
