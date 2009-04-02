/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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
#include "btkMetaDataEntry.h"
#include "btkMarkerCollection.h"
#include "btkAnalogCollection.h"

#include <list>

namespace btk
{
  class Acquisition : public DataObject
  {
  public:
	  typedef SharedPtr<Acquisition> Pointer;
    typedef SharedPtr<const Acquisition> ConstPointer;
    
    typedef MarkerCollection::Iterator MarkerIterator;
    typedef MarkerCollection::ConstIterator MarkerConstIterator;
    typedef AnalogCollection::Iterator AnalogIterator;
    typedef AnalogCollection::ConstIterator AnalogConstIterator;
    typedef std::list<MetaDataEntry::Pointer>::iterator MetaDataIterator;
    typedef std::list<MetaDataEntry::Pointer>::const_iterator MetaDataConstIterator;
    
    static Pointer New() {return Pointer(new Acquisition());};
    
    // ~Acquisition(); // Implicit.
    
    MarkerIterator BeginMarker() {return this->m_Markers->Begin();};
    MarkerConstIterator BeginMarker() const {return this->m_Markers->Begin();};
    MarkerIterator EndMarker() {return this->m_Markers->End();};
    MarkerConstIterator EndMarker() const {return this->m_Markers->End();};
    AnalogIterator BeginAnalog() {return this->m_Analogs->Begin();};
    AnalogConstIterator BeginAnalog() const {return this->m_Analogs->Begin();};
    AnalogIterator EndAnalog() {return this->m_Analogs->End();};
    AnalogConstIterator EndAnalog() const {return this->m_Analogs->End();};
    MetaDataIterator BeginMetaData() {return this->mp_MetaData->Begin();};
    MetaDataConstIterator BeginMetaData() const {return this->mp_MetaData->Begin();};
    MetaDataIterator EndMetaData() {return this->mp_MetaData->End();};
    MetaDataConstIterator EndMetaData() const {return this->mp_MetaData->End();};
    
    void Init(int markerNumber, int frameNumber, int analogNumber = 0, int analogSampleNumberPerMarkerFrame = 1);
    
    double GetDuration() {return this->m_MarkerFrequency * this->m_MarkerFrameNumber;};
    int GetFirstFrame() const {return this->m_FirstFrame;};
    void SetFirstFrame(int num);
    int GetLastFrame() const {return (this->m_FirstFrame + this->GetMarkerFrameNumber() - 1);};
    double GetMarkerFrequency() {return this->m_MarkerFrequency;};
    void SetMarkerFrequency(double frequency);
    double GetAnalogFrequency() {return this->m_MarkerFrequency * this->m_AnalogSampleNumberPerMarkerFrame;};
    int GetMarkerFrameNumber() const {return this->m_MarkerFrameNumber;};
    int GetAnalogFrameNumber() const {return this->m_MarkerFrameNumber * this->m_AnalogSampleNumberPerMarkerFrame;};
    
    int GetMarkerNumber() const {return this->m_Markers->GetItemNumber();};
    void SetMarkerNumber(int num);
    int GetAnalogNumber() const {return this->m_Analogs->GetItemNumber();};
    void SetAnalogNumber(int num);
    
    Marker::Pointer GetMarker(int idx);
    Marker::ConstPointer GetMarker(int idx) const;
    void SetMarker(int idx, Marker::Pointer marker);
    MarkerCollection::Pointer GetMarkers() {return this->m_Markers;};
    MarkerCollection::ConstPointer GetMarkers() const {return this->m_Markers;};
    Analog::Pointer GetAnalog(int idx);
    Analog::ConstPointer GetAnalog(int idx) const;
    void SetAnalog(int idx, Analog::Pointer analog);
    AnalogCollection::Pointer GetAnalogs() {return this->m_Analogs;};
    AnalogCollection::ConstPointer GetAnalogs() const {return this->m_Analogs;};
    MetaDataEntry::Pointer GetMetaData() {return this->mp_MetaData;};
    MetaDataEntry::ConstPointer GetMetaData() const {return this->mp_MetaData;};
    void SetMetaData(MetaDataEntry::Pointer metaData);
    
    MarkerIterator FindMarker(const std::string& label);
    MarkerConstIterator FindMarker(const std::string& label) const;
    AnalogIterator FindAnalog(const std::string& label);
    AnalogConstIterator FindAnalog(const std::string& label) const;
    
    void Reset();
    
  protected:
    Acquisition();
    
    void SetMarkerFrameNumber(int frameNumber);
    void SetAnalogFrameNumber(int frameNumber);
    
  private:
    MetaDataEntry::Pointer mp_MetaData;
    MarkerCollection::Pointer m_Markers;
    AnalogCollection::Pointer m_Analogs;
    int m_FirstFrame;    
    double m_MarkerFrequency;
    int m_MarkerFrameNumber;
    int m_AnalogSampleNumberPerMarkerFrame;
  };
};

#endif // __btkAcquisition_h
