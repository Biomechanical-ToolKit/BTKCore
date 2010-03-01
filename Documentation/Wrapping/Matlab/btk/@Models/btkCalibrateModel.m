function h = btkCalibrateModel(hm, ha, procedure, ..., tr) %#ok
%BTKCALIBRATEMODEL Calibrate the model based on the given procedure.
% 
%  H = BTKCALIBRATEMODEL(HM, HA, PROCEDURE, ...) returns a model calibrated
%  by the the procedure given in PROCEDURE by using the handles HM for the model's
%  configuration and HA for the acquisition.  Each procedure has special requirement
%  represented by '...'. Check each procedure to know the requirement.
%  The possible procedures are:
%    - Static: calibration procedure based on a static acquisition. It requires subject's information (sex and weight) set by <a href="matlab:help btkSubject">btkSubject</a>.
%
%  For example:
%    model = btkCalibrateModel(model, acq, 'static', subject);
%
%  H = BTKCALIBRATEMODEL(HM, HA, PROCEDURE, ..., TR) uses the landmark translations
%  given in TR to correspond marker's label of the acquisition with marker's label
%  used in the model.  TR is a vector of cells (row or column) containing cells with 2 elements.
%  The first one corresponds to the marker's label in the model, whereas the second one is for the 
%  marker's label used in the acquisition.  For example:
%    tr = {{'C7', 'C7'}, ...                 % 7th cervical vertebra
%          {'T8', 'T8'}, ...                 % 8th thoracic vertebra
%          {'SS', 'IJ'}, ...                 % Suprasternale
%          {'XP', 'PX'}, ...                 % Xiphoideus process
%          {'AC_L', 'L.Acromion'}, ...       % Left acromion
%          {'AA_L', 'L.AngulusAcromio'}, ... % Left angulus acromion
%          {'GT_L', 'L.Biceps'}, ...         % Left greater tubercule
%          {'LHE_L', 'L.Epi.Lat'}};          % Left lateral humeral epicondyle
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

error(generatemsgid('NotSupported'),'MEX file for BTKCALIBRATEMODEL not found');

% [EOF] btkCalibrateModel.m
