function btkSetMetaDataLabel(h, label_idx, sublabel_idx, ..., newlabel) %#ok
%BTKSETMETADATALABEL Set a metadata's label
% 
%  Warning: Renaming some metadata might alter an acquisition. For example, 
%  renaming the metadata FORCE_PLATFORM has for effect to not be able to extract
%  force platform data as it contains their geometrical informations.
%  
%  BTKSETMETADATALABEL(H, LABEL, NEWLABEL) renames the metatada with the label 
%  LABEL from the root by the string NEWLABEL.
%
%  BTKSETMETADATALABEL(H, LABEL, SUBLABEL, ..., NEWLABEL) renames a metatada with 
%  the label SUBLABEL in its metadata's parent with the label LABEL.  The number 
%  of sub-level is infinite.  However, the main common use of the metadata is to 
%  use LABEL as a group and SUBLABEL as a parameter. So the metadata has 2 levels.
%  This use was introduced in the C3D format.
%
%  BTKSETMETADATALABEL(H, INDEX, NEWLABEL) renames the metadata at the given index 
%  contained in metadata's root.
%
%  BTKSETMETADATALABEL(H, INDEX, SUBINDEX ..., NEWLABEL) renames the metadata 
%  contained  at the index SUBINDEX which is contained in the metadata with the 
%  index INDEX.
%
%  You can mix label and index to select the metadata.
%
%  This function can also returns an updated list of metadata.
%  MD = BTKSETMETADATALABEL(H, LABEL, NEWLABEL)
%  MD = BTKSETMETADATALABEL(H, LABEL, SUBLABEL, ..., NEWLABEL)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETMETADATALABEL not found');

% [EOF] btkSetMetaDataLabel.m
