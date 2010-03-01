function h = btkNewModel(NAME, SIDE) %#ok
%BTKNEWMODEL Create a new and initialized model
% 
%  H = BTKNEWMODEL(NAME, SIDE) returns the handle H of an initialized 
%  biomechanical model. The creation of the model is based on the given NAME and SIDE.
%  The variable NAME corresponds to a string which must contain the label of a predfined model.
%  The predefined models are the following:
%    - UpperLimb: model with trunk, arm(s), forearm(s) and hand(s) segments defining shouler(s), 
                  elbow(s), wrist(s) and thorax joints.
%  The SIDE corresponds to a string with three possible values:
%    - Left: create a model with the left segments.
%    - Right: create a model with the right segments.
%    - Both: create a model with the left and right segments.
%
%  This handle is returned as a 32-bit integer and can be only used with the btk* functions.

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKNEWMODEL not found');

% [EOF] btkNewModel.m
