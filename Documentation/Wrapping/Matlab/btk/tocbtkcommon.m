function tocbtkcommon
% BTK Common            
% ----------
%
% Acquisition accessor
%   <a href="matlab:help btkFindMetaData">btkFindMetaData</a>                   - Find a metadata
%   <a href="matlab:help btkGetAnalogFrameNumber">btkGetAnalogFrameNumber</a>           - Analog's frame number accessor
%   <a href="matlab:help btkGetAnalogFrequency">btkGetAnalogFrequency</a>             - Analog's frequency accessor
%   <a href="matlab:help btkGetAnalogNumber">btkGetAnalogNumber</a>                - Analog's number accessor
%   <a href="matlab:help btkGetAnalogSampleNumberPerFrame">btkGetAnalogSampleNumberPerFrame</a>  - Number of analog samples per video frame
%   <a href="matlab:help btkGetAnalysis">btkGetAnalysis</a>                    - ANALYSIS group extraction
%   <a href="matlab:help btkGetFirstFrame">btkGetFirstFrame</a>                  - Acquisition's first frame accessor
%   <a href="matlab:help btkGetLastFrame">btkGetLastFrame</a>                   - Acquisition's last frame accessor
%   <a href="matlab:help btkGetMaxInterpolationGap">btkGetMaxInterpolationGap</a>         - Acquisition's interpolation gap accessor
%   <a href="matlab:help btkGetMetaData">btkGetMetaData</a>                    - Acquisition's metadata accessor
%   <a href="matlab:help btkGetPointFrameNumber">btkGetPointFrameNumber</a>            - Point's frame number accessor
%   <a href="matlab:help btkGetPointFrequency">btkGetPointFrequency</a>              - Point's frequency accessor
%   <a href="matlab:help btkGetPointNumber">btkGetPointNumber</a>                 - Point's number accessor
%
% Acquisition modifier
%   <a href="matlab:help btkAppendAnalysisParameter">btkAppendAnalysisParameter</a>        - Append or set a paremeter into the analysis
%   <a href="matlab:help btkAppendMetadata">btkAppendMetadata</a>                 - Append a metadata in the acquisition
%   <a href="matlab:help btkClearMetadata">btkClearMetadata</a>                  - Clear metadata in the acquisition
%   <a href="matlab:help btkCloneAcquisition">btkCloneAcquisition</a>               - Clone an acquisition
%   <a href="matlab:help btkClearAnalysis">btkClearAnalysis</a>                  - Clear the metadata ANALYSIS
%   <a href="matlab:help btkCropAcquisition">btkCropAcquisition</a>                - Crop the acquistion and adapt the events' frame/time.
%   <a href="matlab:help btkDeleteAcquisition">btkDeleteAcquisition</a>              - Delete acquisition handle and release used memory
%   <a href="matlab:help btkNewAcquisition">btkNewAcquisition</a>                 - Create an empty acquisition
%   <a href="matlab:help btkRemoveAnalysisParameter">btkRemoveAnalysisParameter</a>        - Erase a paremeter from the analysis
%   <a href="matlab:help btkRemoveMetaData">btkRemoveMetaData</a>                 - Remove a metadata from the acquisition
%   <a href="matlab:help btkSetFirstFrame">btkSetFirstFrame</a>                  - Acquisition's first frame modifier
%   <a href="matlab:help btkSetFrameNumber">btkSetFrameNumber</a>                 - Point and analog frame number modifier
%   <a href="matlab:help btkSetFrequency">btkSetFrequency</a>                   - Set the main frequency
%   <a href="matlab:help btkSetMaxInterpolationGap">btkSetMaxInterpolationGap</a>         - Acquisition's interpolation gap modifier
%   <a href="matlab:help btkSetMetaData">btkSetMetaData</a>                    - Metadata modifier
%   <a href="matlab:help btkSetMetaDataDescription">btkSetMetaDataDescription</a>         - Metadata's description modifier
%   <a href="matlab:help btkSetMetaDataDimensions">btkSetMetaDataDimensions</a>          - Metadata's dimensions modifier
%   <a href="matlab:help btkSetMetaDataFormat">btkSetMetaDataFormat</a>              - Metadata's format modifier
%   <a href="matlab:help btkSetMetaDataLabel">btkSetMetaDataLabel</a>               - Metadata's label modifier
%   <a href="matlab:help btkSetMetaDataUnlock">btkSetMetaDataUnlock</a>              - Metadata's unlock state modifier
%   <a href="matlab:help btkSetMetaDataValue">btkSetMetaDataValue</a>               - Modify one metadata's value
%   <a href="matlab:help btkSetPointNumber">btkSetPointNumber</a>                 - Point's number modifier
%
% Point accessor
%   <a href="matlab:help btkGetPoint">btkGetPoint</a>                       - Point extraction
%   <a href="matlab:help btkGetPointsValues">btkGetPointsValues</a>                - Points' values accessor
%   <a href="matlab:help btkGetPointsResiduals">btkGetPointsResiduals</a>             - Points' residuals accessor
%   <a href="matlab:help btkGetAngles">btkGetAngles</a>                      - Angles extraction
%   <a href="matlab:help btkGetAnglesValues">btkGetAnglesValues</a>                - Extracts angles as a simple matrix 
%   <a href="matlab:help btkGetForces">btkGetForces</a>                      - Forces extraction
%   <a href="matlab:help btkGetForcesValues">btkGetForcesValues</a>                - Extracts forces as a simple matrix 
%   <a href="matlab:help btkGetMarkers">btkGetMarkers</a>                     - Markers extraction
%   <a href="matlab:help btkGetMarkersValues">btkGetMarkersValues</a>               - Extracts markers as a simple matrix 
%   <a href="matlab:help btkGetMarkersResiduals">btkGetMarkersResiduals</a>           - Extracts markers' residuals as a simple matrix 
%   <a href="matlab:help btkGetMoments">btkGetMoments</a>                     - Moments extraction
%   <a href="matlab:help btkGetMomentsValues">btkGetMomentsValues</a>               - Extracts moments as a simple matrix 
%   <a href="matlab:help btkGetPoints">btkGetPoints</a>                      - Points extraction
%   <a href="matlab:help btkGetPowers">btkGetPowers</a>                      - Powers extraction
%   <a href="matlab:help btkGetPowersValues">btkGetPowersValues</a>                - Extracts powers as a simple matrix 
%   <a href="matlab:help btkGetScalars">btkGetScalars</a>                     - Scalars extraction
%   <a href="matlab:help btkGetScalarsValues">btkGetScalarsValues</a>               - Extracts scalars as a simple matrix 
%
% Point modifier
%   <a href="matlab:help btkAppendPoint">btkAppendPoint</a>                    - Append a point into an acquisition
%   <a href="matlab:help btkClearPoints">btkClearPoints</a>                    - Remove all points from an acquisition
%   <a href="matlab:help btkRemovePoint">btkRemovePoint</a>                    - Delete a point from an acquisition
%   <a href="matlab:help btkSetPoint">btkSetPoint</a>                       - Point's data modifier (values, residuals)
%   <a href="matlab:help btkSetPointDescription">btkSetPointDescription</a>            - Point's description modifier
%   <a href="matlab:help btkSetPointLabel">btkSetPointLabel</a>                  - Point's label modifier
%   <a href="matlab:help btkSetPointResiduals">btkSetPointResiduals</a>              - Point's residuals modifier
%   <a href="matlab:help btkSetPointType">btkSetPointType</a>                   - Point's type modifier
%   <a href="matlab:help btkSetPointsUnit">btkSetPointsUnit</a>                  - Set unit for a type of point
%   <a href="matlab:help btkSetPointValues">btkSetPointValues</a>                 - Point's values modifier
%   <a href="matlab:help btkSetPointsValues">btkSetPointsValues</a>                - Set points's values from a matrix
%   <a href="matlab:help btkSetPointsResiduals">btkSetPointsResiduals</a>             - Set points's residuals from a matrix
%   <a href="matlab:help btkSetAnglesValues">btkSetAnglesValues</a>                - Set scalars's values from a matrix
%   <a href="matlab:help btkSetForcesValues">btkSetForcesValues</a>                - Set forces's values from a matrix
%   <a href="matlab:help btkSetMomentsValues">btkSetMomentsValues</a>               - Set moments's values from a matrix
%   <a href="matlab:help btkSetPowersValues">btkSetPowersValues</a>                - Set power's values from a matrix
%   <a href="matlab:help btkSetMarkersValues">btkSetMarkersValues</a>               - Set markers's values from a matrix
%   <a href="matlab:help btkSetMarkersResiduals">btkSetMarkersResiduals</a>            - Set markers's residuals from a matrix
%   <a href="matlab:help btkSetScalarsValues">btkSetScalarsValues</a>               - Set scalars's values from a matrix
%
% Analog accessor
%   <a href="matlab:help btkGetAnalog">btkGetAnalog</a>                      - Analog channel extraction
%   <a href="matlab:help btkGetAnalogs">btkGetAnalogs</a>                     - Analog channels extraction
%   <a href="matlab:help btkGetAnalogsResolution">btkGetAnalogsResolution</a>           - Return ADC resolution
%   <a href="matlab:help btkGetAnalogsValues">btkGetAnalogsValues</a>               - Extract analogs channels' values in a simple matrix 
%   <a href="matlab:help btkGetForcePlatforms">btkGetForcePlatforms</a>              - Force platform channels and configuration extraction
%
% Analog modifier
%   <a href="matlab:help btkAppendAnalog">btkAppendAnalog</a>                   - Append an analog channel into an acquisition
%   <a href="matlab:help btkClearAnalogs">btkClearAnalogs</a>                   - Remove all analog channels from an acquisition
%   <a href="matlab:help btkRemoveAnalog">btkRemoveAnalog</a>                   - Delete an analog channel from an acquisition
%   <a href="matlab:help btkSetAnalogDescription">btkSetAnalogDescription</a>           - Set analog's description
%   <a href="matlab:help btkSetAnalogGain">btkSetAnalogGain</a>                  - Set analog's gain
%   <a href="matlab:help btkSetAnalogLabel">btkSetAnalogLabel</a>                 - Set analog's label
%   <a href="matlab:help btkSetAnalogOffset">btkSetAnalogOffset</a>                - Set analog's offset
%   <a href="matlab:help btkSetAnalogScale">btkSetAnalogScale</a>                 - Set analog's scale factor
%   <a href="matlab:help btkSetAnalogsResolution">btkSetAnalogsResolution</a>           - Set ADC resolution (8, 12, 14 or 16 bits)
%   <a href="matlab:help btkSetAnalogSampleNumberPerFrame">btkSetAnalogSampleNumberPerFrame</a>  - Set analog sample per 3D frame
%   <a href="matlab:help btkSetAnalogUnit">btkSetAnalogUnit</a>                  - Set analog's unit
%   <a href="matlab:help btkSetAnalogsValues">btkSetAnalogValues</a>                - Set analog's values from a vector
%   <a href="matlab:help btkSetAnalogsValues">btkSetAnalogsValues</a>               - Set analogs' values from a matrix 
%
% Event accessor
%   <a href="matlab:help btkGetEventNumber">btkGetEventNumber</a>                 - Number of events
%   <a href="matlab:help btkGetEvents">btkGetEvents</a>                      - Events extraction
%   <a href="matlab:help btkGetEventsValues">btkGetEventsValues</a>                - Events values (time, label, id, description)
%
% Event modifier
%   <a href="matlab:help btkAppendEvent">btkAppendEvent</a>                    - Append event into an acquisition
%   <a href="matlab:help btkClearEvents">btkClearEvents</a>                    - Remove all events from an acquisition
%   <a href="matlab:help btkRemoveEvent">btkRemoveEvent</a>                    - Remove event from an acquisition
%   <a href="matlab:help btkSetEventDescription">btkSetEventDescription</a>            - Set event's description
%   <a href="matlab:help btkSetEventLabel">btkSetEventLabel</a>                  - Set event's label
%   <a href="matlab:help btkSetEventId">btkSetEventId</a>                     - Set event's ID
%   <a href="matlab:help btkSetEventSubject">btkSetEventSubject</a>                - Set event's subject
%   <a href="matlab:help btkSetEventTime">btkSetEventTime</a>                   - Set event's time
%
% <a href="matlab:help btk">Biomechanical Toolkit TOC</a>

% Author(s): A. Barré
% Copyright 2009-2013 Biomechanical ToolKit

% [EOF]