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