# BTKCore

## Introduction

Core of the project Biomechanical-ToolKit (BTK) which represent biomechanical
data, file formats and data processing

BTK is an open-source and cross-platform library for biomechanical analysis.
BTK read and write acquisition files and can modify them. All these operations
can be done by the use of the C++ API or by the wrappers included (Matlab,
Octave, and Python).

All the informations to build the C++ libraries and bindings are provided in
file 'Readme.html' which is located in the root of the source code.

APIs documentation and tutorials are proposed on the main website of the
project: http://b-tk.googlecode.com

## Continuous integration

To test the implemented features as well as to verify the modification in the
code, hundreds of cases representing standard usages of the project are tested
at each revision of the project.

Tests are built for the different supported operating systems (MacOS X, Linux)
as well as for the different supported languages (C++, Matlab/Octave, and
Python). All the tests are realized on the `master` branch of the code which
corresponds to the next version of the project.

### MacOS X 64-bit (C++, Python) [![Build Status](https://travis-ci.org/Biomechanical-ToolKit/BTKCore.png?branch=master)](https://travis-ci.org/Biomechanical-ToolKit/BTKCore)

Built from MacOS 10.8.2 and XCode 4.6.1. Configured to build C++ modules as
static and shared 64-bit libraries (with Clang and GCC). Python binding is
built with the MacOS X embedded version of Python and Numpy, while Swig is
installed with Homebrew.

### Linux 64-bit (C++, Python, Octave) [![Build Status](https://drone.io/Biomechanical-ToolKit/b-tk.core/status.png)](https://drone.io/Biomechanical-ToolKit/b-tk.core/latest)

Built from Ubuntu 12.04 and GCC 4.6. Configured to build C++ modules as shared
64-bit libraries. Python and Octave bindings are built from the
application/libraries furnished with Ubuntu 12.04 (i.e. Python 2.7.3, Numpy
1.6.1, Swig 2.0.4, and Octave 3.2).

### Windows 7 32-bit (C++, Python, Octave) [![Build Status](http://ec2-54-226-241-28.compute-1.amazonaws.com:8080/buildStatus/icon?job=BTKCore)](http://ec2-54-226-241-28.compute-1.amazonaws.com:8080/view/All/job/BTKCore/)

Built from MSVC 2010 (Windows SDK 7.1). Configured to build C++ modules as
shared 32-bit libraries. The Python binding is built with Python 2.7.5, Numpy
1.8.0 and Swig 2.0.11. The Octave binding is built with Octave 3.6.4.
