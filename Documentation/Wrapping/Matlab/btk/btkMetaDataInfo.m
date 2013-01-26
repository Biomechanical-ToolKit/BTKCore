function info = btkMetaDataInfo(format, values, numdims)
%BTKMETADATAINFO Create a structure containing the required fields for the informations of a metadata.
%
%  INFO = BTKMETADATAINFO(FORMAT, VALUES) creates the structure INFO which embed FORMAT and VALUES 
%  in the field 'format' and 'values' respectively.
%
%  INFO = BTKMETADATAINFO(FORMAT, VALUES, NUMDIMS) sets the desired number of dimensions.
%
%  This function simplify the construction of the information used in <a href="matlab:help btkAppendMetaData">btkAppendMetaData</a>.
%  To be used with <a href="matlab:help btkAppendMetaData">btkAppendMetaData</a>, the structure INFO must be set
%  with the following values:
%   - INFO.FORMAT:  contains a string (Char, Byte, Integer or Real) with the format of the information
%   - INFO.VALUES:  contains an array of numerical values or a cell of strings.
%                   - The array is used for information with the format Byte, Integer or Real.
%                   - The cell of strings is only use for information with the format Char.
%   - INFO.NUMDIMS: (desired) number of dimensions. All the dimension missing in the value are set to 1. 
%                   Note: The maximum number of dimensions is set to 7.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

nd = numel(size(values));
if (nargin < 3)
    numdims = nd;
elseif (numdims < nd)
    error('btk:MetaDataInfo', 'The given number of dimensions is lower than the number of dimensions for the given values.');
end

info.format = format;
info.values = values;
info.numdims = numdims;
