function md = btkFindMetaData(h,label,varargin)
%BTKFINDMETADATA Return the searched metadata or 0 if not found.
%
%  MD = BTKFINDMETADATA(H, LABEL) finds the metadata based on the given LABEL from 
%  the root of the acquistion's metadata.  The format of MD is the same than using 
%  the function <a href="matlab:help btkGetMetaData">btkGetMetaData</a>.  
%  The handle H represents an acquistion and is obtained by the use of a btk* function.
%
%  MD = BTKFINDMETADATA(H, LABEL, SUBLABEL) returns the metadata associated with the 
%  label SUBLABEL, child of the metadata with the label LABEL.
%
%  MD = BTKFINDMETADATA(H, LABEL, SUBLABEL, ...) finds the n-th sub-metadata.  There 
%  is not limit of depth.
%
%  By default, BTK, use two stages of metadata representing group and parameters.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

if (nargin < 2)
  error('At least two inputs are required.');
end
if (nargout > 1)
  error('Too many output arguments.');
end

root = btkGetMetaData(h);
if (isstruct(root.children))
    md = extractMetaData_p(root.children, label);
    for i=1:length(varargin)
        md = extractMetaData_p(md.children, varargin{i});
        if (~isstruct(md))
            break;
        end
    end
else
    md = 0;
end

function md = extractMetaData_p(parent, label)
md = 0;
labels = fieldnames(parent);
for i = 1:length(labels)
    if strcmp(label, labels{i})
        md = parent.(labels{i});
        break;
    end
end