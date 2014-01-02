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
LabelIndex = btksrv.GetParameterIndex('POINT','LABELS');
btksrv.SetParameterValue(LabelIndex,0,'Knee_Right');
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
end
