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
#include "btkConvert.h"

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
   * @enum Acquisition::AnalogResolution
   * Enums used to specify the ADC resolution used to store analog channel signal.
   */
  /**
   * @var Acquisition::AnalogResolution Acquisition::Bit8
   * 8 bits ADC.
   */
  /**
   * @var Acquisition::AnalogResolution Acquisition::Bit12
   * 12 bits ADC.
   */
  /**
   * @var Acquisition::AnalogResolution Acquisition::Bit14
   * 14 bits ADC.
   */
  /**
   * @var Acquisition::AnalogResolution Acquisition::Bit16
   * 16 bits ADC.
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
   * @typedef Acquisition::PointIterator
   * Iterator for points contained in the acquisition object.
   */
  
  /**
   * @typedef Acquisition::PointConstIterator
   * Const iterator for points contained in the acquisition object.
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
   * @fn MetaData::Pointer Acquisition::GetMetaData()
   * Returns the metadata.
   */ 
  
  /**
   * @fn MetaData::ConstPointer Acquisition::GetMetaData() const
   * Returns the metadata.
   */
  
  /**
   * Sets the metadata of the acquisition.
   */
  void Acquisition::SetMetaData(MetaData::Pointer metaData)
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
  void Acquisition::SetEvent(int idx, Event::Pointer event)
  {
    if (idx >= this->GetEventNumber())
    {
      btkErrorMacro("Out of range");
      return;
    }
    EventIterator it = this->BeginEvent();
    std::advance(it, idx);
    if (*it == event)
      return;
    *it = event;
    this->Modified();
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
   * @fn bool Acquisition::IsEmptyEvent() const
   * Checks if the events' list is empty.
   */

  /**
   * @fn int Acquisition::GetEventNumber() const
   * Returns the number of events.
   */

  /**
   * Sets the number of points.
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
   * @fn void Acquisition::ClearEvent()
   * Clear events.
   */

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
   * @fn Acquisition::PointIterator Acquisition::BeginPoint()
   * Returns an iterator to the beginning of the list of points.
   */
  
  /**
   * @fn Acquisition::PointConstIterator Acquisition::BeginPoint() const
   * Returns a const iterator to the beginning of the list of points.
   */
  
  /**
   * @fn Acquisition::PointIterator Acquisition::EndPoint()
   * Returns an iterator just past the last point.
   */
  
  /**
   * @fn Acquisition::PointConstIterator Acquisition::EndPoint() const
   * Returns a const iterator just past the last point.
   */

  /**
   * Gets the point at the index @a idx as a smart pointer.
   */
  Point::Pointer Acquisition::GetPoint(int idx)
  {
    if (idx >= this->GetPointNumber())
      throw(OutOfRangeException("Acquisition::GetPoint"));
    PointIterator it = this->BeginPoint();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the point at the index @a idx as a const smart pointer.
   */
  Point::ConstPointer Acquisition::GetPoint(int idx) const
  {
    if (idx >= this->GetPointNumber())
      throw(OutOfRangeException("Acquisition::GetPoint"));
    PointConstIterator it = this->BeginPoint();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Sets the content of @a point at the index @a idx.
   */
  void Acquisition::SetPoint(int idx, Point::Pointer point)
  {
    if (idx >= this->GetPointNumber())
    {
      btkErrorMacro("Out of range");
      return;
    }
    PointIterator it = this->BeginPoint();
    std::advance(it, idx);
    if (*it == point)
      return;
    if (point->GetFrameNumber() != this->m_PointFrameNumber)
    {
      btkErrorMacro("Point's frame number is different than acquisition's frame number. Point's frame number is resized.");
      point->SetFrameNumber(this->m_PointFrameNumber);
    }
    *it = point;
    this->Modified();
  };
  
  /**
   * @fn PointCollection::Pointer Acquisition::GetPoints()
   * Returns the collection of points.
   */
  
  /**
   * @fn PointCollection::ConstPointer Acquisition::GetPoints() const
   * Returns the collection of points.
   */

  /**
   * @fn bool Acquisition::IsEmptyPoint() const
   * Checks if the points' list is empty.
   */

  /**
   * @fn int Acquisition::GetPointNumber() const
   * Returns the number of points.
   */
  
  /**
   * Sets the number of points.
   */
  void Acquisition::SetPointNumber(int num)
  {
    if (num == this->GetPointNumber())
      return;
    if (this->GetPointFrameNumber() == 0)
    {
      btkErrorMacro("The acquisition's duration or the point's frequency is not set.");
      return;
    }
    if (num < this->GetPointNumber())
      this->m_Points->SetItemNumber(num);
    else
    {
      for (int inc = this->GetPointNumber() ; inc < num ; ++inc)
        this->m_Points->InsertItem(Point::New(this->GetPointFrameNumber()));
    }
    this->Modified();
  };

  /**
   * @fn void Acquisition::ClearPoint()
   * Clear points.
   */

  /**
   * Finds the point with the proposed @a label and returns the iterator associed
   * with it. If no point has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::PointIterator Acquisition::FindPoint(const std::string& label)
  {
    PointIterator it = this->BeginPoint();
    while (it != this->EndPoint())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the point with the proposed @a label and returns the const iterator associed
   * with it. If no point has @a label as label, a const iterator pointing to the 
   * end of the collection is returned.
   */
  Acquisition::PointConstIterator Acquisition::FindPoint(const std::string& label) const
  {
    PointConstIterator it = this->BeginPoint();
    while (it != this->EndPoint())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };

  /**
   * @fn void Acquisition::AppendPoint(Point::Pointer p)
   * Convenient method to append a point in the acquisition.
   */

  /**
   * Convenient method to delete a point from the acquisition.
   */
  void Acquisition::RemovePoint(const std::string& label)
  {
    PointIterator it = this->FindPoint(label);
    if (it == this->EndPoint())
      btkErrorMacro("No point with label: '" + label + "'.");
    this->RemovePoint(it);
  };

  /**
   * @fn void Acquisition::RemovePoint(int idx)
   * Convenient method to delete a point from the acquisition.
   */

  /**
   * @fn void Acquisition::RemovePoint(PointIterator it)
   * Convenient method to delete a point from the acquisition.
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
   * @fn bool Acquisition::IsEmptyAnalog() const
   * Checks if the analogs' list is empty.
   */

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
      btkErrorMacro("The acquisition's duration or the point's frequency is not set.");
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
    if (*it == analog)
      return;
    *it = analog;
    this->Modified();
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
   * @fn void Acquisition::ClearAnalog()
   * Clear analogs channels.
   */

  /**
   * Finds the analog channel with the proposed @a label and returns the iterator 
   * associated with it. If no point has @a label as label, an iterator pointing to the 
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
   * associated with it. If no point has @a label as label, a const iterator pointing to the 
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
   * @fn void Acquisition::Init(int pointNumber, int frameNumber, int analogNumber = 0, int analogSampleNumberPerPointFrame = 1)
   * Initialize the acquisition with @a pointNumber which have @a frameNumber
   * frame. The analog part has @a analogNumber analog channels and their sampling rate
   * correspond the integer factor @a analogSampleNumberPerPointFrame.
   */
  void Acquisition::Init(int pointNumber, int frameNumber, int analogNumber, int analogSampleNumberPerPointFrame)
  {
    this->Resize(pointNumber, frameNumber, analogNumber, analogSampleNumberPerPointFrame);
    int inc = 1;
    for (PointIterator itPoint = this->BeginPoint() ; itPoint != this->EndPoint() ; ++itPoint)
      (*itPoint)->SetLabel("uname*" + ToString(inc++));
    inc = 1;
    for (AnalogIterator itAnalog = this->BeginAnalog() ; itAnalog != this->EndAnalog() ; ++itAnalog)
      (*itAnalog)->SetLabel("uname*" + ToString(inc++));
  };

  /**
   * Resize the acquisition with @a pointNumber which have @a frameNumber
   * frame. The analog part has @a analogNumber analog channels and their sampling rate
   * correspond the integer factor @a analogSampleNumberPerPointFrame.
   */
  void Acquisition::Resize(int pointNumber, int frameNumber,
                           int analogNumber, int analogSampleNumberPerPointFrame)
  {
    // Point
    if (frameNumber <= 0)
    {
      btkErrorMacro("Impossible to set the frame number to 0 or lower. The number of frames is now equals to 1.");
      frameNumber = 1;
    }
    if (analogSampleNumberPerPointFrame <= 0)
    {
      btkErrorMacro("Impossible to set the analog sample number to 0. The numbers of analog samples per point frame is now equals to 1.");
      analogSampleNumberPerPointFrame = 1;
    }
    if (pointNumber == 0)
    {
      this->m_Points->SetItemNumber(0);
      this->m_PointFrameNumber = frameNumber;
    }
    else
    {
      if (pointNumber < this->GetPointNumber())
        this->m_Points->SetItemNumber(pointNumber);
      else
      {
        for (int inc = this->GetPointNumber() ; inc < pointNumber ; ++inc)
        {
          Point::Pointer pt = Point::New(frameNumber);
          pt->SetParent(this);
          this->m_Points->InsertItem(pt);
        }
      }
      this->SetPointFrameNumber(frameNumber);
    }
    // Analog
    if (analogNumber == 0)
    {
      this->m_Analogs->SetItemNumber(0);
      this->m_AnalogSampleNumberPerPointFrame = analogSampleNumberPerPointFrame;
    }
    else
    {
      if (analogNumber < this->GetAnalogNumber())
        this->m_Analogs->SetItemNumber(analogNumber);
      else
      {
        for (int inc = this->GetAnalogNumber() ; inc < analogNumber ; ++inc)
        {
          Analog::Pointer pt = Analog::New(this->m_PointFrameNumber * analogSampleNumberPerPointFrame);
          pt->SetParent(this);
          this->m_Analogs->InsertItem(pt);
        }
      }
      this->SetAnalogFrameNumber(analogSampleNumberPerPointFrame);
    }
    // Modification update
    this->Modified();  
  };

  /**
   * Resets the acquisition as an empty acquisition:
   * - Zero points ;
   * - Zero analog channels ;
   * - First frame sets to 1 ;
   * - Point frequency sets to 0 ;
   * - Point frame number sets to 0 ;
   * - Empty metadata ;
   * - Analog resolution: 12 bits ;
   * - Default units.
   * To re-populate this acquisition, you need to re-use the Init() method 
   * to set the point and analog number and their frame number.
   */
  void Acquisition::Reset()
  {
    this->m_Events->SetItemNumber(0);
    this->m_Points->SetItemNumber(0);
    this->m_Analogs->SetItemNumber(0);
    this->m_FirstFrame = 1;
    this->m_PointFrequency = 0;
    this->m_PointFrameNumber = 0;
    this->m_AnalogSampleNumberPerPointFrame = 1;
    this->mp_MetaData = MetaData::New("ROOT");
    this->mp_MetaData->SetParent(this);
    this->m_AnalogResolution = Bit12;
    this->m_Units.resize(7,"");
    this->m_Units[Point::Marker] = "mm";
    this->m_Units[Point::Angle] = "deg";
    this->m_Units[Point::Force] = "N";
    this->m_Units[Point::Moment] = "Nmm";
    this->m_Units[Point::Power] = "W";
    this->m_Units[Point::Scalar] = "";
    this->m_Units[Point::Reaction] = "";
    this->Modified();
  };

  /**
   * @fn double Acquisition::GetDuration()
   * Returns the duration of the thisuistion. The duration is computed as the 
   * multiplication of the points' frequency with the points frame number.
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
    this->Modified();
  };
  
  /**
   * @fn int Acquisition::GetLastFrame() const;
   * Returns the last frame index of the acquisition base on the first frame index and the frame number.
   */

  /**
   * @fn double Acquisition::GetPointFrequency() const
   * Returns the point's frequency.
   */

  /**
   * @fn const std::string Acquisition::GetPointUnit() const
   * Returns the unit for points of type Point::Marker
   */

  /**
   * @fn void Acquisition::SetPointUnit(const std::string& units)
   * Sets the unit for points of type Point::Marker.
   */

  /**
   * @fn const std::string Acquisition::GetPointUnit(Point::Type t) const
   * Returns the unit for points of type @a t.
   */
  
  /**
   * Sets the point's unit for the Point's type @a t with the value @a units.
   */
  void Acquisition::SetPointUnit(Point::Type t, const std::string& units)
  {
    std::string u = units;
    u = u.erase(u.find_last_not_of(' ') + 1);
    u = u.erase(0, u.find_first_not_of(' '));
    u = u.erase(u.find_last_not_of(static_cast<char>(0x00)) + 1);
    u = u.erase(0, u.find_first_not_of(static_cast<char>(0x00)));  
    if (this->m_Units[t].compare(u) == 0)
      return;
    this->m_Units[t] = u;
    this->Modified();
  };

  /**
   * @fn int Acquisition::GetPointFrameNumber() const
   * Returns the number of frames for the points.
   */
  
  /**
   * @fn double Acquisition::GetPointFrequency() const
   * Returns the points sample rate.
   */

  /**
   * Sets the points sample rate.
   */
  void Acquisition::SetPointFrequency(double frequency)
  {
    if (frequency == this->m_PointFrequency)
      return;
    if (frequency == 0)
    {
      btkErrorMacro("Impossible to set the point's frequecy to 0.");
      return;
    }
    this->m_PointFrequency = frequency;
    this->Modified();
  };
  
    
  /**
   * @fn int Acquisition::GetAnalogFrameNumber() const
   * Returns the number of frames for the analog channels.
   */

  /**
   * @fn double Acquisition::GetAnalogFrequency()
   * Returns the points sample rate.
   */

  /**
   * @fn AnalogResolution Acquisition::GetAnalogResolution() const
   * Returns the analog resolution.
   */
      
  /**
   * Sets the analog resolution.
   */
  void Acquisition::SetAnalogResolution(AnalogResolution r)
  {
    if (this->m_AnalogResolution == r)
      return;
    this->m_AnalogResolution = r;
    this->Modified();
  };
    
  /**
   * Constructor.
   */
  Acquisition::Acquisition()
  : DataObject(), m_Units(std::vector<std::string>(7,""))
  {
    this->m_Events = EventCollection::New();
    this->m_Points = PointCollection::New();
    this->m_Analogs = AnalogCollection::New();
    this->mp_MetaData = MetaData::New("ROOT");
    this->mp_MetaData->SetParent(this);
    this->m_FirstFrame = 1;
    this->m_PointFrequency = 0;
    this->m_PointFrameNumber = 0;
    this->m_AnalogSampleNumberPerPointFrame = 1;
    this->m_AnalogResolution = Bit12; 
    this->m_Units[Point::Marker] = "mm";
    this->m_Units[Point::Angle] = "deg";
    this->m_Units[Point::Force] = "N";
    this->m_Units[Point::Moment] = "Nmm";
    this->m_Units[Point::Power] = "W";
    // this->m_Units[Point::Scalar] = "";
    // this->m_Units[Point::Reaction] = "";
  };
  
  /**
   * Set the number of frames with @a frameNumber for each point.
   *
   * @warning This method doesn't activate the Modified() method.
   */
  void Acquisition::SetPointFrameNumber(int frameNumber)
  {
    this->m_PointFrameNumber = frameNumber;
    PointIterator it = this->BeginPoint();
    while (it != this->EndPoint())
    {
      (*it)->SetFrameNumber(frameNumber);
      ++it;
    }
  };
  
  /**
   * Set the number of frames for each analog channels as the multiplication of 
   * the number of frames of the points with @a analogSampleNumberPerPointFrame.
   * The input value @a analogSampleNumberPerPointFrame corresponds to the oversampling
   * of the analog channels related to the sampling rate of the points.
   *
   * @warning This method doesn't activate the Modified() method.
   */  
  void Acquisition::SetAnalogFrameNumber(int analogSampleNumberPerPointFrame)
  {
    this->m_AnalogSampleNumberPerPointFrame = analogSampleNumberPerPointFrame;
    AnalogIterator it = this->BeginAnalog();
    while (it != this->EndAnalog())
    {
      (*it)->SetFrameNumber(this->m_PointFrameNumber * this->m_AnalogSampleNumberPerPointFrame);
      ++it;
    }
  };
}
