function benchmarkC3DserverEmulation
d.filename = '../Data/Input/C3DSamples/sample01/Eb015pi.c3d';
num = 10;
fprintf('\n-----------------------------------------------------\n');
fprintf('      C3Dserver Emulation Benchmark - v0.6\n');
fprintf('-----------------------------------------------------\n\n');
times = {};
times(end+1,:) = runBench(num,d,'COM object creation',@benchCOM);
times(end+1,:) = runBench(num,d,'Open C3D File (whitout object creation)',@benchOpen);
times(end+1,:) = runBench(num,d,'Access C3D Header data',@benchGetHeader);
times(end+1,:) = runBench(num,d,'Access C3D Event data',@benchGetEvent);
times(end+1,:) = runBench(num,d,'Access C3D Parameter data',@benchGetParameter);
times(end+1,:) = runBench(num,d,'Access C3D Point values',@benchGetPointData);
times(end+1,:) = runBench(num,d,'Access C3D Point range (full)',@benchGetPointDataEx);
times(end+1,:) = runBench(num,d,'Access C3D Analog values',@benchGetAnalogData);
times(end+1,:) = runBench(num,d,'Access C3D Analog range (full)',@benchGetAnalogDataEx);

l = 53; % String size
t = 0;
r = 0;
timeFormatC3Dserver = '%5.3f';
timeFormatBTK = '%5.3f';
timeFormatRatio = '%5.3f';
for i = 1:size(times,1)
    if (times{i,2} > 10)
        timeFormatC3Dserver = '%6.3f';
    end
    if (times{i,3} > 10)
        timeFormatBTK = '%6.3f';
    end
    if (times{i,2}/times{i,3} > 10)
        timeFormatRatio = '%6.3f';
    end
end
timeStr = ['| ',timeFormatC3Dserver,' vs ',timeFormatBTK,' '];
ratioStr = ['| ',timeFormatRatio];
for i = 1:size(times,1)
    t = max([t, length(sprintf(timeStr,times{i,2},times{i,3}))]);
    r = max([r, length(sprintf(ratioStr,times{i,2}/times{i,3}))]);
end
tl = l - t - r - 1;
fprintf('-----------------------------------------------------\n');
fprintf('                       Summary\n');
fprintf('-----------------------------------------------------\n');
fprintf('  Mean times (s) (C3Dserver vs BTK) and their ratio\n');
fprintf('-----------------------------------------------------\n');
for i = 1:size(times,1)
    title = times{i,1};
    if (length(title) > tl)
        title = title(1:tl);
        title(end-2:end) = '...';
    end
    fprintf(['%-',num2str(tl),'s ',timeStr,ratioStr,'\n'],title,times{i,2},times{i,3},times{i,2}/times{i,3});
end
fprintf('\n');

function times = runBench(num, d, title, benchHandle)
c3dTt = 0;
btkTt = 0;
title_ = [repmat(' ',1,round((52 - length(title)) / 2)), title];
fprintf('%s\n', title_);
fprintf('-----------------------------------------------------\n');
for i = 1:num
    fprintf('Run %02i/%i: C3Dserver vs BTK: ...\n', i,num);
    c3dT = benchHandle(@()actxserver('C3Dserver.C3D'),d);
    btkT = benchHandle(@()btkEmulateC3Dserver, d);
    fprintf('\b\b\b\b%.3f vs %.3f second(s)\n', c3dT, btkT);
    c3dTt = c3dTt + c3dT;
    btkTt = btkTt + btkT;
end
fprintf('-----------------------------------------------------\n');
fprintf('Total time: C3Dserver vs BTK: %.3f vs %.3f second(s)\n', c3dTt, btkTt);
fprintf('Mean time: C3Dserver vs BTK: %.3f vs %.3f second(s)\n', c3dTt/num, btkTt/num);
fprintf('Ratio (C3Dserver / BTK): %f\n\n', c3dTt/btkTt);
times = {title, c3dTt/num, btkTt/num};

function t = benchCOM(itfHandle,d)
t0 = tic;
itf = itfHandle();
t = toc(t0);

function t = benchOpen(itfHandle,d)
itf = itfHandle();
t0 = tic;
itf.Open(d.filename,3);
t = toc(t0);

function t = benchGetHeader(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
itf.GetNumber3DPoints();
itf.GetAnalogChannels();
itf.GetVideoFrame(0);
itf.GetVideoFrame(1);
itf.GetHeaderScaleFactor();
itf.GetStartingRecord();
itf.GetAnalogVideoRatio();
itf.GetVideoFrameRate();
itf.GetFileType();
itf.GetDataType();
t = toc(t0);

function t = benchGetEvent(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
itf.GetNumberEvents();
for i=1:itf.GetNumberEvents()
    itf.GetEventTime(i-1);
    itf.GetEventStatus(i-1);
    itf.GetEventLabel(i-1);
end
t = toc(t0);

function t = benchGetParameter(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
for i=1:itf.GetNumberGroups()
    itf.GetGroupName(i-1);
    itf.GetGroupDescription(i-1);
    itf.GetGroupNumber(i-1);
    itf.GetGroupLock(i-1);
end
for i=1:itf.GetNumberParameters()
    itf.GetParameterName(i-1);
    itf.GetParameterDescription(i-1);
    itf.GetParameterNumber(i-1);
    itf.GetParameterLock(i-1);
    itf.GetParameterType(i-1);
    itf.GetParameterNumberDim(i-1);
    for j=1:itf.GetParameterLength(i-1); 
        itf.GetParameterValue(i-1, j-1);
    end; 
end
itf.Close();
t = toc(t0);

function t = benchGetPointData(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
% Generate random frame number
num = 10;
frames = itf.GetVideoFrame(0) + ceil(rand(1,num) * (itf.GetVideoFrame(1) - itf.GetVideoFrame(0)));
frames = [itf.GetVideoFrame(0), itf.GetVideoFrame(1), frames];
% Generate random coordinates
coords = floor(rand(1,num) * 3);
% Generate random point index
channels = floor(rand(1,num) * itf.GetNumber3DPoints());
for i=1:numel(frames)
    for j=1:numel(coords)
        for k=1:numel(channels)
            itf.GetPointData(channels(k),coords(j),frames(i),char(1));
        end
    end
end
itf.Close();
t = toc(t0);

function t = benchGetPointDataEx(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
num = 10;
% Generate random coordinates
coords = floor(rand(1,num) * 3);
% Generate random point index
channels = floor(rand(1,num) * itf.GetNumber3DPoints());
for j=1:numel(coords)
    for k=1:numel(channels)
        itf.GetPointDataEx(channels(k),coords(j),itf.GetVideoFrame(0),itf.GetVideoFrame(1),char(1));
    end
end
itf.Close();
t = toc(t0);

function t = benchGetAnalogData(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
% Generate random frame number
num = 10;
frames = itf.GetVideoFrame(0) + ceil(rand(1,num) * (itf.GetVideoFrame(1) - itf.GetVideoFrame(0)));
frames = [itf.GetVideoFrame(0), itf.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * itf.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * itf.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            itf.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0));
        end
    end
end
itf.Close();
t = toc(t0);

function t = benchGetAnalogDataEx(itfHandle,d)
itf = itfHandle();
itf.Open(d.filename,3);
t0 = tic;
num = 10;
% Generate random subframe
subframes = ceil(rand(1,num) * itf.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * itf.GetAnalogChannels());
for j=1:numel(subframes)
    for k=1:numel(channels)
        itf.GetAnalogDataEx(channels(k),itf.GetVideoFrame(0),itf.GetVideoFrame(1),char(0),0,0,char(1));
    end
end
itf.Close();
t = toc(t0);
