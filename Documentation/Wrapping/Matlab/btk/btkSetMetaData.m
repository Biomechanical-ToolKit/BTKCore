function md = btkSetMetaData(h, varargin) %#ok
%BTKSETMETADATA  Set an existing metada and return an updated list of the metadata in the acquisition.
%
%  In case you are not sure the metadata to set does exist you can use instead
%  the method BTKAPPENDMETADATA. This last method is able to set new information if
%  the metadata exists or will create a new one.
%
%  BTKSETMETADATA(H, LABEL, SUBLABEL, ..., INFO) sets a metadata with additional 
%  informations INFO.  This additionnal information is a structure:
%   - INFO.FORMAT: contains a string (Char, Byte, Integer or Real) with the format of the information
%   - INFO.VALUES: contains an array of numerical values or a cell of strings.
%                  - The array is used for information with the format Byte, Integer or Real.
%                  - The cell of strings is only use for information with the format Char.
%  The stucture INFO should be constructed with the use of the function <a href="matlab:help btkMetaDataInfo">btkMetaDataInfo</a>.  
%
%  This function can also returns an updated list of metadata.
%  MD = BTKSETMETADATA(H, LABEL, SUBLABEL, ..., INFO)
%  The format of MD is the same than using the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

if (nargin < 3)
    error('At least three inputs are required.');
end
if (nargout > 1)
    error('Too many output arguments.');
end

if (~isstruct(btkFindMetaData(h, varargin{1:end-1})))
  name = varargin{1};
  for i = 2:length(varargin)-1
    name = [name, ':', varargin{i}];
  end
  error(['The metadata ',name,' was not found.']);
end

md = btkAppendMetaData(h, varargin{:});

% [EOF] btkSetMetaData.m
