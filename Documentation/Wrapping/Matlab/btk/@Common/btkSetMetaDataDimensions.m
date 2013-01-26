function btkSetMetaDataDimensions(h, label_idx, sublabel_idx, ..., newdims) %#ok
%BTKSETMETADATADIMENSIONS Set metadata's dimensions.
%  
%  BTKSETMETADATADIMENSIONS(H, LABEL, NEWDIMS) set the dimensions of the 
%  metadata which has the label LABEL and contained in the root.
%
%  BTKSETMETADATADIMENSIONS(H, LABEL, SUBLABEL, ..., NEWDIMS) set the dimensions
%  for the  metatada with the label SUBLABEL contained in its metadata's parent with
%  the label LABEL.  The number of sub-level is infinite.  However, the main common 
%  use of the metadata is to use LABEL as a group and SUBLABEL as a parameter. So the 
%  metadata has 2 levels.  This use was introduced in the C3D format.
%
%  BTKSETMETADATADIMENSIONS(H, INDEX, NEWDIMS) set the metadata's dimensions at the 
%  given index contained in the  metadata's root.
%
%  BTKSETMETADATADIMENSIONS(H, INDEX, SUBINDEX ..., NEWDIMS) set the metadata's
%  dimensions contained at the index SUBINDEX which is contained in the metadata with 
%  the index INDEX.
%
%  You can mix label and index to select the metadata.
%
%  This function can also returns an updated list of metadata.
%  MD = BTKSETMETADATADIMENSIONS(H, LABEL, NEWDIMS)
%  MD = BTKSETMETADATADIMENSIONS(H, LABEL, SUBLABEL, ..., NEWDIMS)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETMETADATADIMENSIONS not found');

% [EOF] btkSetMetaDataDimensions.m
