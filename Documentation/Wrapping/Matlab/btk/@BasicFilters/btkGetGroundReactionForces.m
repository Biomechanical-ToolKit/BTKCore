function GRW = btkGetGroundReactionWrenches(h, threshold) %#ok
%BTKGETGROUNDREACTIONWRENCHES Extract ground reaction wrenches (GRW) from a biomechanical acquisition in the global frame.
% 
%  GRW = BTKGETGROUNDREACTIONWRENCHES(H) returns a structure of wrenches 
%  computed from force platform data.
%  The structure GRW is a row structure where each row contains 
%  data for one wrench.  Fieldnames for each row are:
%   - GRW(i).P
%   - GRW(i).F
%   - GRW(i).M
%  Field P is a matrix (n,3) where n is the number of frames and contains the
%  positions of the wrench in the global frame.  Field F is a matrix (n,3) 
%  which contains wrench's forces in the global frame.  Field M is matrix 
%  (n,3) which contains wrench's moments in the global frame.
%  The biomechanical acquisition handle H is represented as a 32-bit 
%  integer and can be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009- Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETGROUNDREACTIONWRENCHES not found');

% [EOF] btkGetGroundReactionWrenches.m

