function tocbtkcommon
% BTK Common            
% ----------
%
% Acquisition accessor
%   <a href="matlab:help btkGetAnalogFrameNumber">btkGetAnalogFrameNumber</a>     - Analog's frame number accessor
%   <a href="matlab:help btkGetAnalogFrequency">btkGetAnalogFrequency</a>       - Analog's frequency accessor
%   <a href="matlab:help btkGetAnalogNumber">btkGetAnalogNumber</a>          - Analog's number accessor
%   <a href="matlab:help btkGetAnalysis">btkGetAnalysis</a>              - ANALYSIS group extraction
%   <a href="matlab:help btkGetEvents">btkGetEvents</a>                - Events extraction
%   <a href="matlab:help btkGetFirstFrame">btkGetFirstFrame</a>            - Acquisition's first frame accessor
%   <a href="matlab:help btkGetMetaData">btkGetMetaData</a>              - Acquisition's metadata accessor	
%   <a href="matlab:help btkGetPointFrameNumber">btkGetPointFrameNumber</a>      - Point's frame number accessor
%   <a href="matlab:help btkGetPointFrequency">btkGetPointFrequency</a>        - Point's frequency accessor
%   <a href="matlab:help btkGetPointNumber">btkGetPointNumber</a>           - Point's number accessor
%
% Acquisition modifier
%   <a href="matlab:help btkAppendAnalysisParameter">btkAppendAnalysisParameter</a>  - Append or set a paremeter into the analysis
%   <a href="matlab:help btkClearAnalysis">btkClearAnalysis</a>            - Clear the metadata ANALYSIS
%   <a href="matlab:help btkRemoveAnalysisParameter">btkRemoveAnalysisParameter</a>  - Erase a paremeter from the analysis
%   <a href="matlab:help btkSetFirstFrame">btkSetFirstFrame</a>            - Acquisition's first frame modifier
%   <a href="matlab:help btkSetPointFrameNumber">btkSetPointFrameNumber</a>      - Point's frame number modifier
%   <a href="matlab:help btkSetPointNumber">btkSetPointNumber</a>           - Point's number modifier
%
% Points accessor
%   <a href="matlab:help btkGetAngles">btkGetAngles</a>                - Angles extraction
%   <a href="matlab:help btkGetAnglesValues">btkGetAnglesValues</a>          - Extracts angles as a simple matrix 
%   <a href="matlab:help btkGetForces">btkGetForces</a>                - Forces extraction
%   <a href="matlab:help btkGetForcesValues">btkGetForcesValues</a>          - Extracts forces as a simple matrix 
%   <a href="matlab:help btkGetMarkers">btkGetMarkers</a>               - Markers extraction
%   <a href="matlab:help btkGetMarkersValues">btkGetMarkersValues</a>         - Extracts markers as a simple matrix 
%   <a href="matlab:help btkGetMoments">btkGetMoments</a>               - Moments extraction
%   <a href="matlab:help btkGetMomentsValues">btkGetMomentsValues</a>         - Extracts moments as a simple matrix 
%   <a href="matlab:help btkGetPoints">btkGetPoints</a>                - Points extraction
%   <a href="matlab:help btkGetPowers">btkGetPowers</a>                - Powers extraction
%   <a href="matlab:help btkGetPowersValues">btkGetPowersValues</a>          - Extracts powers as a simple matrix 
%   <a href="matlab:help btkGetScalars">btkGetScalars</a>               - Scalars extraction
%   <a href="matlab:help btkGetScalarsValues">btkGetScalarsValues</a>         - Extracts scalars as a simple matrix 
%
% Point modifier
%   <a href="matlab:help btkAppendPoint">btkAppendPoint</a>              - Append point to an acquisition
%   <a href="matlab:help btkRemovePoint">btkRemovePoint</a>              - Delete point from an acquisition
%   <a href="matlab:help btkSetPoint">btkSetPoint</a>                 - Point's data modifier (values, residuals, masks)
%   <a href="matlab:help btkSetPointDescription">btkSetPointDescription</a>      - Point's description modifier
%   <a href="matlab:help btkSetPointLabel">btkSetPointLabel</a>            - Point's label modifier
%   <a href="matlab:help btkSetPointType">btkSetPointType</a>             - Point's type modifier
%   <a href="matlab:help btkSetAnglesValues">btkSetAnglesValues</a>          - Sets scalars's value from a matrix
%   <a href="matlab:help btkSetForcesValues">btkSetForcesValues</a>          - Sets forces's value from a matrix
%   <a href="matlab:help btkSetMomentsValues">btkSetMomentsValues</a>         - Sets moments's value from a matrix
%   <a href="matlab:help btkSetPowersValues">btkSetPowersValues</a>          - Sets power's value from a matrix
%   <a href="matlab:help btkSetMarkersValues">btkSetMarkersValues</a>         - Sets markers's value from a matrix
%   <a href="matlab:help btkSetScalarsValues">btkSetScalarsValues</a>         - Sets scalars's value from a matrix 
%
% Analogs accessor
%   <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>               - Analog channels extraction
%   <a href="matlab:help btkGetAnalogsValues">btkGetAnalogsValues</a>         - Extracts analogs channels' values in a simple matrix 
%   <a href="matlab:help btkGetForcePlatforms">btkGetForcePlatforms</a>        - Force platform channels and configuration extraction
%
% Analog mutator
%   <a href="matlab:help btkSetAnalogsValues">btkSetAnalogsValues</a>         - Sets analog's value from a matrix 
%
% <a href="matlab:help btk">Biomechanical Toolkit TOC</a>

% Author(s): A. Barré
% Copyright 2009 Biomechanical ToolKit

% [EOF]

