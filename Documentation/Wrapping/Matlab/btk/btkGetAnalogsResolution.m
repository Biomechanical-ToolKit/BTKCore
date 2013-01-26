function ar = btkGetAnalogsResolution(h)
%BTKGETANALOGSRESOLUTION Returns the ADC resolution.
% 
%  AR = BTKGETANALOGSRESOLUTION(H) returns the resolution of the analog to
%  digital converter. The acquisition is represented by the handle H.  
%  This handle is obtained by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

warning('This function is deprecated and will be removed in a next release. Use btkGetAnalogResolution instead.');
return btkGetAnalogResolution(h);
