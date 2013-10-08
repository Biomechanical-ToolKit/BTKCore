function btkAppendForcePlatformType2(h, forces, moments, corners, origin, localFrame)
%BTKAPPENDFORCEPLATFORMTYPE2 Append a virtual force platform of type 2.
%
%  BTKAPPENDFORCEPLATFORMTYPE2(H, FORCES, MOMENTS, CORNERS) creates a
%  virtual force platform based only on the reaction FORCES and MOMENTS
%  expressed in the global frame. This function updates all the metadata in
%  the group FORCE_PLATFORM and append 6 analog channels.
%  NOTE: The moment must be expressed at the (surface) origin of the plate.
%  The CORNERS is a matrix (4,3) which contains the coordinates of the corners
%  expressed in the global frame.  They are are numbered from 1 to 4 and refer
%  to the quadrant numbers in the X-Y plane of the force platform coordinate 
%  system (not the 3D point reference coordinate system).  These are +x+y, 
%  -x+y, -x-y, and +x-y, with respect to the force plate coordinate system.
%  The ORIGIN of the platform is set by default to [0,0,0] meaning that it has
%  no thickness.  If your forces and moments come from raw data recorded by a 
%  force platform (and then recorded to the location of the sensor), it is
%  strongly advised to use the sensor distances given in the data sheet.
%  You can retrieve easily the data of the new force platform by using one
%  of the following functions: 
%  - BTKGETFORCEPLATFORMS
%  - BTKGETFORCEPLATFORMWRENCHES
%  - BTKGETGROUNDREACTIONWRENCHES
%
%  BTKAPPENDFORCEPLATFORMTYPE2(H, FORCES, MOMENTS, CORNERS, ORIGIN) sets
%  also the ORIGIN of the force platform compare to its surface' center. 
%  The ORIGIN must be expressed in the frame of the force plate.  Thus the 
%  verical component should be negative as the surface is higher than the 
%  origin.  Setting the ORIGIN to an empty matrix has the same effect than 
%  giving an origin equals to [0,0,0].
%
%  BTKAPPENDFORCEPLATFORMTYPE2(H, FORCES, MOMENTS, CORNERS, ORIGIN, LOCALFRAME)
%  creates a virtual force platform using FORCES and MOMENTS expressed in the 
%  local frame if LOCALFRAME is not set to 0. The content of CORNERS and ORIGIN
%  still must be expressed in the global frame. The use of LOCALFRAME can be useful
%  if you use data recorded directly from the force platform.

%  History
%  -------
%   - 2011/11/21: Initial version
%   - 2011/11/22: Forces and moments must be given in the global frame
%   - 2012/03/06: New option 'local' to give the forces and moment in the 
%                 force platform (local) frame
%   - 2012/04/19: Automatic check if the force and moment correspond to the
%                 number of the frame for the points or the analog channels
%   - 2012/08/14: Option 'quiet' removed an replaced by Matlab warning() function
%                 combined with the btk:AppendForcePlatformType2 identifier
%   - 2013/02/03: Analog's unit added based on acquisition's configuration (btkGetPointUnit).
%   - 2013/03/22: Computation of adapted scale factor and offset to reduce the problem of 
%                 alisasing in some case. Fake Gain added (+/-10V).
 

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

if (nargin < 4)
    error('Missing input arguments');
end
if ((nargin < 5) || (isempty(origin)))
    origin = [0;0;0];
end
if ((nargin < 6))
    localFrame = 0;
end

numPointFrames = btkGetPointFrameNumber(h);
numAnalogFrames = btkGetAnalogFrameNumber(h);
numAnalogs = btkGetAnalogNumber(h);

% Check the size of the forces
if ((size(forces,1) ~= numPointFrames) && (size(forces,1) ~= numAnalogFrames))
    error('The number of frames for the forces doesn''t correspond to the number of frames in the acquisition.');
end
if (size(forces,2) ~= 3)
    error('The number of components for the forces must be equal to 3.');
end
% Check the size of the moments
if (size(moments,1) ~= size(forces,1))
    error('The number of frames for the moments doesn''t correspond to the number of frames for the given forces.');
end
if (size(moments,2) ~= 3)
    error('The number of components for the moments must be equal to 3.');
end
% Check the size of the corners
if (size(corners,1) ~= 4)
    error('The number of corners must be equal to 4.');
end
if (size(corners,2) ~= 3)
    error('The number of components for the corners must be equal to 3.');
end
% Check the size of the orgin
if (length(origin) ~= 3)
    error('The number of components for the origin must be equal to 3.');
end

% Check if the vertical component of the origin is negative and adapt the
% origin in consequence if it is not the case
if (origin(3) > 0)
    warning('btk:AppendForcePlatformType2', 'The origin must be expressed from the physical origin to the surface one. The opposite of the origin is used.');
    origin = origin * -1;
end

if (localFrame == 0)
    % Compute the moment around the physical origin of the platform
    moments(:,1) = moments(:,1) - (forces(:,2) * origin(3) - origin(2) * forces(:,3));
    moments(:,2) = moments(:,2) - (forces(:,3) * origin(1) - origin(3) * forces(:,1));
    moments(:,3) = moments(:,3) - (forces(:,1) * origin(2) - origin(1) * forces(:,2));
    % Transform the forces and moments to the hardware origin of the platform
    % and expressed them in its frame.
    R = zeros(3);
    R(:,1) = corners(1,:) - corners(2,:); R(:,1) = R(:,1) / norm(R(:,1));
    R(:,3) = cross(R(:,1)', corners(1,:) - corners(4,:)); R(:,3) = R(:,3) / norm(R(:,3)); 
    R(:,2) = cross(R(:,3), R(:,1));
    forces = forces * R;
    moments = moments * R;
end

% Check for the analog sample frequency
ratio = btkGetAnalogSampleNumberPerFrame(h);
if ((ratio ~= 1) && (size(forces,1) == numPointFrames))
    warning('btk:AppendForcePlatformType2', 'As the sample frequency of the analog channels is not the same than the cameras, the force and moments are interpolated by a linear method.');
    forces = interp1(1:numPointFrames, forces, 1:1/ratio:numPointFrames, 'linear');
    moments = interp1(1:numPointFrames, moments, 1:1/ratio:numPointFrames, 'linear');
    % The frames after the last video frame are filled with the value 0
    forces = [forces ; zeros(ratio-1,3)];
    moments = [moments ; zeros(ratio-1,3)];
end

% Check if the required metadata are in the given acquisition
used = 1;
type = 2;
zero = [0,0]; % No baseline by default
md = btkFindMetaData(h, 'FORCE_PLATFORM');
channel = numAnalogs + [1 2 3 4 5 6];
numChannelsPerPF = 6;
corners = corners'; % Coordinates sorted by column order instead of row order.
if (isstruct(md))
    if (isfield(md.children, 'USED'))
        used = md.children.USED.info.values(1) + 1;
        type = zeros(used,1);
        corners_ = zeros(3,4,used);
        origin_ = zeros(3,used);
    end
    if (isfield(md.children,'TYPE'))
        temp = md.children.TYPE.info.values;
        type(1:numel(temp)) = temp;
        type(used) = 2;
        for i=1:used-1
            switch (type(i))
                case {1, 2, 4, 21}
                    if (numChannelsPerPF < 6)
                        numChannelsPerPF = 6;
                    end
                case {3, 5, 7, 11, 12}
                    if (numChannelsPerPF < 8)
                        numChannelsPerPF = 8;
                    end
                case 6,
                    if (numChannelsPerPF < 12)
                        numChannelsPerPF = 12;
                    end
            end
        end
        channel = zeros(numChannelsPerPF, used);
    end
    if (isfield(md.children,'ZERO'))
        zero = md.children.ZERO.info.values;
    end
    if (isfield(md.children,'CORNERS'))
        temp = md.children.CORNERS.info.values;
        corners_(1:numel(temp)) = temp;
        corners_((used-1)*4*3+1:end) = corners(:);
        corners = corners_;
    end
    if (isfield(md.children,'ORIGIN'))
        temp = md.children.ORIGIN.info.values;
        origin_(1:numel(temp)) = temp;
        origin_((used-1)*3+1:end) = origin;
        origin = origin_;
    end
    if (isfield(md.children,'CHANNEL'))
        temp = md.children.CHANNEL.info.values;
        channel(1:numel(temp)) = temp(:);
        channel(numChannelsPerPF * (used-1)+1 : numChannelsPerPF * (used-1)+6) = numAnalogs + [1 2 3 4 5 6];
    end
    % No need to update the CAL_MATRIX metadata
end

offset = 0;
% Set the offset for the virtual analog channels
md = btkFindMetaData(h, 'ANALOG', 'FORMAT');
res = btkGetAnalogResolution(h);
if (isstruct(md))
    if (strcmpi(md.info.values{1},'SIGNED'))
        offset = 0;
    elseif (strcmpi(md.info.values{1},'UNSIGNED'))
        offset = 2^(res-1);
    else
        error('btk:AppendForcePlatformType2', ['Unknown ADC format: ', md.info.values{1}]);
    end
else
  % Assume this is a ADC signed 12-bit card
  offset = 0;
end
% Compute fake scale factors;
scales = zeros(6,1);
for i=1:3
    scales(i) = btkComputeScaleFactor_p(forces(:,i), res);
end
for i=1:3
    scales(i+3) = btkComputeScaleFactor_p(moments(:,i), res);
end
% The gain is also set to ID 1; +/- 10 volts
% Append the (fake) analog channels
str = num2str(used);
% - Forces
unit = btkGetPointsUnit(h, 'Force');
btkAppendAnalog(h, ['Fx',str], forces(:,1), 'Fake analog channel for virtual force platform component Fx');
btkSetAnalogUnit(h, numAnalogs+1, unit);
btkSetAnalogOffset(h, numAnalogs+1, offset);
btkSetAnalogGain(h, numAnalogs+1, 1);
btkSetAnalogScale(h, numAnalogs+1, scales(1));
btkAppendAnalog(h, ['Fy',str], forces(:,2), 'Fake analog channel for virtual force platform component Fy');
btkSetAnalogUnit(h, numAnalogs+2, unit);
btkSetAnalogOffset(h, numAnalogs+2, offset);
btkSetAnalogGain(h, numAnalogs+2, 1);
btkSetAnalogScale(h, numAnalogs+2, scales(2));
btkAppendAnalog(h, ['Fz',str], forces(:,3), 'Fake analog channel for virtual force platform component Fz');
btkSetAnalogUnit(h, numAnalogs+3, unit);
btkSetAnalogOffset(h, numAnalogs+3, offset);
btkSetAnalogGain(h, numAnalogs+3, 1);
btkSetAnalogScale(h, numAnalogs+3, scales(3));
% - Moments
unit = btkGetPointsUnit(h, 'Moment');
btkAppendAnalog(h, ['Mx',str], moments(:,1), 'Fake analog channel for virtual force platform component Mx');
btkSetAnalogUnit(h, numAnalogs+4, unit);
btkSetAnalogOffset(h, numAnalogs+4, offset);
btkSetAnalogGain(h, numAnalogs+4, 1);
btkSetAnalogScale(h, numAnalogs+4, scales(4));
btkAppendAnalog(h, ['My',str], moments(:,2), 'Fake analog channel for virtual force platform component My');
btkSetAnalogUnit(h, numAnalogs+5, unit);
btkSetAnalogOffset(h, numAnalogs+5, offset);
btkSetAnalogGain(h, numAnalogs+5, 1);
btkSetAnalogScale(h, numAnalogs+5, scales(5));
btkAppendAnalog(h, ['Mz',str], moments(:,3), 'Fake analog channel for virtual force platform component Mz');
btkSetAnalogUnit(h, numAnalogs+6, unit);
btkSetAnalogOffset(h, numAnalogs+6, offset);
btkSetAnalogGain(h, numAnalogs+6, 1);
btkSetAnalogScale(h, numAnalogs+6, scales(6));
% Update the metadata
btkAppendMetaData(h, 'FORCE_PLATFORM', 'USED', btkMetaDataInfo('Integer', used));
btkAppendMetaData(h, 'FORCE_PLATFORM', 'TYPE', btkMetaDataInfo('Integer', type));
btkAppendMetaData(h, 'FORCE_PLATFORM', 'ZERO', btkMetaDataInfo('Integer', zero));
btkAppendMetaData(h, 'FORCE_PLATFORM', 'CORNERS', btkMetaDataInfo('Real', corners));
btkAppendMetaData(h, 'FORCE_PLATFORM', 'ORIGIN', btkMetaDataInfo('Real', origin));
btkAppendMetaData(h, 'FORCE_PLATFORM', 'CHANNEL', btkMetaDataInfo('Integer', channel));

function s = btkComputeScaleFactor_p(v, res)
  s = max(abs(v));
  nd = ceil(log10(s));
  if (nd == 0)
      nd = 1;
  end
  if (s ~= 0)
      s = 1.25 * ceil(s / 10^(nd-1)) * 10^(nd-1) / (2^(res-1));
      % s = 10^nd / (2^(res-1));
      if (any(v < 0))
          s = -1 * s;
      end
  else
      s = 1;
  end