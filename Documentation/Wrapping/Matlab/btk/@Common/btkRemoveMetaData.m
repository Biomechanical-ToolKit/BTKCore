function btkRemoveMetaData(h, label_idx, sublabel_idx, ...) %#ok
%BTKREMOVEMETADATA Remove a metada and return an updated list of the metada in the acquisition.
% 
%  Warning: Removing some metadata might alter an acquisition. For example, removing
%  the metadata FORCE_PLATFORM has for effect to not be able to extract force platform
%  data as it contains their geometrical informations.
%  
%  BTKREMOVEMETADATA(H, LABEL) removes a metatada with the label LABEL from the root.
%
%  BTKREMOVEMETADATA(H, LABEL, SUBLABEL, ...) removes a metatada with the label SUBLABEL 
%  in its metadata's parent with the label LABEL.  The number of sub-level is infinite.
%  However, the main common use of the metadata is to use LABEL as a group and SUBLABEL as a parameter.
%  So the metadata has 2 levels.  This use was introduced in the C3D format.
%
%  BTKREMOVEMETADATA(H, INDEX) removes the metadata at the given index contained in metadata's root.
%
%  BTKREMOVEMETADATA(H, INDEX, SUBINDEX ...) removes the metadata contained at the index 
%  SUBINDEX which is contained in the metadata with the index INDEX.
%
%  You can mix label and index to remove a metadata entry.
%
%  This function can also returns an updated list of metadata.
%  MD = BTKREMOVEMETADATA(H, LABEL, )
%  MD = BTKREMOVEMETADATA(H, LABEL, SUBLABEL, ...)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREMOVEMETADATA not found');

% [EOF] btkRemoveMetaData.m
