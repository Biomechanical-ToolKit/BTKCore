function suite = TestSuite(name, location)
%TestSuite Collection of TestComponent objects
%   The TestSuite class defines a collection of TestComponent objects.
%
%   TestSuite methods:
%       TestSuite             - Constructor
%       add                   - Add test component to test suite
%       print                 - Display test suite summary to Command Window
%       run                   - Run the test suite
%       keepMatchingTestCase  - Keep only the named test component
%       fromName              - Construct test suite from directory or MATLAB function file name
%       fromTestCaseClassName - Construct test suite from TestCase class name
%       fromPackageName       - Construct test suite from package name
%       fromPwd               - Construct test suite from present directory
%
%   TestSuite properties:
%       TestComponents - Cell array of TestComponent objects
%
%   Examples
%   --------
%   Run all the test cases in the SampleTests1 class.  Display test suite
%   progress and a summary of results in the Command Window.
%
%       TestSuite('SampleTests1').run()
%
%   Construct a test suite from all test components found in the current
%   directory.
%
%       suite = TestSuite.fromPwd();
%
%   Construct a test suite from all test components found in the package
%   'mytool.tests'. (Note that the "+" character at the beginning of the package
%   folder name on disk is not part of the package name.)
%
%       suite = TestSuite.fromPackageName('mytool.tests');
%
%   Run all the test cases in the SampleTests class.  Display no output to the
%   Command Window.  Upon completion, query the number of test failures and test
%   errors.
%
%       logger = TestRunLogger();
%       TestSuite('SampleTests1').run(logger);
%       numFailures = logger.NumFailures
%       numErrors = logger.NumErrors
%
%   See also CommandWindowTestRunDisplay, TestCase, TestComponent, TestRunLogger

%   Steven L. Eddins
%   Copyright 2008-2010 The MathWorks, Inc.
%
%   Arnaud Barré
%   Copyright 2013, xUnit for Octave

id = xunitTestSuiteRequestNewHandle_p();
suite.add = @(test)xunitTestSuiteAdd(id,test);
suite.run = @(monitor)xunitTestSuiteRun(id,monitor);
suite.emptyTestCase = @()xunitTestSuiteEmptyTestCase(id);
suite.numTestCases = @()xunitTestSuiteNumTestCases(id);
suite.keepMatchingTestCase = @(name)xunitTestSuiteKeepMatchingTestCase(id,name);

function xunitTestSuiteAdd(id,test)
%add Add test component to test suite
%   test_suite.add(component) adds the TestComponent object to the
%   test suite.
global xunitTestSuiteHandles;
idx = xunitTestSuiteCheckHandleID_p(id);
xunitTestSuiteHandles(idx).testfuns(end+1) = test;

function did_pass_out = xunitTestSuiteRun(id,monitor)
%run Execute test cases in test suite
%   did_pass = suite.run() executes all test cases in the test
%   suite, returning a logical value indicating whether or not all
%   test cases passed.
global xunitTestSuiteHandles;
idx = xunitTestSuiteCheckHandleID_p(id);
suite = xunitTestSuiteHandles(idx);
if nargin < 2
    monitor = CommandWindowTestRunDisplay();
end
monitor.testComponentStarted(suite);
did_pass = true;
suite.setUp();
for k = 1:numel(suite.testfuns)
    test = suite.testfuns(k);
    this_component_passed = test.run(test, monitor);
    did_pass = did_pass && this_component_passed;
end
suite.tearDown();
monitor.testComponentFinished(suite, did_pass);
if nargout > 0
    did_pass_out = did_pass;
end
 
function res = xunitTestSuiteEmptyTestCase(id)
global xunitTestSuiteHandles;
idx = xunitTestSuiteCheckHandleID_p(id);
res = isempty(xunitTestSuiteHandles(idx).testfuns);
  
function num = xunitTestSuiteNumTestCases(id)  
global xunitTestSuiteHandles;
idx = xunitTestSuiteCheckHandleID_p(id);
num = length(xunitTestSuiteHandles(idx).testfuns);

function xunitTestSuiteKeepMatchingTestCase(id, name)
%keepMatchingTestCase Keep only the named test component
%   test_suite.keepMatchingTestCase(name) keeps only the test
%   component with a matching name and discards the rest.
global xunitTestSuiteHandles;
idx = xunitTestSuiteCheckHandleID_p(id);
idx_ = [];
for k = 1:numel(xunitTestSuiteHandles(idx).testfuns)
    if strcmp(xunitTestSuiteHandles(idx).testfuns(k).Name, name)
        idx_ = k;
        break;
    end
end
if isempty(idx_)
    xunitTestSuiteHandles(idx).testfuns = [];
else
    xunitTestSuiteHandles(idx).testfuns = xunitTestSuiteHandles(idx).testfuns(idx_);
end

function xunitTestSuiteDummy()
% Nothing to do
;

% -------------------------------------------------------------------------
% Private functions
% -------------------------------------------------------------------------

% Create a new handle and return its ID.
function id = xunitTestSuiteRequestNewHandle_p()
global xunitTestSuiteHandles;
id = size(xunitTestSuiteHandles,1);
idx = id + 1;
xunitTestSuiteHandles(idx).id = id;
xunitTestSuiteHandles(idx).testfuns = {};
xunitTestSuiteHandles(idx).numTestCases = @()xunitTestSuiteNumTestCases(id);
xunitTestSuiteHandles(idx).setUp = @()xunitTestSuiteDummy();
xunitTestSuiteHandles(idx).tearDown = @()xunitTestSuiteDummy();

% Check if the given ID is valid and then return the index of the associated ID.
function idx = xunitTestSuiteCheckHandleID_p(id)
global xunitTestSuiteHandles;
idx = NaN;
for i=1:length(xunitTestSuiteHandles)
    if (xunitTestSuiteHandles(i).id == id)
        idx = i;
        break;
    end
end
if isnan(idx)
    error('xunit:TestSuite:CheckHandleID', 'Unknown TestSuite handle ID.');
end
