function btkSetAnalogsResolution(h, res)
%BTKSETANALOGSRESOLUTION Set the ADC resolution.
% 
%  BTKSETANALOGRESOLUTION(H, res) sets the resolution of the analog to
%  digital converter. The value must be 8, 12, 14 or 16. The acquisition 
%  is represented by the handle H. This handle is obtained by the use of 
%  a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

warning('This function is deprecated and will be removed in a next release. Use btkGetAnalogResolution instead.');
btkSetAnalogResolution(h,res);
