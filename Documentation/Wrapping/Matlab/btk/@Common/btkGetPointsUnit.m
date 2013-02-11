function unit = btkGetPointsUnit(h, type) %#ok
%BTKGETPOINTUNIT Returns requested points' unit.
% 
%  UNIT = BTKGETPOINTUNIT(H, TYPE) return point's unit.
%  TYPE must be a string corresponding to one type of points:
%   - marker
%   - angle
%   - force
%   - moment
%   - power
%   - scalar
%
%  By default an acquisition is set with these units:
%   - marker: millimeter (mm)
%   - angle: degres (deg)
%   - force: newton (N)
%   - moment: newton millimeter (Nmm)
%   - power: watt (W)
%   - scalar: <empty>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETPOINTUNIT not found');

% [EOF] btkGetPointsUnit.m