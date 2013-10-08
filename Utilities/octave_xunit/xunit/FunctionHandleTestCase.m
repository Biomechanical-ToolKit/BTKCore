function test = FunctionHandleTestCase(testFcn, setupFcn, teardownFcn)
%FunctionHandleTestCase Constructor
%   FunctionHandleTestCase(testFcn, setupFcn, teardownFcn) creates a
%   TestCase object that executes by running the function handle
%   TestFcn.  setupFcn is a function handle that will be executed
%   before testFcn, and teardownFcn is a function handle that will
%   be executed after TestFcn.  Either setupFcn or teardownFcn can
%   be empty.
%
%   If setupFcn is function handle that has one output argument,
%   then the three test functions will be called using these
%   syntaxes:
%
%       testData = setupFcn();
%       testFcn(testData);
%       teardownFcn(testData);
%
%   Otherwise, the three test functions are all called with no input
%   arguments:
%
%       setupFcn();
%       TestFcn();
%       teardownFcn();

%   Arnaud Barré
%   Copyright 2013, xUnit for Octave

test.TestFcn = testFcn;
test.SetupFcn = setupFcn;
test.TeardownFcn = teardownFcn;
test.run = @(self,monitor)xunitTestCaseRun(self,monitor);
% Determine the name and M-file location of the function handle.
functionHandleInfo = functions(testFcn);
test.Name = functionHandleInfo.function;
if strcmp(functionHandleInfo.type, 'anonymous')
    % Anonymous function handles don't have an M-file location.
    test.Location = '';
else
    test.Location = functionHandleInfo.file;
end

function did_pass = xunitTestCaseRun(test,monitor)
did_pass = 0;
monitor.testComponentStarted(test);
try
    % Setup test
    testData = xunitFunctionHandleTestCaseSetUp_p(test);
    try
        % Run test
        xunitFunctionHandleTestCaseRunTestCase_p(test, testData);
    catch
        failureException = lasterror;
        monitor.testCaseFailure(test, failureException);
        did_pass = 1;
    end
    % Teardown test
    xunitFunctionHandleTestCaseTearDown_p(test, testData);
catch 
    errorException = lasterror;
    monitor.testCaseError(test, errorException);
    did_pass = 2;
end
monitor.testComponentFinished(test, did_pass);
did_pass = (did_pass == 0);

% -------------------------------------------------------------------------
% Private functions
% -------------------------------------------------------------------------

function xunitFunctionHandleTestCaseRunTestCase_p(test, testData)
%runTestCase Run function handle test
%   test_case.run() calls the test function handle.  If a nonempty
%   SetupFcn was provided and it has at least one output argument,
%   pass testData to the test function.  Otherwise, call the
%   test function with no input arguments.
if ~isempty(test.SetupFcn) && (xunitNumArgOut_p(test.SetupFcn) > 0)
    test.TestFcn(testData);
else
    test.TestFcn();
end

function testData = xunitFunctionHandleTestCaseSetUp_p(test)
%setUp Run test-fixture setup function
%   If a nonempty SetupFcn was provided, run it.  If the SetupFcn
%   has at least one output argument, capture the first output
%   argument in instance data (TestData).
testData = [];
if ~isempty(test.SetupFcn)
    num = xunitNumArgOut_p(test.SetupFcn);
    if num > 0
        if num > 1
            message = sprintf(['A test fixture setup function returns more than one output argument. ', ...
                'The test harness only calls the setup function with one output argument. ', ...
                'Return a struct or a cell array from your setup function if you need to bundle several parts together.', ...
                '\nTest name: %s\nTest location: %s'], ...
                test.Name, test.Location);
            warning('xunit:FunctionHandleTestCase:TooManySetupOutputs', ...
                '%s', message);
        end
        testData = test.SetupFcn();
    else
        test.SetupFcn();
    end
end

function xunitFunctionHandleTestCaseTearDown_p(test, testData)
%tearDown Run test-fixture teardown function
%   If a nonempty TeardownFcn was provided, run it.  If there is
%   TestData (the output of the SetupFcn), then pass it to 
%   TeardownFcn.  Otherwise, call TeardownFcn with no input
%   arguments.
if ~isempty(test.TeardownFcn)
    if ~isempty(test.SetupFcn) && (xunitNumArgOut_p(test.SetupFcn) > 0)
        test.TeardownFcn(testData);
    else
        test.TeardownFcn();
    end
end

function num = xunitNumArgOut_p(fcn_handle)
%nargout Hack to have the similar behavior than Matlab function nargout with function handle (detect if at leat two output arguments are available).
num = 0;
try
  a = fcn_handle();
  num = 1;
  [a,b] = fcn_handle();
  num = 2;
end