function btkCropAcquisition(h, startAt, numFrames)
%BTKCROPACQUISITION Crop the acquisition and adapt the events' frame/time.
% 
%  BTKCROPACQUISITION(H, STARTAT) keeps all the frames from the frame STARTAT.
%  The function shifts also the events' frame/time by adding the difference 
%  between the new first frame and the old one. For this adaptation, the sample
%  frequency must be non-null.
%  The acquisition is represented by the handle H, obtained by the use of a 
%  btk* function.
%
%  BTKCROPACQUISITION(H, STARTAT, NUMFRAMES) keeps NUMFRAMES frames starting
%  from the frame STARTAT.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

ff = btkGetFirstFrame(h);
lf = btkGetLastFrame(h);
if (nargin == 2)
    numFrames = lf - ff + 1 - startAt + 1;
end
if ((startAt < ff) || startAt > lf)
    error('btk:CropAcquisiton','Invalid index.');
elseif (numFrames > lf - startAt + 1)
    error('btk:CropAcquisiton','Incorrect number of frames specified.')
elseif (numFrames == 0) % Clear all
    error('btk:CropAcquisiton','Due to the mechanism used in BTK, it is not possible to remove all the frames. Contact the developers if you really need it.')
end
% Data to keep
% - Point
pidx = (startAt:startAt+numFrames-1)-ff+1;
pv = btkGetPointsValues(h);
pv = pv(pidx,:);
rv = btkGetPointsResiduals(h);
rv = rv(pidx,:);
% - Analog
snpf = btkGetAnalogSampleNumberPerFrame(h);
aidx = (((startAt-ff)*snpf):(startAt-ff+numFrames)*snpf-1)+1;
av = btkGetAnalogsValues(h);
av = av(aidx,:);
% Resizing
btkSetFrameNumber(h, numFrames);
% Storing modifications
btkSetPointsValues(h, pv);
btkSetPointsResiduals(h, rv);
btkSetAnalogsValues(h, av);
% Set the first frame.
btkSetFirstFrame(h, startAt, 1); % 1: Modify also the events' frame/time