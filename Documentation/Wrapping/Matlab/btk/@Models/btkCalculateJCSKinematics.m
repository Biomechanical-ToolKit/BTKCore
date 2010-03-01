function h = btkCalculateJCSKinematics(model) %#ok
%BTKCALCULATEJCSKINEMATICS Calculate kinematics for each defined joint in the model
% 
%  H = BTKCALCULATEJCSKINEMATICS(MODEL) returns a handle to an acquisition containing
%  joints' angles using by default the euler's sequence Z-X'-Y''.
%
%  H = BTKCALCULATEJCSKINEMATICS(MODEL, 'Sequence', JOINT_LABEL, SEQUENCE_CONFIGURATION)
%  H = BTKCALCULATEJCSKINEMATICS(MODEL, 'Sequence', JOINT_INDEX, SEQUENCE_CONFIGURATION)
%  give the possibility to configure the euler's sequence for each joint by using its label
%  (JOINT_LABEL) or its index in the model (JOINT_INDEX). Index starts to 1.  The sequence's 
%  configuration is given by a row or column vector containing 3 elements.  Each element 
%  represents an axis. The axis X is represented by the value 0.  The axis X is represented by 
%  the value 1.  The axis Z is represented by the value 2.
%  Example:
%    acq = btkCalculateJCSKinematics(model, 'Sequence', 'LWrist', [2,1,0]); % Z-X'-Y''
%    acq = btkCalculateJCSKinematics(model, 'Sequence', 'LWrist', [2,1,0], 'Sequence', 'RWrist', [2,1,0]);
%    acq = btkCalculateJCSKinematics(model, 'Sequence', 2, [2,1,0], 'Sequence', 5, [2,1,0]);

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKCALCULATEJCSKINEMATICS not found');

% [EOF] btkCalculateJCSKinematics.m

