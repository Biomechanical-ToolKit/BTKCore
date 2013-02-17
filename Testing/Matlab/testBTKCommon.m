function test_suite = testBTKCommon
initTestSuite;
end

function testNewAcquisition
h = btkNewAcquisition(10,100,2,10);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrameNumber(h), 100);
assertEqual(btkGetAnalogNumber(h), 2);
assertEqual(btkGetAnalogFrameNumber(h), 1000);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 100);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 10);
assertEqual(btkGetPointFrequency(h), 0);
assertEqual(btkGetAnalogFrequency(h), 0);
btkDeleteAcquisition(h);
end

function testGetPointsValues
np = 2;
h = btkNewAcquisition(np,10);
res = btkGetPointsValues(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), np*3);
res_ = zeros(10,np*3);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testSetPointsValues
np = 2;
h = btkNewAcquisition(np,10);
res_ = rand(10,np*3);
btkSetPointsValues(h, res_);
res = btkGetPointsValues(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), np*3);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testGetPointsResiduals
np = 2;
h = btkNewAcquisition(np,10);
res = btkGetPointsResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), np);
res_ = zeros(10,np);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testSetPointsResiduals
np = 2;
h = btkNewAcquisition(np,10);
res_ = rand(10,np);
btkSetPointsResiduals(h, res_);
res = btkGetPointsResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), np);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testCropAcquisition_first50
h = btkNewAcquisition(10,100,2,10);
pv_ = rand(100,10*3);
rv_ = rand(100,10);
av_ = rand(1000,2);
btkSetPointsValues(h, pv_);
btkSetPointsResiduals(h, rv_);
btkSetAnalogsValues(h, av_);
btkCropAcquisition(h,1,50);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 50);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrameNumber(h), 50);
assertEqual(btkGetAnalogNumber(h), 2);
assertEqual(btkGetAnalogFrameNumber(h), 500);
assertEqual(btkGetPointsValues(h), pv_(1:50,:));
assertEqual(btkGetPointsResiduals(h), rv_(1:50,:));
assertEqual(btkGetAnalogsValues(h), av_(1:500,:));
btkDeleteAcquisition(h);
end

function testCropAcquisition_last51
h = btkNewAcquisition(10,100,2,10);
pv_ = rand(100,10*3);
rv_ = rand(100,10);
av_ = rand(1000,2);
btkSetPointsValues(h, pv_);
btkSetPointsResiduals(h, rv_);
btkSetAnalogsValues(h, av_);
btkCropAcquisition(h,50);
assertEqual(btkGetFirstFrame(h), 50);
assertEqual(btkGetLastFrame(h), 100);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrameNumber(h), 51);
assertEqual(btkGetAnalogNumber(h), 2);
assertEqual(btkGetAnalogFrameNumber(h), 510);
assertEqual(btkGetPointsValues(h), pv_(50:end,:));
assertEqual(btkGetPointsResiduals(h), rv_(50:end,:));
assertEqual(btkGetAnalogsValues(h), av_(491:end,:));
btkDeleteAcquisition(h);
end

