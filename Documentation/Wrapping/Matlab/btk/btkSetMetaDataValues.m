function md = btkSetMetaDataValues(h, varargin) %#ok
%BTKSETMETADATAVALUES  Set values of an existing metadata.
%
%  BTKSETMETADATAVALUES is only an alias for the function <a href="matlab:help btkSetMetaData">btkSetMetaData</a>.
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

md = btkSetMetaData(h, varargin{:});

% [EOF] btkSetMetaDataValues.m
