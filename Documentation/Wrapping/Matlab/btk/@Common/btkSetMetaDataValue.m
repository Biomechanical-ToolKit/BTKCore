function btkSetMetaDataValue(h, label_idx, sublabel_idx, ..., index, value) %#ok
%BTKSETMETADATAVALUE Set one metada's value.
%  
%  BTKSETMETADATAVALUE(H, LABEL, INDEX, VALUE) set the VALUE for the INDEX in
%  the metadata which has the label LABEL and contained in the root. The INDEX's value
% start from 1, as all index in Matlab.
%  Based on the format of the metadata, the value set is automaticaly converted to fit the
%  format.  This design gives you the possibiliy to pass a Matlab native number format
%  (double) or a char or a cell containing a char and then its content is converted.
%
%  BTKSETMETADATAVALUE(H, LABEL, SUBLABEL, ..., INDEX, VALUE) set the VALUE
%  for the  metatada with the label SUBLABEL contained in its metadata's parent with
%  the label LABEL.  The number of sub-level is infinite.  However, the main common 
%  use of the metadata is to use LABEL as a group and SUBLABEL as a parameter. So the 
%  metadata has 2 levels.  This use was introduced in the C3D format.
%
%  BTKSETMETADATAVALUE(H, IDX, INDEX, VALUE) set the VALUE at the 
%  given INDEX contained in the  metadata's root.
%
%  BTKSETMETADATAVALUE(H, IDX, SUBIDX ..., INDEX, VALUE) set the metadata's
%  format contained at the index SUBINDEX which is contained in the metadata with 
%  the index INDEX.
%
%  You can mix label and index to select the metadata.
%
%  This function can also returns an updated list of metadata.
%  MD = BTKSETMETADATAVALUE(H, LABEL, INDEX, VALUE)
%  MD = BTKSETMETADATAVALUE(H, LABEL, SUBLABEL, ..., INDEX, VALUE)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETMETADATAVALUE not found');

% [EOF] btkSetMetaDataValue.m
