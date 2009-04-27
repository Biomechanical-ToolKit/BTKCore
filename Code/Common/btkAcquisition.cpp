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

#include "btkAcquisition.h"
#include "btkException.h"

namespace btk
{
  /**
   * @class Acquisition
   * @brief Contains the data related to a biomechanical acquisition.
   *
   * A biomechanical acquisition can contains:
   *  - 3D data (makers, points, forces, moments, ...) ;
   *  - analog channels (force plateform, emg, ...) ;
   *  - events
   *  - acquisition's setup ;
   *  - subject's informations.
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef Acquisition::Pointer
   * Smart pointer associated with an Acquisition object.
   */
  
  /**
   * @typedef Acquisition::ConstPointer
   * Smart pointer associated with a const Acquisition object.
   */
  
	/**
   * @typedef Acquisition::EventIterator
   * Iterator for events contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::EventConstIterator
   * Const iterator for events contained in the acquisition object.
   */

  /**
   * @typedef Acquisition::MarkerIterator
   * Iterator for markers contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::MarkerConstIterator
   * Const iterator for markers contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::AnalogIterator
   * Iterator for analog channels contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::AnalogConstIterator
   * Const iterator for analog channels contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::MetaDataIterator
   * Iterator for metadata contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::MetaDataConstIterator
   * Const iterator for metadata contained in the acquisition object.
   */
  
  /**
   * @fn static Acquisition::Pointer Acquisition::New()
   * Creates an Acquisition object and return it as smart pointer.
   */
  
 	/**
   * @fn Acquisition::EventIterator Acquisition::BeginEvent()
   * Returns an iterator to the beginning of the list of events.
   */
  
  /**
   * @fn Acquisition::EventConstIterator Acquisition::BeginEvent() const
   * Returns a const iterator to the beginning of the list of events.
   */
  
  /**
   * @fn Acquisition::EventIterator Acquisition::EndEvent()
   * Returns an iterator just past the last event.
   */
  
  /**
   * @fn Acquisition::EventConstIterator Acquisition::EndEvent() const
   * Returns a const iterator just past the last event.
   */

  /**
   * @fn Acquisition::MarkerIterator Acquisition::BeginMarker()
   * Returns an iterator to the beginning of the list of markers.
   */
  
  /**
   * @fn Acquisition::MarkerConstIterator Acquisition::BeginMarker() const
   * Returns a const iterator to the beginning of the list of markers.
   */
  
  /**
   * @fn Acquisition::MarkerIterator Acquisition::EndMarker()
   * Returns an iterator just past the last marker.
   */
  
  /**
   * @fn Acquisition::MarkerConstIterator Acquisition::EndMarker() const
   * Returns a const iterator just past the last marker.
   */
  
  /**
   * @fn Acquisition::AnalogIterator Acquisition::BeginAnalog()
   * Returns an iterator to the beginning of the list of analog channels.
   */
  
  /**
   * @fn Acquisition::AnalogConstIterator Acquisition::BeginAnalog() const
   * Returns a const iterator to the beginning of the list of analog channels.
   */
  
  /**
   * @fn Acquisition::AnalogIterator Acquisition::EndAnalog()
   * Returns an iterator just past the last analog channel.
   */
  
  /**
   * @fn Acquisition::AnalogConstIterator Acquisition::EndAnalog() const
   * Returns a const iterator just past the last analog channel.
   */
  
  /**
   * @fn Acquisition::MetaDataIterator Acquisition::BeginMetaData()
   * Returns an iterator to the beginning of the list of metadata.
   */
  
  /**
   * @fn Acquisition::MetaDataConstIterator Acquisition::BeginMetaData() const
   * Returns a const iterator to the beginning of the list of metadata
   */
  
  /**
   * @fn Acquisition::MetaDataIterator Acquisition::EndMetaData()
   * Returns an iterator just past the last metadata.
   */
  
  /**
   * @fn Acquisition::MetaDataConstIterator Acquisition::EndMetaData() const
   * Returns a const iterator just past the last metadata.
   */
  
  /**
   * Initializes the acquisition with @a markerNumber which have @a frameNumber
   * frame. The analog part has @a analogNumber analog channels and their sampling rate
   * correspond the integer factor @a analogSampleNumberPerMarkerFrame.
   */
  void Acquisition::Init(int markerNumber, int frameNumber,
                         int analogNumber, int analogSampleNumberPerMarkerFrame)
  {
    // Marker
    if (frameNumber <= 0)
    {
      btkErrorMacro("Impossible to set the frame number to 0 or lower. The number of frames is now equals to 1.");
			frameNumber = 1;
    }
    if (analogSampleNumberPerMarkerFrame <= 0)
    {
      btkErrorMacro("Impossible to set the analog sample number to 0. The numbers of analog samples per marker frame is now equals to 1.");
      analogSampleNumberPerMarkerFrame = 1;
    }
    if (markerNumber == 0)
    {
      this->m_Markers->SetItemNumber(0);
      this->m_MarkerFrameNumber = frameNumber;
    }
		else
		{
    	if (markerNumber < this->GetMarkerNumber())
	      this->m_Markers->SetItemNumber(markerNumber);
	    else
	    {
	      for (int inc = this->GetMarkerNumber() ; inc < markerNumber ; ++inc)
	      {
        	Marker::Pointer pt = Marker::New(frameNumber);
        	pt->SetParent(this);
        	this->m_Markers->InsertItem(pt);
	      }
    	}
    	this->SetMarkerFrameNumber(frameNumber);
		}
    // Analog
    if (analogNumber == 0)
    {
      this->m_Analogs->SetItemNumber(0);
      this->m_AnalogSampleNumberPerMarkerFrame = analogSampleNumberPerMarkerFrame;
    }
		else
		{
    	if (analogNumber < this->GetAnalogNumber())
	      this->m_Analogs->SetItemNumber(analogNumber);
	    else
	    {
	      for (int inc = this->GetAnalogNumber() ; inc < analogNumber ; ++inc)
	      {
	        Analog::Pointer pt = Analog::New(this->m_MarkerFrameNumber * analogSampleNumberPerMarkerFrame);
	        pt->SetParent(this);
	        this->m_Analogs->InsertItem(pt);
	      }
    	}
    	this->SetAnalogFrameNumber(analogSampleNumberPerMarkerFrame);
		}
    // Modification update
    this->Modified();  
  };
  
  /**
   * @fn double Acquisition::GetDuration()
   * Returns the duration of the acquistion. The duration is computed as the 
   * multiplication of the markers' frequency with the markers frame number.
   */
  
  /**
   * @fn int Acquisition::GetFirstFrame() const
   * Return the first frame index of the acquisition.
   */
  
  /**
   * Sets the first frame index.
   */
  void Acquisition::SetFirstFrame(int num)
  {
    if (num <= 0)
    {
      btkErrorMacro("Impossible to set the first frame to 0 or lower.");
      return;
    }
    this->m_FirstFrame = num;
  };
  
  /**
   * @fn int Acquisition::GetLastFrame() const;
   * Returns the last frame index of the acquisition base on the first frame index and the frame number.
   */
  
   /**
    * @fn double Acquisition::GetMarkerFrequency();
    * Returns the markers sample rate.
    */
  
  /**
   * Sets the markers sample rate.
   */
  void Acquisition::SetMarkerFrequency(double frequency)
  {
    if (frequency == this->m_MarkerFrequency)
      return;
    if (frequency == 0)
    {
      btkErrorMacro("Impossible to set the marker's frequecy to 0.");
      return;
    }
    this->m_MarkerFrequency = frequency;
    this->Modified();
  };
  
  /**
   * @fn double Acquisition::GetAnalogFrequency()
   * Returns the markers sample rate.
   */
  
  /**
   * @fn int Acquisition::GetMarkerFrameNumber() const
   * Returns the number of frames for the markers.
   */
  
  /**
   * @fn int Acquisition::GetAnalogFrameNumber() const
   * Returns the number of frames for the analog channels.
   */

	/**
   * @fn int Acquisition::GetEventNumber() const
   * Returns the number of events.
   */
  
  /**
   * Sets the number of markers.
   */
  void Acquisition::SetEventNumber(int num)
  {
    if (num == this->GetEventNumber())
      return;
		if (num < this->GetEventNumber())
      this->m_Events->SetItemNumber(num);
    else
    {
      for (int inc = this->GetEventNumber() ; inc < num ; ++inc)
        this->m_Events->InsertItem(Event::New());
    }
    this->Modified();
  };
  
  /**
   * @fn int Acquisition::GetMarkerNumber() const
   * Returns the number of markers.
   */
  
  /**
   * Sets the number of markers.
   */
  void Acquisition::SetMarkerNumber(int num)
  {
    if (num == this->GetMarkerNumber())
      return;
    if (this->GetMarkerFrameNumber() == 0)
    {
      btkErrorMacro("The acquisition's duration or the marker's frequency is not set.");
      return;
    }
    if (num < this->GetMarkerNumber())
      this->m_Markers->SetItemNumber(num);
    else
    {
      for (int inc = this->GetMarkerNumber() ; inc < num ; ++inc)
        this->m_Markers->InsertItem(Marker::New(this->GetMarkerFrameNumber()));
    }
    this->Modified();
  };
  
  /**
   * @fn int Acquisition::GetAnalogNumber() const
   * Returns the number of analog channels.
   */
  
  /**
   * Sets the number of analog channels.
   */
  void Acquisition::SetAnalogNumber(int num)
  {
    if (num == this->GetAnalogNumber())
      return;
    if (this->GetAnalogFrameNumber() == 0)
    {
      btkErrorMacro("The acquisition's duration or the marker's frequency is not set.");
      return;
    }
    if (num < this->GetAnalogNumber())
      this->m_Analogs->SetItemNumber(num);
    else
    {
      for (int inc = this->GetAnalogNumber() ; inc < num ; ++inc)
        this->m_Analogs->InsertItem(Analog::New(this->GetAnalogFrameNumber()));
    }
    this->Modified();
  };

	/**
   * Gets the event at the index @a idx as a smart pointer.
   */
  Event::Pointer Acquisition::GetEvent(int idx)
  {
    if (idx >= this->GetEventNumber())
      throw(OutOfRangeException("Acquisition::GetEvent"));
    EventIterator it = this->BeginEvent();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the event at the index @a idx as a const smart pointer.
   */
  Event::ConstPointer Acquisition::GetEvent(int idx) const
  {
    if (idx >= this->GetEventNumber())
      throw(OutOfRangeException("Acquisition::GetEvent"));
    EventConstIterator it = this->BeginEvent();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Sets the content of @a event at the index @a idx.
   */
  void Acquisition::SetEvent(int idx, Event::Pointer marker)
  {
    if (idx >= this->GetEventNumber())
    {
      btkErrorMacro("Out of range");
      return;
    }
    EventIterator it = this->BeginEvent();
    std::advance(it, idx);
    *it = marker;
  };
  
  /**
   * @fn EventCollection::Pointer Acquisition::GetEvents()
   * Returns the collection of events.
   */
  
  /**
   * @fn EventCollection::ConstPointer Acquisition::GetEvents() const
   * Returns the collection of events.
   */
  
  /**
   * Gets the marker at the index @a idx as a smart pointer.
   */
  Marker::Pointer Acquisition::GetMarker(int idx)
  {
    if (idx >= this->GetMarkerNumber())
      throw(OutOfRangeException("Acquisition::GetMarker"));
    MarkerIterator it = this->BeginMarker();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the marker at the index @a idx as a const smart pointer.
   */
  Marker::ConstPointer Acquisition::GetMarker(int idx) const
  {
    if (idx >= this->GetMarkerNumber())
      throw(OutOfRangeException("Acquisition::GetMarker"));
    MarkerConstIterator it = this->BeginMarker();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Sets the content of @a marker at the index @a idx.
   */
  void Acquisition::SetMarker(int idx, Marker::Pointer marker)
  {
    if (idx >= this->GetMarkerNumber())
    {
      btkErrorMacro("Out of range");
      return;
    }
    MarkerIterator it = this->BeginMarker();
    std::advance(it, idx);
    *it = marker;
  };
  
  /**
   * @fn MarkerCollection::Pointer Acquisition::GetMarkers()
   * Returns the collection of markers.
   */
  
  /**
   * @fn MarkerCollection::ConstPointer Acquisition::GetMarkers() const
   * Returns the collection of markers.
   */
  
  /**
   * Gets the analog channel at the index @a idx as a smart pointer.
   */
  Analog::Pointer Acquisition::GetAnalog(int idx)
  {
    if (idx >= this->GetAnalogNumber())
      throw(OutOfRangeException("Acquisition::GetAnalog"));
    AnalogIterator it = this->BeginAnalog();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the analog channel at the index @a idx as a const smart pointer.
   */
  Analog::ConstPointer Acquisition::GetAnalog(int idx) const
  {
    if (idx >= this->GetAnalogNumber())
      throw(OutOfRangeException("Acquisition::GetAnalog"));
    AnalogConstIterator it = this->BeginAnalog();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Sets the analog channel at the index @a idx by the content of @a analog.
   */
  void Acquisition::SetAnalog(int idx, Analog::Pointer analog)
  {
    if (idx >= this->GetAnalogNumber())
    {
      btkErrorMacro("Out of range");
      return;
    }
    AnalogIterator it = this->BeginAnalog();
    std::advance(it, idx);
    *it = analog;
  };
  
  /**
   * @fn AnalogCollection::Pointer Acquisition::GetAnalogs()
   * Returns the collection of analog channels.
   */
  
  /**
   * @fn AnalogCollection::ConstPointer Acquisition::GetAnalogs() const
   * Returns the collection of analog channels.
   */
  
  /**
   * @fn MetaDataEntry::Pointer Acquisition::GetMetaData() {return this->mp_MetaData;};
   * Returns the metadata.
   */ 
  
  /**
   * @fn MetaDataEntry::ConstPointer Acquisition::GetMetaData() const {return this->mp_MetaData;};
   * Returns the metadata.
   */
  
  /**
   * Sets the metadata of the acquisition.
   */
  void Acquisition::SetMetaData(MetaDataEntry::Pointer metaData)
  {
    if (this->mp_MetaData == metaData)
      return;
    this->mp_MetaData->SetParent(static_cast<DataObject*>(0));
    this->mp_MetaData = metaData;
    if (this->mp_MetaData.get() == 0)
      this->mp_MetaData->SetParent(this);
    this->Modified();
  };
  
	/**
   * Finds the event with the proposed @a label and returns the iterator associed
   * with it. If no event has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::EventIterator Acquisition::FindEvent(const std::string& label)
  {
    EventIterator it = this->BeginEvent();
    while (it != this->EndEvent())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the event with the proposed @a label and returns the const iterator associed
   * with it. If no event has @a label as label, a const iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::EventConstIterator Acquisition::FindEvent(const std::string& label) const
  {
    EventConstIterator it = this->BeginEvent();
    while (it != this->EndEvent())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };


  /**
   * Finds the marker with the proposed @a label and returns the iterator associed
   * with it. If no marker has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::MarkerIterator Acquisition::FindMarker(const std::string& label)
  {
    MarkerIterator it = this->BeginMarker();
    while (it != this->EndMarker())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the marker with the proposed @a label and returns the const iterator associed
   * with it. If no marker has @a label as label, a const iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::MarkerConstIterator Acquisition::FindMarker(const std::string& label) const
  {
    MarkerConstIterator it = this->BeginMarker();
    while (it != this->EndMarker())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the analog channel with the proposed @a label and returns the iterator 
   * associated with it. If no marker has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::AnalogIterator Acquisition::FindAnalog(const std::string& label)
  {
    AnalogIterator it = this->BeginAnalog();
    while (it != this->EndAnalog())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the analog channel with the proposed @a label and returns the const iterator 
   * associated with it. If no marker has @a label as label, a const iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::AnalogConstIterator Acquisition::FindAnalog(const std::string& label) const
  {
    AnalogConstIterator it = this->BeginAnalog();
    while (it != this->EndAnalog())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Resets the acquisition as an empty acquisition:
   * - Zero markers
   * - Zero analog channels.
   * - First frame sets to 1.
   * - Marker frequency sets to 0.
   * - Marker frame number sets to 0.
   * - Empty metadata.
   * To re-populate this acquisition, you need to re-use the Init() method to
   * set the marker and analog number and their frame number.
   */
  void Acquisition::Reset()
  {
		this->m_Events->SetItemNumber(0);
    this->m_Markers->SetItemNumber(0);
    this->m_Analogs->SetItemNumber(0);
    this->m_FirstFrame = 1;
    this->m_MarkerFrequency = 0;
    this->m_MarkerFrameNumber = 0;
    this->m_AnalogSampleNumberPerMarkerFrame = 1;
    this->mp_MetaData = MetaDataEntry::New("ROOT");
    this->mp_MetaData->SetParent(this);
    this->Modified();
  }
  
  /**
   * Constructor.
   */
  Acquisition::Acquisition()
  : DataObject()
  {
		this->m_Events = EventCollection::New();
    this->m_Markers = MarkerCollection::New();
    this->m_Analogs = AnalogCollection::New();
    this->mp_MetaData = MetaDataEntry::New("ROOT");
    this->m_FirstFrame = 1;
    this->m_MarkerFrequency = 0;
    this->m_MarkerFrameNumber = 0;
    this->m_AnalogSampleNumberPerMarkerFrame = 1;
  };
  
  /**
   * Set the number of frames with @a frameNumber for each marker.
   */
  void Acquisition::SetMarkerFrameNumber(int frameNumber)
  {
    this->m_MarkerFrameNumber = frameNumber;
    MarkerIterator it = this->BeginMarker();
    while (it != this->EndMarker())
    {
      (*it)->SetFrameNumber(frameNumber);
      ++it;
    }
  };
  
  /**
   * Set the number of frames for each analog channels as the multiplication of 
   * the number of frames of the markers with @a analogSampleNumberPerMarkerFrame.
   * The input value @a analogSampleNumberPerMarkerFrame corresponds to the oversampling
   * of the analog channels related to the sampling rate of the markers.
   */  
  void Acquisition::SetAnalogFrameNumber(int analogSampleNumberPerMarkerFrame)
  {
    this->m_AnalogSampleNumberPerMarkerFrame = analogSampleNumberPerMarkerFrame;
    AnalogIterator it = this->BeginAnalog();
    while (it != this->EndAnalog())
    {
      (*it)->SetFrameNumber(this->m_MarkerFrameNumber * this->m_AnalogSampleNumberPerMarkerFrame);
      ++it;
    }
  };
}
