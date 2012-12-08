function test_suite = testC3DserverEmulation
initTestSuite;
end

function d = setup
d.filename = '../Data/Input/C3DSamples/sample01/Eb015pi.c3d';
d.filename2 = '../Data/Input/C3DSamples/sample01/Eb015vr.c3d';
d.filename16bits = '../Data/Input/C3DSamples/sample19/sample19.c3d';
end

function testOpenMode1(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
assertEqual(c3dsrv.Open(d.filename,1), 0);
assertEqual(btksrv.Open(d.filename,1), 0);
c3dsrv.Close;
btksrv.Close();
end

function testOpenMode2(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
assertEqual(c3dsrv.Open(d.filename,2), 0);
assertEqual(btksrv.Open(d.filename,2), 0);
c3dsrv.Close;
btksrv.Close();
end

function testOpenMode3(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
assertEqual(c3dsrv.Open(d.filename,3), 0);
assertEqual(btksrv.Open(d.filename,3), 0);
c3dsrv.Close;
btksrv.Close();
end

function testIsModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetNumber3DPoints(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogChannels(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.GetAnalogChannels(), btksrv.GetAnalogChannels());
c3dsrv.Close;
btksrv.Close();
end

function testGetVideoFrame(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetVideoFrame(0), c3dsrv.GetVideoFrame(0));
assertEqual(btksrv.GetVideoFrame(1), c3dsrv.GetVideoFrame(1));
c3dsrv.Close;
btksrv.Close();
end

function testGetMaxInterpolationGap(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetMaxInterpolationGap(), c3dsrv.GetMaxInterpolationGap());
c3dsrv.Close;
btksrv.Close();
end

function testSetMaxInterpolationGap(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetMaxInterpolationGap(5), 1);
assertEqual(btksrv.SetMaxInterpolationGap(5), 1);
assertEqual(btksrv.GetMaxInterpolationGap(), c3dsrv.GetMaxInterpolationGap());
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetHeaderScaleFactor(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertElementsAlmostEqual(btksrv.GetHeaderScaleFactor(), c3dsrv.GetHeaderScaleFactor(), 'absolute', 1e-5);
c3dsrv.Close;
btksrv.Close();
end

function testGetHeaderScaleFactor_Real(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
assertEqual(btksrv.GetHeaderScaleFactor(), c3dsrv.GetHeaderScaleFactor());
c3dsrv.Close;
btksrv.Close();
end

function testGetStartingRecord(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetStartingRecord(), c3dsrv.GetStartingRecord());
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogVideoRatio(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetAnalogVideoRatio(), c3dsrv.GetAnalogVideoRatio());
c3dsrv.Close;
btksrv.Close();
end

function testGetVideoFrameRate(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertElementsAlmostEqual(btksrv.GetVideoFrameRate(), c3dsrv.GetVideoFrameRate(), 'absolute', 1e-5);
c3dsrv.Close;
btksrv.Close();
end

function testGetFileType(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetFileType(), c3dsrv.GetFileType());
c3dsrv.Close;
btksrv.Close();
end

function testGetDataType(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetDataType(), c3dsrv.GetDataType());
c3dsrv.Close;
btksrv.Close();
end

function testGetNumberEvents(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetNumberEvents(), c3dsrv.GetNumberEvents());
c3dsrv.Close;
btksrv.Close();
end

function testGetEvents(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), c3dsrv.GetEventTime(i-1), 'absolute', 1e-5);
    % Warning: BTK removes blank spaces in the event's label
    assertEqual(btksrv.GetEventLabel(i-1), strtrim(c3dsrv.GetEventLabel(i-1)));
    assertEqual(btksrv.GetEventStatus(i-1), c3dsrv.GetEventStatus(i-1));
    
end
c3dsrv.Close;
btksrv.Close();
end

function testAddEvent(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Return the index
assertEqual(c3dsrv.AddEvent('TOTO', char(1), 0.15), btksrv.AddEvent('TOTO', char(1), 0.15));
assertEqual(btksrv.GetNumberEvents(), c3dsrv.GetNumberEvents());
for i=1:c3dsrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), c3dsrv.GetEventTime(i-1), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), strtrim(c3dsrv.GetEventLabel(i-1)));
    assertEqual(btksrv.GetEventStatus(i-1), c3dsrv.GetEventStatus(i-1));
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetEventLabel(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetEventLabel(0, 'TOTO'), 1);
assertEqual(btksrv.SetEventLabel(0, 'TOTO'), 1);
assertEqual(btksrv.GetEventLabel(0), c3dsrv.GetEventLabel(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetEventTime(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetEventTime(0, 0.54), 1);
assertEqual(btksrv.SetEventTime(0, 0.54), 1);
assertElementsAlmostEqual(btksrv.GetEventTime(0), c3dsrv.GetEventTime(0), 'absolute', 1e-5);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetEventStatus(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetEventStatus(0, '1'), 1);
assertEqual(btksrv.SetEventStatus(0, '1'), 1);
assertEqual(btksrv.GetEventStatus(0), c3dsrv.GetEventStatus(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteEvent(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.DeleteEvent(0), 1);
assertEqual(btksrv.DeleteEvent(0), 1);
assertEqual(btksrv.GetNumberEvents(), c3dsrv.GetNumberEvents());
for i=1:c3dsrv.GetNumberEvents()
    assertElementsAlmostEqual(btksrv.GetEventTime(i-1), c3dsrv.GetEventTime(i-1), 'absolute', 1e-5);
    assertEqual(btksrv.GetEventLabel(i-1), strtrim(c3dsrv.GetEventLabel(i-1)));
    assertEqual(btksrv.GetEventStatus(i-1), c3dsrv.GetEventStatus(i-1));
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetStrictParameterChecking(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.SetStrictParameterChecking(0);
btksrv.SetStrictParameterChecking(0);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testCompressParameterBlocks(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3dsrv.CompressParameterBlocks(1);
btksrv.CompressParameterBlocks(1);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetNumberGroups(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
c3dsrv.Close;
btksrv.Close();
end

function testGetGroupIndex(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetGroupIndex('POINT'), c3dsrv.GetGroupIndex('POINT'));
assertEqual(btksrv.GetGroupIndex('ANALOG'), c3dsrv.GetGroupIndex('ANALOG'));
assertEqual(btksrv.GetGroupIndex('FORCE_PLATFORM'), c3dsrv.GetGroupIndex('FORCE_PLATFORM'));
assertEqual(btksrv.GetGroupIndex('FOO'), c3dsrv.GetGroupIndex('FOO'));
c3dsrv.Close;
btksrv.Close();
end

function testGetGroupName(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.GetGroupName(i-1), c3dsrv.GetGroupName(i-1));
end
c3dsrv.Close;
btksrv.Close();
end

function testGetGroupNumber(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.GetGroupNumber(i-1), c3dsrv.GetGroupNumber(i-1));
end
c3dsrv.Close;
btksrv.Close();
end

function testGetGroupLock(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.GetGroupLock(i-1), c3dsrv.GetGroupLock(i-1));
end
c3dsrv.Close;
btksrv.Close(); 
end

function testGetGroupDescription(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.GetGroupDescription(i-1), c3dsrv.GetGroupDescription(i-1));
end
c3dsrv.Close;
btksrv.Close(); 
end

function testSetGroupName(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetGroupName(0, 'FOO'), 1);
assertEqual(btksrv.SetGroupName(0, 'FOO'), 1);
assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetGroupName_SameName(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetGroupName(0, 'POINT'), 1);
assertEqual(btksrv.SetGroupName(0, 'POINT'), 1);
assertEqual(c3dsrv.SetGroupName(0, 'POINT'), 1);
assertEqual(btksrv.SetGroupName(0, 'POINT'), 1);
assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetGroupName_ErrorDuplicateName(d)
    function duplicateName(itf)
        itf.SetGroupName(1, 'POINT');
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()duplicateName(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()duplicateName(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetGroupName(1), c3dsrv.GetGroupName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetGroupName_ErrorStrict(d)
    function SetSameSixFirstCharacters(itf)
        itf.SetGroupName(0, 'POINTS');
        itf.SetGroupName(1, 'POINTS2');
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()SetSameSixFirstCharacters(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()SetSameSixFirstCharacters(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
assertEqual(btksrv.GetGroupName(1), c3dsrv.GetGroupName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetGroupName_NotStrict(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3dsrv.SetStrictParameterChecking(0);
btksrv.SetStrictParameterChecking(0);
assertEqual(c3dsrv.SetGroupName(0, 'POINTS'), 1);
assertEqual(c3dsrv.SetGroupName(1, 'POINTS2'), 1);
assertEqual(btksrv.SetGroupName(0, 'POINTS'), 1);
assertEqual(btksrv.SetGroupName(1, 'POINTS2'), 1);
assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
assertEqual(btksrv.GetGroupName(1), c3dsrv.GetGroupName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetGroupNumber(d)
warning('OFF','btk:C3Dserver:UniqueNumber');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetGroupNumber(0, -25), 1);
assertEqual(btksrv.SetGroupNumber(0, -25), 1);
assertEqual(btksrv.GetGroupNumber(0), -1);
assertEqual(c3dsrv.GetGroupNumber(0), -25);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:UniqueNumber');
end

function testSetGroupNumber_ErrorPositiveOrNull(d)
    function SetPositiveNumber(itf)
        itf.SetGroupNumber(0, 12);
    end
    function SetNullNumber(itf)
        itf.SetGroupNumber(0, 0);
    end
warning('OFF','btk:C3Dserver:UniqueNumber');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()SetPositiveNumber(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()SetPositiveNumber(btksrv), 'btk:C3Dserver');
assertExceptionThrown(@()SetNullNumber(c3dsrv), 'MATLAB:COM:E2147500037', 'Incorrect value for group number');
assertExceptionThrown(@()SetNullNumber(btksrv), 'btk:C3Dserver', 'Incorrect value for group number.');
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:UniqueNumber');
end

function testSetGroupDescription(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetGroupDescription(0, 'FOO'), 1);
assertEqual(btksrv.SetGroupDescription(0, 'FOO'), 1);
assertEqual(btksrv.GetGroupDescription(0), c3dsrv.GetGroupDescription(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetGroupLock(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.SetGroupLock(i-1, char(mod(i,2))), 1);
    assertEqual(c3dsrv.SetGroupLock(i-1, char(mod(i,2))), 1);
    assertEqual(btksrv.GetGroupLock(i-1), c3dsrv.GetGroupLock(i-1));
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddGroup(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.AddGroup(0, 'FOO', 'blabla', char(0)), btksrv.GetNumberGroups()-1);
assertEqual(c3dsrv.AddGroup(0, 'FOO', 'blabla', char(0)), c3dsrv.GetNumberGroups()-1);    
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.GetGroupName(i-1), c3dsrv.GetGroupName(i-1));
    assertEqual(btksrv.GetGroupDescription(i-1), c3dsrv.GetGroupDescription(i-1));
    assertEqual(btksrv.GetGroupNumber(i-1), c3dsrv.GetGroupNumber(i-1));
    assertEqual(btksrv.GetGroupLock(i-1), c3dsrv.GetGroupLock(i-1));
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddGroup_ErrorDuplicateName(d)
    function duplicateName(itf)
        itf.AddGroup(0, 'FOO', 'blabla', char(0));
        itf.AddGroup(0, 'FOO', 'blabla', char(0));
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()duplicateName(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()duplicateName(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddGroup_ErrorStrict(d)
    function AddSameSixFirstCharacters(itf)
        itf.AddGroup(0, 'POINTS', 'blabla', char(0));
        itf.AddGroup(0, 'POINTS2', 'blabla', char(0));
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()AddSameSixFirstCharacters(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()AddSameSixFirstCharacters(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddGroup_NotStrict(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3dsrv.SetStrictParameterChecking(0);
btksrv.SetStrictParameterChecking(0);
assertEqual(btksrv.AddGroup(0, 'POINTS', 'blabla', char(0)), btksrv.GetNumberGroups()-1);
assertEqual(btksrv.AddGroup(0, 'POINTS2', 'blabla', char(0)), btksrv.GetNumberGroups()-1);
assertEqual(c3dsrv.AddGroup(0, 'POINTS', 'blabla', char(0)), c3dsrv.GetNumberGroups()-1);
assertEqual(c3dsrv.AddGroup(0, 'POINTS2', 'blabla', char(0)), c3dsrv.GetNumberGroups()-1);
assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteGroup(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberGroups()
    assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
    assertEqual(btksrv.GetGroupDescription(0), c3dsrv.GetGroupDescription(0));
    %assertEqual(btksrv.GetGroupNumber(0), c3dsrv.GetGroupNumber(0));
    assertEqual(btksrv.GetGroupLock(0), c3dsrv.GetGroupLock(0));
    assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
    assertEqual(btksrv.GetNumberParameters(), c3dsrv.GetNumberParameters());
    assertEqual(btksrv.DeleteGroup(0, char(1)), 1);
    assertEqual(c3dsrv.DeleteGroup(0, char(1)), 1);
end
assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteGroup_CheckIndex(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetGroupIndex('POINT'), c3dsrv.GetGroupIndex('POINT'));
assertEqual(btksrv.GetGroupIndex('ANALOG'), c3dsrv.GetGroupIndex('ANALOG'));
assertEqual(btksrv.GetGroupIndex('FORCE_PLATFORM'), c3dsrv.GetGroupIndex('FORCE_PLATFORM'));
assertEqual(btksrv.GetGroupIndex('FOO'), c3dsrv.GetGroupIndex('FOO'));
assertEqual(btksrv.DeleteGroup(0, char(1)), 1); % POINT
assertEqual(c3dsrv.DeleteGroup(0, char(1)), 1); % POINT
assertEqual(btksrv.GetGroupIndex('POINT'), c3dsrv.GetGroupIndex('POINT'));
assertEqual(btksrv.GetGroupIndex('ANALOG'), c3dsrv.GetGroupIndex('ANALOG'));
assertEqual(btksrv.GetGroupIndex('FORCE_PLATFORM'), c3dsrv.GetGroupIndex('FORCE_PLATFORM'));
assertEqual(btksrv.GetGroupIndex('FOO'), c3dsrv.GetGroupIndex('FOO'));
assertEqual(btksrv.GetNumberGroups(), c3dsrv.GetNumberGroups());
assertEqual(btksrv.GetNumberParameters(), c3dsrv.GetNumberParameters());    
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteGroup_CheckInternalParameterIndex(d)
global btkC3DserverHandles;
btksrv = btkEmulateC3Dserver();
btksrv.Open(d.filename,3);
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 37);
assertEqual(btksrv.GetNumberGroups(), 5);
assertEqual(btksrv.GetNumberParameters(), 37);
assertEqual(btksrv.DeleteGroup(0, char(1)), 1); % POINT
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 27);
assertEqual(btksrv.GetNumberGroups(), 4);
assertEqual(btksrv.GetNumberParameters(), 27);
assertEqual(btksrv.GetGroupIndex('POINT'), -1);
assertEqual(btksrv.GetGroupIndex('FOO'), -1);
assertEqual(btksrv.GetGroupIndex('ANALOG'), 0);
assertEqual(btksrv.GetGroupIndex('FORCE_PLATFORM'), 1);
assertEqual(btksrv.GetGroupIndex('FPLOC'), 2);
assertEqual(btksrv.GetGroupIndex('SUBJECT'), 3);
assertEqual(btksrv.DeleteGroup(1, char(1)), 1); % FORCE_PLATFORM
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 19);
assertEqual(btksrv.GetNumberGroups(), 3);
assertEqual(btksrv.GetNumberParameters(), 19);
assertEqual(btksrv.GetGroupIndex('POINT'), -1);
assertEqual(btksrv.GetGroupIndex('FOO'), -1);
assertEqual(btksrv.GetGroupIndex('ANALOG'), 0);
assertEqual(btksrv.GetGroupIndex('FORCE_PLATFORM'), -1);
assertEqual(btksrv.GetGroupIndex('FPLOC'), 1);
assertEqual(btksrv.GetGroupIndex('SUBJECT'), 2);
assertEqual(btksrv.DeleteGroup(2, char(1)), 1); % SUBJECT
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 11);
assertEqual(btksrv.GetNumberGroups(), 2);
assertEqual(btksrv.GetNumberParameters(), 11);
assertEqual(btksrv.GetGroupIndex('POINT'), -1);
assertEqual(btksrv.GetGroupIndex('FOO'), -1);
assertEqual(btksrv.GetGroupIndex('ANALOG'), 0);
assertEqual(btksrv.GetGroupIndex('FORCE_PLATFORM'), -1);
assertEqual(btksrv.GetGroupIndex('FPLOC'), 1);
assertEqual(btksrv.GetGroupIndex('SUBJECT'), -1);
btksrv.Close();
end

function testGetNumberParameters(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetNumberParameters(), c3dsrv.GetNumberParameters());
c3dsrv.Close;
btksrv.Close();
end

function testGetParameterIndex(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterIndex('POINT','DESCRIPTIONS'), c3dsrv.GetParameterIndex('POINT','DESCRIPTIONS'));
assertEqual(btksrv.GetParameterIndex('POINT','X_SCREEN'), c3dsrv.GetParameterIndex('POINT','X_SCREEN'));
assertEqual(btksrv.GetParameterIndex('POINT','Y_SCREEN'), c3dsrv.GetParameterIndex('POINT','Y_SCREEN'));
assertEqual(btksrv.GetParameterIndex('FOO', 'FOO'), c3dsrv.GetParameterIndex('FOO','FOO'));
assertEqual(btksrv.GetParameterIndex('POINT','LABELS'), 3);
c3dsrv.Close;
btksrv.Close();
end

function testGetParameterName(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterName(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterName(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterName(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterName(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterName(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterName(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close();
end

function testGetParameterNumber(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterNumber(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterNumber(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterNumber(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterNumber(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterNumber(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterNumber(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close();
end

function testGetParameterLock(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterLock(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterLock(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterLock(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterLock(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterLock(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterLock(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterDescription(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterDescription(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterDescription(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterDescription(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterDescription(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterDescription(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterDescription(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterType(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterType(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterType(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterType(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterType(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterType(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterType(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterNumberDim(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterNumberDim(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterNumberDim(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterNumberDim(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterNumberDim(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterNumberDim(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterNumberDim(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterDimension(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
btkidx = btksrv.GetParameterIndex('POINT','LABELS'); c3didx = c3dsrv.GetParameterIndex('POINT','LABELS');
for j=1:c3dsrv.GetParameterNumberDim(c3didx); assertEqual(btksrv.GetParameterDimension(btkidx, j-1), c3dsrv.GetParameterDimension(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','LABELS'); c3didx = c3dsrv.GetParameterIndex('ANALOG','LABELS');
for j=1:c3dsrv.GetParameterNumberDim(c3didx); assertEqual(btksrv.GetParameterDimension(btkidx, j-1), c3dsrv.GetParameterDimension(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','USED'); c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED');
for j=1:c3dsrv.GetParameterNumberDim(c3didx); assertEqual(btksrv.GetParameterDimension(btkidx, j-1), c3dsrv.GetParameterDimension(c3didx, j-1)); end;
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterLength(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('POINT','LABELS')), c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('POINT','LABELS')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','LABELS')), c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('FORCE_PLATFORM','USED')), c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED')));
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterValue(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
btkidx = btksrv.GetParameterIndex('POINT','SCALE'); c3didx = c3dsrv.GetParameterIndex('POINT','SCALE');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('POINT','LABELS'); c3didx = c3dsrv.GetParameterIndex('POINT','LABELS');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','LABELS'); c3didx = c3dsrv.GetParameterIndex('ANALOG','LABELS');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','USED'); c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','USED'); c3didx = c3dsrv.GetParameterIndex('ANALOG','USED');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','CHANNEL'); c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','CHANNEL');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
c3dsrv.Close;
btksrv.Close(); 
end

function testGetParameterValue_InvalidIndex(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
btkidx = btksrv.GetParameterIndex('POINT','SCALE'); c3didx = c3dsrv.GetParameterIndex('POINT','SCALE');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('POINT','LABELS'); c3didx = c3dsrv.GetParameterIndex('POINT','LABELS');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','LABELS'); c3didx = c3dsrv.GetParameterIndex('ANALOG','LABELS');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','USED'); c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','USED');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','USED'); c3didx = c3dsrv.GetParameterIndex('ANALOG','USED');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','CHANNEL'); c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','CHANNEL');
for j=1:50; assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
c3dsrv.Close;
btksrv.Close(); 
end

function testSetParameterName(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetParameterName(0, 'FOO'), 1);
assertEqual(btksrv.SetParameterName(0, 'FOO'), 1);
assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterName_SameName(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetParameterName(0, 'DESCRIPTIONS'), 1);
assertEqual(btksrv.SetParameterName(0, 'DESCRIPTIONS'), 1);
assertEqual(c3dsrv.SetParameterName(0, 'DESCRIPTIONS'), 1);
assertEqual(btksrv.SetParameterName(0, 'DESCRIPTIONS'), 1);
assertEqual(btksrv.GetGroupName(0), c3dsrv.GetGroupName(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterName_ErrorDuplicateName(d)
    function duplicateName(itf)
        itf.SetParameterName(1, 'DESCRIPTIONS');
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()duplicateName(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()duplicateName(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetParameterName(1), c3dsrv.GetParameterName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterName_InvalidIndex(d)
    function invalidIndex(itf)
        itf.SetParameterName(100, 'DESCRIPTIONS');
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()invalidIndex(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()invalidIndex(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetParameterName(1), c3dsrv.GetParameterName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterName_ErrorStrict(d)
    function SetSameSixFirstCharacters(itf)
        itf.SetParameterName(0, 'POINTS');
        itf.SetParameterName(1, 'POINTS2');
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()SetSameSixFirstCharacters(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()SetSameSixFirstCharacters(btksrv), 'btk:C3Dserver');
assertEqual(btksrv.GetParameterName(0), c3dsrv.GetParameterName(0));
assertEqual(btksrv.GetParameterName(1), c3dsrv.GetParameterName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterName_NotStrict(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3dsrv.SetStrictParameterChecking(0);
btksrv.SetStrictParameterChecking(0);
assertEqual(c3dsrv.SetParameterName(0, 'POINTS'), 1);
assertEqual(c3dsrv.SetParameterName(1, 'POINTS2'), 1);
assertEqual(btksrv.SetParameterName(0, 'POINTS'), 1);
assertEqual(btksrv.SetParameterName(1, 'POINTS2'), 1);
assertEqual(btksrv.GetParameterName(0), c3dsrv.GetParameterName(0));
assertEqual(btksrv.GetParameterName(1), c3dsrv.GetParameterName(1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterNumber(d)
warning('OFF','btk:C3Dserver:UniqueNumber');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetParameterNumber(0, 25), 1);
assertEqual(btksrv.SetParameterNumber(0, 25), 1);
assertEqual(btksrv.GetParameterNumber(0), 1);
assertEqual(c3dsrv.GetParameterNumber(0), 25);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:UniqueNumber');
end

function testSetParameterNumber_ErrorNegativeOrNull(d)
    function SetNegativeNumber(itf)
        itf.SetParameterNumber(0, -12);
    end
    function SetNullNumber(itf)
        itf.SetParameterNumber(0, 0);
    end
warning('OFF','btk:C3Dserver:UniqueNumber');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()SetNegativeNumber(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()SetNegativeNumber(btksrv), 'btk:C3Dserver');
assertExceptionThrown(@()SetNullNumber(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()SetNullNumber(btksrv), 'btk:C3Dserver');
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:UniqueNumber');
end

function testSetParameterDescription(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.SetParameterDescription(0, 'FOO'), 1);
assertEqual(btksrv.SetParameterDescription(0, 'FOO'), 1);
assertEqual(btksrv.GetParameterDescription(0), c3dsrv.GetParameterDescription(0));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterLock(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:c3dsrv.GetNumberParameters()
    assertEqual(btksrv.SetParameterLock(i-1, char(mod(i,2))), 1);
    assertEqual(c3dsrv.SetParameterLock(i-1, char(mod(i,2))), 1);
    assertEqual(btksrv.GetParameterLock(i-1), c3dsrv.GetParameterLock(i-1));
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterType(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% POINT:USED
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
% - Integer
btksrv.SetParameterType(btkidx, 2); c3dsrv.SetParameterType(c3didx, 2);
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
% - Byte
btksrv.SetParameterType(btkidx, 1); c3dsrv.SetParameterType(c3didx, 1);
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
% - Float
btksrv.SetParameterType(btkidx, 4); c3dsrv.SetParameterType(c3didx, 4);
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
% - Char (C3Dserver doesn't convert number into string)
% btksrv.SetParameterType(btkidx, -1); c3dsrv.SetParameterType(c3didx, -1);
% assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
% assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
% for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
c3dsrv.SetParameterType(c3didx, 2); % Required because Matlab doesn't respond anymore in the other case.
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetParameterValue(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% POINT:USED
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
assertEqual(btksrv.SetParameterValue(btkidx, 0, 40), 1);
assertEqual(c3dsrv.SetParameterValue(c3didx, 0, 40), 1);
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
% POINT:LABELS
btkidx = btksrv.GetParameterIndex('POINT','LABELS'); c3didx = c3dsrv.GetParameterIndex('POINT','LABELS');
assertEqual(btksrv.SetParameterValue(btkidx, 0, 'ALLO'), 1);
assertEqual(c3dsrv.SetParameterValue(c3didx, 0, 'ALLO'), 1);
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
% POINT:SCALE
btkidx = btksrv.GetParameterIndex('POINT','SCALE'); c3didx = c3dsrv.GetParameterIndex('POINT','SCALE');
assertEqual(btksrv.SetParameterValue(btkidx, 0, 0.0882), 1);
assertEqual(c3dsrv.SetParameterValue(c3didx, 0, 0.0882), 1);
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterChar(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([32,0]);
data = {};
btkidx = btksrv.AddParameter('FOOCHAR','TestAddParameterChar','POINT',char(0),-1,numel(dims),dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','TestAddParameterChar','POINT',char(0),-1,numel(dims),dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterChar_WrongDims(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([32,0]);
data = {};
btkidx = btksrv.AddParameter('FOOCHAR','TestAddParameterChar','POINT',char(0),-1,4,dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','TestAddParameterChar','POINT',char(0),-1,4,dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterByte(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([3,2]);
data = int8(ones(1,6));
btkidx = btksrv.AddParameter('FOOCHAR','TestAddParameterByte','POINT',char(0),1,numel(dims),dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','TestAddParameterByte','POINT',char(0),1,numel(dims),dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterByte_WrongDims(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([3,2]);
data = int8(ones(1,6));
btkidx = btksrv.AddParameter('FOOCHAR','TestAddParameterByte','POINT',char(0),1,4,dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','TestAddParameterByte','POINT',char(0),1,4,dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterInteger(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([3,2]);
data = int16(ones(1,6));
btkidx = btksrv.AddParameter('FOOCHAR','testAddParameterInteger','POINT',char(0),2,numel(dims),dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','testAddParameterInteger','POINT',char(0),2,numel(dims),dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterInteger_WrongDims(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([3,2]);
data = int16(ones(1,6));
btkidx = btksrv.AddParameter('FOOCHAR','testAddParameterInteger','POINT',char(0),2,4,dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','testAddParameterInteger','POINT',char(0),2,4,dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterReal(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([3,2]);
data = single(ones(1,6));
btkidx = btksrv.AddParameter('FOOCHAR','testAddParameterReal','POINT',char(0),4,numel(dims),dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','testAddParameterReal','POINT',char(0),4,numel(dims),dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterReal_WrongDims(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
dims = int16([3,2]);
data = single(ones(1,6));
btkidx = btksrv.AddParameter('FOOCHAR','testAddParameterReal','POINT',char(0),4,4,dims,data);
c3didx = c3dsrv.AddParameter('FOOCHAR','testAddParameterReal','POINT',char(0),4,4,dims,data);
assertEqual(btkidx, c3didx);
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteParameter(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 0;
for i=1:c3dsrv.GetNumberParameters()
    id = c3dsrv.GetParameterNumber(c3didx);
    btkidx = [];
    for j=1:c3dsrv.GetNumberGroups()
        if (abs(c3dsrv.GetGroupNumber(j-1)) == id)
            btkidx = btksrv.GetParameterIndex(c3dsrv.GetGroupName(j-1), c3dsrv.GetParameterName(c3didx));
            break;
        end
    end
    assertEqual(isempty(btkidx),false);
    assertEqual(btksrv.GetNumberParameters(),c3dsrv.GetNumberParameters());
    assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
    assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
    assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
    assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
    assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
    for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
    assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
    assertEqual(btksrv.DeleteParameter(btkidx), 1);
    assertEqual(c3dsrv.DeleteParameter(c3didx), 1);
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteParameter_CheckInternalParameterIndex(d)
global btkC3DserverHandles;
btksrv = btkEmulateC3Dserver();
btksrv.Open(d.filename,3);
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 37);
assertEqual(btksrv.GetNumberGroups(), 5);
assertEqual(btksrv.GetNumberParameters(), 37);
assertEqual(btksrv.DeleteParameter(0), 1); % POINT:DESCRIPTIONS
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 36);
assertEqual(btksrv.GetNumberGroups(), 5);
assertEqual(btksrv.GetNumberParameters(), 36);
assertEqual(btksrv.GetParameterIndex('POINT','DESCRIPTIONS'), -1);
assertEqual(btksrv.GetParameterIndex('POINT','X_SCREEN'), 0);
assertEqual(btksrv.GetParameterIndex('POINT','Y_SCREEN'), 1);
assertEqual(btksrv.DeleteParameter(1), 1); % POINT:Y_SCREEN
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 35);
assertEqual(btksrv.GetNumberGroups(), 5);
assertEqual(btksrv.GetNumberParameters(), 35);
assertEqual(btksrv.GetParameterIndex('POINT','DESCRIPTIONS'), -1);
assertEqual(btksrv.GetParameterIndex('POINT','X_SCREEN'), 0);
assertEqual(btksrv.GetParameterIndex('POINT','Y_SCREEN'), -1);
assertEqual(btksrv.DeleteParameter(0), 1); % POINT:X_SCREEN
s = size(btkC3DserverHandles(end).parameterIndexMapping,1);
assertEqual(s, 34);
assertEqual(btksrv.GetNumberGroups(), 5);
assertEqual(btksrv.GetNumberParameters(), 34);
assertEqual(btksrv.GetParameterIndex('POINT','DESCRIPTIONS'), -1);
assertEqual(btksrv.GetParameterIndex('POINT','X_SCREEN'), -1);
assertEqual(btksrv.GetParameterIndex('POINT','Y_SCREEN'), -1);
btksrv.Close();
end

function testCopyParameter(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.CopyParameter(0), 1);
assertEqual(btksrv.CopyParameter(0), 1);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testCopyParameter_Accessor(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.CopyParameter(0), 1);
assertEqual(btksrv.CopyParameter(0), 1);
assertEqual(btksrv.GetParameterName(-1), c3dsrv.GetParameterName(-1));
assertEqual(btksrv.GetParameterType(-1), c3dsrv.GetParameterType(-1));
assertEqual(btksrv.GetParameterLock(-1), c3dsrv.GetParameterLock(-1));
assertEqual(c3dsrv.GetParameterNumberDim(-1), btksrv.GetParameterNumberDim(-1));
assertEqual(c3dsrv.GetParameterLength(-1), btksrv.GetParameterLength(-1));
for j=1:btksrv.GetParameterLength(-1); assertEqual(btksrv.GetParameterValue(-1, j-1), c3dsrv.GetParameterValue(-1, j-1)); end;
assertEqual(btksrv.GetParameterDescription(-1), c3dsrv.GetParameterDescription(-1));
assertEqual(btksrv.GetParameterNumber(-1), c3dsrv.GetParameterNumber(-1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testCopyParameter_AccessorBis(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Index 0
assertEqual(c3dsrv.CopyParameter(0), 1);
assertEqual(btksrv.CopyParameter(0), 1);
assertEqual(btksrv.GetParameterName(-1), c3dsrv.GetParameterName(-1));
assertEqual(btksrv.GetParameterType(-1), c3dsrv.GetParameterType(-1));
assertEqual(btksrv.GetParameterLock(-1), c3dsrv.GetParameterLock(-1));
assertEqual(c3dsrv.GetParameterNumberDim(-1), btksrv.GetParameterNumberDim(-1));
assertEqual(c3dsrv.GetParameterLength(-1), btksrv.GetParameterLength(-1));
for j=1:btksrv.GetParameterLength(-1); assertEqual(btksrv.GetParameterValue(-1, j-1), c3dsrv.GetParameterValue(-1, j-1)); end;
assertEqual(btksrv.GetParameterDescription(-1), c3dsrv.GetParameterDescription(-1));
% Index 2
assertEqual(c3dsrv.CopyParameter(2), 1);
assertEqual(btksrv.CopyParameter(2), 1);
assertEqual(btksrv.GetParameterName(-1), c3dsrv.GetParameterName(-1));
assertEqual(btksrv.GetParameterType(-1), c3dsrv.GetParameterType(-1));
assertEqual(btksrv.GetParameterLock(-1), c3dsrv.GetParameterLock(-1));
assertEqual(c3dsrv.GetParameterNumberDim(-1), btksrv.GetParameterNumberDim(-1));
assertEqual(c3dsrv.GetParameterLength(-1), btksrv.GetParameterLength(-1));
for j=1:btksrv.GetParameterLength(-1); assertEqual(btksrv.GetParameterValue(-1, j-1), c3dsrv.GetParameterValue(-1, j-1)); end;
assertEqual(btksrv.GetParameterDescription(-1), c3dsrv.GetParameterDescription(-1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testCopyParameter_AccessorNoCopy(d)
    function GetParameterName(itf)
        itf.GetParameterName(-1);
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()GetParameterName(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()GetParameterName(btksrv), 'btk:C3Dserver');
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testCopyParameter_Mutator(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.CopyParameter(30), 1); % POINT:USED
assertEqual(btksrv.CopyParameter(5), 1); % POINT:USED
% Mutate
assertEqual(btksrv.SetParameterName(-1,'FOO'), c3dsrv.SetParameterName(-1,'FOO'));
assertEqual(btksrv.SetParameterLock(-1,char(1)), c3dsrv.SetParameterLock(-1,char(1)));
assertEqual(btksrv.SetParameterDescription(-1,'testCopyParameter_Mutator'), c3dsrv.SetParameterDescription(-1,'testCopyParameter_Mutator'));
assertEqual(btksrv.SetParameterType(-1,1), c3dsrv.SetParameterType(-1,1));
assertEqual(btksrv.SetParameterValue(-1,0,25), c3dsrv.SetParameterValue(-1,0,25));
% Access
assertEqual(btksrv.GetParameterName(-1), c3dsrv.GetParameterName(-1));
assertEqual(btksrv.GetParameterType(-1), c3dsrv.GetParameterType(-1));
assertEqual(btksrv.GetParameterLock(-1), c3dsrv.GetParameterLock(-1));
assertEqual(c3dsrv.GetParameterNumberDim(-1), btksrv.GetParameterNumberDim(-1));
assertEqual(c3dsrv.GetParameterLength(-1), btksrv.GetParameterLength(-1));
for j=1:btksrv.GetParameterLength(-1); assertEqual(btksrv.GetParameterValue(-1, j-1), c3dsrv.GetParameterValue(-1, j-1)); end;
assertEqual(btksrv.GetParameterDescription(-1), c3dsrv.GetParameterDescription(-1));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testCopyParameter_MutatorNoCopy(d)
    function SetParameterName(itf, name)
        itf.SetParameterName(-1, name);
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()SetParameterName(c3dsrv, 'FOO'), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()SetParameterName(btksrv, 'FOO'), 'btk:C3Dserver');
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRetrieveParameter(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
idxPr = 1;
assertEqual(c3dsrv.CopyParameter(idxPr), 1);
assertEqual(btksrv.CopyParameter(idxPr), 1);
assertEqual(c3dsrv.RetrieveParameter(idxPr), 1);
assertEqual(btksrv.RetrieveParameter(idxPr), 1);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRetrieveParameter_NoCopy(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
idxPr = 1;
assertEqual(c3dsrv.RetrieveParameter(idxPr), 0);
assertEqual(btksrv.RetrieveParameter(idxPr), 0);
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRetrieveParameter_Accessor(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
idxPr = 1;
assertEqual(c3dsrv.CopyParameter(idxPr), 1);
assertEqual(btksrv.CopyParameter(idxPr), 1);
assertEqual(c3dsrv.RetrieveParameter(idxPr), 1);
assertEqual(btksrv.RetrieveParameter(idxPr), 1);
assertEqual(btksrv.GetParameterName(idxPr), c3dsrv.GetParameterName(idxPr));
assertEqual(btksrv.GetParameterType(idxPr), c3dsrv.GetParameterType(idxPr));
assertEqual(btksrv.GetParameterLock(idxPr), c3dsrv.GetParameterLock(idxPr));
assertEqual(c3dsrv.GetParameterNumberDim(idxPr), btksrv.GetParameterNumberDim(idxPr));
assertEqual(c3dsrv.GetParameterLength(idxPr), btksrv.GetParameterLength(idxPr));
for j=1:btksrv.GetParameterLength(idxPr); assertEqual(btksrv.GetParameterValue(idxPr, j-1), c3dsrv.GetParameterValue(idxPr, j-1)); end;
assertEqual(btksrv.GetParameterDescription(idxPr), c3dsrv.GetParameterDescription(idxPr));
assertEqual(btksrv.GetParameterNumber(idxPr), c3dsrv.GetParameterNumber(idxPr));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRetrieveParameter_Mutator(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
idxPr = 1;
assertEqual(c3dsrv.CopyParameter(idxPr), 1);
assertEqual(btksrv.CopyParameter(idxPr), 1);
% Modification
assertEqual(btksrv.SetParameterName(-1,'FOO'), c3dsrv.SetParameterName(-1,'FOO'));
assertEqual(btksrv.SetParameterLock(-1,char(1)), c3dsrv.SetParameterLock(-1,char(1)));
assertEqual(btksrv.SetParameterDescription(-1,'testCopyParameter_Mutator'), c3dsrv.SetParameterDescription(-1,'testCopyParameter_Mutator'));
assertEqual(c3dsrv.RetrieveParameter(idxPr), 1);
assertEqual(btksrv.RetrieveParameter(idxPr), 1);
% Access
assertEqual(btksrv.GetParameterName(idxPr), c3dsrv.GetParameterName(idxPr));
assertEqual(btksrv.GetParameterType(idxPr), c3dsrv.GetParameterType(idxPr));
assertEqual(btksrv.GetParameterLock(idxPr), c3dsrv.GetParameterLock(idxPr));
assertEqual(c3dsrv.GetParameterNumberDim(idxPr), btksrv.GetParameterNumberDim(idxPr));
assertEqual(c3dsrv.GetParameterLength(idxPr), btksrv.GetParameterLength(idxPr));
for j=1:btksrv.GetParameterLength(idxPr); assertEqual(btksrv.GetParameterValue(idxPr, j-1), c3dsrv.GetParameterValue(idxPr, j-1)); end;
assertEqual(btksrv.GetParameterDescription(idxPr), c3dsrv.GetParameterDescription(idxPr));
assertEqual(btksrv.GetParameterNumber(idxPr), c3dsrv.GetParameterNumber(idxPr));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_FPC(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 11; % FORCE_PLATFORM:CORNERS
btkidx = 20; % FORCE_PLATFORM:CORNERS
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,0), c3dsrv.RemoveParameterData(c3didx,0));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_FPC_Last(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 11; % FORCE_PLATFORM:CORNERS
btkidx = 20; % FORCE_PLATFORM:CORNERS
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,1), c3dsrv.RemoveParameterData(c3didx,1));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_PD(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 0; % POINT:DESCRIPTIONS
btkidx = 0; % POINT:DESCRIPTIONS
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,0), c3dsrv.RemoveParameterData(c3didx,0));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_PU(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 30; % POINT:USED
btkidx = 5; % POINT:USED
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,0), c3dsrv.RemoveParameterData(c3didx,0));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_PX(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 1; % POINT:X_SCREEN
btkidx = 1; % POINT:X_SCREEN
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,0), c3dsrv.RemoveParameterData(c3didx,0));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_PX_Middle(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 1; % POINT:X_SCREEN
btkidx = 1; % POINT:X_SCREEN
% Modification
% The index 10 doesn't exist.
assertEqual(btksrv.RemoveParameterData(btkidx,10), c3dsrv.RemoveParameterData(c3didx,10));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_AS(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 5; % ANALOG:SCALE
btkidx = 12; % ANALOG:SCALE
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,0), c3dsrv.RemoveParameterData(c3didx,0));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_AS_Middle(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 5; % ANALOG:SCALE
btkidx = 12; % ANALOG:SCALE
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,10), c3dsrv.RemoveParameterData(c3didx,10));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testRemoveParameterData_AS_Last(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 5; % ANALOG:SCALE
btkidx = 12; % ANALOG:SCALE
% Modification
assertEqual(btksrv.RemoveParameterData(btkidx,31), c3dsrv.RemoveParameterData(c3didx,31));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterData_FPC(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 11; % FORCE_PLATFORM:CORNERS
btkidx = 20; % FORCE_PLATFORM:CORNERS
% Modification
assertEqual(btksrv.AddParameterData(btkidx,1), c3dsrv.AddParameterData(c3didx,1));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterData_FPC_error(d)
    function AddZeroData(itf, idx)
        itf.AddParameterData(idx, 0);
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 11; % FORCE_PLATFORM:CORNERS
btkidx = 20; % FORCE_PLATFORM:CORNERS
% Modification
assertExceptionThrown(@()AddZeroData(c3dsrv, c3didx), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()AddZeroData(btksrv, btkidx), 'btk:C3Dserver');
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterData_PD(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 0; % POINT:DESCRIPTIONS
btkidx = 0; % POINT:DESCRIPTIONS
% Modification
assertEqual(btksrv.AddParameterData(btkidx,5), c3dsrv.AddParameterData(c3didx,5));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterData_PU(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 30; % POINT:USED
btkidx = 5; % POINT:USED
% Modification
assertEqual(btksrv.AddParameterData(btkidx,1), c3dsrv.AddParameterData(c3didx,1));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterData_PX(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 1; % POINT:X_SCREEN
btkidx = 1; % POINT:X_SCREEN
% Modification
assertEqual(btksrv.AddParameterData(btkidx,2), c3dsrv.AddParameterData(c3didx,2));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddParameterData_AS(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
c3didx = 5; % ANALOG:SCALE
btkidx = 12; % ANALOG:SCALE
% Modification
assertEqual(btksrv.AddParameterData(btkidx,1), c3dsrv.AddParameterData(c3didx,1));
% Access
assertEqual(btksrv.GetParameterName(btkidx), c3dsrv.GetParameterName(c3didx));
assertEqual(btksrv.GetParameterType(btkidx), c3dsrv.GetParameterType(c3didx));
assertEqual(btksrv.GetParameterLock(btkidx), c3dsrv.GetParameterLock(c3didx));
assertEqual(c3dsrv.GetParameterNumberDim(c3didx), btksrv.GetParameterNumberDim(btkidx));
assertEqual(c3dsrv.GetParameterLength(c3didx), btksrv.GetParameterLength(btkidx));
for j=1:btksrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(btksrv.GetParameterDescription(btkidx), c3dsrv.GetParameterDescription(c3didx));
assertEqual(btksrv.GetParameterNumber(btkidx), c3dsrv.GetParameterNumber(c3didx));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_NoScale(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', num2str(frames(i)), num2str(subframes(j)), num2str(channels(k))));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_NoScale_ScaleModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the ANALOG:SCALE parameter
scales = rand(btksrv.GetAnalogChannels(),1);
btkidx = btksrv.GetParameterIndex('ANALOG','SCALE');
c3didx = c3dsrv.GetParameterIndex('ANALOG','SCALE');
for i = 1:length(scales)
    btksrv.SetParameterValue(btkidx,i-1,scales(i));
    c3dsrv.SetParameterValue(c3didx,i-1,scales(i));
end
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_NoScale_16Bits(d)
warning('OFF','btk:ReadAcquisition'); % To not display the warnings when you open this file with BTK. 
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename16bits,3);
btksrv.Open(d.filename16bits,3);  fprintf('\b'); %Remove the extra newlinw character added by BTK due to the warning(s).
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function testGetAnalogData_NoScale_OffsetModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the ANALOG:OFFSET parameter
offsets = round(2048 * rand(btksrv.GetAnalogChannels(),1));
btkidx = btksrv.GetParameterIndex('ANALOG','OFFSET');
c3didx = c3dsrv.GetParameterIndex('ANALOG','OFFSET');
for i = 1:length(offsets)
    btksrv.SetParameterValue(btkidx,i-1,offsets(i));
    c3dsrv.SetParameterValue(c3didx,i-1,offsets(i));
end
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_DefaultScale(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_DefaultScale_16Bits(d)
warning('OFF','btk:ReadAcquisition'); % To not display the warnings when you open this file with BTK.
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename16bits,3);
btksrv.Open(d.filename16bits,3); fprintf('\b'); %Remove the extra newlinw character added by BTK due to the warning(s).
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function testGetAnalogData_DefaultScale_ScaleModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the ANALOG:SCALE parameter
scales = rand(btksrv.GetAnalogChannels(),1);
btkidx = btksrv.GetParameterIndex('ANALOG','SCALE');
c3didx = c3dsrv.GetParameterIndex('ANALOG','SCALE');
for i = 1:length(scales)
    btksrv.SetParameterValue(btkidx,i-1,scales(i));
    c3dsrv.SetParameterValue(c3didx,i-1,scales(i));
end
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_DefaultScale_OffsetModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the ANALOG:OFFSET parameter
offsets = round(2048 * rand(btksrv.GetAnalogChannels(),1));
btkidx = btksrv.GetParameterIndex('ANALOG','OFFSET');
c3didx = c3dsrv.GetParameterIndex('ANALOG','OFFSET');
for i = 1:length(offsets)
    btksrv.SetParameterValue(btkidx,i-1,offsets(i));
    c3dsrv.SetParameterValue(c3didx,i-1,offsets(i));
end
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_DefaultScale_ScaleRemoved(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Remove the ANALOG:SCALE parameter
btksrv.DeleteParameter(btksrv.GetParameterIndex('ANALOG','SCALE'));
c3dsrv.DeleteParameter(c3dsrv.GetParameterIndex('ANALOG','SCALE'));
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_DefaultScale_FirstFrame(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Shift the first frame to test our code.
btkSetFirstFrame(btksrv.GetHandle(), 43);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames2 = frames - 42;
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames2(i),subframes(j),char(1),0,0,char(0)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_UserScale(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(...
                c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),24,0.5,char(1)), ...
                btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),24,0.5,char(1)), ...
                sprintf('Frame %03i ; Subframe %i ; Channel %02i', num2str(frames(i)), num2str(subframes(j)), num2str(channels(k))));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_UserScale_ScaleModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the ANALOG:SCALE parameter
scales = rand(btksrv.GetAnalogChannels(),1);
btkidx = btksrv.GetParameterIndex('ANALOG','SCALE');
c3didx = c3dsrv.GetParameterIndex('ANALOG','SCALE');
for i = 1:length(scales)
    btksrv.SetParameterValue(btkidx,i-1,scales(i));
    c3dsrv.SetParameterValue(c3didx,i-1,scales(i));
end
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0.2,char(1)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),0,0.2,char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogData_UserScale_OffsetModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the ANALOG:OFFSET parameter
offsets = round(2048 * rand(btksrv.GetAnalogChannels(),1));
btkidx = btksrv.GetParameterIndex('ANALOG','OFFSET');
c3didx = c3dsrv.GetParameterIndex('ANALOG','OFFSET');
for i = 1:length(offsets)
    btksrv.SetParameterValue(btkidx,i-1,offsets(i));
    c3dsrv.SetParameterValue(c3didx,i-1,offsets(i));
end
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),128,0.5,char(1)), ...
                        btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(0),128,0.5,char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogDataEx_NoScale(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function xtestGetAnalogDataEx_NoScale_16Bits(d)
warning('OFF','btk:ReadAcquisition');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename16bits,3);
btksrv.Open(d.filename16bits,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function testGetAnalogDataEx_NoScale_FloatFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogDataEx_DefaultScale(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function xtestGetAnalogDataEx_DefaultScale_16Bits(d)
warning('OFF','btk:ReadAcquisition');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename16bits,3);
btksrv.Open(d.filename16bits,3); fprintf('\b'); %Remove the extra newlinw character added by BTK due to the warning(s).
channels = [0 1]; % Only 2 channels in this acquisitions
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:100:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:100:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function testGetAnalogDataEx_DefaultScale_FloatFormat(d)
warning('OFF','btk:ReadAcquisition');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3); fprintf('\b'); %Remove the extra newlinw character added by BTK due to the warning(s).
channels = [0 1]; % Only 2 channels in this acquisitions
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:100:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:100:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function testGetAnalogDataEx_DefaultScale_OffsetModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Modify the ANALOG:OFFSET parameter
offsets = round(2048 * rand(btksrv.GetAnalogChannels(),1));
btkidx = btksrv.GetParameterIndex('ANALOG','OFFSET');
c3didx = c3dsrv.GetParameterIndex('ANALOG','OFFSET');
for i = 1:length(offsets)
    btksrv.SetParameterValue(btkidx,i-1,offsets(i));
    c3dsrv.SetParameterValue(c3didx,i-1,offsets(i));
end
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogDataEx_DefaultScale_ScaleModified(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Modify the ANALOG:SCALE parameter
scales = rand(btksrv.GetAnalogChannels(),1);
btkidx = btksrv.GetParameterIndex('ANALOG','SCALE');
c3didx = c3dsrv.GetParameterIndex('ANALOG','SCALE');
for i = 1:length(scales)
    btksrv.SetParameterValue(btkidx,i-1,scales(i));
    c3dsrv.SetParameterValue(c3didx,i-1,scales(i));
end
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogDataEx_DefaultScale_FirstFrame(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Shift the first frame for BTK
btkSetFirstFrame(btksrv.GetHandle(), 43);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1),0,0,char(0));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetAnalogDataEx_UserScale(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),512,0.1,char(1));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),512,0.1,char(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0),512,0.1,char(1));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0),512,0.1,char(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function xtestGetAnalogDataEx_UserScale_16bits(d)
warning('OFF','btk:ReadAcquisition');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename16bits,3);
btksrv.Open(d.filename16bits,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),512,0.1,char(1));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),512,0.1,char(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0),512,0.1,char(1));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0),512,0.1,char(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function testGetAnalogDataEx_UserScale_FloatFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
num = 5;
% Generate random channel
channels = btksrv.GetAnalogChannels() * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),512,0.1,char(1));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),512,0.1,char(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0),512,0.1,char(1));
    d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0),512,0.1,char(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetAnalogData(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random subframe
subframes = ceil(rand(1,num) * btksrv.GetAnalogVideoRatio());
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for j=1:numel(subframes)
        for k=1:numel(channels)
            val = round(2048*rand(1)-1024);
            % Set
            assertEqual(...
                c3dsrv.SetAnalogData(channels(k),frames(i),subframes(j),val), ...
                btksrv.SetAnalogData(channels(k),frames(i),subframes(j),val));
            % Get
            assertEqual(...
                c3dsrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                btksrv.GetAnalogData(channels(k),frames(i),subframes(j),char(1),0,0,char(0)), ...
                sprintf('Frame %03i ; Subframe %i ; Channel %02i', num2str(frames(i)), num2str(subframes(j)), num2str(channels(k))));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetAnalogDataEx(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
% Generate random sample number
samples = round(rand(1,num) * btkGetAnalogFrameNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        val = int16(round(2048*rand(1,samples(k))-1024));
        % Set
        assertEqual(...
            c3dsrv.SetAnalogDataEx(channels(k),frames(i),val), ...
            btksrv.SetAnalogDataEx(channels(k),frames(i),val));
        % Get
        d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
        d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
        assertEqual(d1, d2);
        for j = 1:length(d1)
            assertEqual(d1{j}, d2{j}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteAnalogChannel(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for i=1:btksrv.GetAnalogChannels()
    assertEqual(btksrv.GetAnalogChannels(), c3dsrv.GetAnalogChannels());
    assertEqual(c3dsrv.GetAnalogData(0,1,1,char(0),0,0,char(0)), ...
                        btksrv.GetAnalogData(0,1,1,char(0),0,0,char(0)));
    assertEqual(btksrv.GetParameterValue(btksrv.GetParameterIndex('ANALOG','USED'),0), ...
                c3dsrv.GetParameterValue(c3dsrv.GetParameterIndex('ANALOG','USED'),0));
    assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','SCALE')), ...
                c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','SCALE')));
    assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','OFFSET')), ...
                c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','OFFSET')));
    assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','LABELS')), ...
                c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
    assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','DESCRIPTIONS')), ...
                c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','DESCRIPTIONS')));
    c3dsrv.DeleteAnalogChannel(0);
    btksrv.DeleteAnalogChannel(0);
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteAnalogChannelWithParameter(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.GetAnalogChannels(), btksrv.GetAnalogChannels());
assertEqual(c3dsrv.DeleteAnalogChannelWithParam(6), btksrv.DeleteAnalogChannelWithParam(6));
assertEqual(c3dsrv.GetAnalogChannels(), btksrv.GetAnalogChannels());
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetAnalogNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),1,char(0),0,0,char(0)), ...
                    btksrv.GetAnalogData(channels(k),frames(i),1,char(0),0,0,char(0)));
    end
end
btkidx = btksrv.GetParameterIndex('ANALOG','USED'); c3didx = c3dsrv.GetParameterIndex('ANALOG','USED');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','LABELS'); c3didx = c3dsrv.GetParameterIndex('ANALOG','LABELS');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','DESCRIPTIONS'); c3didx = c3dsrv.GetParameterIndex('ANALOG','DESCRIPTIONS');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','SCALE'); c3didx = c3dsrv.GetParameterIndex('ANALOG','SCALE');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','OFFSET'); c3didx = c3dsrv.GetParameterIndex('ANALOG','OFFSET');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('ANALOG','UNITS'); c3didx = c3dsrv.GetParameterIndex('ANALOG','UNITS');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','CHANNEL'); c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','CHANNEL');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetAVRatio(d)
    function setAVRatioWithChannel(itf)
        itf.SetAVRatio(5);
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()setAVRatioWithChannel(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()setAVRatioWithChannel(btksrv), 'btk:C3Dserver');
for i=1:c3dsrv.GetAnalogChannels();c3dsrv.DeleteAnalogChannel(0); end;
for i=1:btksrv.GetAnalogChannels();btksrv.DeleteAnalogChannel(0); end;
assertEqual(c3dsrv.SetAVRatio(5), btksrv.SetAVRatio(5));
assertEqual(c3dsrv.GetAnalogVideoRatio(), btksrv.GetAnalogVideoRatio());
assertEqual(c3dsrv.SetAVRatio(5.2), btksrv.SetAVRatio(5.2));
assertEqual(c3dsrv.GetAnalogVideoRatio(), btksrv.GetAnalogVideoRatio());
assertEqual(c3dsrv.SetAVRatio(5.5), btksrv.SetAVRatio(5.5));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
assertEqual(c3dsrv.GetAnalogVideoRatio(), btksrv.GetAnalogVideoRatio());
c3dsrv.Close;
btksrv.Close();
end

function testAddAnalogChannel(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.AddAnalogChannel(), btksrv.AddAnalogChannel());
assertEqual(btksrv.GetAnalogChannels(), c3dsrv.GetAnalogChannels());
assertEqual(c3dsrv.GetAnalogData(c3dsrv.GetAnalogChannels()-1,1,1,char(0),0,0,char(0)), ...
    btksrv.GetAnalogData(btksrv.GetAnalogChannels()-1,1,1,char(0),0,0,char(0)));
assertEqual(btksrv.GetParameterValue(btksrv.GetParameterIndex('ANALOG','USED'),0), ...
    c3dsrv.GetParameterValue(c3dsrv.GetParameterIndex('ANALOG','USED'),0));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','SCALE')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','SCALE')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','OFFSET')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','OFFSET')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','LABELS')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','DESCRIPTIONS')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','DESCRIPTIONS')));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddDelAnalogChannel(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.AddAnalogChannel(), btksrv.AddAnalogChannel());
assertEqual(c3dsrv.DeleteAnalogChannel(btksrv.GetAnalogChannels()-1), btksrv.DeleteAnalogChannel(btksrv.GetAnalogChannels()-1));
assertEqual(btksrv.GetAnalogChannels(), c3dsrv.GetAnalogChannels());
assertEqual(c3dsrv.GetAnalogData(c3dsrv.GetAnalogChannels()-1,1,1,char(0),0,0,char(0)), ...
    btksrv.GetAnalogData(btksrv.GetAnalogChannels()-1,1,1,char(0),0,0,char(0)));
assertEqual(btksrv.GetParameterValue(btksrv.GetParameterIndex('ANALOG','USED'),0), ...
    c3dsrv.GetParameterValue(c3dsrv.GetParameterIndex('ANALOG','USED'),0));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','SCALE')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','SCALE')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','OFFSET')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','OFFSET')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','LABELS')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','LABELS')));
assertEqual(btksrv.GetParameterLength(btksrv.GetParameterIndex('ANALOG','DESCRIPTIONS')), ...
    c3dsrv.GetParameterLength(c3dsrv.GetParameterIndex('ANALOG','DESCRIPTIONS')));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function xtestGetForceData(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for k=0:1
    for j=0:2
        % Full frames (from -1 to -1)
        d1 = c3dsrv.GetForceData(j,k,-1,-1);
        d2 = btksrv.GetForceData(j,k,-1,-1);
%         assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Full frames
        d1 = c3dsrv.GetForceData(j,k,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
        d2 = btksrv.GetForceData(j,k,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Ranged frames
        d1 = c3dsrv.GetForceData(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50);
        d2 = btksrv.GetForceData(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50);
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function xtestGetMomentData(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
for k=0:1
    for j=0:2
        % Full frames (from -1 to -1)
        d1 = c3dsrv.GetMomentData(j,k,-1,-1);
        d2 = btksrv.GetMomentData(j,k,-1,-1);
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Full frames
        d1 = c3dsrv.GetMomentData(j,k,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
        d2 = btksrv.GetMomentData(j,k,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Ranged frames
        d1 = c3dsrv.GetMomentData(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50);
        d2 = btksrv.GetMomentData(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50);
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testZeroAnalogChannel_Method1(d)
prototypeZeroAnalogChannel(d.filename, 1);
prototypeZeroAnalogChannel(d.filename2, 1);
% prototypeZeroAnalogChannel(d.filename16bits, 1);
end

function testZeroAnalogChannel_Method2(d)
prototypeZeroAnalogChannel(d.filename, 2);
prototypeZeroAnalogChannel(d.filename2, 2);
% prototypeZeroAnalogChannel(d.filename16bits, 1);
end

function testZeroAnalogChannel_Method3(d)
prototypeZeroAnalogChannel(d.filename, 3);
prototypeZeroAnalogChannel(d.filename2, 3);
% prototypeZeroAnalogChannel(d.filename16bits, 1);
end

function testZeroAnalogChannel_Method4(d)
prototypeZeroAnalogChannel(d.filename, 4);
prototypeZeroAnalogChannel(d.filename2, 4);
% prototypeZeroAnalogChannel(d.filename16bits, 1);
end

function testZeroAnalogChannel_Method2_ShiftedFPZERO(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Moify the parameter FORCE_PLATFORM:ZERO
btkidx = btksrv.GetParameterIndex('FORCE_PLATFORM','ZERO');
c3didx = c3dsrv.GetParameterIndex('FORCE_PLATFORM','ZERO');
btksrv.SetParameterValue(btkidx,0,5); btksrv.SetParameterValue(btkidx,1,15);
c3dsrv.SetParameterValue(c3didx,0,5); c3dsrv.SetParameterValue(c3didx,1,15);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for k=1:numel(channels)
        % Set
        assertEqual(...
            c3dsrv.ZeroAnalogChannel(channels(k),2, frames(i)), ...
            btksrv.ZeroAnalogChannel(channels(k),2, frames(i)));
        % Get
        d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
        d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
        assertEqual(d1, d2);
        for j = 1:length(d1)
            assertEqual(d1{j}, d2{j}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testZeroAnalogChannel_Method2_NoFPZERO(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Remove the parameter FORCE_PLATFORM:ZERO
btksrv.DeleteParameter(btksrv.GetParameterIndex('FORCE_PLATFORM','ZERO'));
c3dsrv.DeleteParameter(c3dsrv.GetParameterIndex('FORCE_PLATFORM','ZERO'));
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for k=1:numel(channels)
        % Set
        assertEqual(...
            c3dsrv.ZeroAnalogChannel(channels(k),2, frames(i)), ...
            btksrv.ZeroAnalogChannel(channels(k),2, frames(i)));
        % Get
        d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
        d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
        assertEqual(d1, d2);
        for j = 1:length(d1)
            assertEqual(d1{j}, d2{j}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end


function testGetPointData_NoScale_IntegerFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(0)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_NoScale_RealFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(0)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(0)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_DefaultScale_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_DefaultScale_Real(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_DefaultScale_ScaleModified_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modify the POINT:SCALE parameter
btkidx = btksrv.GetParameterIndex('POINT','SCALE');
c3didx = c3dsrv.GetParameterIndex('POINT','SCALE');
btksrv.SetParameterValue(btkidx,0,0.4);
c3dsrv.SetParameterValue(c3didx,0,0.4);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_DefaultScale_ScaleModified_Real(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Modify the POINT:SCALE parameter
btkidx = btksrv.GetParameterIndex('POINT','SCALE');
c3didx = c3dsrv.GetParameterIndex('POINT','SCALE');
btksrv.SetParameterValue(btkidx,0,0.4);
c3dsrv.SetParameterValue(c3didx,0,0.4);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_DefaultScale_ScaleRemoved_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Remove the ANALOG:SCALE parameter
btksrv.DeleteParameter(btksrv.GetParameterIndex('POINT','SCALE'));
c3dsrv.DeleteParameter(c3dsrv.GetParameterIndex('POINT','SCALE'));
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointData_DefaultScale_ScaleRemoved_Real(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Remove the ANALOG:SCALE parameter
btksrv.DeleteParameter(btksrv.GetParameterIndex('POINT','SCALE'));
c3dsrv.DeleteParameter(c3dsrv.GetParameterIndex('POINT','SCALE'));
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random cord
cord = floor(rand(1,num) * 3);
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for j=1:numel(cord)
        for k=1:numel(channels)
            assertEqual(c3dsrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cord(j),frames(i),char(1)), ...
                        sprintf('Frame %03i ; Subframe %i ; Channel %02i', i,j,k));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointDataEx_NoScale_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    for j=0:2
        % Full frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Ranged frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointDataEx_NoScale_Real(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    for j=0:2
        % Full frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Ranged frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(0));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(0));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointDataEx_Scaled_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    for j=0:2
        % Full frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Ranged frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointDataEx_Scaled_Real(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    for j=0:2
        % Full frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(1));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(1));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
        % Ranged frames
        d1 = c3dsrv.GetPointDataEx(channels(k),j,c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50,char(1));
        d2 = btksrv.GetPointDataEx(channels(k),j,btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50,char(1));
        assertEqual(d1, d2);
        for i = 1:length(d1)
            assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointResidualEx_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetPointResidualEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
    d2 = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
    %assertEqual(d1, d2);
    for i = 1:length(d1)
        assertElementsAlmostEqual(d1{i}, d2{i}, 'absolute', 1e-5);
    end
    % Ranged frames
    d1 = c3dsrv.GetPointResidualEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50);
    d2 = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50);
    %assertEqual(d1, d2);
    for i = 1:length(d1)
        assertElementsAlmostEqual(d1{i}, d2{i}, 'absolute', 1e-5);
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointResidualEx_Float(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetPointResidualEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
    d2 = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
    %assertEqual(d1, d2);
    for i = 1:length(d1)
        assertElementsAlmostEqual(d1{i}, d2{i}, 'absolute', 1e-5);
    end
    % Ranged frames
    d1 = c3dsrv.GetPointResidualEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50);
    d2 = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50);
    %assertEqual(d1, d2);
    for i = 1:length(d1)
        assertElementsAlmostEqual(d1{i}, d2{i}, 'absolute', 1e-5);
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

% Removed as the support of the camera mask was removed from BTK
function xtestGetPointMaskEx_Integer(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
num = 5;
% Generate random channel
channels = btkGetPointNumber(btksrv.GetHandle()) * floor(rand(1,num));
for k=1:numel(channels)
    % Full frames
    d1 = c3dsrv.GetPointMaskEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
    d2 = btksrv.GetPointMaskEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
    % Ranged frames
    d1 = c3dsrv.GetPointMaskEx(channels(k),c3dsrv.GetVideoFrame(0)+50,c3dsrv.GetVideoFrame(1)-50);
    d2 = btksrv.GetPointMaskEx(channels(k),btksrv.GetVideoFrame(0)+50,btksrv.GetVideoFrame(1)-50);
    assertEqual(d1, d2);
    for i = 1:length(d1)
        assertEqual(d1{i}, d2{i}, sprintf('Index %03i',i));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointResidual_IntegerFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertElementsAlmostEqual(...
            c3dsrv.GetPointResidual(channels(k),frames(i)), ...
            btksrv.GetPointResidual(channels(k),frames(i)),...
            'absolute', 1e-5);
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testGetPointResidual_RealFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertElementsAlmostEqual(...
            c3dsrv.GetPointResidual(channels(k),frames(i)), ...
            btksrv.GetPointResidual(channels(k),frames(i)),...
            'absolute', 1e-5);
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

% Removed as the support of the camera mask was removed from BTK
function xtestGetPointMask_IntegerFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
                    btksrv.GetPointMask(channels(k),frames(i)));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

% Removed as the support of the camera mask was removed from BTK
function xtestGetPointMask_FloatFormat(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
                    btksrv.GetPointMask(channels(k),frames(i)));
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testSetPointData_IntegerFormat(d)
warning('OFF','btk:C3Dserver:FakeMask');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Modifier
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
% General cord channel
cords = floor(rand(1,num) * 3);
for i=1:numel(frames)
    for j=1:numel(cords)
        for k=1:numel(channels)
            % Set
            val = round(2048*rand(1)-1024);
            assertEqual(...
                c3dsrv.SetPointData(channels(k),cords(j),frames(i),val), ...
                btksrv.SetPointData(channels(k),cords(j),frames(i),val));
            val = rand(1);
            assertEqual(...
                c3dsrv.SetPointData(channels(k),3,frames(i),val), ...
                btksrv.SetPointData(channels(k),3,frames(i),val));
            val = dec2bin(floor(rand(1)*128),7);
            assertEqual(...
                c3dsrv.SetPointData(channels(k),4,frames(i),val), ...
                btksrv.SetPointData(channels(k),4,frames(i),val));
            % Get (unscaled & scaled)
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(0)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(0)));
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(1)));
            assertElementsAlmostEqual(...
                        c3dsrv.GetPointResidual(channels(k),frames(i)), ...
                        btksrv.GetPointResidual(channels(k),frames(i)),...
                        'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%            assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
%                        btksrv.GetPointMask(channels(k),frames(i)));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:FakeMask');
end

function testSetPointData_FloatFormat(d)
warning('OFF','btk:C3Dserver:FakeMask');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Modifier
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
% Generate random cord
cords = floor(rand(1,num) * 3);
for i=1:numel(frames)
    for j=1:numel(cords)
        for k=1:numel(channels)
            % Set
            val = round(2048*rand(1)-1024);
            assertEqual(...
                c3dsrv.SetPointData(channels(k),cords(j),frames(i),val), ...
                btksrv.SetPointData(channels(k),cords(j),frames(i),val));
            val = rand(1);
            assertEqual(...
                c3dsrv.SetPointData(channels(k),3,frames(i),val), ...
                btksrv.SetPointData(channels(k),3,frames(i),val));
            val = dec2bin(floor(rand(1)*128),7);
            assertEqual(...
                c3dsrv.SetPointData(channels(k),4,frames(i),val), ...
                btksrv.SetPointData(channels(k),4,frames(i),val));
            % Get (unscaled & scaled)
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(0)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(0)));
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(1)));
            assertElementsAlmostEqual(...
                        c3dsrv.GetPointResidual(channels(k),frames(i)), ...
                        btksrv.GetPointResidual(channels(k),frames(i)),...
                        'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%            assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
%                        btksrv.GetPointMask(channels(k),frames(i)));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:FakeMask');
end

function testSetPointDataEx_Integer(d)
warning('OFF','btk:C3Dserver:FakeMask');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
% Generate random sample number
samples = round(rand(1,num) * btkGetPointFrameNumber(btksrv.GetHandle()));
% Generate random cord
cords = floor(rand(1,num) * 3);
for i=1:numel(frames)
    for j = 1:numel(cords)
        for k=1:numel(channels)
            % Set
            val = int16(round(2048*rand(1,samples(k))-1024));
            assertEqual(...
                c3dsrv.SetPointDataEx(channels(k),cords(j),frames(i),val), ...
                btksrv.SetPointDataEx(channels(k),cords(j),frames(i),val));
            val = single(rand(1,samples(k)));
            assertEqual(...
                c3dsrv.SetPointDataEx(channels(k),3,frames(i),val), ...
                btksrv.SetPointDataEx(channels(k),3,frames(i),val));
%             val = dec2bin(round(rand(1,samples(k)) * 128),7);
%             assertEqual(...
%                 c3dsrv.SetPointDataEx(channels(k),4,frames(i),val), ...
%                 btksrv.SetPointDataEx(channels(k),4,frames(i),val));
            % Get
            d1X = c3dsrv.GetPointDataEx(channels(k),0,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
            d2X = btksrv.GetPointDataEx(channels(k),0,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
            d1Y = c3dsrv.GetPointDataEx(channels(k),1,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
            d2Y = btksrv.GetPointDataEx(channels(k),1,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
            d1Z = c3dsrv.GetPointDataEx(channels(k),2,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
            d2Z = btksrv.GetPointDataEx(channels(k),2,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
            d1R = c3dsrv.GetPointResidualEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
            d2R = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
% Removed as the support of the camera mask was removed from BTK
%            d1M = c3dsrv.GetPointMaskEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
%            d2M = btksrv.GetPointMaskEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
            for l = 1:length(d1X)
                assertEqual(d1X{l}, d2X{l});
                assertEqual(d1Y{l}, d2Y{l});
                assertEqual(d1Z{l}, d2Z{l});
                assertElementsAlmostEqual(d1R{l}, d2R{l}, 'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%                assertEqual(d1M{l}, d2M{l});
            end
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:FakeMask');
end

function testSetPointDataEx_Float(d)
warning('OFF','btk:C3Dserver:FakeMask');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename2,3);
btksrv.Open(d.filename2,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
% Generate random sample number
samples = round(rand(1,num) * btkGetPointFrameNumber(btksrv.GetHandle()));
% Generate random cord
cords = floor(rand(1,num) * 3);
for i=1:numel(frames)
    for j = 1:numel(cords)
        for k=1:numel(channels)
            % Set
            val = single(1000 * rand(1,samples(k)));
            assertEqual(...
                c3dsrv.SetPointDataEx(channels(k),cords(j),frames(i),val), ...
                btksrv.SetPointDataEx(channels(k),cords(j),frames(i),val));
            val = single(rand(1,samples(k)));
            assertEqual(...
                c3dsrv.SetPointDataEx(channels(k),3,frames(i),val), ...
                btksrv.SetPointDataEx(channels(k),3,frames(i),val));
%             val = dec2bin(round(rand(1,samples(k)) * 128),7);
%             assertEqual(...
%                 c3dsrv.SetPointDataEx(channels(k),4,frames(i),val), ...
%                 btksrv.SetPointDataEx(channels(k),4,frames(i),val));
            % Get
            d1X = c3dsrv.GetPointDataEx(channels(k),0,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
            d2X = btksrv.GetPointDataEx(channels(k),0,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
            d1Y = c3dsrv.GetPointDataEx(channels(k),1,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
            d2Y = btksrv.GetPointDataEx(channels(k),1,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
            d1Z = c3dsrv.GetPointDataEx(channels(k),2,c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0));
            d2Z = btksrv.GetPointDataEx(channels(k),2,btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0));
            d1R = c3dsrv.GetPointResidualEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
            d2R = btksrv.GetPointResidualEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
% Removed as the support of the camera mask was removed from BTK
%            d1M = c3dsrv.GetPointMaskEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1));
%            d2M = btksrv.GetPointMaskEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1));
            for l = 1:length(d1X)
                assertEqual(d1X{l}, d2X{l});
                assertEqual(d1Y{l}, d2Y{l});
                assertEqual(d1Z{l}, d2Z{l});
                assertElementsAlmostEqual(d1R{l}, d2R{l}, 'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%                assertEqual(d1M{l}, d2M{l});
            end
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:C3Dserver:FakeMask');
end

function testAddMarker(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
assertEqual(c3dsrv.AddMarker(), btksrv.AddMarker()); % Same index?
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close(); 
end

function testDeleteMarker(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
assertEqual(c3dsrv.DeleteMarker(3), btksrv.DeleteMarker(3)); % Same index?
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        cords = floor(rand(1,num) * 3);
        for j = 1:numel(cords)
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(1)));
        end
            assertElementsAlmostEqual(...
                    c3dsrv.GetPointResidual(channels(k),frames(i)), ...
                    btksrv.GetPointResidual(channels(k),frames(i)),...
                    'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%            assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
%                    btksrv.GetPointMask(channels(k),frames(i)));
    end
end
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteMarker_WrongIndex(d)
    function wrongIndex(itf)
        itf.DeleteMarker(999);
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()wrongIndex(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()wrongIndex(btksrv), 'btk:C3Dserver');
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteMarkerWithParameter(d)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
assertEqual(c3dsrv.DeleteMarkerWithParam(3), btksrv.DeleteMarkerWithParam(3));
assertEqual(c3dsrv.GetNumber3DPoints(), btksrv.GetNumber3DPoints());
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        cords = floor(rand(1,num) * 3);
        for j = 1:numel(cords)
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(1)));
        end
            assertElementsAlmostEqual(...
                    c3dsrv.GetPointResidual(channels(k),frames(i)), ...
                    btksrv.GetPointResidual(channels(k),frames(i)),...
                    'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%            assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
%                    btksrv.GetPointMask(channels(k),frames(i)));
    end
end
btkidx = btksrv.GetParameterIndex('POINT','USED'); c3didx = c3dsrv.GetParameterIndex('POINT','USED');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('POINT','LABELS'); c3didx = c3dsrv.GetParameterIndex('POINT','LABELS');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
btkidx = btksrv.GetParameterIndex('POINT','DESCRIPTIONS'); c3didx = c3dsrv.GetParameterIndex('POINT','DESCRIPTIONS');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testAddFrames_Begin1(d)
prototypeAddFrames(d.filename, 1, -1);
end

function testAddFrames_Begin10(d)
prototypeAddFrames(d.filename, 10, -1);
end

function testAddFrames_End1(d)
prototypeAddFrames(d.filename, 1, -2);
end

function testAddFrames_End10(d)
prototypeAddFrames(d.filename, 10, -2);
end

function testDeleteFrames_1_1(d)
prototypeDeleteFrames(d.filename, 1, 1);
end

function testDeleteFrames_1_10(d)
prototypeDeleteFrames(d.filename, 1, 10);
end

function testDeleteFrames_450_1(d)
prototypeDeleteFrames(d.filename, 450, 1);
end

function testDeleteFrames_450_10(d)
    function wrongNumFrames(itf)
        itf.DeleteFrames(450,10);
    end
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(d.filename,3);
btksrv.Open(d.filename,3);
assertExceptionThrown(@()wrongNumFrames(c3dsrv), 'MATLAB:COM:E2147500037');
assertExceptionThrown(@()wrongNumFrames(btksrv), 'btk:C3Dserver');
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function testDeleteFrames_440_10(d)
prototypeDeleteFrames(d.filename, 440, 10);
end

function testDeleteFrames_200_200(d)
prototypeDeleteFrames(d.filename, 200, 200);
end

% -------------------------------------------------------------------------
%                                   UTILS
% -------------------------------------------------------------------------
function prototypeZeroAnalogChannel(filename, method)
warning('OFF','btk:ReadAcquisition');
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(filename,3);
btksrv.Open(filename,3);
% Generate random frame number
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btksrv.GetAnalogChannels());
for i=1:numel(frames)
    for k=1:numel(channels)
        % Set
        assertEqual(...
            c3dsrv.ZeroAnalogChannel(channels(k),method, frames(i)), ...
            btksrv.ZeroAnalogChannel(channels(k),method, frames(i)));
        % Get
        d1 = c3dsrv.GetAnalogDataEx(channels(k),c3dsrv.GetVideoFrame(0),c3dsrv.GetVideoFrame(1),char(0),0,0,char(0));
        d2 = btksrv.GetAnalogDataEx(channels(k),btksrv.GetVideoFrame(0),btksrv.GetVideoFrame(1),char(0),0,0,char(0));
%         assertEqual(d1, d2);
        for j = 1:length(d1)
            assertEqual(d1{j}, d2{j}, sprintf('Index %03i',i));
        end
    end
end
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
warning('ON','btk:ReadAcquisition');
end

function prototypeAddFrames(filename, frame, idx)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(filename,3);
btksrv.Open(filename,3);
assertEqual(c3dsrv.GetVideoFrame(0), btksrv.GetVideoFrame(0));
assertEqual(c3dsrv.GetVideoFrame(1), btksrv.GetVideoFrame(1));
assertEqual(c3dsrv.AddFrames(frame,idx), btksrv.AddFrames(frame,idx));
assertEqual(c3dsrv.GetVideoFrame(0), btksrv.GetVideoFrame(0));
assertEqual(c3dsrv.GetVideoFrame(1), btksrv.GetVideoFrame(1));
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        cords = floor(rand(1,num) * 3);
        for j = 1:numel(cords)
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(1)));
        end
        assertElementsAlmostEqual(...
                c3dsrv.GetPointResidual(channels(k),frames(i)), ...
                btksrv.GetPointResidual(channels(k),frames(i)),...
                'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%        assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
%                btksrv.GetPointMask(channels(k),frames(i)));
    end
end
channels = floor(rand(1,num) * btkGetAnalogNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),1,char(0),0,0,char(0)), ...
                    btksrv.GetAnalogData(channels(k),frames(i),1,char(0),0,0,char(0)));
    end
end
btkidx = btksrv.GetParameterIndex('POINT','FRAMES'); c3didx = c3dsrv.GetParameterIndex('POINT','FRAMES');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end

function prototypeDeleteFrames(filename, frame, idx)
c3dsrv = actxserver('C3Dserver.C3D');
btksrv = btkEmulateC3Dserver();
c3dsrv.Open(filename,3);
btksrv.Open(filename,3);
assertEqual(c3dsrv.GetVideoFrame(0), btksrv.GetVideoFrame(0));
assertEqual(c3dsrv.GetVideoFrame(1), btksrv.GetVideoFrame(1));
assertEqual(c3dsrv.DeleteFrames(frame,idx), btksrv.DeleteFrames(frame,idx));
assertEqual(c3dsrv.GetVideoFrame(0), btksrv.GetVideoFrame(0));
assertEqual(c3dsrv.GetVideoFrame(1), btksrv.GetVideoFrame(1));
num = 5;
frames = btksrv.GetVideoFrame(0) + ceil(rand(1,num) * (btksrv.GetVideoFrame(1) - btksrv.GetVideoFrame(0)));
frames = [btksrv.GetVideoFrame(0), btksrv.GetVideoFrame(1), frames];
% Generate random channel
channels = floor(rand(1,num) * btkGetPointNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        cords = floor(rand(1,num) * 3);
        for j = 1:numel(cords)
            assertEqual(c3dsrv.GetPointData(channels(k),cords(j),frames(i),char(1)), ...
                        btksrv.GetPointData(channels(k),cords(j),frames(i),char(1)));
        end
        assertElementsAlmostEqual(...
                c3dsrv.GetPointResidual(channels(k),frames(i)), ...
                btksrv.GetPointResidual(channels(k),frames(i)),...
                'absolute', 1e-5);
% Removed as the support of the camera mask was removed from BTK
%        assertEqual(c3dsrv.GetPointMask(channels(k),frames(i)), ...
%                btksrv.GetPointMask(channels(k),frames(i)));
    end
end
channels = floor(rand(1,num) * btkGetAnalogNumber(btksrv.GetHandle()));
for i=1:numel(frames)
    for k=1:numel(channels)
        assertEqual(c3dsrv.GetAnalogData(channels(k),frames(i),1,char(0),0,0,char(0)), ...
                    btksrv.GetAnalogData(channels(k),frames(i),1,char(0),0,0,char(0)));
    end
end
btkidx = btksrv.GetParameterIndex('POINT','FRAMES'); c3didx = c3dsrv.GetParameterIndex('POINT','FRAMES');
assertEqual(btksrv.GetParameterLength(btkidx), c3dsrv.GetParameterLength(c3didx));
for j=1:c3dsrv.GetParameterLength(c3didx); assertEqual(btksrv.GetParameterValue(btkidx, j-1), c3dsrv.GetParameterValue(c3didx, j-1)); end;
assertEqual(double(c3dsrv.IsModified()), double(btksrv.IsModified()));
c3dsrv.Close;
btksrv.Close();
end


