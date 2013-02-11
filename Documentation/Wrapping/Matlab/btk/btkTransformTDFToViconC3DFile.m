function btkTransformTDFToViconC3DFile(TDF_INPUT, C3D_OUTPUT)
%BTKTRANSFORMTDFTOVICONC3DFILE transforms a TDF file (BTS Bioengineering) to a compatible Vicon C3D file.
%  
%  Several modifications are done in the data file:
%   - First the data are transformed in millimeters and Newton-millimeters.
%   - The coordinates of the markers and the geometry of the force 
%     plateforms are also modified to have the Z axis as the vertical axis.
%     These modifications correspond to a rotation of 90 degrees around 
%     the X axis.
%   - The force plates are converted from type I (FX/FY/FZ/PX/PY/MZ) to 
%     type II (FX/FY/FZ/MX/MY/MZ). 
%   - Finally, the analog channels used by the original force plates are
%     deleted.
%
%  BTKTRANSFORMTDFTOVICONC3DFILE(TDF_INPUT) transforms the file and write 
%  it in the same folder but the extension C3D is concatenated to the
%  filename.
%
%  BTKTRANSFORMTDFTOVICONC3DFILE(TDF_INPUT, C3D_OUTPUT) transforms the file
%  and write it in the given output filename. The given pathname (aka
%  folders) must exist. Be sure that the given output has a final C3D
%  extension. Otherwise, the ouput file could be in another format or raise
%  an exception if the extension is not recognized.

%  History
%  -------
%   - 2012/06/04: Initial version
%   - 2012/06/04: Clear only the analog channels used by the forceplates
%   - 2012/08/09: Check if the analog channels configured for each forceplates
%                 exists really before trying to remove them.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

if (nargin < 2)
    C3D_OUTPUT = [TDF_INPUT,'.c3d'];
end
if (nargin < 1)
    error('Missing input argument. You must give the path of the TDF file');
end

% Read the TDF file
warning('OFF', 'btk:ReadAcquisition');
acq = btkReadAcquisition(TDF_INPUT);
warning('ON', 'btk:ReadAcquisition');
% Convert the unit of the markers in millimeters (as Vicon does not read correctly values in meters: no scale)
% AND also transfert the coordinates Y and Z to have the Z axis as the
% vertical. This is like have a rotation of 90 degree around the axis X
data = btkGetMarkersValues(acq)*1000;
numMarkers = btkGetPointNumber(acq); % IT IS ASSUMED THAT THERE IS ONLY MARKERS IN THE ACQUISITION
idxCoordNew = zeros(3*numMarkers,1);
idxCoordOpp = zeros(numMarkers,1);
for i = 1:numMarkers
    idxCoordNew(((i-1)*3)+1:i*3) = [1 3 2] + (i-1)*3;
    idxCoordOpp(i) = (i-1)*3+2;
end
data = data(:,idxCoordNew);
data(:,idxCoordOpp) = data(:,idxCoordOpp) * -1;
btkSetMarkersValues(acq, data);
btkSetPointsUnit(acq, 'marker','mm');
% Extract force platform data (channels and geometry)
warning('OFF', 'btk:GetForcePlatforms');
fp = btkGetForcePlatforms(acq);
warning('OFF', 'btk:GetForcePlatforms');
% Extract force platform wrenches (force, moment, position) for each forceplate
warning('OFF', 'btk:GetForcePlatformWrenches');
fpw = btkGetForcePlatformWrenches(acq,0);
warning('OFF', 'btk:GetForcePlatformWrenches');
if (strcmp(btkGetVersion(), '0.1.10'))
    % BUG: Because, it is force platform Type I, the position is not set to the
    % origin, but measured to the COP. The moment must be corrected!
    % Will be corrected in the next version of BTK.
    for i = 1:length(fpw)
        fpw(i).M(:,1) = fpw(i).M(:,1) - (fpw(i).F(:,2) .* fpw(i).P(:,3) - fpw(i).P(:,2) .* fpw(i).F(:,3));
        fpw(i).M(:,2) = fpw(i).M(:,2) - (fpw(i).F(:,3) .* fpw(i).P(:,1) - fpw(i).P(:,3) .* fpw(i).F(:,1));
        fpw(i).M(:,3) = fpw(i).M(:,3) - (fpw(i).F(:,1) .* fpw(i).P(:,2) - fpw(i).P(:,1) .* fpw(i).F(:,2));
    end
end
% Transfer the coordinates Y and Z of the forceplates' geometry
% And suppress the analog channels used by the forceplates
analogLabels = fieldnames(btkGetAnalogs(acq));
for i = 1:length(fp)
    fp(i).corners([2,3],:) = fp(i).corners([3,2],:);
    fp(i).corners(2,:) = -1*fp(i).corners(2,:);
    fp(i).origin([2,3]) = fp(i).origin([3,2]);
    fp(i).origin(2) = -1*fp(i).origin(2);
    fpChannels = fieldnames(fp(i).channels);
    for j = 1:length(fpChannels)
        if (isfield(analogLabels, fpChannels{j}))
            btkRemoveAnalog(acq,fpChannels{j});
        end
    end
end
% Remove the metadata (for TDF file, it contains only force platforms configuration)
btkClearMetaData(acq);
% Create the new force plateforms
for i = 1:length(fpw)
    % Add a new force platform based on the data of the first force platform
    % WARNING: Moments are converted in newtons-millimeters!
    % WARNING: Geometry is converted in millimeters!
    btkAppendForcePlatformType2(acq,fpw(i).F,fpw(i).M*1000,fp(i).corners'*1000,fp(i).origin*1000,1);
end
% Set the coordinate axes for the screen (to have the good orientation of the ground in Mokka)
btkAppendMetaData(acq,'POINT','X_SCREEN',btkMetaDataInfo('Char',{'+X'}));
btkAppendMetaData(acq,'POINT','Y_SCREEN',btkMetaDataInfo('Char',{'+Z'}));
% Create the C3D file
btkWriteAcquisition(acq, C3D_OUTPUT);