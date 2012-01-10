function d = btkConvertBinaryMasks2Decimals(b)
%BTKCONVERTBINARYMASKS2DECIMALS Convert binary values into decimals.
% 
%  D = BTKCONVERTBINARYMASKS2DECIMALS(B) converts cell of strings into a column vector 
%  of decimal values using the inverse order than BIN2DEC.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

s = char(b);
[m,n] = size(s);
v = s - '0'; 
twos = pow2(0:1:n-1);
d = sum(v .* twos(ones(m,1),:),2);