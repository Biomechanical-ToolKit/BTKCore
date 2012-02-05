function ratio = btkGetAnalogSampleNumberPerFrame(h)
%BTKGETANALOGSAMPLENUMBERPERFRAME Return the number of analog samples per video frame.
%
%  RATIO = BTKGETANALOGSAMPLENUMBERPERFRAME(H) divides the number of analog frames by 
%  the number of point frames to determine the ratio between them. This ratio is always 
%  greater or equal than 1. The handle H represents an acquistion and is obtained by the 
%  use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

ratio = btkGetAnalogFrameNumber(h) / btkGetPointFrameNumber(h);
