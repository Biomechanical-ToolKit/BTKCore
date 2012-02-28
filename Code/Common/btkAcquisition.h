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

#ifndef __btkAcquisition_h
#define __btkAcquisition_h

#include "btkDataObject.h"
#include "btkMetaData.h"
#include "btkEventCollection.h"
#include "btkPointCollection.h"
#include "btkAnalogCollection.h"

#include <list>

namespace btk
{
  class Acquisition : public DataObject
  {
  public:
    typedef enum {Bit8 = 8, Bit10 = 10, Bit12 = 12, Bit14 = 14, Bit16 = 16}  AnalogResolution;

    typedef SharedPtr<Acquisition> Pointer;
    typedef SharedPtr<const Acquisition> ConstPointer;
    
    typedef EventCollection::Iterator EventIterator;
    typedef EventCollection::ConstIterator EventConstIterator;
    typedef PointCollection::Iterator PointIterator;
    typedef PointCollection::ConstIterator PointConstIterator;
    typedef AnalogCollection::Iterator AnalogIterator;
    typedef AnalogCollection::ConstIterator AnalogConstIterator;
    typedef std::list<MetaData::Pointer>::iterator MetaDataIterator;
    typedef std::list<MetaData::Pointer>::const_iterator MetaDataConstIterator;
    
    static Pointer New() {return Pointer(new Acquisition());};
    
    // ~Acquisition(); // Implicit.
    
    // Metadata
    MetaDataIterator BeginMetaData() {return this->mp_MetaData->Begin();};
    MetaDataConstIterator BeginMetaData() const {return this->mp_MetaData->Begin();};
    MetaDataIterator EndMetaData() {return this->mp_MetaData->End();};
    MetaDataConstIterator EndMetaData() const {return this->mp_MetaData->End();};
    MetaData::Pointer GetMetaData() {return this->mp_MetaData;};
    MetaData::ConstPointer GetMetaData() const {return this->mp_MetaData;};
    BTK_COMMON_EXPORT void SetMetaData(MetaData::Pointer metaData);
    
    // Event
    EventIterator BeginEvent() {return this->m_Events->Begin();};
    EventConstIterator BeginEvent() const {return this->m_Events->Begin();};
    EventIterator EndEvent() {return this->m_Events->End();};
    EventConstIterator EndEvent() const {return this->m_Events->End();};
    BTK_COMMON_EXPORT Event::Pointer GetEvent(int idx);
    BTK_COMMON_EXPORT Event::ConstPointer GetEvent(int idx) const;
    BTK_COMMON_EXPORT void SetEvent(int idx, Event::Pointer point);
    EventCollection::Pointer GetEvents() {return this->m_Events;};
    EventCollection::ConstPointer GetEvents() const {return this->m_Events;};
    BTK_COMMON_EXPORT void SetEvents(EventCollection::Pointer evts);
    bool IsEmptyEvent() const {return this->m_Events->IsEmpty();};
    int GetEventNumber() const {return this->m_Events->GetItemNumber();};
    BTK_COMMON_EXPORT void SetEventNumber(int num);
    void ClearEvents() {this->m_Events->Clear();};
    BTK_COMMON_EXPORT EventIterator FindEvent(const std::string& label);
    BTK_COMMON_EXPORT EventConstIterator FindEvent(const std::string& label) const;
    BTK_COMMON_EXPORT void AppendEvent(Event::Pointer e);
    BTK_COMMON_EXPORT void RemoveEvent(int idx);
    BTK_COMMON_EXPORT EventIterator RemoveEvent(EventIterator loc);

    // Point
    PointIterator BeginPoint() {return this->m_Points->Begin();};
    PointConstIterator BeginPoint() const {return this->m_Points->Begin();};
    PointIterator EndPoint() {return this->m_Points->End();};
    PointConstIterator EndPoint() const {return this->m_Points->End();};
    BTK_COMMON_EXPORT Point::Pointer GetPoint(int idx);
    BTK_COMMON_EXPORT Point::ConstPointer GetPoint(int idx) const;
    BTK_COMMON_EXPORT Point::Pointer GetPoint(const std::string& label);
    BTK_COMMON_EXPORT Point::ConstPointer GetPoint(const std::string& label) const;
    BTK_COMMON_EXPORT void SetPoint(int idx, Point::Pointer point);
    PointCollection::Pointer GetPoints() {return this->m_Points;};
    PointCollection::ConstPointer GetPoints() const {return this->m_Points;};
    BTK_COMMON_EXPORT void SetPoints(PointCollection::Pointer points);
    bool IsEmptyPoint() const {return this->m_Points->IsEmpty();};
    int GetPointNumber() const {return this->m_Points->GetItemNumber();};
    BTK_COMMON_EXPORT void SetPointNumber(int num);
    void ClearPoints() {this->m_Points->Clear();};
    BTK_COMMON_EXPORT PointIterator FindPoint(const std::string& label);
    BTK_COMMON_EXPORT PointConstIterator FindPoint(const std::string& label) const;
    BTK_COMMON_EXPORT void AppendPoint(Point::Pointer p);
    BTK_COMMON_EXPORT void RemovePoint(const std::string& label);
    BTK_COMMON_EXPORT void RemovePoint(int idx);
    BTK_COMMON_EXPORT PointIterator RemovePoint(PointIterator loc);
        
    // Analog
    AnalogIterator BeginAnalog() {return this->m_Analogs->Begin();};
    AnalogConstIterator BeginAnalog() const {return this->m_Analogs->Begin();};
    AnalogIterator EndAnalog() {return this->m_Analogs->End();};
    AnalogConstIterator EndAnalog() const {return this->m_Analogs->End();}; 
    bool IsEmptyAnalog() const {return this->m_Analogs->IsEmpty();};    
    int GetAnalogNumber() const {return this->m_Analogs->GetItemNumber();};
    BTK_COMMON_EXPORT void SetAnalogNumber(int num);
    BTK_COMMON_EXPORT Analog::Pointer GetAnalog(int idx);
    BTK_COMMON_EXPORT Analog::ConstPointer GetAnalog(int idx) const;
    BTK_COMMON_EXPORT Analog::Pointer GetAnalog(const std::string& label);
    BTK_COMMON_EXPORT Analog::ConstPointer GetAnalog(const std::string& label) const;
    BTK_COMMON_EXPORT void SetAnalog(int idx, Analog::Pointer analog);
    AnalogCollection::Pointer GetAnalogs() {return this->m_Analogs;};
    AnalogCollection::ConstPointer GetAnalogs() const {return this->m_Analogs;};
    BTK_COMMON_EXPORT void SetAnalogs(AnalogCollection::Pointer analogs);
    void ClearAnalogs() {this->m_Analogs->Clear();};
    BTK_COMMON_EXPORT AnalogIterator FindAnalog(const std::string& label);
    BTK_COMMON_EXPORT AnalogConstIterator FindAnalog(const std::string& label) const;
    BTK_COMMON_EXPORT void AppendAnalog(Analog::Pointer ac);
    BTK_COMMON_EXPORT void RemoveAnalog(const std::string& label);
    BTK_COMMON_EXPORT void RemoveAnalog(int idx);
    BTK_COMMON_EXPORT AnalogIterator RemoveAnalog(AnalogIterator loc);
        
    // Others
    BTK_COMMON_EXPORT void Init(int pointNumber, int frameNumber, int analogNumber = 0, int analogSampleNumberPerPointFrame = 1);
    BTK_COMMON_EXPORT void Resize(int pointNumber, int frameNumber, int analogNumber = 0, int analogSampleNumberPerPointFrame = 1);
    BTK_COMMON_EXPORT void ResizePointNumber(int pointNumber);
    BTK_COMMON_EXPORT void ResizeAnalogNumber(int analogNumber);
    BTK_COMMON_EXPORT void ResizeFrameNumber(int frameNumber);
    BTK_COMMON_EXPORT void ResizeFrameNumberFromEnd(int frameNumber);
    BTK_COMMON_EXPORT void Reset();
    double GetDuration() const {return ((this->m_PointFrequency == 0) ? 0 : 1 / this->m_PointFrequency * this->m_PointFrameNumber);};
    int GetFirstFrame() const {return this->m_FirstFrame;};
    BTK_COMMON_EXPORT void SetFirstFrame(int num);
    int GetLastFrame() const {return (this->m_FirstFrame + this->GetPointFrameNumber() - 1);};
    const std::string GetPointUnit() const {return this->m_Units[Point::Marker];};
    void SetPointUnit(const std::string& unit = "mm") {this->SetPointUnit(Point::Marker, unit);};
    const std::string GetPointUnit(Point::Type t) const {return this->m_Units[t];};
    BTK_COMMON_EXPORT void SetPointUnit(Point::Type t, const std::string& units);
    const std::vector<std::string>& GetPointUnits() const {return this->m_Units;};
    BTK_COMMON_EXPORT void SetPointUnits(const std::vector<std::string>& units);
    int GetPointFrameNumber() const {return this->m_PointFrameNumber;};
    double GetPointFrequency() const {return this->m_PointFrequency;};
    BTK_COMMON_EXPORT void SetPointFrequency(double frequency);
    int GetAnalogFrameNumber() const {return this->m_PointFrameNumber * this->m_AnalogSampleNumberPerPointFrame;};    
    int GetNumberAnalogSamplePerFrame() const {return this->m_AnalogSampleNumberPerPointFrame;};
    double GetAnalogFrequency() const {return this->m_PointFrequency * static_cast<double>(this->m_AnalogSampleNumberPerPointFrame);};
    AnalogResolution GetAnalogResolution() const {return this->m_AnalogResolution;};
    BTK_COMMON_EXPORT void SetAnalogResolution(AnalogResolution r);
    int GetMaxInterpolationGap() const {return this->m_MaxInterpolationGap;};
    BTK_COMMON_EXPORT void SetMaxInterpolationGap(int gap);
    
    Pointer Clone() const {return Pointer(new Acquisition(*this));};
    
  protected:
    BTK_COMMON_EXPORT Acquisition();
    
    BTK_COMMON_EXPORT void SetPointFrameNumber(int frameNumber);
    BTK_COMMON_EXPORT void SetAnalogFrameNumber(int frameNumber);
    
  private:
    BTK_COMMON_EXPORT Acquisition(const Acquisition& toCopy);
    Acquisition& operator=(const Acquisition& ); // Not implemented.
    
    MetaData::Pointer mp_MetaData;
    EventCollection::Pointer m_Events;
    PointCollection::Pointer m_Points;
    AnalogCollection::Pointer m_Analogs;
    int m_FirstFrame;    
    double m_PointFrequency;
    int m_PointFrameNumber;
    int m_AnalogSampleNumberPerPointFrame;
    AnalogResolution m_AnalogResolution;
    std::vector<std::string> m_Units;
    int m_MaxInterpolationGap;
  };
};

#endif // __btkAcquisition_h
