function h = btkCalculateModelMotion(hm, ha, procedure, ..., tr) %#ok
%BTKCALCULATEMODELMOTION Calculate the model's motions from the given motion builder.
% 
%  H = BTKCALCULATEMODELMOTION(HM, HA, BUILDER) returns a model (by the handle H) containing 
%  model's motions (i.e. segments' motions) based on data of the given acquisitions HA.
%  Generally, segment's motions correspond to the movement of the associated frame expressed in
%  the global frame.
%  The possible builder are:
%    - Basic: use the same frames' definition than in the static calibration procedure.
%
%  For example:
%    model = btkCalculateModelMotion(model, acq, 'basic');
%
%  H = BTKCALCULATEMODELMOTION(HM, HA, BUILDER, TR) uses the landmark translations
%  given in TR to correspond marker's label of the acquisition with marker's label
%  used in the model.  See the function <a href="matlab:help btkCalibrateModel">btkCalibrateModel</a> for more information.
% 
%  To know the landmark's label used in the models, check the documentation of the function <a href="matlab:help btkNewModel">btkNewModel</a>
%
%  The input model is represented by the handle HM.  The acquisition is represented by the handle HA.  The handles are obtained
%  by the use of btk* functions.

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCALCULATEMODELMOTION not found');

% [EOF] btkCalculateModelMotion.m

