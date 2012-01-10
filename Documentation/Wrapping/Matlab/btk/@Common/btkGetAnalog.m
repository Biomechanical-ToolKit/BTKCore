function [values info] = btkGetAnalog(h) %#ok
%BTKGETANALOG Extract one analog channel from a biomechanical acquisition
% 
%  VALUES = BTKGETANALOG(H, INDEX) returns a column vector with the values for one 
%  analog channel based on the given INDEX. The biomechanical acquisition handle H is 
%  represented as a double and can be only used with the btk* functions.
%
%  [VALUES INFO] = BTKGETANALOG(H, INDEX) returns the informations associated with the 
%  the selected analog channel. The structure INFO has the followig fields:
%    - INFO.GAIN
%    - INFO.OFFSET
%    - INFO.SCALE
%    - INFO.FREQUENCY
%    - INFO.UNITS
%  INFO.GAIN, INFO.OFFSET and INFO.SCALE contains the required parameters
%  to convert the digital measure into its analog variant. INFO.FREQUENCY contains the 
%  analogs' frequency.  INFO.UNITS contains the unit of the analog channel.
%
%  You can get the analog channel's data by using its LABEL instead of its index.
%  VALUES = BTKGETANALOG(H, LABEL)
%  [VALUES, INFO] = BTKGETANALOG(H, LABEL)

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETANALOG not found');

% [EOF] btkGetAnalog.m

