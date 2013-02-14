% Simple M-script file to launch unit tests for BTK Matlab binding
function TDD
    clc
    
    % The toolbox btk is supposed to be already in the Matlab paths.
    % Let's add the path for Matlab xUnit
    addpath('../../Utilities/matlab_xunit/xunit');
    
    % Run the unit tests
    fprintf('\n===========================================================\n');
    runtests testBTKCommon
    fprintf('\n===========================================================\n');
    runtests testBTKIO
    fprintf('\n===========================================================\n');
    runtests testBTKBasicFilters
    
    % Extra tests for the function btkC3DserverEmulation
    % if the original C3DServer is intalled
    % - Check for the OS
    if strcmp(computer, 'PCWIN')
        c3dserverDetected = true;
        % - Test if C3DServer is installed.
        try 
          actxserver('C3Dserver.C3D');
        catch
            c3dserverDetected = false;
        end
        c3dserverDetected = false;
        if (c3dserverDetected)
            % - Run the unit tests
            fprintf('\n===========================================================\n');
            runtests testC3DserverEmulation;
        end
    end