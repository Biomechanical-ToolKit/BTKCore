function btkSetMetaDataFormat(h, label_idx, sublabel_idx, ..., newFormat) %#ok
%BTKSETMETADATAFORMAT Set a metadata's format.
%  
%  BTKSETMETADATAFORMAT(H, LABEL, NEWFORMAT) set the format of the 
%  metadata which has the label LABEL and contained in the root. The variable 
%  NEWFORMAT is a string with the following format supported:
%   - Char: metadata's values are strings;
%   - Byte: metadata's values are bytes (coded on 8 bits)
%   - Integer: metadata's values are integer (coded on 16 bits)
%   - Real: metadata's values are real (float coded on 32 bits)
%  If you modify the format of a metadata, than its values are transformed to fit 
%  (if possible) the new format. There is no problem to transform a byte into an integer
%  or in a float or a string. But the reverse is not always true. A string with a number
%  inside (for example 1.2345) can be converted as a float. But the string HEEL will be 
%  converted as the value 0. Moreover, converting a string to a number (or the reverse)
%  will adapt automatically the dimensions.
%
%  BTKSETMETADATAFORMAT(H, LABEL, SUBLABEL, ..., NEWFORMAT) set the format
%  for the  metatada with the label SUBLABEL contained in its metadata's parent with
%  the label LABEL.  The number of sub-level is infinite.  However, the main common 
%  use of the metadata is to use LABEL as a group and SUBLABEL as a parameter. So the 
%  metadata has 2 levels.  This use was introduced in the C3D format.
%
%  BTKSETMETADATAFORMAT(H, INDEX, NEWFORMAT) set the metadata's format at the 
%  given index contained in the  metadata's root.
%
%  BTKSETMETADATAFORMAT(H, INDEX, SUBINDEX ..., NEWFORMAT) set the metadata's
%  format contained at the index SUBINDEX which is contained in the metadata with 
%  the index INDEX.
%
%  You can mix label and index to select the metadata.
%
%  This function can also returns an updated list of metadata.
%  MD = BTKSETMETADATAFORMAT(H, LABEL, NEWFORMAT)
%  MD = BTKSETMETADATAFORMAT(H, LABEL, SUBLABEL, ..., NEWFORMAT)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKSETMETADATAFORMAT not found');

% [EOF] btkSetMetaDataFormat.m
