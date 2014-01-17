/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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
%include <std_list.i>
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
//                             AnalogOffsetRemover                           //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(AnalogOffsetRemover)
{
public:
  void SetRawInput(btkAcquisition );
  btkAcquisition GetRawInput();
  void SetOffsetInput(btkAcquisition );
  btkAcquisition GetOffsetInput();
  btkAcquisition GetOutput();
  
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(AnalogOffsetRemover);
};

// ------------------------------------------------------------------------- //
//                              CollectionAssembly                           //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                               DownsampleFilter                            //
// ------------------------------------------------------------------------- //

// - Concrete implementation: Wrench
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(WrenchDownsampleFilter)
{
public:
  btkWrench GetInput();
  void SetInput(btkWrench );
  btkWrench GetOutput();
  
  int GetUpDownRatio() const;
  void SetUpDownRatio(int ratio);

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(WrenchDownsampleFilter);
};

// - Concrete implementation: WrenchCollection
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(WrenchCollectionDownsampleFilter)
{
public:
  btkWrenchCollection GetInput();
  void SetInput(btkWrenchCollection );
  btkWrenchCollection GetOutput();
  
  int GetUpDownRatio() const;
  void SetUpDownRatio(int ratio);

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(WrenchCollectionDownsampleFilter);
};

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
  
  void AppendVirtualReferenceFrame(const std::string& , const std::string& , const std::string& , const std::string& );
  void AppendVirtualReferenceFrame(const btkStringAxes& );
  void AppendVirtualReferenceFrames(const std::list<btkStringAxes>& );
  void SetVirtualReferenceFrames(const std::list<btkStringAxes>& );
  const std::list<btkStringAxes>& GetVirtualReferenceFrames() const;
  
  void AppendVirtualMarker(const std::string& );
  void AppendVirtualMarkers(const std::list<std::string>& );
  void SetVirtualMarkers(const std::list<std::string>& );
  const std::list<std::string>& GetVirtualMarkers() const;
  
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
//                            SubAcquisitionFilter                           //
// ------------------------------------------------------------------------- //

%extend btkSubAcquisitionFilter
{
  btkSubAcquisitionFilter::ExtractionOption GetExtractionOption() const {return static_cast<btkSubAcquisitionFilter::ExtractionOption>((*$self)->GetExtractionOption());};  
  void SetExtractionOption(btkSubAcquisitionFilter::ExtractionOption option) {(*$self)->SetExtractionOption(static_cast<btk::SubAcquisitionFilter::ExtractionOption>(option));};
  
  btkSubAcquisitionFilter::ExtractionOption GetExtractionOption(std::vector<int>& ids) const
  {
    std::list<int> ids_;
    btk::SubAcquisitionFilter::ExtractionOption option = (*$self)->GetExtractionOption(ids_);
    ids.resize(ids_.size());
    int inc = 0;
    for (std::list<int>::const_iterator it = ids_.begin() ; it != ids_.end() ; ++it)
      ids[inc++] = *it;
    return static_cast<btkSubAcquisitionFilter::ExtractionOption>(option);
  };
  
  void SetExtractionOption(btkSubAcquisitionFilter::ExtractionOption option, const std::vector<int>& ids)
  {
    std::list<int> ids_;
    for (size_t i = 0 ; i < ids.size() ; ++i)
      ids_.push_back(ids[i]);
    (*$self)->SetExtractionOption(static_cast<btk::SubAcquisitionFilter::ExtractionOption>(option), ids_);
  };
}

BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(SubAcquisitionFilter)
{
public:
  typedef enum {All = btk::SubAcquisitionFilter::All, PointsOnly = btk::SubAcquisitionFilter::PointsOnly, AnalogsOnly = btk::SubAcquisitionFilter::AnalogsOnly, EventsOnly = btk::SubAcquisitionFilter::EventsOnly} ExtractionOption;
  
  void SetInput(btkAcquisition );
  btkAcquisition GetInput();
  btkAcquisition GetOutput();
  
  const int* GetFramesIndex() const;
  void SetFramesIndex(int lb = -1, int ub = -1);
  // ExtractionOption GetExtractionOption() const;
  // void SetExtractionOption(ExtractionOption );
  // ExtractionOption GetExtractionOption(std::list<int>& ) const;
  // void SetExtractionOption(ExtractionOption , const std::list<int>& );

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(SubAcquisitionFilter);
};

// ------------------------------------------------------------------------- //
//                VerticalGroundReactionForceGaitEventDetector               //
// ------------------------------------------------------------------------- //
BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(VerticalGroundReactionForceGaitEventDetector)
{
public:
  btkWrenchCollection GetInput();
  void SetInput(btkWrench);
  void SetInput(btkWrenchCollection);
  btkEventCollection GetOutput();
  
  void SetThresholdValue(int );
  int GetThresholdValue() const;
  void SetForceplateContextMapping(const std::vector<std::string>& );
  const std::vector<std::string>& GetForceplateContextMapping() const;
  void SetRegionOfInterest(int lb = -1, int ub = -1);
  const int* GetRegionOfInterest() const;
  void SetAcquisitionInformation(int , double , const std::string& );
  void GetAcquisitionInformation(int& , double& , std::string& );

protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(VerticalGroundReactionForceGaitEventDetector);
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