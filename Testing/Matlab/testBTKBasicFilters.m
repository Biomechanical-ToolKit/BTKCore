function test_suite = testBTKBasicFilters
initTestSuite;
end

function testGetMarkersResiduals
h = btkNewAcquisition(2,10);
res = btkGetMarkersResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), 2);
res_ = zeros(10,2);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testSetMarkersResiduals
h = btkNewAcquisition(2,10);
res_ = rand(10,2);
btkSetMarkersResiduals(h, res_);
res = btkGetMarkersResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), 2);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testAppendForcePlatformType2
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

function testAppendForcePlatformType2_interpolate
h = btkNewAcquisition(0,10,0,10);
btkSetFrequency(h,100);
force = rand(10,3);
moment = rand(10,3);
corners = [-200,200,0;200,200,0;-200,-200,0;200,-200,0];
warning('off','btk:AppendForcePlatformType2');
btkAppendForcePlatformType2(h, force, moment, corners,[],1)
warning('off','btk:AppendForcePlatformType2');
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

function testAppendForcePlatformType2_global
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
