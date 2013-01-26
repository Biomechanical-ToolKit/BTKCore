function btkDeleteAcquisition(h) %#ok
%BTKDELETEACQUISITION Deletes the acquisiton associated to the handle.
% 
%  BTKDELETEACQUISITION(H) deletes the acquisitions associated to the handle 
%  and erase the memory allocated to the acquisition.
%
%  NOTE: This function is generally not necessary as Matlab uses a garbage collector
%  to free the memory automatically. However, when you launch a batch and need to
%  extract some parameters from many acquisitions, it is advised to use this function.
%  For example:
%  % files: array of cells containing 500 filenames.
%  for i = 1:length
%    acq = btkReadAcquisiton(files{i});
%    outcome = computeOutcome(outcome, acq);
%    btkDeleteAcquisition(acq);
%  end
%
%  The acquistion is represented by the handle H, obtained by the use of a 
%  btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKDELETEACQUISITION not found');

% [EOF] btkDeleteAcquisition.m
