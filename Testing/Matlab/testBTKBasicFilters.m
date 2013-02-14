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
