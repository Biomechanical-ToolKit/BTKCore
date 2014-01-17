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

#include "btkAcquisition.h"
#include "btkException.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class Acquisition btkAcquisition.h
   * @brief Contains the data related to a biomechanical acquisition.
   *
   * A biomechanical acquisition can contains:
   *  - 3D data (makers, points, forces, moments, ...) ;
   *  - analog channels (force platform, emg, ...) ;
   *  - events
   *  - acquisition's setup ;
   *  - subject's informations.
   *
   * By default, the first frame is set to 1, the analog resolution is set to 12-bit, the maximum interpolation gap to 10 frames and the units are:
   *  - Marker: millimeters ;
   *  - Angle: degrees ;
   *  - Force: newtons ;
   *  - Moment: millimeter-newtons ;
   *  - Power: watts ;
   *  - Scalar: millimeters ;
   *  - Reaction: (nothing).
   *
   * After the creation of the acquisition, you should use the Init() method to populate the acquisition.
   *
   * The member used for the maximum interpolation gap is only for information and is not used in the acquisition. It could be used later in a filter to fill gap.
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
   * @var Acquisition::AnalogResolution Acquisition::Bit10
   * 10 bits ADC.
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
   * @typedef Acquisition::NullPointer
   * Special null pointer associated with a Acquisition object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer Acquisition::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
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
    if (this->mp_MetaData != MetaData::Null)
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
    if (this->m_Events->SetItem(idx, event))
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
   * Sets events for this acquisition.
   */
  void Acquisition::SetEvents(EventCollection::Pointer evts)
  {
    if (this->m_Events == evts)
      return;
    this->m_Events = evts;
    this->Modified();
  };

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
   * @fn void Acquisition::ClearEvents()
   * Clear events.
   */

  /**
   * Finds the event with the proposed @a label and returns the iterator associated
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
   * Finds the event with the proposed @a label and returns the const iterator associated
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
   * Append the event @a e in the acquisition.
   */
  void Acquisition::AppendEvent(Event::Pointer e)
  {
    if (this->m_Events->InsertItem(e))
      this->Modified();
  };
   
  /**
   * Remove the event associated with the index @a idx.
   */
  void Acquisition::RemoveEvent(int idx)
  {
    int before = this->GetEventNumber();
    this->m_Events->RemoveItem(idx);
    int after = this->GetEventNumber();
    if (before != after)
      this->Modified();
  };
  
  /**
   * Remove the event associated with the iterator @a loc.
   */
  Acquisition::EventIterator Acquisition::RemoveEvent(Acquisition::EventIterator loc)
  {
    if (loc == this->EndEvent())
    {
      btkErrorMacro("Out of range.");
      return loc;
    }
    Event::Pointer evt = *loc;
    EventIterator it = this->m_Events->RemoveItem(loc);
    if ((it == this->EndEvent()) || (*it != evt)) // Must use this inequality instead of (it != loc) due to an assertion error in MSVC
      this->Modified();
    return it;
  }

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
   *
   * If no Point exists, then a btk::OutOfRangeException exception is thrown.
   */
  Point::Pointer Acquisition::GetPoint(int idx)
  {
    if (idx >= this->GetPointNumber())
      throw(OutOfRangeException("Acquisition::GetPoint(int)"));
    PointIterator it = this->BeginPoint();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the point at the index @a idx as a const smart pointer.
   *
   * If no Point exists, then a btk::OutOfRangeException exception is thrown.
   */
  Point::ConstPointer Acquisition::GetPoint(int idx) const
  {
    if (idx >= this->GetPointNumber())
      throw(OutOfRangeException("Acquisition::GetPoint(int) const"));
    PointConstIterator it = this->BeginPoint();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the point with the label @a label as a smart pointer.
   *
   * If no Point exists, then a btk::OutOfRangeException exception is thrown.
   */
  Point::Pointer Acquisition::GetPoint(const std::string& label)
  {
    PointIterator it = this->FindPoint(label);
    if (it == this->EndPoint())
      throw(OutOfRangeException("Acquisition::GetPoint(string)"));
    return *it;
  };
  
  /**
   * Gets the point with the label @a label as a const smart pointer.
   *
   * If no Point exists, then a btk::OutOfRangeException exception is thrown.
   */
  Point::ConstPointer Acquisition::GetPoint(const std::string& label) const
  {
    PointConstIterator it = this->FindPoint(label);
    if (it == this->EndPoint())
      throw(OutOfRangeException("Acquisition::GetPoint(string) const"));
    return *it;
  };
  
  /**
   * Sets the content of @a point at the index @a idx.
   */
  void Acquisition::SetPoint(int idx, Point::Pointer point)
  {
    if (this->m_Points->SetItem(idx, point))
    {
      if (point->GetFrameNumber() != this->m_PointFrameNumber)
      {
        btkWarningMacro("Point's frame number is different than acquisition's frame number. Point's frame number is resized.");
        point->SetFrameNumber(this->m_PointFrameNumber);
      }
      this->Modified();
    }
    
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
   * Sets points for this acquisition.
   */
  void Acquisition::SetPoints(PointCollection::Pointer points)
  {
    if (this->m_Points == points)
      return;
    this->m_Points = points;
    this->Modified();
  };

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
   * @fn void Acquisition::ClearPoints()
   * Clear points.
   */

  /**
   * Finds the point with the proposed @a label and returns the iterator associated
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
   * Finds the point with the proposed @a label and returns the const iterator associated
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
   * Convenient method to append a point in the acquisition.
   * This method also resizes the frame number of the inserted point if necessary.
   */
  void Acquisition::AppendPoint(Point::Pointer p)
  {
    if (this->m_Points->InsertItem(p))
    {
      if (p->GetFrameNumber() != this->m_PointFrameNumber)
      {
        btkWarningMacro("Point's frame number is different than acquisition's frame number. Point's frame number is resized.");
        p->SetFrameNumber(this->m_PointFrameNumber);
      }
      this->Modified();
    }
  };

  /**
   * Convenient method to delete a point from the acquisition.
   */
  void Acquisition::RemovePoint(const std::string& label)
  {
    PointIterator it = this->FindPoint(label);
    if (it == this->EndPoint())
      btkWarningMacro("No point with label: '" + label + "'.");
    this->RemovePoint(it);
  };

  /**
   * Convenient method to delete a point from the acquisition.
   */
  void Acquisition::RemovePoint(int idx)
  {
    int before = this->GetPointNumber();
    this->m_Points->RemoveItem(idx);
    int after = this->GetPointNumber();
    if (before != after)
      this->Modified();
  };

  /**
   * Convenient method to delete a point from the acquisition.
   */
  Acquisition::PointIterator Acquisition::RemovePoint(Acquisition::PointIterator loc)
  {
    if (loc == this->EndPoint())
    {
      btkWarningMacro("Out of range.");
      return loc;
    } 
    Point::Pointer pnt = *loc;
    PointIterator it = this->m_Points->RemoveItem(loc);
    if ((it == this->EndPoint()) || (*it != pnt)) // Must use this inequality instead of (it != loc) due to an assertion error in MSVC
      this->Modified();
    return it;
  };
  
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
   *
   * If no Analog exists, then an exception is thrown.
   */
  Analog::Pointer Acquisition::GetAnalog(int idx)
  {
    if (idx >= this->GetAnalogNumber())
      throw(OutOfRangeException("Acquisition::GetAnalog(int)"));
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
      throw(OutOfRangeException("Acquisition::GetAnalog(int) const"));
    AnalogConstIterator it = this->BeginAnalog();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the analog channel with the label @a label as a smart pointer.
   */
  Analog::Pointer Acquisition::GetAnalog(const std::string& label)
  {
    AnalogIterator it = this->FindAnalog(label);
    if (it == this->EndAnalog())
      throw(OutOfRangeException("Acquisition::GetAnalog(string)"));
    return *it;
  };
  
  /**
   * Gets the analog channel with the label @a label as a const smart pointer.
   */
  Analog::ConstPointer Acquisition::GetAnalog(const std::string& label) const
  {
    AnalogConstIterator it = this->FindAnalog(label);
    if (it == this->EndAnalog())
      throw(OutOfRangeException("Acquisition::GetAnalog(string) const"));
    return *it;
  };
  
  /**
   * Sets the analog channel at the index @a idx by the content of @a analog.
   */
  void Acquisition::SetAnalog(int idx, Analog::Pointer analog)
  {
    if (this->m_Analogs->SetItem(idx, analog))
    {
      if (analog->GetFrameNumber() != this->m_PointFrameNumber * m_AnalogSampleNumberPerPointFrame)
      {
        btkWarningMacro("Analog's frame number is different than acquisition's frame number. Analog's frame number is resized.");
        analog->SetFrameNumber(this->m_PointFrameNumber * m_AnalogSampleNumberPerPointFrame);
      }
      this->Modified();
    }
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
   * Sets analog channels for this acquisition.
   */
  void Acquisition::SetAnalogs(AnalogCollection::Pointer analogs)
  {
    if (this->m_Analogs == analogs)
      return;
    this->m_Analogs = analogs;
    this->Modified();
  };
  
  /**
   * @fn void Acquisition::ClearAnalogs()
   * Clear analogs channels.
   */

  /**
   * Finds the analog channel with the proposed @a label and returns the iterator 
   * associated with it. If no analog channel has @a label as label, an iterator pointing to the 
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
   * Append the analog channel @a ac in the acquisition
   * This method also resizes the frame number of the inserted analog channel if necessary.
   */
  void Acquisition::AppendAnalog(Analog::Pointer ac)
  {
    if (this->m_Analogs->InsertItem(ac))
    {
      if (ac->GetFrameNumber() != this->m_PointFrameNumber * m_AnalogSampleNumberPerPointFrame)
      {
        btkWarningMacro("Analog's frame number is different than acquisition's frame number. Analog's frame number is resized.");
        ac->SetFrameNumber(this->m_PointFrameNumber * m_AnalogSampleNumberPerPointFrame);
      }
      this->Modified();
    }
  };
  
  /**
   * Convenient method to delete an analog channel from the acquisition.
   */
  void Acquisition::RemoveAnalog(const std::string& label)
  {
    AnalogIterator it = this->FindAnalog(label);
    if (it == this->EndAnalog())
      btkWarningMacro("No analog channel with label: '" + label + "'.");
    this->RemoveAnalog(it);
  };
  
  /**
   * Convenient method to delete an analog channel from the acquisition.
   */
  void Acquisition::RemoveAnalog(int idx)
  {
    int before = this->GetAnalogNumber();
    this->m_Analogs->RemoveItem(idx);
    int after = this->GetAnalogNumber();
    if (before != after)
      this->Modified();
  };
  
  /**
   * Convenient method to delete an analog channel from the acquisition.
   */
  Acquisition::AnalogIterator Acquisition::RemoveAnalog(AnalogIterator loc)
  {
    if (loc == this->EndAnalog())
    {
      btkWarningMacro("Out of range.");
      return loc;
    } 
    Analog::Pointer anl = *loc;
    AnalogIterator it = this->m_Analogs->RemoveItem(loc);
    if ((it == this->EndAnalog()) || (*it !=anl))
      this->Modified();
    return it;
  };
  
  /**
   * Initialize the acquisition with @a pointNumber which have @a frameNumber
   * frame. The analog part has @a analogNumber analog channels and their number of frames
   * corresponds to the integer factor @a analogSampleNumberPerPointFrame multiplied by @a frameNumber.
   *
   * This method label added points and analog channels using the string "uname*" concatenated with the index of the channel.
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
    this->Modified();
  };

  /**
   * Resize the acquisition with @a pointNumber which have @a frameNumber
   * frame. The analog part has @a analogNumber analog channels and their number of frames
   * corresponds to the integer factor @a analogSampleNumberPerPointFrame multiplied by @a frameNumber.
   *
   * This method has the same behavior than the method Init(), but does not label added points and analog channels
   */
  void Acquisition::Resize(int pointNumber, int frameNumber,
                           int analogNumber, int analogSampleNumberPerPointFrame)
  {
    if (analogSampleNumberPerPointFrame <= 0)
    {
      btkWarningMacro("Impossible to set the analog sample number to 0. The numbers of analog samples per point frame is now equals to 1.");
      analogSampleNumberPerPointFrame = 1;
    }
    this->m_AnalogSampleNumberPerPointFrame = analogSampleNumberPerPointFrame;
    this->ResizeFrameNumber(frameNumber);
    this->ResizePointNumber(pointNumber);
    this->ResizeAnalogNumber(analogNumber);
    this->Modified();
  };
  
  /**
   * Resize the number of points.
   * Using this method will set the object as modified even if the given number of points is the same than in the acquisition.
   * This method forces the analog channels to have this object (the acquisition) as their parent.
   */
  void Acquisition::ResizePointNumber(int pointNumber)
  {
    // Reduce the number of items if necessary
    if (pointNumber < this->GetPointNumber())
      this->m_Points->SetItemNumber(pointNumber);
    // Force the existing items to have this object as their parent
    for (PointIterator it = this->BeginPoint() ; it != this->EndPoint() ; ++it)
      (*it)->SetParent(this);
    // Create new points if necessary
    const int numPoints = this->GetPointNumber();
    for (int inc = numPoints ; inc < pointNumber ; ++inc)
    {
      Point::Pointer pt = Point::New(this->m_PointFrameNumber);
      pt->SetParent(this);
      this->m_Points->InsertItem(pt);
    }
    // Set the object as modified
    this->Modified();
  };
  
  /**
   * Resize the number of analog channels.
   * Using this method will set the object as modified even if the given number of analog channels is the same than in the acquisition.
   * This method forces the analog channels to have this object (the acquisition) as their parent.
   */
  void Acquisition::ResizeAnalogNumber(int analogNumber)
  {
    // Reduce the number of items if necessary
    if (analogNumber < this->GetAnalogNumber())
      this->m_Analogs->SetItemNumber(analogNumber);
    // Force the existing items to have this object as their parent
    for (AnalogIterator it = this->BeginAnalog() ; it != this->EndAnalog() ; ++it)
      (*it)->SetParent(this);
    // Create new analog channels if necessary
    const int numAnalogs = this->GetAnalogNumber();
    for (int inc = numAnalogs ; inc < analogNumber ; ++inc)
    {
      Analog::Pointer pt = Analog::New(this->m_PointFrameNumber * this->m_AnalogSampleNumberPerPointFrame);
      pt->SetParent(this);
      this->m_Analogs->InsertItem(pt);
    }
    // Set the object as modified
    this->Modified();
  };
  
  /**
   * Resize the number of frames.
   */
  void Acquisition::ResizeFrameNumber(int frameNumber)
  {
    if (frameNumber == this->m_PointFrameNumber)
      return;
    this->SetPointFrameNumber(frameNumber);
    this->SetAnalogFrameNumber(this->m_AnalogSampleNumberPerPointFrame);
    this->Modified();
  };
  
  /**
   * Resize the number of frames by adding the new frames at the beginning of the acquisition and 
   * set automatically the new first frame index.
   */
  void Acquisition::ResizeFrameNumberFromEnd(int frameNumber)
  {
    if (frameNumber == this->m_PointFrameNumber)
      return;
      
    if (frameNumber < this->m_PointFrameNumber)
    {
      int startRow = this->m_PointFrameNumber - frameNumber;
      for (PointIterator it = this->BeginPoint() ; it != this->EndPoint() ; ++it)
      {
        Point::Values v = (*it)->GetValues().block(startRow,0,frameNumber,3);
        (*it)->SetValues(v);
        Point::Residuals r = (*it)->GetResiduals().block(startRow,0,frameNumber,1);
        (*it)->SetResiduals(r);
      }
      for (AnalogIterator it = this->BeginAnalog() ; it != this->EndAnalog() ; ++it)
      {
        Analog::Values v = (*it)->GetValues().block(startRow * this->m_AnalogSampleNumberPerPointFrame, 0, frameNumber * this->m_AnalogSampleNumberPerPointFrame, 1);
        (*it)->SetValues(v);
      } 
      this->m_FirstFrame = startRow + 1;
    }
    else
    {
      int startRow = frameNumber - this->m_PointFrameNumber;
      int actualFrameNumber = this->m_PointFrameNumber;
      for (PointIterator it = this->BeginPoint() ; it != this->EndPoint() ; ++it)
      {
        Point::Values v = Point::Values::Zero(frameNumber, 3);
        v.block(startRow,0, actualFrameNumber,3) = (*it)->GetValues();
        (*it)->SetValues(v);

        Point::Residuals r = Point::Residuals::Zero(frameNumber, 1);
        r.block(startRow,0,actualFrameNumber,1) = (*it)->GetResiduals();
        (*it)->SetResiduals(r);
      }
      for (AnalogIterator it = this->BeginAnalog() ; it != this->EndAnalog() ; ++it)
      {
        Analog::Values v = Analog::Values::Zero(frameNumber * this->m_AnalogSampleNumberPerPointFrame, 1);
        v.block(startRow * this->m_AnalogSampleNumberPerPointFrame, 0, actualFrameNumber * this->m_AnalogSampleNumberPerPointFrame, 1) = (*it)->GetValues();
        (*it)->SetValues(v);
      }
      this->m_FirstFrame = this->m_FirstFrame - frameNumber + this->m_PointFrameNumber;
      if (this->m_FirstFrame <= 0)
      {
        btkWarningMacro("The index of the first frame can't be set to 0 or lower. It is forced to 1.");
        this->m_FirstFrame = 1;
      }
    }
    this->m_PointFrameNumber = frameNumber;
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
   *
   * To re-populate this acquisition, you need to re-use the Init() method 
   * to set the point and analog number and their frame number.
   */
  void Acquisition::Reset()
  {
    this->m_Events->SetItemNumber(0);
    this->m_Points->SetItemNumber(0);
    this->m_Analogs->SetItemNumber(0);
    this->m_FirstFrame = 1;
    this->m_PointFrequency = 0.0;
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
    this->m_Units[Point::Scalar] = "mm";
    this->m_Units[Point::Reaction] = "";
    this->m_MaxInterpolationGap = 10;
    this->Modified();
  };

  /**
   * @fn double Acquisition::GetDuration() const
   * Returns the duration of the acquisition.
   * 
   * The duration is computed as the multiplication of the points' frequency with the points frame number.
   */
  
  /**
   * @fn int Acquisition::GetFirstFrame() const
   * Return the first frame index of the acquisition.
   */
  
  /**
   * Sets the first frame index (and optionally adapt events' frame/time, false by default).
   *
   * To adapt the events' frame/time, you have to set the option @a adaptEvents to true.
   * The event's frame is shifted by the difference between the new first frame and the old one.
   * The event's time is recomputed using the new frame and the current point's frequency.
   */
  void Acquisition::SetFirstFrame(int num, bool adaptEvents)
  {
    if (num <= 0)
    {
      btkErrorMacro("Impossible to set the first frame to 0 or lower.");
      return;
    }
    else if (this->m_FirstFrame == num)
      return;
    int shift = num - this->m_FirstFrame;
    this->m_FirstFrame = num;
    this->Modified();
    
    if (adaptEvents && (shift != 0))
    {
      if (fabs(this->m_PointFrequency) < std::numeric_limits<double>::epsilon())
      {
        btkErrorMacro("Events' informations are not adated as the sample frequency is not set.");
        return;
      }
      else
      {
        double dt = 1.0 / this->m_PointFrequency;
        for (EventIterator it = this->BeginEvent() ; it != this->EndEvent() ; ++it)
        {
          (*it)->SetFrame((*it)->GetFrame()+shift);
          (*it)->SetTime(static_cast<double>((*it)->GetFrame()-1)*dt);
        }
      }
    }
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
   * @fn void Acquisition::SetPointUnit(const std::string& unit)
   * Sets the unit for points of type Point::Marker.
   */

  /**
   * @fn const std::string Acquisition::GetPointUnit(Point::Type t) const
   * Returns the unit for points of type @a t.
   */
  
  /**
   * @fn const std::vector<std::string>& Acquisition::GetPointUnits() const
   * Returns all the units used for the points (Marker, Angle, Force, Moment, Power, Scalar).
   */
   
  /**
   * Sets the units for all the kinds of points.
   */
  void Acquisition::SetPointUnits(const std::vector<std::string>& units)
  {
    if (this->m_Units == units)
      return;
    this->m_Units = units;
    this->Modified();
  };
  
  /**
   * Sets the point's unit for the Point's type @a t with the value @a units.
   *
   * The type Point::Reaction cannot have any unit. You cannot set it.
   */
  void Acquisition::SetPointUnit(Point::Type t, const std::string& units)
  {
    if (t == Point::Reaction)
      return;
    std::string u = btkTrimString(btkTrimString(units), static_cast<char>(0x00));
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
      btkErrorMacro("Impossible to set the point's frequency to 0.");
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
   * @fn int Acquisition::GetNumberAnalogSamplePerFrame() const
   * Returns the number of analog sample (acquired by each channel) per point frame.
   */

  /**
   * @fn double Acquisition::GetAnalogFrequency() const
   * Returns the analogs sample rate.
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
   * @fn int Acquisition::GetMaxInterpolationGap() const
   * Gets the maximum gap length that any interpolation method would fill for the 3D point data.
   *
   * @warning Only for information. Could be use later by a filter to fill gap 3D trajectories.
   */
   
  /**
   * Sets the maximum gap length that any interpolation method would fill for the 3D point data.
   *
   * @warning Only for information. Could be use later by a filter to fill gap 3D trajectories.
   */
  void Acquisition::SetMaxInterpolationGap(int gap)
  {
    if (this->m_MaxInterpolationGap == gap)
      return;
    this->m_MaxInterpolationGap = gap;
    this->Modified();
  };
  
  /**
   * @fn Pointer Acquisition::Clone() const
   * Returns a deep copy of this object.
   */
    
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
    this->m_PointFrequency = 0.0;
    this->m_PointFrameNumber = 0;
    this->m_AnalogSampleNumberPerPointFrame = 1;
    this->m_AnalogResolution = Bit12; 
    this->m_Units[Point::Marker] = "mm";
    this->m_Units[Point::Angle] = "deg";
    this->m_Units[Point::Force] = "N";
    this->m_Units[Point::Moment] = "Nmm";
    this->m_Units[Point::Power] = "W";
    this->m_Units[Point::Scalar] = "mm";
    // this->m_Units[Point::Reaction] = "";
    this->m_MaxInterpolationGap = 10;
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
  
  /**
   * Constructor of copy. Timestamp, source and parent are reset.
   */
  Acquisition::Acquisition(const Acquisition& toCopy)
  : DataObject(), m_Units(toCopy.m_Units)
  {
    this->m_Events = toCopy.m_Events->Clone();
    this->m_Points = toCopy.m_Points->Clone();
    this->m_Analogs = toCopy.m_Analogs->Clone();
    this->mp_MetaData = toCopy.mp_MetaData->Clone();
    this->mp_MetaData->SetParent(this);
    this->m_FirstFrame = toCopy.m_FirstFrame;
    this->m_PointFrequency = toCopy.m_PointFrequency;
    this->m_PointFrameNumber = toCopy.m_PointFrameNumber;
    this->m_AnalogSampleNumberPerPointFrame = toCopy.m_AnalogSampleNumberPerPointFrame;
    this->m_AnalogResolution = toCopy.m_AnalogResolution;
    this->m_MaxInterpolationGap = toCopy.m_MaxInterpolationGap;
  };
}
