function test_suite = testC3DserverEmulation
initTestSuite;
end

function d = setup
d = TDDConfigure();
end

function testDoubleConstructor(d)
btksrv_ = btkEmulateC3Dserver();
btksrv_.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample09/PlugInC3D.c3d'),3);
assertEqual(btksrv_.GetVideoFrame(0),1)
assertEqual(btksrv_.GetVideoFrame(1),450)
assertEqual(btksrv.GetVideoFrame(0),1)
assertEqual(btksrv.GetVideoFrame(1),281)
btksrv_.Close();
btksrv.Close();
end

function testSetParameterValue_issue74(d)
btksrv = btkEmulateC3Dserver();
% Generate file
name = strcat(d.out,'/C3DSamples/btkC3DserverEmulation_SetParameterValue_issue74.c3d');
btksrv.NewFile(name,1,1,5,2,10,60,1,100);
btksrv.SaveFile('', -1);
btksrv.Close();
% Update 
btksrv.Open(name, 3);
labelIndex = btksrv.GetParameterIndex('POINT','LABELS');
btksrv.SetParameterValue(labelIndex,0,'Knee_Right');
btksrv.SaveFile('', -1);
btksrv.Close();
% Test
h = btkReadAcquisition(name);
assertEqual(btkGetAnalogNumber(h), 5);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 2);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrequency(h), 60);
assertEqual(btkGetPointFrameNumber(h), 100);
[val, res, info] = btkGetPoint(h, 1);
assertEqual(info.label, 'Knee_Right');
btkCloseAcquisition(h)
end

function testReadWriteEvent_issue74(d)
times = [2.72, 5.4, 7.32];
labels = {'RIC', 'RHS', 'RTO'};
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
assertEqual(btksrv.GetNumberEvents(), 3);
for i = 1:btksrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), times(i), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), labels{i});
end
name = strcat(d.out,'/C3DSamples/btkC3DserverEmulation_ReadWriteEvent_issue74.c3d');
btksrv.SaveFile(name, -1);
btksrv.Close();
btksrv.Open(name, 3);
assertEqual(btksrv.GetNumberEvents(), 3);
for i = 1:btksrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), times(i), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), labels{i});
end
times = [2.15, 6.12, 6.1];
labels = {'FOO1', 'BAR1', 'TOTO'};
for i = 1:btksrv.GetNumberEvents()
    btksrv.SetEventTime(i-1,times(i));
    btksrv.SetEventLabel(i-1,labels{i});
end
for i = 1:btksrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), times(i), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), labels{i});
end
btksrv.Close();
end

function testReadWriteEventBis_issue74(d)
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample09/PlugInC3D.c3d'),3);
assertEqual(btksrv.GetNumberEvents(), 0);
name = strcat(d.out,'/C3DSamples/btkC3DserverEmulation_ReadWriteEventBis_issue74.c3d');
btksrv.SaveFile(name, -1);
btksrv.Close();
btksrv.Open(name, 3);
assertEqual(btksrv.GetNumberEvents(), 0);
num = 2;
times = [0.15, 3.12];
labels = {'FOO', 'BAR'};
for i = 1:num
    btksrv.AddEvent(labels{i}, char(1), times(i));
end
assertEqual(btksrv.GetNumberEvents(), num);
for i = 1:btksrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), times(i), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), labels{i});
end
times = [2.15, 6.12];
labels = {'FOO1', 'BAR1'};
for i = 1:btksrv.GetNumberEvents()
    btksrv.SetEventTime(i-1,times(i));
    btksrv.SetEventLabel(i-1,labels{i});
end
for i = 1:btksrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), times(i), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), labels{i});
end
btksrv.SaveFile(name, -1);
btksrv.Close();
btksrv.Open(name, 3);
assertEqual(btksrv.GetNumberEvents(), num);
for i = 1:btksrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), times(i), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), labels{i});
end
btksrv.Close();
end

function testGetAnalogDataEx(d)
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
% Generate random channel
channels = [1,4,7,12,15];
frames = [1,34,176,250,399,653,820,1000,1497,1771];
values = [2048, 2048, 2048, 2049, 2048, 2048, 2048, 2048, 2048, 2048 ; ...
          2056, 2058, 2056, 2056, 2054, 2056, 2056, 2056, 2057, 2057 ; ...
          2042, 1989, 1986, 1985, 1984, 1984, 1988, 1984, 1985, 1989 ; ...
          2053, 2053, 2054, 2054, 2053, 1579, 1297, 2050, 2054, 2053 ; ...
          1827, 2003, 2003, 2002, 2001, 2133, 1830, 2005, 2003, 2006]; 
for k=1:numel(channels)
    % Full frames
    d = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
    for i = 1:length(frames)
        assertEqual(d{frames(i)}, int16(values(k,i)), sprintf('Channel index %02i - frame index %i',k,i));
    end
    % Ranged frames
    d = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+10,btksrv.GetVideoFrame(1)-10,char(0),0,0,char(0));
    for i = 3:9
        assertEqual(d{frames(i)-40}, int16(values(k,i)), sprintf('Channel index %02i - frame index %i',k,i));
    end
end
btksrv.Close();
end

function testGetPointDataEx(d)
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
% Generate random channel
channels = [2,5,9,11,14,19,22];
frames = [15,16,21,44,125,143,313,318,371,428];
values = [1914, 1910, 1919, 1923, 6207, 9044, 17313, 17316, 19218, 26166 ; ... 
          102, 89, 22, -666, 5156, 5162, 14463, 15896, 23002, 23046 ; ...
          1983, 1979, 1998, 2019, 7027, 8692, 17447, 17482, 21029, 26301 ; ... 
          623, 630, 689, 1097, 6358, 6253, 17004, 18078, 24169, 25464 ; ...
          1387, 1406, 1516, 1798, 6225, 6932, 17171, 17318, 22274, 26045 ; ... 
          1278, 1304, 1415, 2122, 6507, 6556, 17601, 18120, 24321, 26564 ; ... 
          0, 0, -252, 289, 3812, 4817, 15661, 15871, 20513, 24543];
for k=1:numel(channels)
    % Full frames
    d = btksrv.GetPointDataEx(channels(k),1,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
    for i = 1:length(frames)
        assertEqual(d{frames(i)}, int16(values(k,i)), sprintf('Point index %02i - frame index %i',k,i));
    end
    % Ranged frames
    d = btksrv.GetPointDataEx(channels(k),1,btksrv.GetVideoFrame(0)+20,btksrv.GetVideoFrame(1)-20,char(0));
    for i = 3:10
        assertEqual(d{frames(i)-20}, int16(values(k,i)), sprintf('Point index %02i - frame index %i',k,i));
    end
end
btksrv.Close(); 
end

function testGetPointResidualEx(d)
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
channels = [2,5,9,11,14,19,22];
frames = [15,16,21,44,125,143,313,318,371,428];
values = [1.666667, 1.750000, 1.916667, 1.916667, 0.583333, 1.666667, 0.833333, 0.416667, 0.666667, 0.833333 ; ... 
          6.166667, 0.416667, 0.416667, 3.833333, 2.833333, 0.916667, 0.416667, 0.916667, 0.916667, 1.166667 ; ... 
          0.833333, 1.333333, 1.416667, 0.666667, 2.666667, 1.166667, 0.666667, 0.666667, 1.083333, 2.833333 ; ...
          1.666667, 1.333333, 1.416667, 1.083333, 2.166667, 1.916667, 0.833333, 1.666667, 1.166667, 4.416667 ; ...
          0.583333, 0.416667, 1.916667, 0.333333, 0.666667, 0.416667, 0.416667, 0.666667, 2.166667, 0.166667 ; ...
          1.416667, 1.416667, 1.083333, 2.833333, 0.583333, 1.083333, 1.416667, 0.833333, 0.916667, 0.416667 ; ...
          -1.000000, -1.000000, 1.666667, 2.333333, 0.833333, 1.166667, 0.833333, 0.833333, 0.666667, 1.333333];
for k=1:numel(channels)
    % Full frames
    d = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
    for i = 1:length(frames)
        assertElementsAlmostEqual(d{frames(i)}, values(k,i), 'absolute', 1e-5);
    end
    % Ranged frames
    d = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0)+10,btksrv.GetVideoFrame(1)-10);
    for i = 1:length(frames)
        assertElementsAlmostEqual(d{frames(i)-10}, values(k,i), 'absolute', 1e-5);
    end
end
btksrv.Close();
end

function testAddFrames(d)
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
assertEqual(btksrv.GetVideoFrame(0),1)
assertEqual(btksrv.GetVideoFrame(1),450)
numFrames = btksrv.AddFrames(45,-1);
assertEqual(btksrv.GetVideoFrame(0),1)
assertEqual(btksrv.GetVideoFrame(1),495)
assertEqual(numFrames,495)
for i = 1:btksrv.GetNumber3DPoints()
    for k = 0:2
        d = btksrv.GetPointDataEx(i-1,k,1,45,char(0));
        for j = 1:length(d)
            assertEqual(d{j}, int16(0));
        end
    end
end
offsetIndex = btksrv.GetParameterIndex('ANALOG','OFFSET');
for i = 1:btksrv.GetAnalogChannels()
    d = btksrv.GetAnalogDataEx(i-1,1,45,char(0),0,0,char(0));
    v = btksrv.GetParameterValue(offsetIndex, i-1);
    for j = 1:length(d)
        assertEqual(d{j}, int16(v), sprintf('Channel index %02i - frame index %i',i,j));
    end
end
btksrv.Close();
end

function testDeleteFrames(d)
btksrv_ = btkEmulateC3Dserver();
btksrv_.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
btksrv = btkEmulateC3Dserver();
btksrv.Open(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'),3);
assertEqual(btksrv.GetVideoFrame(0),1)
assertEqual(btksrv.GetVideoFrame(1),450)
numFrames = btksrv.DeleteFrames(10,25);
assertEqual(btksrv.GetVideoFrame(0),1)
assertEqual(btksrv.GetVideoFrame(1),425)
assertEqual(btksrv_.GetVideoFrame(0),1)
assertEqual(btksrv_.GetVideoFrame(1),450)
assertEqual(numFrames,425)
indices = [1:9,35:450];
for i = 1:btksrv.GetNumber3DPoints()
    for k = 0:2
        d = btksrv.GetPointDataEx(i-1,k,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
        d_ = btksrv_.GetPointDataEx(i-1,k,btksrv_.GetVideoFrame(0),btksrv_.GetVideoFrame(1),char(0));
        
        for j = 1:length(d)
            assertEqual(d{j}, d_{indices(j)}, sprintf('Point index %02i - Col index %i - frame index %i',i,k+1,j));
        end
    end
end
avRatio = btksrv.GetAnalogVideoRatio();
ai = zeros(425 * avRatio);
for i = 1:length(indices)
    for j = 1:avRatio
        ai((i-1)*avRatio+j) = (indices(i)-1)*avRatio+j;
    end
end
for i = 1:btksrv.GetAnalogChannels()
    d = btksrv.GetAnalogDataEx(i-1,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
    d_ = btksrv_.GetAnalogDataEx(i-1,btksrv_.GetVideoFrame(0),btksrv_.GetVideoFrame(1),char(0),0,0,char(0));
    for j = 1:length(d)
        assertEqual(d{j}, d_{ai(j)}, sprintf('Channel index %02i - frame index %i',i,j));
    end
end
btksrv_.Close();
btksrv.Close();
end