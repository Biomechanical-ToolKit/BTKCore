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

#include "btkSubAcquisitionFilter.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * Constructor.
   * By default, the points, the analog channels and the events are extracted all along the acquisition.
   * The metadata are also added in the result.
   */
  SubAcquisitionFilter::SubAcquisitionFilter()
  : ProcessObject(), m_ExtractionOption(), m_Ids()
  {
    this->mp_FramesIndex[0] = -1;
    this->mp_FramesIndex[1] = -1;
    
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
  };

  /**
   * Creates an Acquisition:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer SubAcquisitionFilter::MakeOutput(int /* idx */)
  {
    return Acquisition::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void SubAcquisitionFilter::GenerateData()
  {
    Acquisition::Pointer input = this->GetInput();
    if (!input)
    {
      btkErrorMacro("Missing input. Impossible to extract any part of the acquisition.");
      return;
    }
    
    int lb = this->mp_FramesIndex[0];
    int ub = this->mp_FramesIndex[1];
    if ((lb == -1) && (ub == -1))
    {
      lb = 0;
      ub = input->GetPointFrameNumber()-1;
    }
    
    if (lb > ub)
    {
      btkErrorMacro("The boundaries are inverted and has been reverted.");
      int temp = ub;
      ub = lb;
      lb = temp;
    }
    if ((ub-lb+1) > input->GetPointFrameNumber())
    {
      btkErrorMacro("The number of frames to extract is greater than the total number of frames. The upper boundary is adapted.");
      ub = input->GetPointFrameNumber()-1;
    }
    if (lb < 0)
    {
      btkErrorMacro("The lower boundary is negative and reset to 0.");
      lb = 0;
    }
    int bounds[2] = {lb, ub};
    
    Acquisition::Pointer output = this->GetOutput();
    output->Reset();
    if (this->m_ExtractionOption == All)
    {
      this->SubPoints(output, input, bounds);
      this->SubAnalogs(output, input, bounds);
      this->SubEvents(output, input, bounds);
    }
    else if (this->m_ExtractionOption == PointsOnly)
    {
      this->SubPoints(output, input, bounds);
    }
    else if (this->m_ExtractionOption == AnalogsOnly)
    {
      this->SubAnalogs(output, input, bounds);
    }
    else if (this->m_ExtractionOption == EventsOnly)
    {
      this->SubEvents(output, input, bounds);
    }
    output->SetFirstFrame(input->GetFirstFrame());
    output->SetPointFrequency(input->GetPointFrequency());
    output->SetAnalogResolution(input->GetAnalogResolution());
    output->SetPointUnits(input->GetPointUnits());
    output->SetMetaData(input->GetMetaData());
    output->Resize(output->GetPointNumber(), ub-lb+1, output->GetAnalogNumber(), input->GetNumberAnalogSamplePerFrame());
  };
  
  void SubAcquisitionFilter::SetFramesIndex(int lb, int ub)
  {
    if ((lb == this->mp_FramesIndex[0]) && (ub == this->mp_FramesIndex[1]))
      return;
    this->mp_FramesIndex[0] = lb;
    this->mp_FramesIndex[1] = ub;
    this->Modified();
  };
  
  void SubAcquisitionFilter::SetExtractionOption(ExtractionOption option)
  {
    if (option == this->m_ExtractionOption)
      return;
    this->m_ExtractionOption = option;
    this->m_Ids.clear();
    this->Modified();
  };
  
  /**
   * Only accept option PointsOnly, AnalogsOnly
   * If the option All is given to this method, then a warning is displayed and the command is not taken into account.
   */
  void SubAcquisitionFilter::SetExtractionOption(ExtractionOption option, const std::list<int>& ids)
  {
    if ((option != PointsOnly) && (option != AnalogsOnly))
    {
      btkErrorMacro("Unsupported extraction option.");
      return;
    }
    if ((option == this->m_ExtractionOption) && (ids == this->m_Ids))
      return;
    this->m_ExtractionOption = option;
    this->m_Ids = ids;
    this->Modified();
  };
  
  void SubAcquisitionFilter::SubPoints(Acquisition::Pointer out, Acquisition::Pointer in, int bounds[2])
  {
    PointCollection::Pointer temp;
    if ((this->m_ExtractionOption == PointsOnly) && !m_Ids.empty())
    {
      temp = PointCollection::New();
      int numPoints = in->GetPointNumber();
      for (std::list<int>::const_iterator it = this->m_Ids.begin() ; it != this->m_Ids.end() ; ++it)
      {
        if (*it < numPoints)
        {
          PointCollection::Iterator itP = in->BeginPoint();
          std::advance(itP,*it);
          temp->InsertItem(*itP);
        }
        else
        {
          btkErrorMacro("There is no point with the ID #" + ToString(*it));
        }
      }
    }
    else
      temp = in->GetPoints();
    
    if ((this->mp_FramesIndex[0] == -1) && (this->mp_FramesIndex[1] == -1))
      out->SetPoints(temp);
    else
    {
      PointCollection::Pointer points = PointCollection::New();
      int numFrames = bounds[1] - bounds[0] + 1;
      for (PointCollection::Iterator it = temp->Begin() ; it != temp->End() ; ++it)
      {
        Point::Pointer point = Point::New();
        point->SetLabel((*it)->GetLabel());
        point->SetDescription((*it)->GetDescription());
        point->SetType((*it)->GetType());
        point->SetFrameNumber(numFrames);
        point->SetValues((*it)->GetValues().block(bounds[0],0,numFrames,3));
        point->SetResiduals((*it)->GetResiduals().block(bounds[0],0,numFrames,1));
        points->InsertItem(point);
      }
      out->SetPoints(points);
    }
  }
  
  void SubAcquisitionFilter::SubAnalogs(Acquisition::Pointer out, Acquisition::Pointer in, int bounds[2])
  {
    AnalogCollection::Pointer temp;
    if ((this->m_ExtractionOption == AnalogsOnly) && !m_Ids.empty())
    {
      temp = AnalogCollection::New();
      int numAnalogs = in->GetAnalogNumber();
      for (std::list<int>::const_iterator it = this->m_Ids.begin() ; it != this->m_Ids.end() ; ++it)
      {
        if (*it < numAnalogs)
        {
          AnalogCollection::Iterator itA = in->BeginAnalog();
          std::advance(itA,*it);
          temp->InsertItem(*itA);
        }
        else
        {
          btkErrorMacro("There is no analog channel with the ID #" + ToString(*it));
        }
      }
    }
    else
      temp = in->GetAnalogs();
    
    if ((this->mp_FramesIndex[0] == -1) && (this->mp_FramesIndex[1] == -1))
      out->SetAnalogs(temp);
    else
    {
      AnalogCollection::Pointer analogs = AnalogCollection::New();
      int numFrames = (bounds[1] - bounds[0] + 1) * in->GetNumberAnalogSamplePerFrame();
      for (AnalogCollection::Iterator it = temp->Begin() ; it != temp->End() ; ++it)
      {
        Analog::Pointer analog = Analog::New();
        analog->SetLabel((*it)->GetLabel());
        analog->SetDescription((*it)->GetDescription());
        analog->SetUnit((*it)->GetUnit());
        analog->SetGain((*it)->GetGain());
        analog->SetOffset((*it)->GetOffset());
        analog->SetScale((*it)->GetScale());
        analog->SetFrameNumber(numFrames);
        analog->SetValues((*it)->GetValues().block(bounds[0]*in->GetNumberAnalogSamplePerFrame(),0,numFrames,1));
        analogs->InsertItem(analog);
      }
      out->SetAnalogs(analogs);
    }
  }
  
  void SubAcquisitionFilter::SubEvents(Acquisition::Pointer out, Acquisition::Pointer in, int bounds[2])
  {
    if ((this->mp_FramesIndex[0] == -1) && (this->mp_FramesIndex[1] == -1))
    {
      out->SetEvents(in->GetEvents());
    }
    else
    {
      EventCollection::Pointer events = EventCollection::New();
      int lf = bounds[0] + in->GetFirstFrame();
      int uf = bounds[1] + in->GetFirstFrame();
      for (EventCollection::Iterator it = in->BeginEvent() ; it != in->EndEvent() ; ++it)
      {
        if (((*it)->GetFrame() >= lf) && ((*it)->GetFrame() <= uf))
          events->InsertItem(*it);
      }
      out->SetEvents(events);
    }
  }
};
