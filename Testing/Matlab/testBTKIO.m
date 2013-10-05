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
