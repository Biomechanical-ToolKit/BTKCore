/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

#ifndef TimeEventFunctors_h
#define TimeEventFunctors_h

#include <btkVTKChartTimeSeries.h>

class TimeEventControlerWidget;

class CurrentFrameFunctor : public btk::VTKCurrentFrameFunctor
{
public:
  typedef SharedPtr<CurrentFrameFunctor> Pointer;
  static CurrentFrameFunctor::Pointer New(TimeEventControlerWidget* w) {return Pointer(new CurrentFrameFunctor(w));};
  
  virtual ~CurrentFrameFunctor() {};
  virtual int operator()();
  
protected:
  CurrentFrameFunctor(TimeEventControlerWidget* w);
  
private:
  CurrentFrameFunctor(const CurrentFrameFunctor& ); // Not implemented.
  CurrentFrameFunctor& operator=(const CurrentFrameFunctor& ); // Not implemented.
  
  TimeEventControlerWidget* mp_Object;
};

class RegionOfInterestFunctor : public btk::VTKRegionOfInterestFunctor
{
public:
  typedef SharedPtr<RegionOfInterestFunctor> Pointer;
  static RegionOfInterestFunctor::Pointer New(TimeEventControlerWidget* w) {return Pointer(new RegionOfInterestFunctor(w));};
  
  virtual ~RegionOfInterestFunctor() {};
  
  virtual void operator()(int& left, int& right);
  
protected:
  RegionOfInterestFunctor(TimeEventControlerWidget* w);
  
private:
  RegionOfInterestFunctor(const RegionOfInterestFunctor& ); // Not implemented.
  RegionOfInterestFunctor& operator=(const RegionOfInterestFunctor& ); // Not implemented.
  
  TimeEventControlerWidget* mp_Object;
};

class EventsFunctor : public btk::VTKEventsFunctor
{
public:
  typedef SharedPtr<EventsFunctor> Pointer;
  static EventsFunctor::Pointer New(TimeEventControlerWidget* w) {return Pointer(new EventsFunctor(w));};
  
  virtual ~EventsFunctor() {};
  
  virtual bool operator()(int index, int& typeId, int& frame, double rgb[3]);
  
protected:
  EventsFunctor(TimeEventControlerWidget* w);
  
private:
  EventsFunctor(const EventsFunctor& ); // Not implemented.
  EventsFunctor& operator=(const EventsFunctor& ); // Not implemented.
  
  TimeEventControlerWidget* mp_Object;
};

#endif // TimeEventFunctors_h