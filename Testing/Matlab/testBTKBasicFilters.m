function test_suite = testBTKBasicFilters
initTestSuite;
end

function d = setup
d = TDDConfigure();
end

function testGetMarkersResiduals(d)
h = btkNewAcquisition(2,10);
res = btkGetMarkersResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), 2);
res_ = zeros(10,2);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testSetMarkersResiduals(d)
h = btkNewAcquisition(2,10);
res_ = rand(10,2);
btkSetMarkersResiduals(h, res_);
res = btkGetMarkersResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), 2);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testAppendForcePlatformType2(d)
h = btkNewAcquisition(0,10);
btkSetFrequency(h,1000);
force = rand(10,3);
moment = rand(10,3);
corners = [-200,200,0;200,200,0;-200,-200,0;200,-200,0];
btkAppendForcePlatformType2(h, force, moment, corners,[],1)
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 10);
assertEqual(btkGetPointNumber(h), 0);
assertEqual(btkGetPointFrameNumber(h), 10);
assertEqual(btkGetAnalogNumber(h), 6);
assertEqual(btkGetAnalogFrameNumber(h), 10);
[fp fpi] = btkGetForcePlatforms(h);
assertEqual(length(fp), 1);
assertEqual(length(fpi), 1);
assertEqual(fp(1).corners, corners');
assertEqual(fp(1).channels.Fx1, force(:,1));
assertEqual(fp(1).channels.Fy1, force(:,2));
assertEqual(fp(1).channels.Fz1, force(:,3));
assertEqual(fp(1).channels.Mx1, moment(:,1));
assertEqual(fp(1).channels.My1, moment(:,2));
assertEqual(fp(1).channels.Mz1, moment(:,3));
assertEqual(fp(1).origin, [0;0;0]);
assertEqual(fp(1).type, 2);
assertEqual(fpi(1).frequency, 1000);
assertEqual(fpi(1).units.Fx1, 'N');
assertEqual(fpi(1).units.Fy1, 'N');
assertEqual(fpi(1).units.Fz1, 'N');
assertEqual(fpi(1).units.Mx1, 'Nmm');
assertEqual(fpi(1).units.My1, 'Nmm');
assertEqual(fpi(1).units.Mz1, 'Nmm');
assertEqual(fpi(1).cal_matrix, eye(6));
btkDeleteAcquisition(h);
end

function testAppendForcePlatformType2_interpolate(d)
h = btkNewAcquisition(0,10,0,10);
btkSetFrequency(h,100);
force = rand(10,3);
moment = rand(10,3);
corners = [-200,200,0;200,200,0;-200,-200,0;200,-200,0];
warning('off','btk:AppendForcePlatformType2');
btkAppendForcePlatformType2(h, force, moment, corners,[],1)
warning('on','btk:AppendForcePlatformType2');
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 10);
assertEqual(btkGetPointNumber(h), 0);
assertEqual(btkGetPointFrameNumber(h), 10);
assertEqual(btkGetAnalogNumber(h), 6);
assertEqual(btkGetAnalogFrameNumber(h), 100);
[fp fpi] = btkGetForcePlatforms(h);
assertEqual(length(fp), 1);
assertEqual(length(fpi), 1);
assertEqual(fp(1).corners, corners');
assertEqual(fp(1).channels.Fx1(1:10:end), force(:,1));
assertEqual(fp(1).channels.Fy1(1:10:end), force(:,2));
assertEqual(fp(1).channels.Fz1(1:10:end), force(:,3));
assertEqual(fp(1).channels.Mx1(1:10:end), moment(:,1));
assertEqual(fp(1).channels.My1(1:10:end), moment(:,2));
assertEqual(fp(1).channels.Mz1(1:10:end), moment(:,3));
assertEqual(fp(1).origin, [0;0;0]);
assertEqual(fp(1).type, 2);
assertEqual(fpi(1).frequency, 1000);
assertEqual(fpi(1).units.Fx1, 'N');
assertEqual(fpi(1).units.Fy1, 'N');
assertEqual(fpi(1).units.Fz1, 'N');
assertEqual(fpi(1).units.Mx1, 'Nmm');
assertEqual(fpi(1).units.My1, 'Nmm');
assertEqual(fpi(1).units.Mz1, 'Nmm');
assertEqual(fpi(1).cal_matrix, eye(6));
btkDeleteAcquisition(h);
end

function testAppendForcePlatformType2_global(d)
h = btkNewAcquisition(0,10);
btkSetFrequency(h,1000);
btkSetPointsUnit(h,'Marker','Nm');
btkSetPointsUnit(h,'Moment','Nm');
force = rand(10,3);
moment = rand(10,3);
corners = [-200,200,0;200,200,0;-200,-200,0;200,-200,0];
btkAppendForcePlatformType2(h, force, moment, corners)
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 10);
assertEqual(btkGetPointNumber(h), 0);
assertEqual(btkGetPointFrameNumber(h), 10);
assertEqual(btkGetAnalogNumber(h), 6);
assertEqual(btkGetAnalogFrameNumber(h), 10);
[fp fpi] = btkGetForcePlatforms(h);
assertEqual(length(fp), 1);
assertEqual(length(fpi), 1);
assertEqual(fp(1).corners, corners');
assertEqual(fp(1).channels.Fx1, -force(:,1));
assertEqual(fp(1).channels.Fy1, force(:,2));
assertEqual(fp(1).channels.Fz1, -force(:,3));
assertEqual(fp(1).channels.Mx1, -moment(:,1));
assertEqual(fp(1).channels.My1, moment(:,2));
assertEqual(fp(1).channels.Mz1, -moment(:,3));
assertEqual(fp(1).origin, [0;0;0]);
assertEqual(fp(1).type, 2);
assertEqual(fpi(1).frequency, 1000);
assertEqual(fpi(1).units.Fx1, 'N');
assertEqual(fpi(1).units.Fy1, 'N');
assertEqual(fpi(1).units.Fz1, 'N');
assertEqual(fpi(1).units.Mx1, 'Nm');
assertEqual(fpi(1).units.My1, 'Nm');
assertEqual(fpi(1).units.Mz1, 'Nm');
assertEqual(fpi(1).cal_matrix, eye(6));
btkDeleteAcquisition(h);
end

function testAppendForcePlatformType2_exportC3D(d)
warning('off','btk:ReadAcquisition');
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/sample09/PlugInC3D.c3d'));
warning('on','btk:ReadAcquisition');
frw = btkGetForcePlatformWrenches(h,0);
corners = [-200,-200,0;200,-200,0;200,-400,0;-200,-400,0];
btkAppendForcePlatformType2(h, frw(1).F, frw(1).M, corners, [0,0,0], 1)
btkWriteAcquisition(h, strcat(d.out,'/C3DSamples/sample09_PluginC3D.c3d'));
h2 = btkReadAcquisition(strcat(d.out,'/C3DSamples/sample09_PluginC3D.c3d'));
fp1 = btkGetForcePlatforms(h);
fp2 = btkGetForcePlatforms(h2);
assertEqual(length(fp1), 3);
assertEqual(length(fp2), 3);
assertElementsAlmostEqual(fp1(1).corners, fp2(1).corners);
assertElementsAlmostEqual(fp1(1).channels.FX1, fp2(1).channels.FX1, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.FY1, fp2(1).channels.FY1, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.FZ1, fp2(1).channels.FZ1, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.MX1, fp2(1).channels.MX1, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.MY1, fp2(1).channels.MY1, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.MZ1, fp2(1).channels.MZ1, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).corners, fp2(2).corners, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).channels.FX2, fp2(2).channels.FX2, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).channels.FY2, fp2(2).channels.FY2, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).channels.FZ2, fp2(2).channels.FZ2, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).channels.MX2, fp2(2).channels.MX2, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).channels.MY2, fp2(2).channels.MY2, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(2).channels.MZ2, fp2(2).channels.MZ2, 'absolute', 1e-5);
assertElementsAlmostEqual(corners', fp2(3).corners, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.FX1, fp2(3).channels.Fx3, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.FY1, fp2(3).channels.Fy3, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(1).channels.FZ1, fp2(3).channels.Fz3, 'absolute', 5e-5); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(1).channels.MX1, fp2(3).channels.Mx3, 'absolute', 5e-3); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(1).channels.MY1, fp2(3).channels.My3, 'absolute', 5e-3); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(1).channels.MZ1, fp2(3).channels.Mz3, 'absolute', 5e-3); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(3).corners, fp2(3).corners, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(3).channels.Fx3, fp2(3).channels.Fx3, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(3).channels.Fy3, fp2(3).channels.Fy3, 'absolute', 1e-5);
assertElementsAlmostEqual(fp1(3).channels.Fz3, fp2(3).channels.Fz3, 'absolute', 5e-5); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(3).channels.Mx3, fp2(3).channels.Mx3, 'absolute', 5e-3); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(3).channels.My3, fp2(3).channels.My3, 'absolute', 5e-3); % Due to the modification in the scale factors ...
assertElementsAlmostEqual(fp1(3).channels.Mz3, fp2(3).channels.Mz3, 'absolute', 5e-3); % Due to the modification in the scale factors ...
btkDeleteAcquisition(h);
btkDeleteAcquisition(h2);
end