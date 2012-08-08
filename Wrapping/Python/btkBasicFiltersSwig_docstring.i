/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// ------------------------------------------------------------------------- //
//                          AcquisitionUnitConverter                         //
// ------------------------------------------------------------------------- //
%feature("docstring") btkAcquisitionUnitConverter "
This filter can convert units for trajectory, force, moment, angle and power.
The known unit for the trajectory (length) are:
 - meter (m);
 - decimeter (dm);
 - centimeter (cm)
 - millimeter (mm)
 - inch (in)
The known unit for the force are:
 - newton (N);
The known unit for the moment are:
 - newton by meter (Nm);
 - newton by decimeter (Ndm);
 - newton by centimeter (Ncm);
 - newton by millimeter (Nmm);
 - newton by inch (Nin);
The known unit for the angle are:
 - radian (rad);
 - degree (deg);
The known unit for the power are:
 - watt (W);
Moreover, the metadata known to be influenced by these unit (FORCE_PLATFORM, 
SEG) are also converted. The metadata POINT:*UNITS are not updated as they are not
used in BTK (instead, you can use the method btkAcquisition::GetPointUnit()).

Use the method btkAcquisitionUnitConverter::SetUnit() with the following enumeration 
value to select the wanted units:
 - btk.btkAcquisitionUnitConverter.Length
 - btk.btkAcquisitionUnitConverter.Angle
 - btk.btkAcquisitionUnitConverter.Force
 - btk.btkAcquisitionUnitConverter.Moment
 - btk.btkAcquisitionUnitConverter.Power

By default, the data will be converted in millimeter, newton, newton by millimeter, 
degree and watt. Even if you could convert trajectories and moments with the desired 
units, it is recommended to use the same length unit (i.e. mm and Nmm or m with Nm)."

BTK_SWIG_AUTODOC_IMPL(AcquisitionUnitConverter, SetInput, "SetInput(self, btkAcquisition)");
BTK_SWIG_AUTODOC_IMPL(AcquisitionUnitConverter, GetUnit, "GetUnit(self, btk.btkAcquisitionUnitConverter.Unit) -> string");
BTK_SWIG_AUTODOC_IMPL(AcquisitionUnitConverter, SetUnit, "SetUnit(self, btk.btkAcquisitionUnitConverter.Unit, string)");

BTK_SWIG_DOCSTRING_IMPL(AcquisitionUnitConverter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionUnitConverter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionUnitConverter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionUnitConverter, SetUnit, "Set the given unit with the given string");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionUnitConverter, GetUnit, "Returns the string corresponding to the given unit");

// ------------------------------------------------------------------------- //
//                              CollectionAssembly                           //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                               DownsampleFilter                            //
// ------------------------------------------------------------------------- //
// Warning: C++ template. Need concrete implementation

// - Wrench
%feature("docstring") btkWrenchDownsampleFilter "
Downsample data stored in a wrench

To downsample data, you need to set the up/down sample ratio using the method SetUpDownRatio().
The given value is an integer corresponding to the ratio used to extract only the value of interest.
For example, if you have 200 frames and a ratio of 10, then 20 frames will be extracted (one frame each 10 frames)."

BTK_SWIG_AUTODOC_IMPL(WrenchDownsampleFilter, SetInput(btkWrench ), "SetInput(self, btkWrench)");
BTK_SWIG_AUTODOC_IMPL(WrenchDownsampleFilter, SetUpDownRatio(int ), "SetUpDownRatio(self, int)");

BTK_SWIG_DOCSTRING_IMPL(WrenchDownsampleFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchDownsampleFilter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchDownsampleFilter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchDownsampleFilter, SetUpDownRatio, "Sets the ratio used to downsample the data.");
BTK_SWIG_DOCSTRING_IMPL(WrenchDownsampleFilter, GetUpDownRatio, "Gets the ratio used to downsample the data.");

// - WrenchCollection
%feature("docstring") btkWrenchCollectionDownsampleFilter "
Downsample data stored in a collection of wrenches

To downsample data, you need to set the up/down sample ratio using the method SetUpDownRatio().
The given value is an integer corresponding to the ratio used to extract only the value of interest.
For example, if you have 200 frames and a ratio of 10, then 20 frames will be extracted (one frame each 10 frames)."

BTK_SWIG_AUTODOC_IMPL(WrenchCollectionDownsampleFilter, SetInput(btkWrenchCollection ), "SetInput(self, btkWrenchCollection)");
BTK_SWIG_AUTODOC_IMPL(WrenchCollectionDownsampleFilter, SetUpDownRatio(int ), "SetUpDownRatio(self, int)");

BTK_SWIG_DOCSTRING_IMPL(WrenchCollectionDownsampleFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollectionDownsampleFilter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollectionDownsampleFilter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollectionDownsampleFilter, SetUpDownRatio, "Sets the ratio used to downsample the data.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollectionDownsampleFilter, GetUpDownRatio, "Gets the ratio used to downsample the data.");

// ------------------------------------------------------------------------- //
//                           ForcePlatformsExtractor                         //
// ------------------------------------------------------------------------- //
%feature("docstring") btkForcePlatformsExtractor "
Based on the content of the metadata FORCE_PLATFORM and the analog channels stored in the given 
acquisition, this filter creates a collection of btkForcePlatform returned in the output.

Depending of the type of the force platform, the analog channels' data are transformed to take into account a calibration matrix.

The metadata FORCE_PLATFORM must be structured as the following:
 - FORCE_PLATFORM:USED: single integer storing the number of force plates ;
 - FORCE_PLATFORM:TYPE: 1D array of integer containing the type of force plates contained in the acquisition (see below for the supported types) ;
 - FORCE_PLATFORM:CHANNEL: 2D array of integer containing of 1-based indices of the analog channels used ;
 - FORCE_PLATFORM:LABELS: 1D array of strings containing the force plates' label ;
 - FORCE_PLATFORM:DESCRIPTIONS: 1D array of strings containing the force plates' description ;
 - FORCE_PLATFORM:ORIGIN: 2D array of reals (float) containing the coordinates of the origin of the surface of each force platform expressed in their frame ;
 - FORCE_PLATFORM:CORNERS: 3D array of reals containing the coordinates of the corners. The third dimensions corresponds to the number of force platforms ;
 - FORCE_PLATFORM:CAL_MATRIX: (could be optional, depend on the FP type). 3D array containing the calibration to transform the volts into forces and moments ;
You can have more details on each metadata by following this link: http://www.c3d.org/HTML/theforceplatformgroup.htm

This filter is able to manage the following force platform's type:
 - Type 1: 6 channels with Forces X,Y,Z Position X,Y of the COP, and Moment Z around the COP ;
 - Type 2: 6 channels (FX, FY, FZ, MX, MY, MZ) ;
 - Type 3: 8 channels (FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23) ;
 - Type 4: Same as Type-2 + calibration matrix 6 (columns) by 6 (rows) ;
 - Type 5: Same as Type-3 + calibration matrix 6 (columns) by 8 (rows).

However, some other types are not yet supported due to the lack of information and data on them:
 - Type 6: 12 channels (FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4]) + calibration matrix 12 by 12 ;
 - Type 7: 8 channels (FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23) + calibration matrix 8 by 8 ;
 - Type 11: Kistler Split Belt Treadmill: 8 channels + calibration matrix 8X8 + polynomial correction matrix 2x6 + COP translation + COP rotation ;
 - Type 12: Gaitway treadmill: 8 channels (Fz11, Fz12, Fz13, Fz14, Fz21, Fz22, Fz23, and Fz24) + calibration matrix 8X8 ;
 - Type 21: AMTI-Stairs: 2 force plates with 6 channels + a calibration matrix 6x6 + data to locate the corners of the 4 steps."

BTK_SWIG_DOCSTRING_IMPL(ForcePlatformsExtractor, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformsExtractor, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformsExtractor, GetOutput, "Returns the output created with this process.");

// ------------------------------------------------------------------------- //
//                          ForcePlatformWrenchFilter                         //
// ------------------------------------------------------------------------- //
%feature("docstring") btkForcePlatformWrenchFilter "
Based on the given collection of forceplate set in input, this filter transform the associated analog channels in forces and moments.
This transformation take into account the type of each force platform.

You can use the method btkForcePlatformWrenchFilter::SetTransformToGlobalFrame() to have the wrench expressed in the frame of the force platform."

BTK_SWIG_AUTODOC_IMPL(ForcePlatformWrenchFilter, SetInput(btkForcePlatform ), "SetInput(self, btkForcePlatform)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformWrenchFilter, SetInput(btkForcePlatformCollection ), "SetInput(self, btkForcePlatformCollection)");

BTK_SWIG_DOCSTRING_IMPL(ForcePlatformWrenchFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformWrenchFilter, SetInput(btkForcePlatform ), "Sets a force platform as input of this this process.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformWrenchFilter, SetInput(btkForcePlatformCollection ), "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformWrenchFilter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformWrenchFilter, SetTransformToGlobalFrame, "Activate or deactivate the computation of the wrenches in the global frame.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformWrenchFilter, GetTransformToGlobalFrame, "Returns the state of the activation for the computation of the wrenches in the global frame.");

// ------------------------------------------------------------------------- //
//                         GroundReactionWrenchFilter                        //
// ------------------------------------------------------------------------- //
%feature("docstring") btkGroundReactionWrenchFilter "
Transform force platform data into ground reaction wrenches.

Theses wrenches are expressed in the global frame. The point of application (PWA)
of each wrench is calculated from the paper of Shimba (1984). 
Compared to the center of pressure (COP), the PWA take into account all the forces
and moments (and does not assume any null forces).

Note: For gait analysis, the COP and PWA are very similar.
Because the computation of the PWA (as the COP) is sensitive to small (vertical) forces, the methods 
SetThresholdValue and SetThresholdState() are provided to not compute the PWA if the vertical forces is
the threshold. You first need to set the value and then activate the threshold method.
By default, this threshold is not activated and the values is set to 0.

# Example
grwf = btk.btkGroundReactionWrenchFilter()
grwf.SetThresholdValue(5.0); # 5 newtons
grwf.SetThresholdState(true);

Finally, you can also use the method btkGroundReactionWrenchFilter::SetTransformToGlobalFrame() to have the wrench expressed in the frame of the force platform."

BTK_SWIG_AUTODOC_IMPL(GroundReactionWrenchFilter, SetInput(btkForcePlatform ), "SetInput(self, btkForcePlatform)");
BTK_SWIG_AUTODOC_IMPL(GroundReactionWrenchFilter, SetInput(btkForcePlatformCollection ), "SetInput(self, btkForcePlatformCollection)");
BTK_SWIG_AUTODOC_IMPL(GroundReactionWrenchFilter, SetThresholdValue, "SetThresholdValue(self, double)");

BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, SetTransformToGlobalFrame, "Activate or deactivate the computation of the wrenches in the global frame.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, GetTransformToGlobalFrame, "Returns the state of the activation for the computation of the wrenches in the global frame.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, SetThresholdState, "Sets the threshold state.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, GetThresholdState, "Returns the state of the threshold used to suppress false PWA.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, SetThresholdValue, "Sets the threshold value.");
BTK_SWIG_DOCSTRING_IMPL(GroundReactionWrenchFilter, GetThresholdValue, "Returns the value used to suppress PWA computed with a Fz value lower or equal than it.");

// ------------------------------------------------------------------------- //
//                                IMUsExtractor                              //
// ------------------------------------------------------------------------- //
%feature("docstring") btkIMUsExtractor "
Extracts inertial measurement units (IMUs) from a btkAcquisition object. 

Based on the content of the metadata IMU and the analog channels stored in the given acquisition(s),\nthis filter creates a collection of btkIMU returned in the output.

The metadata IMU must be structured as the following:
 - IMU:USED: single integer storing the number of IMUs ;
 - IMU:TYPE: 1D array of integer containing the type of IMU contained in the acquisition (see below for the supported types) ;
 - IMU:CHANNEL: 2D array of integer containing of 1-based indices of the analog channels used ;
 - IMU:LABELS: 1D array of strings containing the force plates' label ;
 - IMU:DESCRIPTIONS: 1D array of strings containing the force plates' description ;

The supported types are the following:
 - Type 1: 6D IMU with a 3D accelerometer and 3D gyroscope.

Note: This class is still experimental and could be modified in the next release."

BTK_SWIG_AUTODOC_IMPL(IMUsExtractor, GetInput(int ), "GetInput(self, int) -> btkAcquisition");
BTK_SWIG_AUTODOC_IMPL(IMUsExtractor, SetInput(btkAcquisition ), "SetInput(self, btkAcquisition)");
BTK_SWIG_AUTODOC_IMPL(IMUsExtractor, SetInput(int, btkAcquisition ), "SetInput(self, int, btkAcquisition)");

BTK_SWIG_DOCSTRING_IMPL(IMUsExtractor, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(IMUsExtractor, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(IMUsExtractor, GetOutput, "Returns the output created with this process.");

// ------------------------------------------------------------------------- //
//                            MeasureFrameExtractor                          //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                           MergeAcquisitionFilter                          //
// ------------------------------------------------------------------------- //
%feature("docstring") btkMergeAcquisitionFilter "
Merges or concatenates two or more btkAcquisition objects into a single new one.

The difference between the merging or the concatenating depends of the acquisitions' informations.

If the acquisitions are exactly the same (same label for points and analog channels) and the first 
frame of the n+1th acquisition corresponds to the last frame of the nth acquisition, then the 
acquisitions are merged, otherwise, the data are concatenated. Merged acquisitions means that the 
output will have the same number of points, analog channels, events than each output but the number
of frames corresponds to the sum of the input's frame number. 

Rules to merge/concatenate acquisitions are:
 - Must have the same acquisition frequency (or 0).
 - Must have the same number of analog samples per point frame (if there are points and analog channels).
 - Must have the same analog resolution.
 - Must have the same units (Use btkAcquisitionUnitConvert to convert them).
 - Must have the same global frame orientation (check POINT:X_SCREEN and POINT:Y_SCREEN)
 - The first input which has a non-zero acquisition frequency is used.
 - The first input which has analog channels is used to set the analog resolution of the output.

Actions during the merging/concatenation:
 - If the first frame of the nth acquisition correspond to the last frame + 1 of the nth-1 acquisition, 
   AND the points / analog channels correspond exactly, then acquisitions are truncated.
 - If the first frame of two inputs are not the same, the final acquisition is adapted to contain the number of frames plus
   the difference between the first frame of each input.
 - If points have the same label (and the first frames doesn't correspond), then, the second and more are incremented (i.e. from Heel to Heel_2).
 - Idem for the analog channels.
 - Events with the same label, context, id and time are merged.
 - Metadata (if number of inputs greater than 1):
    - Only metadata defined by the user are inserted (i.e. POINT, ANALOG, EVENTS, etc. are not inserted as they are only used in a C3D file.).
    - If metadata are equivalent or equal, then they are merged, else the metadata is appended with a suffix
      (i.e. from STATIC_CALIBRATION:USED to STATIC_CALIBRATION:USED_2)

Moreover, you can set a rule to keep only the data from the higher or the lower first frame.
Use the method SetFirstFrameRule() with the values KeepAllFrames or KeepFromHighestFirstFrame."

BTK_SWIG_AUTODOC_IMPL(MergeAcquisitionFilter, GetInput(int ), "GetInput(self, int) -> btkAcquisition");
BTK_SWIG_AUTODOC_IMPL(MergeAcquisitionFilter, SetInput(int , btkAcquisition ), "SetInput(self, int, btkAcquisition)");
BTK_SWIG_AUTODOC_IMPL(MergeAcquisitionFilter, SetFirstFrameRule(int ), "SetFirstFrameRule(self, int)");

BTK_SWIG_DOCSTRING_IMPL(MergeAcquisitionFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(MergeAcquisitionFilter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(MergeAcquisitionFilter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(MergeAcquisitionFilter, GetFirstFrameRule, "Sets the rule for the first frame kept. Two rules are available:\n - KeepAllFrames: Use the lower first frame;\n - KeepFromHighestFirstFrame: Use the higher first frame.");
BTK_SWIG_DOCSTRING_IMPL(MergeAcquisitionFilter, SetFirstFrameRule, "Returns the rule for the first frame kept.");

// ------------------------------------------------------------------------- //
//                    SeparateKnownVirtualMarkersFilter                      //
// ------------------------------------------------------------------------- //
%feature("docstring") btkSeparateKnownVirtualMarkersFilter "
Separate a collection of points in five categories to be able to distinguish real markers from the others.

These categories are:
 - all kind of markers (real and virtual) as stored in the acquisition
 - markers
 - virtual markers used for frame
 - other virtual markers (CenterOfMass, ...)
 - other points (angle, force, moment, power, ...)

To select the output corresponding to the chosen category, you can use the method GetOutput() 
with one of these enum values: AllMarkers, Markers, VirtualMarkersForFrame, VirtualMarkersOther, OtherPoints.

By default, the list of labels known as virtual markers used for frame is:
 - HED(O|A|L|P): HEDO, HEDA, HEDL, HELP
 - LCL(O|A|L|P)
 - LFE(O|A|L|P)
 - LFO(O|A|L|P)
 - LHN(O|A|L|P)
 - LHU(O|A|L|P)
 - LRA(O|A|L|P)
 - LTI(O|A|L|P)
 - LTO(O|A|L|P)
 - PEL(O|A|L|P)
 - RCL(O|A|L|P)
 - RFE(O|A|L|P)
 - RFO(O|A|L|P)
 - RHN(O|A|L|P)
 - RHU(O|A|L|P)
 - RRA(O|A|L|P)
 - RTI(O|A|L|P)
 - RTO(O|A|L|P)
 - TRX(O|A|L|P)

By default, the list of labels known as other virtual markers is:
 - CenterOfMass
 - CenterOfMassFloor

You can also add a prefix for the markers' label by using the method SetLabelPrefix. This prefix will be concatenated to the markers' label during the separation.

This filter only copies the pointer associated which each point instead of using a deep copy of it."

BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelForAxes(const std::string& , const std::string& , const std::string& , const std::string&), "AppendKnownVirtualMarkerLabelForAxes(self, string, string, string, string)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelForAxes(const btkStringAxes& ), "AppendKnownVirtualMarkerLabelForAxes(self, btkStringAxes)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelsForAxes, "AppendKnownVirtualMarkerLabelForAxes(self, btkStringAxesList)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelForOthers, "AppendKnownVirtualMarkerLabelForOthers(self, string)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelsForOthers, "AppendKnownVirtualMarkerLabelForOthers(self, btkStringList)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, SetInput(btkPointCollection ), "SetInput(self, btkAcquisition)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, GetOutput(int ), "GetOutput(self, int) -> btkPointCollection");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, SetKnownVirtualMarkerLabelsForAxes, "SetKnownVirtualMarkerLabelsForAxes(self, btkStringAxesList)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, SetKnownVirtualMarkerLabelsForOthers, "SetKnownVirtualMarkerLabelsForOthers(self, btkStringList)");
BTK_SWIG_AUTODOC_IMPL(SeparateKnownVirtualMarkersFilter, SetLabelPrefix(const std::string& ), "SetLabelPrefix(self, string)");

BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelForAxes(const std::string& , const std::string& , const std::string& , const std::string&), "Append a new set of labels for a virtual frame. The labels correspond to the origin, the axis X, Y and Z respectively.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelForAxes(const btkStringAxes& ), "Append a new set of labels for a virtual frame.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelsForAxes, "Append a list of axes' definition in the category of the virtual markers used to defined frames.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, SetKnownVirtualMarkerLabelsForAxes, "Sets the list of labels for the virtual markers used to create frames.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, GetKnownVirtualMarkerLabelsForAxes, "Returns the list of labels for the virtual markers used to create frame.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, AppendKnownVirtualMarkerLabelsForOthers, "Append a list of labels in the category of the other virtual markers.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, SetKnownVirtualMarkerLabelsForOthers, "Sets the list of labels for the virtual markers used in another context than virtual frames. Each label is checked before to be inserted to not have duplication.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, GetKnownVirtualMarkerLabelsForOthers, "Returns the list of labels for the virtual markers used in another context than virtual frames");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, SetLabelPrefix, "Sets the prefix which will be concatenated to the markers' label during the separation.");
BTK_SWIG_DOCSTRING_IMPL(SeparateKnownVirtualMarkersFilter, GetLabelPrefix, "Returns the prefix which will be concatenated to the markers' label during the separation.");

%feature("docstring") btkStringAxes "
Workaround to represent the C++ class btk::SeparateKnownVirtualMarkersFilter::StringAxes with SWIG.

Store four labels corresponding to markers used to define a frame."

%feature("docstring") btkStringAxesList "
List of btkStringAxes."

%feature("docstring") btkStringList "
List of strings used to define virtual markers which are not set for a frame."

// ------------------------------------------------------------------------- //
//                         SpecializedPointsExtractor                        //
// ------------------------------------------------------------------------- //
%feature("docstring") btkSpecializedPointsExtractor "
Extracts only the points corresponding to the given type (without cloning the extracted points).

The type is set by the SetPointType() method. By default, if no type is set, this filter extract markers."

BTK_SWIG_AUTODOC_IMPL(SpecializedPointsExtractor, SetPointType, "SetPointType(self, btk.btkPoint.Type)");

BTK_SWIG_DOCSTRING_IMPL(SpecializedPointsExtractor, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(SpecializedPointsExtractor, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(SpecializedPointsExtractor, GetOutput, "Returns the output created with this process.");
BTK_SWIG_DOCSTRING_IMPL(SpecializedPointsExtractor, GetPointType, "Returns the type of points to extract.");
BTK_SWIG_DOCSTRING_IMPL(SpecializedPointsExtractor, SetPointType, "Sets the type of points to extract.");

// ------------------------------------------------------------------------- //
//                         WrenchDirectionAngleFilter                        //
// ------------------------------------------------------------------------- //
%feature("docstring") btkWrenchDirectionAngleFilter "
Calculate the direction angle of the force vector for each wrench.

The direction angle is projected on each plane of the global frame in this order:
 - Angle yOz: set in the X component of the output ;
 - Angle xOz: set in the Y component of the output ;
 - Angle xOy: set in the Z component of the output.

The output angles are expressed in degrees and the range is between 0 and 360 degrees. 
Then a shift from 360 to 0 is possible if the force turns around itself."

BTK_SWIG_DOCSTRING_IMPL(WrenchDirectionAngleFilter, GetInput, "Gets the input registered with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchDirectionAngleFilter, SetInput, "Sets the input required with this process.");
BTK_SWIG_DOCSTRING_IMPL(WrenchDirectionAngleFilter, GetOutput, "Returns the output created with this process.");