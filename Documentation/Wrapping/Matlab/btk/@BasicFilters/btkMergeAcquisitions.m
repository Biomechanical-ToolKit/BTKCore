function acq = btkMergeAcquisitions(acq1, acq2, ...) %#ok
%BTKMERGEACQUISITIONS Merge acquisitions together.
% 
%  ACQ = BTKMERGEACQUISITIONS(ACQ1, ACQ2, ...) merges or contatenates the acquisitions given in input.
%  The difference between the merging or the concatenating depends of the acquisitions' informations.
%
%  The release of the memory associated with the handle ACQ can be done automatically
%  by Matlab when you use the command 'clear all', or you can use the function
%  BTKDELETEACQUISITION.  The use of the function BTKDELETEACQUISITION is greatly
%  advised when you are doing batch processing as Matlab does not manage the C++
%  memory and an "Out of memory" error could be thrown.
%
%  If the acquisitions are exactly the same (same label for points and analog channels) and the first 
%  frame of the n+1th acquistion corresponds to the last frame of the nth acquistion, then the 
%  acquisitions are merged, otherwise, the data are concatenated. Merged acquisitions means that the 
%  output will have the same number of points, analog channels, events than each output but the number
%  of frames corresponds to the sum of the input's frame number.
%
%  Rules to merge/concatenate acquisitions are:
%   - Must have the same acquisition frequency (or 0) if there are points.
%   - Must have the same number of analog samples per point frame (if there are points and analog channels).
%   - Must have the same analog resolution if there are .
%   - Must have the same units.
%   - Must have the same global frame orientation (check POINT:X_SCREEN and POINT:Y_SCREEN)
%   - The first input which has a non-zero acquisition frequency is used.
%   - The first input which has analog channels is used to set the analog resolution of the output.
%
%  Actions during the merging/concatenation:
%   - If the first frame of the nth acquisition correspond to the last frame + 1 of the nth-1 acquisition, 
%     AND the points / analog channels correspond exactly, then acquisitions are truncated.
%   - If the first frame of two inputs are not the same, the final acquisition is adapted to contain the 
%     number of frames plus the difference between the first frame of each input.
%   - If points have the same label (and the first frames doesn't correspond), then, the second and more are 
%     incremented (i.e. from Heel to Heel_2).
%   - Idem for the analog channels.
%   - Events with the same label, context, id and time are merged.
%   - Metadata (if number of inputs greater than 1):
%      - Only metdata defined by the user are inserted (i.e. POINT, ANALOG, EVENTS, etc. are not inserted as 
%        they are only used in a C3D file.).
%      - If metadata are equivalent or equal, then they are merged, else the metadata is appended with a 
%        suffix (i.e. from STATIC_CALIBRATION:USED to STATIC_CALIBRATION:USED_2)

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKMERGEACQUISITIONS not found');

% [EOF] btkMergeAcquisitions.m

