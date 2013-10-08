xUnit for Octave 

This toolbox is an adaptation of the "MATLAB® xUnit Test Framework" version 3.1 (http://www.mathworks.com/matlabcentral/fileexchange/22846-matlab-xunit-test-framework) to be useable under Octave 3.6.4 or later (previous versions were not tested). The original toolbox cannot run under Octave as the new way to create a class into Matlab (using classdef keyword) is not yet supported into Octave. 

Only the code folder was kept from the Matlab toolbox. All the documentation is avaiblable online on the MATLAB Central website (http://www.mathworks.com/matlabcentral/fileexchange/22846-matlab-xunit-test-framework/content/matlab_xunit_3_1_1/doc/xunit_product_page.html)

To use this toolbox, you need to add its path into Octave as well as its subfolder '+xunit/+utils'. For that you can use directly the command:
  addpath(genpath('<SOURCE_DIR>/octave_xunit/xunit'));

Due to the difference between Octave and Matlab on the support of classes, some features of the "MATLAB® xUnit Test Framework" were removed:
 - Test a class
 - Test a subclass
 - Test a package

WARNING: The modification of the code attemps to emulate Matlab classes behavior using function handle and global variables. This might be not the best way to adapt this toolbox, but this seems an apropriate way to not modify all the source code to be compatible with Octave.

Release Notes
=============

* 2013-10-08, Initial release
  ---------------------------
   - Modification of the code to use commands like:
     - runtests mySuite
     - runtests mySuite:testMyFunction
   - The verbose mode and the use of a log file had not been adapted.