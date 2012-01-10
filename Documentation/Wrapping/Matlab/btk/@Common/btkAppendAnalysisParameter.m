function btkAppendAnalysisParameter(h, name, context, unit, value, subject, description) %#ok
%BTKAPPENDANALYSISPARAMETER Insert or set a parameter in the analysis. 
% 
%  BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE) insert or set a 
%  parameter designed by its NAME and CONTEXT.  The NAME, CONTEXT and UNIT are string,
%  and VALUE is a real scalar value.  The SUBJECT and DESCRIPTION are empty
%
%  BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE, SUBJECT) insert or 
%  set a parameter with a non empty SUBJECT.
%
%  BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE, SUBJECT, DESCRIPTION)
%  add a description to the inserted parameter.
%
%  This function can also returns an updated list of parameter.
%  ANALYSIS = BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE)
%  ANALYSIS = BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE, SUBJECT)
%  ANALYSIS = BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE, SUBJECT, DESCRIPTION)
%  The format of ANALYSIS is the same than using the function <a href="matlab:help btkGetAnalysis">btkGetAnalysis</a>
%
%  This function can also returns an updated list of analysis parameters' informations.
%  [ANALYSIS, ANALYSISINFO] = BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE)
%  [ANALYSIS, ANALYSISINFO] = BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE, SUBJECT)
%  [ANALYSIS, ANALYSISINFO] = BTKAPPENDANALYSISPARAMETER(H, NAME, CONTEXT, UNIT, VALUE, SUBJECT, DESCRIPTION)
%  The format of ANALYSISINFO is the same than using the function <a href="matlab:help btkGetAnalysis">btkGetAnalysis</a>
%
%  The acquisition is represented by the handle H.  This handle is obtained
%  by the use of a btk* function.

%  Author: A. Barré
%  Copyright 2009-2012 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKAPPENDANALYSISPARAMETER not found');

% [EOF] btkAppendAnalysisParameter.m
