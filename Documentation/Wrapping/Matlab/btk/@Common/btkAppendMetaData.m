function btkAppendMetaData(h, label, sublabel, ..., info) %#ok
%BTKAPPENDMETADATA Append a new metada and return an updated list of the metadata in the acquisition.
% 
%  BTKAPPENDMETADATA(H, LABEL) appends a metadata with the label LABEL to the root.
%  This metadata has no child, nor information.
%
%  BTKAPPENDMETADATA(H, LABEL, SUBLABEL) appends a metadata with the label SUBLABEL in
%  another metadata with the label LABEL.  The metadata 'LABEL' will be automaticaly generated
%  if it doesn't exist. The metadata 'SUBLABEL' has no child, nor information.
%  The number of sub-level is infinite. However, the main common use of the metadata is to 
%  use LABEL as a group and SUBLABEL as a parameter.  So the metadata has 2 levels.  This use was 
%  introduced in the C3D format.
%
%  BTKAPPENDMETADATA(H, LABEL, SUBLABEL, ..., INFO) appends a metadata with additional 
%  informations INFO.  This additionnal information is a structure:
%   - INFO.FORMAT: contains a string (Char, Byte, Integer or Real) with the format of the information
%   - INFO.VALUES: contains an array of numerical values or a cell of strings.
%                  - The array is used for information with the format Byte, Integer or Real.
%                  - The cell of strings is only use for information with the format Char.
%  The stucture INFO should be constructed with the use of the function <a href="matlab:help btkMetaDataInfo">btkMetaDataInfo</a>.  
%
%  This function can also returns an updated list of metadata.
%  MD = BTKAPPENDMETADATA(H, LABEL, )
%  MD = BTKAPPENDMETADATA(H, LABEL, SUBLABEL, ...)
%  MD = BTKAPPENDMETADATA(H, LABEL, SUBLABEL, ..., INFO)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKAPPENDMETADATA not found');

% [EOF] btkAppendMetaData.m
