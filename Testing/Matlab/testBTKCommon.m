function test_suite = testBTKCommon
initTestSuite;
end

function d = setup
d = TDDConfigure();
end

function testNewAcquisition(d)
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

function testNewAcquisition_bis(d)
h = btkNewAcquisition(10, 5);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrameNumber(h), 5);
assertEqual(btkGetAnalogNumber(h), 0);
assertEqual(btkGetAnalogFrameNumber(h), 5);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 5);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 1);
btkDeleteAcquisition(h);
end


function testNewAcquisition_ter(d)
h = btkNewAcquisition(10,5,1);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrameNumber(h), 5);
assertEqual(btkGetAnalogNumber(h), 1);
assertEqual(btkGetAnalogFrameNumber(h), 5);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 5);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 1);
btkDeleteAcquisition(h);
end

function testNewAcquisition_empty(d)
h = btkNewAcquisition();
assertEqual(btkGetPointNumber(h), 0);
assertEqual(btkGetPointFrameNumber(h), 0);
assertEqual(btkGetAnalogNumber(h), 0);
assertEqual(btkGetAnalogFrameNumber(h), 0);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 0);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 1);
assertEqual(btkGetPointFrequency(h), 0);
assertEqual(btkGetAnalogFrequency(h), 0);
btkDeleteAcquisition(h);
end

function testNewAcquisition_onlyPoint(d)
h = btkNewAcquisition(10);
assertEqual(btkGetPointNumber(h), 10);
assertEqual(btkGetPointFrameNumber(h), 0);
assertEqual(btkGetAnalogNumber(h), 0);
assertEqual(btkGetAnalogFrameNumber(h), 0);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 0);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 1);
assertEqual(btkGetPointFrequency(h), 0);
assertEqual(btkGetAnalogFrequency(h), 0);
btkDeleteAcquisition(h);
end

function testResize(d)
h = btkNewAcquisition();
btkSetFrameNumber(h, 100);
btkSetPointNumber(h, 9);
assertEqual(btkGetPointNumber(h), 9);
assertEqual(btkGetPointFrameNumber(h), 100);
assertEqual(btkGetAnalogNumber(h), 0);
assertEqual(btkGetAnalogFrameNumber(h), 100);
assertEqual(btkGetFirstFrame(h), 1);
assertEqual(btkGetLastFrame(h), 100);
assertEqual(btkGetAnalogSampleNumberPerFrame(h), 1);
pv = btkGetPointsValues(h);
assertEqual(size(pv,1), 100);
assertEqual(size(pv,2), 27);
assertEqual(all(pv(:) == 0), true);
btkDeleteAcquisition(h);
end

function testGetPointsValues_empty(d)
h = btkNewAcquisition();
pv = btkGetPointsValues(h);
assertEqual(isempty(pv), true);
btkDeleteAcquisition(h);
end

function testGetPointsValues(d)
np = 2;
h = btkNewAcquisition(np,10);
res = btkGetPointsValues(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), np*3);
res_ = zeros(10,np*3);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testSetPointsValues(d)
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

function testGetPointsResiduals(d)
np = 2;
h = btkNewAcquisition(np,10);
res = btkGetPointsResiduals(h);
assertEqual(size(res,1), 10);
assertEqual(size(res,2), np);
res_ = zeros(10,np);
assertEqual(res,res_);
btkDeleteAcquisition(h);
end

function testSetPointsResiduals(d)
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

function testCropAcquisition_first50(d)
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

function testCropAcquisition_last51(d)
h = btkNewAcquisition(10,100,2,10);
pv_ = rand(100,10*3);
rv_ = rand(100,10);
av_ = rand(1000,2);
btkSetPointsValues(h, pv_);
btkSetPointsResiduals(h, rv_);
btkSetAnalogsValues(h, av_);
btkSetFrequency(h,100);
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

function testGetAnalysis(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
analysis = btkGetAnalysis(h);
labels = fieldnames(analysis);
assertEqual(length(labels), 34);
assertEqual(labels{1}, 'Right_Vitesse__m_s_')
assertElementsAlmostEqual(analysis.(labels{1}), 0.681109, 'absolute', 1e-5)
assertEqual(labels{2}, 'Right_Cadence__pas_min_')
assertElementsAlmostEqual(analysis.(labels{2}), 38.4735, 'absolute', 1e-4)
assertEqual(labels{34}, 'Left_GPS')
assertElementsAlmostEqual(analysis.(labels{34}), 8.37584, 'absolute', 1e-5)
btkDeleteAcquisition(h);
end

function testClearAnalysis(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
btkClearAnalysis(h);
analysis = btkGetAnalysis(h);
md = btkGetMetaData(h, 'ANALYSIS');
assertEqual(isstruct(analysis), true);
assertEqual(isempty(fieldnames(analysis)), true);
assertEqual(isstruct(md.children), true);
assertEqual(md.children.USED.info.values, 0);
assertEqual(md.children.NAMES.info.values, cell(0,1));
assertEqual(md.children.CONTEXTS.info.values, cell(0,1));
assertEqual(md.children.UNITS.info.values, cell(0,1));
assertEqual(md.children.VALUES.info.values, zeros(0,1));
assertEqual(md.children.SUBJECTS.info.values, cell(0,1));
assertEqual(md.children.DESCRIPTIONS.info.values, cell(0,1));
btkDeleteAcquisition(h);
end

function testRemoveAnalysisParameters_Index(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
btkRemoveAnalysisParameter(h,1);
btkRemoveAnalysisParameter(h,33);
analysis = btkGetAnalysis(h);
labels = fieldnames(analysis);
assertEqual(length(labels), 32);
assertEqual(labels{1}, 'Right_Cadence__pas_min_')
assertElementsAlmostEqual(analysis.(labels{1}), 38.4735, 'absolute', 1e-4)
assertEqual(labels{32}, 'Left_GDI')
assertElementsAlmostEqual(analysis.(labels{32}), 68.0304, 'absolute', 1e-4)
btkDeleteAcquisition(h);
end

function testRemoveAnalysisParameters_Label(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
btkRemoveAnalysisParameter(h,'Right','Vitesse (m/s)');
btkRemoveAnalysisParameter(h,'Left','GPS');
analysis = btkGetAnalysis(h);
labels = fieldnames(analysis);
assertEqual(length(labels), 32);
assertEqual(labels{1}, 'Right_Cadence__pas_min_')
assertElementsAlmostEqual(analysis.(labels{1}), 38.4735, 'absolute', 1e-4)
assertEqual(labels{32}, 'Left_GDI')
assertElementsAlmostEqual(analysis.(labels{32}), 68.0304, 'absolute', 1e-4)
btkDeleteAcquisition(h);
end

function testSetMetaData(d)
h = btkNewAcquisition();
btkAppendMetaData(h, 'POINT', 'USED', btkMetaDataInfo('Integer', 0));
md = btkSetMetaData(h, 'POINT', 'USED', btkMetaDataInfo('Integer', 1));
assertEqual(md.children.POINT.children.USED.info.values, 1);
btkDeleteAcquisition(h);
end

function testSetMetaData_error(d)
h = btkNewAcquisition();
btkAppendMetaData(h, 'POINT', 'USED', btkMetaDataInfo('Integer', 0));
try
  md = btkSetMetaData(h, 'POINT', 'LABELS', btkMetaDataInfo('Char', {'foo', 'bar'}));
end
assertEqual(isempty(strfind(lasterr, 'The metadata POINT:LABELS was not found.')), false);
btkDeleteAcquisition(h);
end

function testAppendMetaData_issue66(d)
h1 = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
h2 = btkNewAcquisition();
md = btkGetMetaData(h1,'POINT','USED');
btkAppendMetaData(h2, 'POINT', 'USED', md.info);
md = btkGetMetaData(h2,'POINT','USED');
assertEqual(md.info.format, 'Integer');
assertEqual(isempty(md.info.dims), true);
assertEqual(md.info.values, 24);
btkDeleteAcquisition(h1);
btkDeleteAcquisition(h2);
end

function testSetFirstFrame_WithoutEventAdaptation(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
evts = btkGetEventsValues(h);
assertEqual(btkGetFirstFrame(h),1);
assertEqual(evts(1),0.0);
assertEqual(evts(2),1.0);
assertElementsAlmostEqual(evts(3),0.583733,'absolute',1e-5);
assertEqual(evts(4),0.0);
assertEqual(evts(5),1.0);
assertElementsAlmostEqual(evts(6),0.722626,'absolute',1e-5);
btkSetFirstFrame(h,100);
evts = btkGetEventsValues(h);
assertEqual(btkGetFirstFrame(h),100,0);
assertEqual(evts(1),0.0);
assertEqual(evts(2),1.0);
assertElementsAlmostEqual(evts(3),0.583733,'absolute',1e-5);
assertEqual(evts(4),0.0);
assertEqual(evts(5),1.0);
assertElementsAlmostEqual(evts(6),0.722626,'absolute',1e-5);
btkDeleteAcquisition(h);
end

function testSetFirstFrame_WithEventAdaptation(d)
h = btkReadAcquisition(strcat(d.in,'/C3DSamples/others/Analysis.c3d'));
evts = btkGetEventsValues(h);
assertEqual(btkGetFirstFrame(h),1);
assertEqual(evts(1),0.0);
assertEqual(evts(2),1.0);
assertElementsAlmostEqual(evts(3),0.583733,'absolute',1e-5);
assertEqual(evts(4),0.0);
assertEqual(evts(5),1.0);
assertElementsAlmostEqual(evts(6),0.722626,'absolute',1e-5);
btkSetFirstFrame(h,100,1);
evts = btkGetEventsValues(h);
assertEqual(btkGetFirstFrame(h),100);
assertEqual(evts(1),0.99);
assertEqual(evts(2),1.99);
assertElementsAlmostEqual(evts(3),0.58+0.99,'absolute',1e-5);
assertEqual(evts(4),0.99);
assertEqual(evts(5),1.99);
assertElementsAlmostEqual(evts(6),0.72+0.99,'absolute',1e-5);
btkDeleteAcquisition(h);
end

function testSetAnalogValues_double(d)
h = btkNewAcquisition(0,100,1);
values = rand(100,1);
btkSetAnalogsValues(h,values);
assertElementsAlmostEqual(btkGetAnalogsValues(h), values, 'absolute', 1e-15);
btkDeleteAcquisition(h);
end

function testSetAnalogValues_single(d)
h = btkNewAcquisition(0,100,1);
values = single(rand(100,1));
try
  btkSetAnalogsValues(h,values);
end
assertEqual(isempty(strfind(lasterr, 'The second input must be a matrix of real (double) values corresponding to the new analog channels values to assign.')), false);
btkDeleteAcquisition(h);
end

function testGetPointsInfoResiduals(d)
h = btkNewAcquisition(1,100);
values = rand(100,3);
residuals = rand(100,1);
btkSetPointLabel(h, 1, 'Toto');
btkSetPointValues(h, 1, values);
btkSetPointResiduals(h, 1, residuals);
[points, pointsInfo] = btkGetPoints(h);
assertElementsAlmostEqual(points.Toto, values, 'absolute', 1e-15);
assertElementsAlmostEqual(pointsInfo.residuals.Toto, residuals, 'absolute', 1e-15);
btkDeleteAcquisition(h);
end
