function btkRemoveAnalysisParameter(h, idx_or_context, name) %#ok
%BTKREMOVEANALYSISPARAMETER Erase in the metadata ANALYSIS the parameter the given index or context/name. 
% 
%  BTKREMOVEANALYSISPARAMETER(H, INDEX) erases the parameter at the given index.
%
%  The parameter to erase can also be selected by its CONTEXT/NAME.
%  BTKREMOVEANALYSISPARAMETER(H, CONTEXT, NAME)
%
%  This function can also returns an updated list of parameter.
%  ANALYSIS = BTKREMOVEANALYSISPARAMETER(H, INDEX)
%  ANALYSIS = BTKREMOVEANALYSISPARAMETER(H, CONTEXT, NAME)
%  The format of ANALYSIS is the same than using the function <a href="matlab:help btkGetAnalysis">btkGetAnalysis</a>
%
%  This function can also returns an updated list of analysis parameters' informations.
%  [ANALYSIS, ANALYSISINFO] = BTKREMOVEANALYSISPARAMETER(H, INDEX)
%  [ANALYSIS, ANALYSISINFO] = BTKREMOVEANALYSISPARAMETER(H, CONTEXT, NAME)
%  The format of ANALYSISINFO is the same than using the function <a href="matlab:help btkGetAnalysis">btkGetAnalysis</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKREMOVEANALYSISPARAMETER not found');

% [EOF] btkRemoveAnalysisParameter.m
