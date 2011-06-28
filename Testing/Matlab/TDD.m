% Simple M-script file to launch unit tests for btkC3DserverEmulation
% Due to the use of C3Dserver to compare the result, these tests works
% only under Microsoft Windows 32-bit.
function TDD
    % Check for the OS
    if ~strcmp(computer, 'PCWIN')
      error('btk:TDD', 'These unit tests are only available for Microsoft Windows 32-bit due to the use of C3Dserver');
    end

    % Test if C3DServer is installed.
    try
      actxserver('C3Dserver.C3D');
    catch
      error('btk:TDD', 'C3Dserver is not installed on you computer');
    end

    % We suppose the toolbox btk is already in the path.
    % Let's add the path pour Matlab xUnit
    addpath('../../Utilities/matlab_xunit/xunit');

    % Run the unit tests
    runtests testC3DserverEmulation;
