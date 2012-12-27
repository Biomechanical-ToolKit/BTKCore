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

#ifndef __btkVTKChartExtraAcquisitionFunctor_h
#define __btkVTKChartExtraAcquisitionFunctor_h

#include "btkSharedPtr.h"

namespace btk
{
  class VTKCurrentFrameFunctor
  {
  public:
    typedef SharedPtr<VTKCurrentFrameFunctor> Pointer;
    virtual ~VTKCurrentFrameFunctor() {};
    virtual int operator()() = 0;
    virtual VTKCurrentFrameFunctor::Pointer ResetedSuperClone() = 0;
  protected:
    VTKCurrentFrameFunctor() {};
  private:
    VTKCurrentFrameFunctor(const VTKCurrentFrameFunctor& ); // Not implemented.
    VTKCurrentFrameFunctor& operator=(const VTKCurrentFrameFunctor& ); // Not implemented.
  };

  class VTKRegionOfInterestFunctor
  {
  public:
    typedef SharedPtr<VTKRegionOfInterestFunctor> Pointer;
    virtual ~VTKRegionOfInterestFunctor() {};
    virtual void operator()(int& left, int& right) = 0;
    virtual VTKRegionOfInterestFunctor::Pointer ResetedSuperClone() = 0;
  protected:
    VTKRegionOfInterestFunctor() {};
  private:
    VTKRegionOfInterestFunctor(const VTKRegionOfInterestFunctor& ); // Not implemented.
    VTKRegionOfInterestFunctor& operator=(const VTKRegionOfInterestFunctor& ); // Not implemented.
  };

  class VTKEventsFrameMapperFunctor
  {
  public:
    typedef SharedPtr<VTKEventsFrameMapperFunctor> Pointer;
    virtual ~VTKEventsFrameMapperFunctor() {};
    virtual float operator()(int index, int side, int shift) = 0;
  protected:
    VTKEventsFrameMapperFunctor() {};
  private:
    VTKEventsFrameMapperFunctor(const VTKEventsFrameMapperFunctor& ); // Not implemented.
    VTKEventsFrameMapperFunctor& operator=(const VTKEventsFrameMapperFunctor& ); // Not implemented.
  };

  class VTKEventsFunctor
  {
  public:
    typedef SharedPtr<VTKEventsFunctor> Pointer;
    virtual ~VTKEventsFunctor() {};
    virtual bool operator()(int index, int& typeId, float& x, double rgb[3]) = 0;
    virtual VTKEventsFunctor::Pointer ResetedSuperClone() = 0;
    VTKEventsFrameMapperFunctor::Pointer GetFrameMapper() const {return this->mp_FrameMapper;};
    void SetFrameMapper(VTKEventsFrameMapperFunctor::Pointer mapper) {this->mp_FrameMapper = mapper;};
  protected:
    VTKEventsFunctor() : mp_FrameMapper() {};
    
    VTKEventsFrameMapperFunctor::Pointer mp_FrameMapper;
  private:
    VTKEventsFunctor(const VTKEventsFunctor& ); // Not implemented.
    VTKEventsFunctor& operator=(const VTKEventsFunctor& ); // Not implemented.
  };
  
  /**
   * @class VTKCurrentFrameFunctor btkVTKChartExtraAcquisitionFunctor.h
   * @brief Functor to get easily to this chart the current frame displayed.
   *
   * This is usefull when combined with a timer or other view, like a 3D view.
   */
  /**
   * @typedef VTKCurrentFrameFunctor::Pointer
   * Smart pointer associated with a VTKCurrentFrameFunctor object.
   */
  /**
   * @fn virtual int VTKCurrentFrameFunctor::operator()() = 0;
   * Operator used to return the current frame displayed.
   */
  /**
   * @fn virtual VTKCurrentFrameFunctor::Pointer VTKCurrentFrameFunctor::ResetedSuperClone() = 0;
   * Create a clone of the object and reset it.
   */
   
  /**
   * @class VTKRegionOfInterestFunctor btkVTKChartExtraAcquisitionFunctor.h
   * @brief Functor to get easily to this chart the region of interest of the time series.
   */
  /**
   * @typedef VTKRegionOfInterestFunctor::Pointer
   * Smart pointer associated with a VTKCurrentFrameFunctor object.
   */
  /**
   * @fn virtual void VTKRegionOfInterestFunctor::operator()(int& left, int& right) = 0;
   * Operator used to get the left and right bounds of the region of interest.
   */
  /**
   * @fn virtual VTKRegionOfInterestFunctor::Pointer VTKRegionOfInterestFunctor::ResetedSuperClone() = 0;
   * Create a clone of the object and reset it.
   */
  
  /**
   * @class VTKEventsFrameMapperFunctor btkVTKChartExtraAcquisitionFunctor.h
   * @brief Functor to map event's frame to another value.
   */
  /**
   * @typedef VTKEventsFrameMapperFunctor::Pointer
   * Smart pointer associated with a VTKEventsFrameMapperFunctor object.
   */
  /**
   * @fn virtual float VTKEventsFrameMapperFunctor::operator()(int index, int side, int shift) = 0;
   * Return a value for the event @a index. The side and a frame's shift can be also used to compute the value.
   */
   
  /**
   * @class VTKEventsFunctor btkVTKChartExtraAcquisitionFunctor.h
   * @brief Functor to get easily to this chart the events as types, frames and colors.
   *
   * The types are represented by integer where the values 0, 1 and 2 are for 
   * "General", "Foot strike" and "Foot off" respectively. All of the ohers values have no specific meanings.
   */
  /**
   * @typedef VTKEventsFunctor::Pointer
   * Smart pointer associated with a VTKEventsFunctor object.
   */
  /**
   * @fn virtual bool VTKEventsFunctor::operator()(int index, int& typeId, float& x, double rgb[3]) = 0;
   * Operator used to extract each event. Asking for an event out of range returns false.
   */
  /**
   * @fn virtual VTKEventsFunctor::Pointer VTKEventsFunctor::ResetedSuperClone() = 0;
   * Create a clone of the object and reset it.
   */
  /**
   * @fn VTKEventsFrameMapperFunctor::Pointer VTKEventsFunctor::GetFrameMapper() const
   * Returns the mapper used to map a given frame to another value.
   */
  /**
   * @fn void VTKEventsFunctor::SetFrameMapper(VTKEventsFrameMapperFunctor::Pointer mapper)
   * Sets the mapper used to map a given frame to another value.
   */
  /**
   * @var VTKEventsFunctor::mp_FrameMapper
   * Mapper used to convert a given frame to another value.
   */
};

#endif // __btkVTKChartExtraAcquisitionFunctor_h