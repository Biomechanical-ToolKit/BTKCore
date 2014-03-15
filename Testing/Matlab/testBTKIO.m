function test_suite = testBTKIO
initTestSuite;
end

function d = setup
d = TDDConfigure();
end

function testReadC3DSample01Eb015pi(d)
[h, bo, sf] = btkReadAcquisition(strcat(d.in,'/C3DSamples/sample01/Eb015pi.c3d'));
assertEqual(bo, 'IEEE_LittleEndian');
assertEqual(sf, 'Integer');
btkDeleteAcquisition(h);
end

function testReadC3DSampleUTF8(d)
% The Windows charset seems not compatible with hardcoded UTF-8 strings. The test is discarded under Windows.
if (isunix)
    [h, bo, sf] = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Я могу есть стекло/оно мне не вредит.c3d'));
    assertEqual(bo, 'IEEE_LittleEndian');
    assertEqual(sf, 'Float');
    btkDeleteAcquisition(h);
end
end

function testWriteC3DSampleUTF8(d)
h1 = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/shd01.c3d'));
btkWriteAcquisition(h1, strcat(d.out,'/C3DSamples/Я могу есть стекло, оно мне не вредит - Matlab.c3d'));
h2 = btkReadAcquisition(strcat(d.out,'/C3DSamples/Я могу есть стекло, оно мне не вредит - Matlab.c3d'));
assertEqual(btkGetPointNumber(h1), btkGetPointNumber(h2));
assertEqual(btkGetPointFrameNumber(h1), btkGetPointFrameNumber(h2));
assertEqual(btkGetAnalogNumber(h1), btkGetAnalogNumber(h2));
assertEqual(btkGetAnalogFrameNumber(h1), btkGetAnalogFrameNumber(h2));
assertEqual(btkGetFirstFrame(h1), btkGetFirstFrame(h2));
assertEqual(btkGetLastFrame(h1), btkGetLastFrame(h2));
assertEqual(btkGetAnalogSampleNumberPerFrame(h1), btkGetAnalogSampleNumberPerFrame(h2));
assertEqual(btkGetPointFrequency(h1), btkGetPointFrequency(h2));
assertEqual(btkGetAnalogFrequency(h1), btkGetAnalogFrequency(h2));
btkDeleteAcquisition(h1);
btkDeleteAcquisition(h2);
end

function testCloseAcquisition(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/shd01.c3d'));
btkCloseAcquisition(h);
try
  fn = btkGetPointFrameNumber(h)
end
assertEqual(isempty(strfind(lasterr, 'Parameter does not represent a MEXObjectHandle object.')), false);
end

function testInternalsUpdateMetaDataBased(d)
h1 = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/PiGMotion-FlatFoot-Full.c3d'));
md = btkGetMetaData(h1,'ANALYSIS');
num = md.children.USED.info.values(1);
blank = repmat({''},num,1);
btkSetMetaDataValues(h1,'ANALYSIS','DESCRIPTIONS',btkMetaDataInfo('Char',blank));
btkSetMetaDataValues(h1,'ANALYSIS','SUBJECTS',btkMetaDataInfo('Char',blank));
numPoints = btkGetPointNumber(h1);
for i = 1:numPoints
    btkSetPointLabel(h1,i,['MyPoint#',num2str(i)]);
    btkSetPointDescription(h1,i,'test');
end
numAnalogs = btkGetAnalogNumber(h1);
for i = 1:numAnalogs
    btkSetAnalogLabel(h1,i,['MyAnalog#',num2str(i)]);
    btkSetAnalogDescription(h1,i,'test');
    btkSetAnalogScale(h1,i,1.0);
    btkSetAnalogOffset(h1,i,0.0);
end
btkSetMetaDataValues(h1,'POINT','RATE',btkMetaDataInfo('Real',400));
btkWriteAcquisition(h1,strcat(d.out,'/C3DSamples/InternalsUpdateUpdateMetaDataBased_Matlab.c3d'),'InternalsUpdate','MetaDataBasedUpdate');
btkDeleteAcquisition(h1);

h1 = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/PiGMotion-FlatFoot-Full.c3d'));
h2 = btkReadAcquisition(strcat(d.out,'/C3DSamples/InternalsUpdateUpdateMetaDataBased_Matlab.c3d'));
assertEqual(btkGetFirstFrame(h1), btkGetFirstFrame(h2));
assertEqual(400.0, btkGetPointFrequency(h2));
assertEqual(btkGetPointNumber(h1), btkGetPointNumber(h2));
assertEqual(btkGetPointFrameNumber(h1), btkGetPointFrameNumber(h2));
assertEqual(4000.0, btkGetAnalogFrequency(h2));
assertEqual(btkGetAnalogNumber(h1), btkGetAnalogNumber(h2));
assertEqual(btkGetAnalogFrameNumber(h1), btkGetAnalogFrameNumber(h2));
for i = 1:numPoints
    [val,res,info1] = btkGetPoint(h1,i);
    [val,res,info2] = btkGetPoint(h2,i);
    assertEqual(info1.label, info2.label);
    assertEqual(info1.description, info2.description);
end
for i = 1:numAnalogs
    [val,info1] = btkGetAnalog(h1,i);
    [val,info2] = btkGetAnalog(h2,i);
    assertEqual(info1.label, info2.label);
    assertEqual(info1.description, info2.description);
    assertEqual(info1.scale, info2.scale);
    assertEqual(info1.offset, info2.offset);
end
md2 = btkGetMetaData(h2,'ANALYSIS','DESCRIPTIONS');
for i = 1:num
    assertEqual(md2.info.values{i},'');
end
md2 = btkGetMetaData(h2,'ANALYSIS','SUBJECTS');
for i = 1:num
    assertEqual(md2.info.values{i},'');
end
btkDeleteAcquisition(h1);
btkDeleteAcquisition(h2);
end