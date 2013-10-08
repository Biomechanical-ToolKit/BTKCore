function suite = TestSuiteFromName(name)
%fromName Construct test suite from M-file name
%   test_suite = TestSuite.fromName(name) constructs a TestSuite
%   object from an M-file with the given name.  The name can be of a
%   directory, a TestCase subclass, or an M-file containing a simple
%   test or containing subfunction-based tests.
%
%   Optionally, name can contain a colon (':') followed by filter
%   string.  The filter string is used to select a particular named
%   test case.  For example, TestSuite.fromName('MyTests:testA')
%   constructs a TestSuite object containing only the test case
%   named 'testA' found in the TestCase subclass MyTests.

%   Arnaud Barré
%   Copyright 2013, xUnit for Octave

if isdir(name)
    error('xunit:NotYetImplemented','This part of the code has not yet been translated to Octave.');
    suite = TestSuiteInDir(name);
    suite.gatherTestCases();
    return;
end

[name, filter_string] = strtok(name, ':');
if ~isempty(filter_string)
    filter_string = filter_string(2:end);
end

% NOTE: No support to test (sub)class or package under Octave
% if isTestCaseSubclass(name)
%     suite = TestSuite.fromTestCaseClassName(name);  
% elseif ~isempty(meta.class.fromName(name))
%     % Input is the name of a class that is not a TestCase subclass.
%     % Return an empty test suite.
%     suite = TestSuite();
%     suite.Name = name;
%elseif isPackage(name)
%     error('xunit:NotYetImplemented','This part of the code has not yet been translated to Octave.');
%     suite = TestSuite.fromPackageName(name);
% else    
    try
        if nargout(name) == 0
            suite = TestSuite();
            suite.Name = name;
            suite.add(FunctionHandleTestCase(str2func(name), [], []));
            suite.Location = which(name);
        else
            suite = feval(name);
            % TODO: Test also the fieldnames to be sure it is a TestSuite "object"
            if ~isstruct(suite)
                error('xunit:TestSuiteFromName','Function did not return a TestSuite object.');
            end
        end
        
    catch
        % Ordinary function does not appear to contain tests.
        % Return an empty test suite.
        suite = TestSuite();
        suite.Name = name;
        suite.Location = '';
    end
% end

if ~isempty(filter_string)
    suite.keepMatchingTestCase(filter_string);
end