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
 
BTK_SWIG_DECLARE_CLASS(AcquisitionUnitConverter)
{
public:
  enum {Length = btk::AcquisitionUnitConverter::Length, Angle = btk::AcquisitionUnitConverter::Angle, Force = btk::AcquisitionUnitConverter::Force, Moment = btk::AcquisitionUnitConverter::Moment, Power = btk::AcquisitionUnitConverter::Power};
  BTK_SWIG_DECLARE_DEFAULT_CTOR(AcquisitionUnitConverter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(AcquisitionUnitConverter);
};

BTK_SWIG_DECLARE_CLASS(AnalogOffsetRemover)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(AnalogOffsetRemover);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(AnalogOffsetRemover);
};

// btkCollectionAssembly: Check SWIG with C++ template

// btkDownsampleFilter: C++ template. Need specialization
// - Wrench
BTK_SWIG_DECLARE_CLASS_TEMPLATE(WrenchDownsampleFilter, DownsampleFilter, btk::Wrench)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(WrenchDownsampleFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(WrenchDownsampleFilter);
};
// - WrenchCollection
BTK_SWIG_DECLARE_CLASS_TEMPLATE(WrenchCollectionDownsampleFilter, DownsampleFilter, btk::WrenchCollection)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(WrenchCollectionDownsampleFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(WrenchCollectionDownsampleFilter);
};

BTK_SWIG_DECLARE_CLASS(ForcePlatformsExtractor)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(ForcePlatformsExtractor);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(ForcePlatformsExtractor);
};

BTK_SWIG_DECLARE_CLASS(ForcePlatformWrenchFilter)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(ForcePlatformWrenchFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(ForcePlatformWrenchFilter);
};

BTK_SWIG_DECLARE_CLASS(GroundReactionWrenchFilter)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(GroundReactionWrenchFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(GroundReactionWrenchFilter);
};

BTK_SWIG_DECLARE_CLASS(IMUsExtractor)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(IMUsExtractor);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(IMUsExtractor);
};

// btkMeasureFrameExtractor: Check SWIG with C++ template

BTK_SWIG_DECLARE_CLASS(MergeAcquisitionFilter)
{
public:
  enum {KeepAllFrames = btk::MergeAcquisitionFilter::KeepAllFrames, KeepFromHighestFirstFrame = btk::MergeAcquisitionFilter::KeepFromHighestFirstFrame};
  BTK_SWIG_DECLARE_DEFAULT_CTOR(MergeAcquisitionFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(MergeAcquisitionFilter);
};

BTK_SWIG_DECLARE_CLASS(SeparateKnownVirtualMarkersFilter)
{
public:
  enum {AllMarkers = btk::SeparateKnownVirtualMarkersFilter::AllMarkers, Markers = btk::SeparateKnownVirtualMarkersFilter::Markers, VirtualReferenceFrames = btk::SeparateKnownVirtualMarkersFilter::VirtualReferenceFrames, VirtualMarkers = btk::SeparateKnownVirtualMarkersFilter::VirtualMarkers, OtherPoints = btk::SeparateKnownVirtualMarkersFilter::OtherPoints};
  BTK_SWIG_DECLARE_DEFAULT_CTOR(SeparateKnownVirtualMarkersFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(SeparateKnownVirtualMarkersFilter);
};

BTK_SWIG_DECLARE_CLASS(SpecializedPointsExtractor)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(SpecializedPointsExtractor);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(SpecializedPointsExtractor);
};

BTK_SWIG_DECLARE_CLASS(SubAcquisitionFilter)
{
public:
  typedef enum {All = btk::SubAcquisitionFilter::All, PointsOnly = btk::SubAcquisitionFilter::PointsOnly, AnalogsOnly = btk::SubAcquisitionFilter::AnalogsOnly, EventsOnly = btk::SubAcquisitionFilter::EventsOnly} ExtractionOption;
  BTK_SWIG_DECLARE_DEFAULT_CTOR(SubAcquisitionFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(SubAcquisitionFilter);
};

BTK_SWIG_DECLARE_CLASS(VerticalGroundReactionForceGaitEventDetector)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(VerticalGroundReactionForceGaitEventDetector);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(VerticalGroundReactionForceGaitEventDetector);
};

BTK_SWIG_DECLARE_CLASS(WrenchDirectionAngleFilter)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(WrenchDirectionAngleFilter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(WrenchDirectionAngleFilter);
};
