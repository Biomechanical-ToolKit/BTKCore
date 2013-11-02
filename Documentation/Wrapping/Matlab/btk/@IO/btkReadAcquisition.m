function h = btkReadAcquisition(filename) %#ok
%BTKREADACQUISITION Read an acquisition's file (C3D, TRC, ...)
% 
%  H = BTKREADACQUISITION(FILENAME) returns the handle H of a biomechanical 
%  acquisition stored in file FILENAME.  This handle is returned 
%  as a double and can be only used with the btk* function.
%
%  The release of the memory associated with the handle H can be done automatically
%  by Matlab when you use the command 'clear all', or you can use the function
%  BTKDELETEACQUISITION.  The use of the function BTKDELETEACQUISITION is greatly
%  advised when you are doing batch processing as Matlab does not manage the C++
%  memory and an "Out of memory" error could be thrown.
%
%  [H, BYTEORDER] = BTKREADACQUISITION(FILENAME) returns the byte order of the
%  file as a string. The known values are:
%   - OrderNotApplicable (In the case the file is an ASCII file).
%   - IEEE_LittleEndian
%   - VAX_LittleEndian
%   - IEEE_BigEndian
%
%  [H, BYTEORDER, STORAGEFORMAT] = BTKREADACQUISITION(FILENAME) returns the storage
%  format of the file as a string. The known values are:
%   - StorageNotApplicable (In the case the file is an ASCII file).
%   - Float
%   - Integer

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREADACQUISITION not found');

% [EOF] btkReadAcquisition.m
