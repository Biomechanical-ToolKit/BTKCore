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

#include "btkEliteFileIOUtils_p.h"
#include "btkMetaDataUtils.h"

#include <limits>

namespace btk
{
  void ReadEliteHeader_p(Acquisition::Pointer output, IEEELittleEndianBinaryFileStream* bifs, bool extractEvents, bool checkDataFiltered)
  {
    bifs->ReadU16(); // Camera number
    int numFra = bifs->ReadU16();
    int numMkr = bifs->ReadU16();
    // 4 types of events:
    //  - Left toe-off
    //  - Right toe-off
    //  - Left heel-strike
    //  - Right heel-strike
    // 12 events by type
    if (extractEvents)
    {
      bifs->SeekRead(2, BinaryFileStream::Current); 
      for (int i = 0 ; i < 12 ; ++i)
      {
        int f = bifs->ReadU16();
        if ((f != 0xFFFF) && (f != 0x0000))
          output->AppendEvent(Event::New("Foot Off", f, "Left", Event::Unknown, "", "", 2));
      }
      for (int i = 0 ; i < 12 ; ++i)
      {
        int f = bifs->ReadU16();
        if ((f != 0xFFFF) && (f != 0x0000))
          output->AppendEvent(Event::New("Foot Off", f, "Right", Event::Unknown, "", "", 2));
      }
      for (int i = 0 ; i < 12 ; ++i)
      {
        int f = bifs->ReadU16();
        if ((f != 0xFFFF) && (f != 0x0000))
          output->AppendEvent(Event::New("Foot Strike", f, "Left", Event::Unknown, "", "", 1));
      }
      for (int i = 0 ; i < 12 ; ++i)
      {
        int f = bifs->ReadU16();
        if ((f != 0xFFFF) && (f != 0x0000))
          output->AppendEvent(Event::New("Foot Strike", f, "Right", Event::Unknown, "", "", 1));
      }
      bifs->SeekRead(136, BinaryFileStream::Current); 
    }
    else
      bifs->SeekRead(234, BinaryFileStream::Current); 
    std::vector<int16_t> date = std::vector<int16_t>(3);
    // The date is converted from DD/MM/YYYY to YYYY/MM/DD (Western calendar system as proposed by the C3D format).
    date[2] = bifs->ReadU16(); // day
    date[1] = bifs->ReadU16(); // month
    date[0] = bifs->ReadU16(); // year
    // Filter flag
    bifs->SeekRead(2, BinaryFileStream::Current);
    int16_t filtered = bifs->ReadI16();
    // First frame
    bifs->SeekRead(24, BinaryFileStream::Current);
    int ff = bifs->ReadU16();
    // Frame rate
    bifs->SeekRead(154, BinaryFileStream::Current);
    double framerate = static_cast<double>(bifs->ReadU16());
    // Go to the end of the header
    bifs->SeekRead(80, BinaryFileStream::Current);
    
    output->Init(numMkr, numFra);
    output->SetFirstFrame(ff);
    output->SetPointFrequency(framerate);
    if ((date[0] != 0) && (date[1] != 0) && (date[2] != 0))
    {
      MetaData::Pointer trial = MetaDataCreateChild(output->GetMetaData(), "TRIAL");
      MetaDataCreateChild(trial, "DATE", date);
    }
    // If the flag is acticated, create the metadata POINT:MARKERS_FILTERED and set its value.
    if (checkDataFiltered)
    {
      MetaData::Pointer point = MetaDataCreateChild(output->GetMetaData(), "POINT");
      MetaDataCreateChild(point, "MARKERS_FILTERED", filtered);
    }
    // Compute the event's time.
    for (Acquisition::EventIterator it = output->BeginEvent() ; it != output->EndEvent() ; ++it)
      (*it)->SetTime(static_cast<double>((*it)->GetFrame()) / framerate);
  };
  
  void ReadEliteMarkersValues_p(Acquisition::Pointer output, IEEELittleEndianBinaryFileStream* bifs)
  {
    int inc = 0;
    Acquisition::PointIterator it = output->BeginPoint();
    while (inc < output->GetPointFrameNumber())
    {
      double x = bifs->ReadFloat();
      double y = bifs->ReadFloat();
      double z = bifs->ReadFloat();
      //double w = bifs->ReadFloat(); // What is w?
      bifs->SeekRead(4, BinaryFileStream::Current);
      
      if ((fabs(x + 9999.0) < std::numeric_limits<double>::epsilon())
          && (fabs(y + 9999.0) < std::numeric_limits<double>::epsilon())
          && (fabs(z + 9999.0) < std::numeric_limits<double>::epsilon()))
      {
        // Invalid coordinates
        (*it)->GetResiduals().coeffRef(inc) = -1.0;
      }
      else
      {
        (*it)->GetValues().coeffRef(inc, 0) = x;
        (*it)->GetValues().coeffRef(inc, 1) = y;
        (*it)->GetValues().coeffRef(inc, 2) = z;
      }
      
      ++it;
      if (it == output->EndPoint())
      {
        it = output->BeginPoint();
        ++inc;
      }
    }
    
    bifs->SeekRead(512 - (output->GetPointFrameNumber() * output->GetPointNumber() * 16) % 512, BinaryFileStream::Current);
  };
  
  void ReadEliteLabel_p(Acquisition::Pointer output, IEEELittleEndianBinaryFileStream* bifs)
  {
    for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
    {
      std::string label = btkTrimString(btkTrimString(bifs->ReadString(8)), static_cast<char>(0x00));
      if (!label.empty())
        (*it)->SetLabel(label);
    }
  };
};