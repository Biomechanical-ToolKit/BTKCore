function test_suite = testBTKIO
initTestSuite;
end

function testReadC3DSample01Eb015pi
[h, bo, sf] = btkReadAcquisition('../Data/Input/C3DSamples/sample01/Eb015pi.c3d');
assertEqual(bo, 'IEEE_LittleEndian');
assertEqual(sf, 'Integer');
btkDeleteAcquisition(h);
end
