function info = btkMetaDataInfo(format, values)
%BTKMETADATAINFO Create a structure containing the required fields for the informations of a metadata.
%
%  INFO = BTKMETADATAINFO(FORMAT, VALUES) creates the structure INFO which embed FORMAT and VALUES 
%  in the field 'format' and 'values' respectively.
%
%  This function simplify the construction of the information used in <a href="matlab:help btkAppendMetaData">btkAppendMetaData</a>.
%  To be used with <a href="matlab:help btkAppendMetaData">btkAppendMetaData</a>, the structure INFO must be set
%  with the following values:
%   - INFO.FORMAT: contains a string (Char, Byte, Integer or Real) with the format of the information
%   - INFO.VALUES: contains an array of numerical values or a cell of strings.
%                  - The array is used for information with the format Byte, Integer or Real.
%                  - The cell of strings is only use for information with the format Char.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

info.format = format;
info.values = values;
