function btkSetMetaDataUnlock(h, label_idx, sublabel_idx, ..., isunlocked) %#ok
%BTKSETMETADATAUNLOCK Set the unlock state of a metadata.
%  
%  BTKSETMETADATAUNLOCK(H, LABEL, ISUNLOCKED) sets the unlock state for the metadata
%  which has the label LABEL and contained in the root. A metadata is unlocked if 
%  the input variable ISUNLOCKED is not equal to zero.
%
%  BTKSETMETADATAUNLOCK(H, LABEL, SUBLABEL, ..., ISUNLOCKED) sets the unlock state
%  for the  metatada with the label SUBLABEL contained in its metadata's parent with
%  the label LABEL.  The number of sub-level is infinite.  However, the main common 
%  use of the metadata is to use LABEL as a group and SUBLABEL as a parameter. So the 
%  metadata has 2 levels.  This use was introduced in the C3D format.
%
%  BTKSETMETADATAUNLOCK(H, INDEX, ISUNLOCKED) sets the metadata's unlock state at the 
%  given index contained in the  metadata's root.
%
%  BTKSETMETADATAUNLOCK(H, INDEX, SUBINDEX ..., ISUNLOCKED) sets the metadata's
%  unlock state contained at the index SUBINDEX which is contained in the metadata with 
%  the index INDEX.
%
%  You can mix label and index to select the metadata.
%
%  This function can also returns an updated list of metadata.
%  MD = BTKSETMETADATAUNLOCK(H, LABEL, ISUNLOCKED)
%  MD = BTKSETMETADATAUNLOCK(H, LABEL, SUBLABEL, ..., ISUNLOCKED)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETMETADATAUNLOCK not found');

% [EOF] btkSetMetaDataUnlock.m
