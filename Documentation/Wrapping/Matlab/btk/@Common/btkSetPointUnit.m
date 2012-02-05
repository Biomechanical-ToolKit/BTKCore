function btkSetPointUnit(h, type, unit) %#ok
%BTKSETPOINTUNIT Modify point's unit.
% 
%  BTKSETPOINTUNIT(H, TYPE, UNIT) modifies point's unit by UNIT.
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
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETPOINTUNIT not found');

% [EOF] btkSetPointUnit.m
