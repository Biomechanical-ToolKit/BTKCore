#BTKCore#

##Introduction##

Core of the project Biomechanical-ToolKit (BTK) which represents biomechanical data, file formats and data processing

BTK is an open-source and cross-platform library for biomechanical analysis. The goal of BTK is to provide tools to scientists to perform computations and analysis based on published and/or commercial algorithms without the need to modify their workflow. For example, most of the motion capture (mocap) system provides their own biomechanical model with an associated marker setup. However, it would be beneficial to be independent of a mocap system to choose a model (because this one is known as better than the provided one, in term of reliability for a specific population, etc.). The project Biomechanical-ToolKit wants to provide such of tools.

The code in this branch (`wip-new-api`) introduces several new concepts and a complete refactored API to provide more generic containers and tools for biomechanical analysis (e.g. CGM: Clinical Gait Analysis, intra- / inter- subject analysis, etc.). At this time (05-10-2015), it possible to extract data from C3D files and compute segments' motion associated with the Plugin Gait model (Vicon Motion System Ltd, Oxford, UK).

**Disclaimer** : The code in this branch uses heavily the features provided by the C++11 standard (variadic template, smart pointer, unordered map, auto keyword, etc.). It was compiled and tested only with a Clang compiler (AppleClang 5.1). The code still needs to be tested and adapted on other compiler (MSVC, GCC, MinGW, etc.).