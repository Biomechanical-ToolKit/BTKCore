function h = btkReadAcquisition(filename) %#ok
%BTKREADACQUISITION Read an acquisition's file (C3D, TRC, ...)
% 
%  H = BTKREADACQUISITION(FILENAME) returns the handle H of a biomechanical 
%  acquisition stored in file FILENAME.  This handle is returned 
%  as a double and can be only used with the btk* function.  Supported 
%  file formats are ANC, C3D AND TRC.
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
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREADACQUISITION not found');

% [EOF] btkReadAcquisition.m
