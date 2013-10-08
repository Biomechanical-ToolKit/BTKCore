function monitor = TestRunDisplay(output)
%TestRunDisplay Print test suite execution results.
%   TestRunDisplay is a subclass of TestRunMonitor.  If a TestRunDisplay
%   object is passed to the run method of a TestComponent, such as a
%   TestSuite or a TestCase, it will print information to the Command
%   Window (or specified file handle) as the test run proceeds.
%
%   TestRunDisplay methods:
%       testComponentStarted  - Update Command Window display
%       testComponentFinished - Update Command Window display
%       testCaseFailure       - Log test failure information
%       testCaseError         - Log test error information
%
%   TestRunDisplay properties:
%       TestCaseCount         - Number of test cases executed
%       Faults                - Struct array of test fault info
%
%   See also TestRunLogger, TestRunMonitor, TestSuite

%   Steven L. Eddins
%   Copyright 2008-2010 The MathWorks, Inc.
%
%   Arnaud Barré
%   Copyright 2013, xUnit for Octave

% WARNING: Only one monitor can be instantiated at the same time. 
% Trying to create a new one will erase the content of the old one.

global xunitTestMonitor;
xunitTestMonitor = [];
xunitTestMonitor.FileHandle = [];
xunitTestMonitor.InitialComponent = [];
xunitTestMonitor.TestCaseCount = [];
xunitTestMonitor.Faults = [];
if nargin > 0
    if ischar(output)
        xunitTestMonitor.FileHandle = fopen(output, 'w');
        if xunitTestMonitor.FileHandle < 0
            error('xunit:TestRunDisplay:FileOpenError', 'Could not open file "%s" for writing.', filename);
        end
    else
        xunitTestMonitor.FileHandle = output;
    end
end
xunitTestMonitor.testComponentStarted = @(component)xunitTestRunDisplayComponentStarted(component);
xunitTestMonitor.testComponentFinished = @(component,did_pass)xunitTestRunDisplayComponentFinished(component,did_pass);
xunitTestMonitor.testCaseFailure = @(test_case,failure_exception)xunitTestRunDisplayCaseFailure(test_case,failure_exception);
xunitTestMonitor.testCaseError = @(test_case,error_exception)xunitTestRunDisplayCaseError(test_case,error_exception);
monitor = xunitTestMonitor;

function xunitTestRunDisplayComponentStarted(component)
%testComponentStarted Update Command Window display
%    If the InitialComponent property is not yet set, 
%    obj.testComponentStarted(component) sets the property and calls
%    obj.testRunStarted(component).
global xunitTestMonitor;
if isempty(xunitTestMonitor.InitialComponent)
    xunitTestMonitor.InitialComponent = component;
    xunitTestRunDisplayRunStarted_p(component);
end
  
function xunitTestRunDisplayComponentFinished(component,did_pass)
%testComponentFinished Update Command Window display
%    If component is a TestCase object, then 
%    obj.testComponentFinished(component, did_pass) prints pass/fail
%    information to the Command Window.
%
%    If component is the InitialComponent, then
%    obj.testRunFinished(did_pass) is called.
global xunitTestMonitor;
if (~isfield(component, 'id'))
    xunitTestMonitor.TestCaseCount = xunitTestMonitor.TestCaseCount + 1;
    if did_pass == 0
        fprintf(xunitTestMonitor.FileHandle, '.');
    elseif did_pass == 1
        fprintf(xunitTestMonitor.FileHandle, 'F');
    elseif did_pass == 2
        fprintf(xunitTestMonitor.FileHandle, 'E');
    end
    line_length = 20;
    if mod(xunitTestMonitor.TestCaseCount, line_length) == 0
        fprintf(xunitTestMonitor.FileHandle, '\n');
    end
else
    if isequal(component, xunitTestMonitor.InitialComponent)
        xunitTestRunDisplayRunFinished_p(did_pass);
    end
end

function xunitTestRunDisplayCaseFailure(test_case,failure_exception)
%testCaseFailure Log test failure information
xunitTestRunDisplayLogFault_p('failure',test_case,failure_exception);

function xunitTestRunDisplayCaseError(test_case,error_exception)
%testCaseError Log test error information
xunitTestRunDisplayLogFault_p('error',test_case,error_exception);
  
% -------------------------------------------------------------------------
% Private functions
% -------------------------------------------------------------------------

function xunitTestRunDisplayRunStarted_p(component)
%testRunStarted Update Command Window display
%    obj.testRunStarted(component) displays information about the test
%    run to the Command Window.
global xunitTestMonitor;
% xunitTestMonitor.InitialTic = tic(); % NOTE: Octave does not support nested tic/toc
tic
xunitTestMonitor.TestCaseCount = 0;
num_cases = component.numTestCases();
if num_cases == 1
    str = 'case';
else
    str = 'cases';
end
fprintf(xunitTestMonitor.FileHandle, 'Starting test run with %d test %s.\n', num_cases, str);
    
function xunitTestRunDisplayRunFinished_p(did_pass)
%testRunFinished Update Command Window display
%    obj.testRunFinished(component) displays information about the test
%    run results, including any test failures, to the Command Window.
global xunitTestMonitor;
if did_pass
    result = 'PASSED';
else
    result = 'FAILED';
end
fprintf(xunitTestMonitor.FileHandle, '\n%s in %.3f seconds.\n', result, toc); % toc(xunitTestMonitor.InitialTic) % NOTE: Octave does not support nested tic/toc
xunitTestRunDisplayDisplayFaults_p();

function xunitTestRunDisplayLogFault_p(type, test_case, exception)
global xunitTestMonitor;
xunitTestMonitor.Faults(end+1).Type = type;
xunitTestMonitor.Faults(end).TestCase = test_case;
xunitTestMonitor.Faults(end).Exception = exception;

function xunitTestRunDisplayDisplayFaults_p()
%displayFaults Display test fault info to Command Window
%    obj.displayFaults() displays a summary of each test failure and
%    test error to the command window.
global xunitTestMonitor;
for k = 1:numel(xunitTestMonitor.Faults)
    faultData = xunitTestMonitor.Faults(k);
    if strcmp(faultData.Type, 'failure')
        str = 'Failure';
    else
        str = 'Error';
    end
    fprintf(xunitTestMonitor.FileHandle, '\n===== Test Case %s =====\nLocation: %s\nName:     %s\n\n', str, faultData.TestCase.Location, faultData.TestCase.Name);
    xunitTestRunDisplayDisplayStack_p(xunitTestRunDisplayFilterStack_p(faultData.Exception.stack), xunitTestMonitor.FileHandle);
    fprintf(xunitTestMonitor.FileHandle, '\n%s\n', faultData.Exception.message);
    fprintf(xunitTestMonitor.FileHandle, '\n');
end

function xunitTestRunDisplayDisplayStack_p(stack, file_handle)
%displayStack Display stack trace from MException instance
%   displayStack(stack) prints information about an exception stack to the
%   command window. 
for k = 1:numel(stack)
    filename = stack(k).file;
    linenumber = stack(k).line;
    fprintf(file_handle, '%s at line %d\n', filename, linenumber);
end

function new_stack = xunitTestRunDisplayFilterStack_p(stack)
%filterStack Remove unmeaningful stack trace calls
%    new_stack = filterStack(stack) removes from the input stack trace calls
%    that are framework functions and methods that are not likely to be
%    meaningful to the user.

% Testing stack traces follow this common pattern:
%
% 1. The first function call in the trace is often one of the assert functions
% in the framework directory.  This is _not_ useful to see.
%
% 2. The next function calls are in the user-written test functions/methods and
% the user-written code under test.  These calls are useful to see.
%
% 3. The final set of function calls are methods in the various framework
% classes.  There are usually several of these calls, which clutter up the 
% stack display without being that useful.
mtest_directory = fileparts(which('runtests'));
keeper = logical(ones(numel(stack),1));
for k = 1:numel(stack)
    directory = fileparts(stack(k).file);
    if strcmp(directory, mtest_directory)
        keeper(k) = false;
    end
end
new_stack = stack(keeper);


