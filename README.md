#BTKCore#

##Introduction##

Core of the project Biomechanical-ToolKit (BTK) which represents biomechanical data, file formats and data processing

BTK is an open-source and cross-platform library for biomechanical analysis. The goal of BTK is to provide tools to scientists to perform computations and analysis based on published and/or commercial algorithms without the need to modify their workflow. For example, most of the motion capture (mocap) system provides their own biomechanical model with an associated marker setup. However, it would be beneficial to be independent of a mocap system to choose a model (because this one is known as better than the provided one, in term of reliability for a specific population, etc.). The project Biomechanical-ToolKit wants to provide such of tools.

The code in this branch (`wip-new-api`) introduces several new concepts and a complete refactored API to provide more generic containers and tools for biomechanical analysis (e.g. CGM: Clinical Gait Analysis, intra- / inter- subject analysis, etc.). At this time (05-10-2015), it possible to extract data from C3D files and compute segments' motion associated with the Plugin Gait model (Vicon Motion System Ltd, Oxford, UK).

##Documentation##

All the documentation (user guide, developer guide, etc.) is centralized and accessible over [Read the Docs](https://readthedocs.org/projects/btkdocumentation/). The following items lists the different version of the documentation available for the releases of BTKCore

 Version     | Documentation link | Note
 --- | --- | ---
 Release-1.0 | [![Documentation Status](https://readthedocs.org/projects/btkdocumentation/badge/?version=release-1.0)](https://readthedocs.org/projects/btkdocumentation/?badge=latest) | Not yet released!
 Development | [![Documentation Status](https://readthedocs.org/projects/btkdocumentation/badge/?version=latest)](https://readthedocs.org/projects/btkdocumentation/?badge=latest)

##Technologies##

The code in this branch uses heavily the features proposed by the **C++11 standard** (variadic template, smart pointer, unordered map, auto keyword, etc.). For this reason, a compiler supporting the C++11 standard must be used (e.g MSVC 2013, GCC 4.8, or clang 3.3). For this reason, the application [CMake](http://www.cmake.org) used for the build system must be at least the release version 3.1.

The numerical analysis is realized using the [Eigen](http://eigen.tuxfamily.org) library that uses **template expression**. On top of Eigen, BTKCore propose the nested namespace `lard` (Linear Algebra for Reconstructed Data). Several type and operations are proposed to handle automatically data occlusion. Thus, the implementation of numerical analysis is simplified (e.g. no need of for loop to process each sample, or to know if the sample is occluded or not).

The **opaque pointer** technique (known also as the pimpl idiom) is used in order to keep ABI compatibility between minor release of BTKCore. This point is important for third parties software (and plugins) which uses BTKCore as dynamic libraries.

The internal data structure relies on a **dynamic hierarchical tree structure** (i.e. a collection of nodes). This kind of structure has the advantage to be very flexible. Indeed, the addition of new kind of data type is possible without modifying the internal structure of existing ones.

The operations on files are realized using the **memory-mapped** mechanism to increase input/output performance. Thus, the data loading is faster for files physically present on-disk (or present on the network and temporary downloaded on your computer).

##Development testing##

In order to ensure the development and its quality over supported operating systems and processors, different automated processes are executed each time modification in the code is done on the server. Hence, it strengthens the development, but also reduce undetected errors (bugs, missing documentation, etc.) when a new version is released.

###Continuous integration###

This part compiles the code on different operating systems in release mode and verify that all units tests passed. The unit tests are implemented using a modified version of [CxxTest](http://cxxtest.com) embedded into the source of BTKCore.

 - **MacOS X [![Build Status](https://travis-ci.org/Biomechanical-ToolKit/BTKCore.svg?branch=wip-new-api)](https://travis-ci.org/Biomechanical-ToolKit/BTKCore)**
  - Compiler(s): Clang (XCode 6.1) / GCC (4.8.3)
  - Processor architecture(s): x86_64
  - Libraries build mode(s): Static / Shared

 - **Linux [![Build Status](https://drone.io/github.com/Biomechanical-ToolKit/BTKCore/status.png)](https://drone.io/github.com/Biomechanical-ToolKit/BTKCore/latest)**
  - Compiler(s): GCC (4.8.1)
  - Processor architecture(s): x86_64
  - Libraries build mode(s): Shared

 - **Microsoft Windows [![Build status](https://ci.appveyor.com/api/projects/status/lu8x5pri28fcje3c?svg=true)](https://ci.appveyor.com/project/Alzathar/btkcore)**
  - Compiler(s): MSVC 2013 Update 4
  - Processor architecture(s): x86 / x86_64
  - Libraries build mode(s): Shared

###Static code analysis [![Coverity Scan Build Status](https://scan.coverity.com/projects/5271/badge.svg)](https://scan.coverity.com/projects/5271)###

This part verifies the quality of the implementation (i.e. no lexical, syntactic, and semantic mistakes which could crash the software or create vulnerabilities) as well as its maintenance.

###Test coverage [![Coveralls Coverage Status](https://coveralls.io/repos/Biomechanical-ToolKit/BTKCore/badge.svg?branch=wip-new-api)](https://coveralls.io/r/Biomechanical-ToolKit/BTKCore?branch=wip-new-api)###

To be sure that unit tests executed in the *continuous integration* part covers all the classes/functions proposed in BTKCore, a coverage analysis is realized. It gives a metrics regarding the number of calls used in the unit tests for each methods/functions.

###API documentation [![Doxygen Coverage Status](https://drone.io/github.com/Biomechanical-ToolKit/BTKCore/files/_build/doxy-coverage.svg)](https://drone.io/github.com/Biomechanical-ToolKit/BTKCore/files/_build/doxy-coverage.resume.txt)###
The documentation of the API is built each time a commit is sent on the server. Internally, [Doxygen](http://www.doxygen.org) is used to verify and generate documentation of the C++ code (classes, methods, etc.). Then,  [Breathe](https://github.com/michaeljones/breathe) integrates it into the central documentation generated by [Sphinx](http://sphinx-doc.org).

##License##

BTK use the generous open-source New BSD license. Yes, you can use BTK in commercial products. The complete text of the copyright is presented below:

```
The Biomechanical ToolKit
Copyright (c) 2009-2015, Arnaud Barré
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above
      copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials
      provided with the distribution.
    * Neither the name(s) of the copyright holders nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
```
