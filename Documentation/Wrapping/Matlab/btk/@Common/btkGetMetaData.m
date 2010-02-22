function btkGetMetaData(h) %#ok
%btkGetMetaData Extract acquisition's metadata
% 
%  METADATA = BTKGETMETADATA(H) returns a structure containing the entire 
%  metadata.  The structure METADATA contains the following fields:
%    - METADATA.INFO contains metadata's informations if present 
%                    or the value 0
%    - METADATA.INFO.FORMAT contains information's format Only the following
%                           formats are used: Char, Byte, Integer, Real
%    - METADATA.INFO.VALUES contains information's values. Depending of the format,
%                           data are scalar, matrix or array. Metadata with format
%                           Char are stored in cells.
%    - METADATA.CHILDREN represent children of this metadata or 0, 
%                        if there is no child.
%    - METADATA.CHILDREN.<label> contains the same structure 
%                                (INFO, CHILDREN, DESCRIPTION)
%    - METADATA.DESCRIPTION is the description of the metadata.
%  The biomechanical acquisition handle H is represented as a 32-bit integer 
%  and can be only used with the btk* functions.
%
%  METADATA = BTKGETMETADATA(H, LABEL) returns the metadata from the root
%  with the label LABEL.  If the label is not find in the children of the 
%  metadata's root an error is thrown.
%
%  METADATA = BTKGETMETADATA(H, LABEL, SUBLABEL) returns the metadata with the 
%  label SUBLABEL.  If the label is not find in the children of the metadata 
%  with the label LABEL, an error is thrown.
%
%  METADATA = BTKGETMETADATA(H, LABEL, SUBLABEL, SUBSUBLABEL, ...) returns the 
%  metadata with label SUBSUBLABEL from the SUBLABEL metadata extracted from 
%  LABEL metadata. There is not limit of depth.
%
%  By default, BTK, use two stages of metadata representing group and parameters.

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKGETMETADATA not found');

% [EOF] btkGetMetaData.m

