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

%module btkBasicFilters
%{
#include "btkCommonSwig.h"
#include "btkBasicFiltersSwig.h"
%}
%include <std_string.i>

#undef BTK_SWIG_HEADER_DECLARATION

#include "btkSwigMacros.h"

%include "btkBasicFiltersSwig_docstring.i"

%include "BasicFilters/btkBasicFiltersSwig_Filter.h"

// ------------------------------------------------------------------------- //
//                          AcquisitionUnitConverter                         //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(AcquisitionUnitConverter)
{
public:
  typedef enum {Length = btk::AcquisitionUnitConverter::Length, Angle = btk::AcquisitionUnitConverter::Angle, Force = btk::AcquisitionUnitConverter::Force, Moment = btk::AcquisitionUnitConverter::Moment, Power = btk::AcquisitionUnitConverter::Power} Unit;
  
  btkAcquisition GetInput();
  void SetInput(btkAcquisition );
  btkAcquisition GetOutput();
  
  void SetUnit(Unit , const std::string& );
  const std::string& GetUnit(Unit );
  
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(AcquisitionUnitConverter);
};

// ------------------------------------------------------------------------- //
//                              CollectionAssembly                           //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                               DownsampleFilter                            //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                           ForcePlatformsExtractor                         //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(ForcePlatformsExtractor)
{
public:
  btkAcquisition GetInput();
  void SetInput(btkAcquisition );
  btkForcePlatformCollection GetOutput();

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(ForcePlatformsExtractor);
};

// ------------------------------------------------------------------------- //
//                          ForcePlatformWrenchFilter                         //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(ForcePlatformWrenchFilter)
{
public:
  btkForcePlatformCollection GetInput();
  void SetInput(btkForcePlatform );
  void SetInput(btkForcePlatformCollection );
  btkWrenchCollection GetOutput();

  void SetTransformToGlobalFrame(bool activation = false);
  bool GetTransformToGlobalFrame() const;

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(ForcePlatformWrenchFilter);
};

// ------------------------------------------------------------------------- //
//                         GroundReactionWrenchFilter                        //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(GroundReactionWrenchFilter)
{
public:
  btkForcePlatformCollection GetInput();
  void SetInput(btkForcePlatform );
  void SetInput(btkForcePlatformCollection );
  btkWrenchCollection GetOutput();

  void SetTransformToGlobalFrame(bool activation = false);
  bool GetTransformToGlobalFrame() const;
  
  bool GetThresholdState() const;
  void SetThresholdState(bool activated = false);
  double GetThresholdValue() const;
  void SetThresholdValue(double );

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(GroundReactionWrenchFilter);
};

// ------------------------------------------------------------------------- //
//                                IMUsExtractor                              //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(IMUsExtractor)
{
public:
  btkAcquisition GetInput();
  btkAcquisition GetInput(int );
  void SetInput(btkAcquisition );
  void SetInput(int, btkAcquisition );
  btkIMUCollection GetOutput();

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(IMUsExtractor);
};


// ------------------------------------------------------------------------- //
//                            MeasureFrameExtractor                          //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                           MergeAcquisitionFilter                          //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(MergeAcquisitionFilter)
{
public:
  enum {KeepAllFrames = btk::MergeAcquisitionFilter::KeepAllFrames, KeepFromHighestFirstFrame = btk::MergeAcquisitionFilter::KeepFromHighestFirstFrame};
  int GetFirstFrameRule() const;
  void SetFirstFrameRule(int );

  btkAcquisition GetInput(int );
  void SetInput(int , btkAcquisition );
  btkAcquisition GetOutput();

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(MergeAcquisitionFilter);
};

// ------------------------------------------------------------------------- //
//                    SeparateKnownVirtualMarkersFilter                      //
// ------------------------------------------------------------------------- //

// Redefine nested class in global scope in order for SWIG to generate a proxy class.
struct btkStringAxes
{
  btkStringAxes(const std::string& o, const std::string& a1, const std::string& a2, const std::string& a3)
  : Origin(o), Axis1(a1), Axis2(a2), Axis3(a3)
  {};
  std::string Origin;
  std::string Axis1;
  std::string Axis2;
  std::string Axis3;
};

%nestedworkaround SeparateKnownVirtualMarkersFilter::StringAxes;

%{
  typedef btkSeparateKnownVirtualMarkersFilter_impl::StringAxes btkStringAxes;
%}

namespace std
{
  %template(btkStringAxesList) list<btkStringAxes>;
  %template(btkStringList) list<std::string>;
}

BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(SeparateKnownVirtualMarkersFilter)
{
public:
  enum {AllMarkers = btk::SeparateKnownVirtualMarkersFilter::AllMarkers, Markers = btk::SeparateKnownVirtualMarkersFilter::Markers, VirtualMarkersForFrame = btk::SeparateKnownVirtualMarkersFilter::VirtualMarkersForFrame, VirtualMarkersOther = btk::SeparateKnownVirtualMarkersFilter::VirtualMarkersOther, OtherPoints = btk::SeparateKnownVirtualMarkersFilter::OtherPoints};
  
  void AppendKnownVirtualMarkerLabelForAxes(const std::string& , const std::string& , const std::string& , const std::string& );
  void AppendKnownVirtualMarkerLabelForAxes(const btkStringAxes& );
  void AppendKnownVirtualMarkerLabelsForAxes(const std::list<btkStringAxes>& );
  void SetKnownVirtualMarkerLabelsForAxes(const std::list<btkStringAxes>& );
  const std::list<btkStringAxes>& GetKnownVirtualMarkerLabelsForAxes() const;
  
  void AppendKnownVirtualMarkerLabelForOthers(const std::string& );
  void AppendKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& );
  void SetKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& );
  const std::list<std::string>& GetKnownVirtualMarkerLabelsForOthers() const;
  
  void SetLabelPrefix(const std::string& );
  const std::string& GetLabelPrefix() const;
  
  btkPointCollection GetInput();
  void SetInput(btkPointCollection );
  btkPointCollection GetOutput(int );

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(SeparateKnownVirtualMarkersFilter);
};

// ------------------------------------------------------------------------- //
//                         SpecializedPointsExtractor                        //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(SpecializedPointsExtractor)
{
public:
  btkAcquisition GetInput();
  void SetInput(btkAcquisition input);
  btkPointCollection GetOutput();

  btkPoint_impl::Type GetPointType();
  void SetPointType(btkPoint_impl::Type );

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(SpecializedPointsExtractor);
};

// ------------------------------------------------------------------------- //
//                         WrenchDirectionAngleFilter                        //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(WrenchDirectionAngleFilter)
{
public:
  btkWrenchCollection GetInput();
  void SetInput(btkWrenchCollection input);
  btkPointCollection GetOutput();

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(WrenchDirectionAngleFilter);
};