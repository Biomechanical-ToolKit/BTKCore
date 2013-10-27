/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#include "btkMergeAcquisitionFilter.h"
#include "btkAcquisitionUnitConverter.h"
#include "btkMetaDataUtils.h"

#include "btkConvert.h"

namespace btk
{
  /**
   * @class MergeAcquisitionFilter btkMergeAcquisitionFilter.h
   * @brief Merges or concatenates two or more btk::Acquisition objects into a single new one.
   *
   * The difference between the merging or the concatenating depends of the acquisitions' informations.
   *
   * If the acquisitions are exactly the same (same label for points and analog channels) and the first 
   * frame of the n+1th acquisition corresponds to the last frame of the nth acquisition, then the 
   * acquisitions are merged, otherwise, the data are concatenated. Merged acquisitions means that the 
   * output will have the same number of points, analog channels, events than each output but the number
   * of frames corresponds to the sum of the input's frame number. 
   *
   * Rules to merge/concatenate acquisitions are:
   *  - Must have the same acquisition frequency (or 0).
   *  - Must have the same number of analog samples per point frame (if there are points and analog channels).
   *  - Must have the same analog resolution.
   *  - Must have the same units (Use btk::AcquisitionUnitConvert to convert them).
   *  - Must have the same global frame orientation (check POINT:X_SCREEN and POINT:Y_SCREEN)
   *  - The first input which has a non-zero acquisition frequency is used.
   *  - The first input which has analog channels is used to set the analog resolution of the output.
   *
   * Actions during the merging/concatenation:
   *  - If the first frame of the nth acquisition correspond to the last frame + 1 of the nth-1 acquisition, 
   *    AND the points / analog channels correspond exactly, then acquisitions are truncated.
   *  - If the first frame of two inputs are not the same, the final acquisition is adapted to contain the number of frames plus the difference between the first frame of each input.
   *  - If points have the same label (and the first frames doesn't correspond), then, the second and more are incremented (i.e. from Heel to Heel_2).
   *  - Idem for the analog channels.
   *  - Events with the same label, context, id and time are merged.
   *  - Metadata (if number of inputs greater than 1):
   *     - Only metadata defined by the user are inserted (i.e. POINT, ANALOG, EVENTS, etc. are not inserted as they are only used in a C3D file.).
   *     - If metadata are equivalent or equal, then they are merged, else the metadata is appended with a suffix (i.e. from STATIC_CALIBRATION:USED to STATIC_CALIBRATION:USED_2)
   *
   * Moreover, you can set a rule to keep only the data from the higher or the lower first frame. Use the method SetFirstFrameRule() with the values KeepAllFrames or KeepFromHighestFirstFrame.
   *
   * @ingroup BTKBasicFilters
   */
  /**
   * @var MergeAcquisitionFilter::KeepAllFrames
   * Option to keep all the frames of the given acquisitions during the merging process
   */
  /**
   * @var MergeAcquisitionFilter::KeepFromHighestFirstFrame
   * Option to keep only the frames starting from the highest first frame extracted from the given acquisitions.
   */
  
  /**
   * @typedef MergeAcquisitionFilter::Pointer
   * Smart pointer associated with a MergeAcquisitionFilter object.
   */
  
  /**
   * @typedef MergeAcquisitionFilter::ConstPointer
   * Smart pointer associated with a const MergeAcquisitionFilter object.
   */
    
  /**
   * @fn static Pointer MergeAcquisitionFilter::New();
   * Creates a smart pointer associated with a MergeAcquisitionFilter object.
   */
   
  /**
   * @fn int MergeAcquisitionFilter::GetFirstFrameRule() const
   * Returns the rule for the first frame kept.
   *
   * Two rules are available:
   *  - KeepAllFrames: Use the lower first frame;
   *  - KeepFromHighestFirstFrame: Use the higher first frame.
   */
  
  /**
   * Sets the rule for the first frame kept.
   *
   * Two rules are available:
   *  - KeepAllFrames: Use the lower first frame;
   *  - KeepFromHighestFirstFrame: Use the higher first frame.
   */ 
  void MergeAcquisitionFilter::SetFirstFrameRule(int rule)
  {
    if (this->m_FirstFrameRule == rule)
      return;
    this->m_FirstFrameRule = rule;
    this->Modified();
  };

  /**
   * @fn Acquisition::Pointer MergeAcquisitionFilter::GetInput(int idx)
   * Gets the input registered with this process at the index @a idx.
   */
  
  /**
   * @fn void MergeAcquisitionFilter::SetInput(int idx, Acquisition::Pointer input)
   * Sets the input required with this process at the index @a idx.
   */
  
  /**
   * @fn Acquisition::Pointer MergeAcquisitionFilter::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * Constructor. Sets the number of inputs to 2 and outputs to 1.
   */
  MergeAcquisitionFilter::MergeAcquisitionFilter()
  : ProcessObject()
  {
    this->SetInputNumber(2);
    this->SetOutputNumber(1);
    this->m_FirstFrameRule = KeepAllFrames;
  };
  
  /**
   * @fn Acquisition::Pointer MergeAcquisitionFilter::GetOutput(int idx)
   * Gets the output created with this process at the index @a idx.
   */
  
  /**
   * Creates a Acquisition::Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer MergeAcquisitionFilter::MakeOutput(int /* idx */)
  {
    return Acquisition::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void MergeAcquisitionFilter::GenerateData()
  {
    Acquisition::Pointer output = this->GetOutput();
    output->Reset();
    bool firstInput = true;
    int finalFirstFrame = 1;
    
    for (int idx = 0 ; idx < this->GetInputNumber() ; ++idx)
    {
      Acquisition::Pointer in = this->GetInput(idx);
      // Check null input
      if (!in)
      {
        btkWarningMacro("Input #" + ToString(idx) + " is not merged: Impossible to merge a null input.");
        continue;
      }
      // Check the point's frequency
      if ((in->GetPointFrequency() != 0) && (output->GetPointFrequency() != 0) && !in->IsEmptyPoint() && !output->IsEmptyPoint() && (in->GetPointFrequency() != output->GetPointFrequency()))
      {
        btkWarningMacro("Input #" + ToString(idx) + " is not merged: Point's frame rates are not equal.");
        return;
      }
      // Check the analog's frequency
      if ((in->GetAnalogFrequency() != 0) && (output->GetAnalogFrequency() != 0) && !in->IsEmptyAnalog() && !output->IsEmptyAnalog() && (in->GetAnalogFrequency() != output->GetAnalogFrequency()))
      {
        btkWarningMacro("Input #" + ToString(idx) + " is not merged: Analog's frame rates are not equal.");
        return;
      }
      // Check the analog resolution
      if ((in->GetAnalogResolution() != 0) && (output->GetAnalogResolution() != 0) && !in->IsEmptyAnalog() && !output->IsEmptyAnalog() && (in->GetAnalogResolution() != output->GetAnalogResolution()))
      {
        btkWarningMacro("Input #" + ToString(idx) + " is not merged: Analog resolutions are not equal.");
        return;
      }
      // Check the number of analog samples per point frame
      if (!in->IsEmptyPoint() && !in->IsEmptyAnalog() && !output->IsEmptyAnalog() && (in->GetAnalogFrameNumber() != 0) && (output->GetAnalogFrameNumber() != 0) && (in->GetNumberAnalogSamplePerFrame() != output->GetNumberAnalogSamplePerFrame()))
      {
        btkWarningMacro("Input #" + ToString(idx) + " is not merged: Number of analog samples per point frame are not equal.");
        return;
      }
      /*
      if ((in->GetAnalogFrameNumber() != 0) && (output->GetAnalogFrameNumber() != 0) && (in->GetAnalogFrameNumber() != output->GetAnalogFrameNumber()))
      {
        btkWarningMacro("Input #" + ToString(idx) + " is not merged: Number of analog samples per point frame are not equal.");
        continue;
      }
      */
      
      if (firstInput)
      {
        firstInput = false;
        output->SetEvents(in->GetEvents()->Clone());
        output->SetPoints(in->GetPoints()->Clone());
        output->SetAnalogs(in->GetAnalogs()->Clone());
        output->SetMetaData(in->GetMetaData()->Clone());
        output->SetFirstFrame(in->GetFirstFrame());
        output->SetPointFrequency(in->GetPointFrequency());
        if ((in->GetPointNumber() != 0) || (in->GetPointFrameNumber() != 0) ||  (in->GetAnalogNumber() != 0))
          output->Resize(in->GetPointNumber(), in->GetPointFrameNumber(), in->GetAnalogNumber(), in->GetNumberAnalogSamplePerFrame());
        output->SetAnalogResolution(in->GetAnalogResolution());
        output->SetPointUnit(Point::Marker, in->GetPointUnit(Point::Marker));
        output->SetPointUnit(Point::Angle, in->GetPointUnit(Point::Angle));
        output->SetPointUnit(Point::Force, in->GetPointUnit(Point::Force));
        output->SetPointUnit(Point::Moment, in->GetPointUnit(Point::Moment));
        output->SetPointUnit(Point::Power, in->GetPointUnit(Point::Power));
        output->SetPointUnit(Point::Scalar, in->GetPointUnit(Point::Scalar));
        finalFirstFrame = in->GetFirstFrame();
      }
      else
      {
        finalFirstFrame = std::max(finalFirstFrame, in->GetFirstFrame());
      
        // Final check
        btk::Acquisition::Pointer input = in->Clone();
        // Check units
        if ((output->GetPointUnit(Point::Scalar).compare(input->GetPointUnit(Point::Scalar)) != 0)
            || (output->GetPointUnit(Point::Angle).compare(input->GetPointUnit(Point::Angle)) != 0)
            || (output->GetPointUnit(Point::Force).compare(input->GetPointUnit(Point::Force)) != 0)
            || (output->GetPointUnit(Point::Moment).compare(input->GetPointUnit(Point::Moment)) != 0)
            || (output->GetPointUnit(Point::Power).compare(input->GetPointUnit(Point::Power)) != 0)
            || (output->GetPointUnit(Point::Scalar).compare(input->GetPointUnit(Point::Scalar)) != 0))
        {
          btkWarningMacro("Input #" + ToString(idx) + " is not merged: Units do not correspond.");
          return;
        }
        // Global orientation
        MetaData::ConstIterator pointItIn, pointItOut;
        pointItIn = in->GetMetaData()->FindChild("POINT");
        pointItOut = output->GetMetaData()->FindChild("POINT");
        if ((pointItIn != in->EndMetaData()) && (pointItOut != output->EndMetaData()))
        {
          MetaDataInfo::Pointer xScreenIn = (*pointItIn)->ExtractChildInfo("X_SCREEN", MetaDataInfo::Char, 1);
          MetaDataInfo::Pointer yScreenIn = (*pointItIn)->ExtractChildInfo("Y_SCREEN", MetaDataInfo::Char, 1);
          MetaDataInfo::Pointer xScreenOut = (*pointItOut)->ExtractChildInfo("X_SCREEN", MetaDataInfo::Char, 1);
          MetaDataInfo::Pointer yScreenOut = (*pointItOut)->ExtractChildInfo("Y_SCREEN", MetaDataInfo::Char, 1);
          if (xScreenIn && yScreenIn && xScreenOut && yScreenOut)
          {
            if ((xScreenIn->ToString(0).compare(xScreenOut->ToString(0)) != 0)
                || (yScreenIn->ToString(0).compare(yScreenOut->ToString(0)) != 0))
            {
              btkWarningMacro("Input #" + ToString(idx) + " is not merged: Global orientation does not correspond.");
              return;
            }
          }
        }
        
        // Transform acquisition (input or output) to priorize the point data.
        if ((output->GetPointNumber() != 0) && (input->GetPointNumber() == 0) && (input->GetAnalogNumber() != 0) && (input->GetAnalogFrameNumber() != 0))
        {
          int r = 0;
          if ((output->GetPointFrequency() == 0) || (input->GetPointFrequency() == 0))
          {
            r = input->GetAnalogFrameNumber() /  output->GetPointFrameNumber();
            btkWarningMacro("At least one frequency is not set in the output or the input #" + ToString(idx) + " and may corrupt the final output.");
          }
          else
            r = static_cast<int>(input->GetPointFrequency() / output->GetPointFrequency());
          if (r > 1)
          {
            input->ResizeFrameNumberFromEnd(input->GetPointFrameNumber() + (input->GetFirstFrame() - 1) % r);
            input->Resize(0, output->GetPointFrameNumber() - input->GetFirstFrame() / r, input->GetAnalogNumber(), r);
            input->SetFirstFrame(input->GetFirstFrame() / r + 1);
            input->SetPointFrequency(input->GetPointFrequency() / r);
            output->Resize(output->GetPointNumber(), output->GetPointFrameNumber(), output->GetAnalogNumber(), r);
          }
        }
        else if ((input->GetPointNumber() != 0) && (output->GetPointNumber() == 0) && (output->GetAnalogNumber() != 0) && (output->GetAnalogFrameNumber() != 0))
        {
          int r = 0;
          if ((output->GetPointFrequency() == 0) || (input->GetPointFrequency() == 0))
          {
            r = output->GetAnalogFrameNumber() /  input->GetPointFrameNumber();
            btkWarningMacro("At least one frequency is not set in the output or the input #" + ToString(idx) + " and may corrupt the final output.");
          }
          else
            r = static_cast<int>(output->GetPointFrequency() / input->GetPointFrequency());
          if (r > 1)
          {
            output->ResizeFrameNumberFromEnd(output->GetPointFrameNumber() + (output->GetFirstFrame() - 1) % r);
            output->Resize(0, input->GetPointFrameNumber() - output->GetFirstFrame() / r, output->GetAnalogNumber(), r);
            output->SetFirstFrame(output->GetFirstFrame() / r + 1 );
            output->SetPointFrequency(output->GetPointFrequency() / r);
            input->Resize(input->GetPointNumber(), input->GetPointFrameNumber(), input->GetAnalogNumber(), r);
          }
        }
        
        // Check if the first frame is set correctly (no metadata BTK_POINT_CONFIG:NO_FIRST_FRAME)
        bool noFirstFrameIn = false, noFirstFrameOut = false;
        MetaData::Iterator btkPointConfig;
        MetaDataInfo::Pointer noFirstFrame;
        // - Input
        btkPointConfig = input->GetMetaData()->FindChild("BTK_POINT_CONFIG");
        if (btkPointConfig != input->EndMetaData())
        {
          noFirstFrame = (*btkPointConfig)->ExtractChildInfo("NO_FIRST_FRAME", MetaDataInfo::Byte, 0);
          if (noFirstFrame)
            noFirstFrameIn = (noFirstFrame->ToInt(0) != 0);
        }
        // - Ouput
        btkPointConfig = output->GetMetaData()->FindChild("BTK_POINT_CONFIG");
        if (btkPointConfig != output->EndMetaData())
        {
          noFirstFrame = (*btkPointConfig)->ExtractChildInfo("NO_FIRST_FRAME", MetaDataInfo::Byte, 0);
          if (noFirstFrame)
            noFirstFrameOut = (noFirstFrame->ToInt(0) != 0);
        }
        // - Check their content
        if (noFirstFrameIn != noFirstFrameOut)
        {
          MetaData::Pointer pntCfg;
          Acquisition::Pointer src;
          if (noFirstFrameIn)
          {
            input->SetFirstFrame(output->GetFirstFrame());
            pntCfg = input->GetMetaData()->GetChild("BTK_POINT_CONFIG");
            src = input;
          }
          else
          {
            output->SetFirstFrame(input->GetFirstFrame());
            pntCfg = output->GetMetaData()->GetChild("BTK_POINT_CONFIG");
            src = output;
          }
          // No more need of the metadata used for that
          pntCfg->RemoveChild("NO_FIRST_FRAME");
          // No more child? => Remove the metadata BTK_POINT_CONFIG
          if (!pntCfg->HasChildren())
            src->GetMetaData()->RemoveChild("BTK_POINT_CONFIG");
        }
        
        // Frequency
        if (output->GetPointFrequency() == 0.0)
          output->SetPointFrequency(input->GetPointFrequency());
        // Analog resolution
        if (output->IsEmptyAnalog())
          output->SetAnalogResolution(input->GetAnalogResolution());
        
        // Merge or concat?
        bool mergeData = false;
        // First frame
        int diffFF = output->GetFirstFrame() - input->GetFirstFrame();
        if ((diffFF < 0) && (input->GetPointFrameNumber() != 0))
        {
          if (abs(diffFF) == output->GetLastFrame())
            mergeData = true;
          input->ResizeFrameNumberFromEnd(input->GetPointFrameNumber() - diffFF);
          if (input->GetPointFrameNumber() > output->GetPointFrameNumber())
            output->ResizeFrameNumber(input->GetPointFrameNumber());
          else
            input->ResizeFrameNumber(output->GetPointFrameNumber());
        }
        else if ((diffFF > 0) && (input->GetPointFrameNumber() != 0))
        {
          if (diffFF == input->GetLastFrame())
            mergeData = true;
          output->ResizeFrameNumberFromEnd(output->GetPointFrameNumber() + diffFF);
          if (output->GetPointFrameNumber() > input->GetPointFrameNumber())
            input->ResizeFrameNumber(output->GetPointFrameNumber());
          else
            output->ResizeFrameNumber(input->GetPointFrameNumber());
        }
        else
        {
          int diffFN = output->GetPointFrameNumber() - input->GetPointFrameNumber();
          if ((diffFN > 0) && (!output->IsEmptyPoint() || !output->IsEmptyAnalog()))
            input->ResizeFrameNumber(output->GetPointFrameNumber());
          else if ((diffFN < 0) && (!input->IsEmptyPoint() || !input->IsEmptyAnalog()))
            output->ResizeFrameNumber(input->GetPointFrameNumber());
        }
        
        // To be sure to merge the data, it is necessary to check if the label of each point and analog channel 
        // are exactly the same.
        if (mergeData)
        {
          for (Acquisition::PointIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
          {
            if (output->FindPoint((*it)->GetLabel()) == output->EndPoint())
            {
              mergeData = false;
              break;
            }
          }
        }
        if (mergeData)
        {
          for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
          {
            if (output->FindAnalog((*it)->GetLabel()) == output->EndAnalog())
            {
              mergeData = false;
              break;
            }
          }
        }
        if (mergeData)
          this->MergeData(output, input, diffFF);
        else
          this->ConcatData(output, input);
        
        // Event
        for (Acquisition::EventIterator itIn = input->BeginEvent() ; itIn != input->EndEvent() ; ++itIn)
        {
          // Compute the event's time if necessary (for example, if the acquisition comes from an XLS file).
          if (((*itIn)->GetTime() + 1.0)  <= std::numeric_limits<double>::epsilon())
            (*itIn)->SetTime(static_cast<double>((*itIn)->GetFrame() - 1) / output->GetPointFrequency());
          Acquisition::EventIterator itOut;
          for (itOut = output->BeginEvent() ; itOut != output->EndEvent() ; ++itOut)
          {
            if ((*itIn)->GetLabel().compare((*itOut)->GetLabel()) == 0)
            {
              if (((*itIn)->GetFrame() == (*itOut)->GetFrame())
                  && ((*itIn)->GetContext().compare((*itOut)->GetContext()) == 0)
                  && ((*itIn)->GetSubject().compare((*itOut)->GetSubject()) == 0))
                break;
              (*itIn)->SetId((*itOut)->GetId());
            }
          }
          if (itOut == output->EndEvent())
            output->AppendEvent(*itIn);
        }
        
        // Metadata
        // Clean obsolete metadata
        if (!mergeData)
        {
          this->RemoveDeprecatedMetaData(output->GetMetaData());
          this->RemoveDeprecatedMetaData(input->GetMetaData(), true);
        }
        this->CleanMetaData(output->GetMetaData());
        this->CleanMetaData(input->GetMetaData());
        
        // Append new metadata coming from the input.
        // Only the first level is checked
        Acquisition::MetaDataIterator it = input->GetMetaData()->Begin();
        while (it != input->GetMetaData()->End())
        {
          if (output->GetMetaData()->FindChild((*it)->GetLabel()) == output->GetMetaData()->End())
          {
            output->GetMetaData()->AppendChild(*it);
            it = input->GetMetaData()->RemoveChild(it);
          }
          else
            ++it;
        }
        this->UpdateKnownMetaData(output, input->GetMetaData(), mergeData);
        // Merge metadata which is exactly the same (including children & info) than one found in the output.
        it = input->GetMetaData()->Begin();
        while (it != input->GetMetaData()->End())
        {
          MetaData::Iterator it2 = output->GetMetaData()->FindChild((*it)->GetLabel());
          if ((it2 != output->GetMetaData()->End()) && (*(*it) == *(*it2)))
            it = input->GetMetaData()->RemoveChild(it);
          else
            ++it;
        }
        // Add metadata which exist in both (input and output) but which are not identical.
        // This addition will correspond to a suffix ("_<inc>") added in the metadata's label of the first level.
        it = input->GetMetaData()->Begin();
        while (it != input->GetMetaData()->End())
        {
          int inc = 2;
          while (1)
          {
            std::string suffix = "_" + ToString(inc);
            (*it)->SetLabel((*it)->GetLabel() + suffix);
            if (output->GetMetaData()->FindChild((*it)->GetLabel()) == output->GetMetaData()->End())
            {
              output->GetMetaData()->AppendChild(*it);
              it = input->GetMetaData()->RemoveChild(it);
              break;
            }
            ++inc;
          }
        }
        
        // Special case: BTK metadata
        this->UpdatePartialForcePlateMetaData(output->GetMetaData());
      }
    }
    
    if ((this->m_FirstFrameRule == KeepFromHighestFirstFrame) && (finalFirstFrame != output->GetFirstFrame()))
    {
      output->ResizeFrameNumberFromEnd(output->GetPointFrameNumber() - finalFirstFrame - output->GetFirstFrame() + 1);
      output->SetFirstFrame(finalFirstFrame);
    }
  };
  
  void MergeAcquisitionFilter::MergeData(Acquisition::Pointer output, Acquisition::Pointer input, int frameNumber) const
  {
    int startFrame = 0;
    if (frameNumber < 0)
      startFrame = abs(frameNumber);
    else
      startFrame = output->GetFirstFrame() - 1;
      
    for (Acquisition::PointIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
    {
      Point::Pointer p = *(output->FindPoint((*it)->GetLabel()));
      p->GetValues().block(startFrame, 0, abs(frameNumber), 3) = (*it)->GetValues().block(startFrame, 0, abs(frameNumber), 3);
      p->GetResiduals().block(startFrame, 0, abs(frameNumber), 1) = (*it)->GetResiduals().block(startFrame, 0, abs(frameNumber), 1);
    }
    // Analog
    for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
    {
      Analog::Pointer ac = *(output->FindAnalog((*it)->GetLabel()));
      ac->GetValues().block(startFrame * input->GetNumberAnalogSamplePerFrame(), 0, abs(frameNumber), 1) = (*it)->GetValues().block(startFrame, 0, abs(frameNumber), 1);
    }
  };
  
  void MergeAcquisitionFilter::ConcatData(Acquisition::Pointer output, Acquisition::Pointer input) const
  {
    // Point
    for (Acquisition::PointIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
    {
      std::string suffix = "";
      int inc = 2;
      while (1)
      {
        (*it)->SetLabel((*it)->GetLabel() + suffix);
        if (output->FindPoint((*it)->GetLabel()) == output->EndPoint())
        {
          output->AppendPoint(*it);
          break;
        }
        suffix = "_" + ToString(inc);
        ++inc;
      }
    }
    // Analog
    for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
    {
      std::string suffix = "";
      int inc = 2;
      while (1)
      {
        (*it)->SetLabel((*it)->GetLabel() + suffix);
        if (output->FindAnalog((*it)->GetLabel()) == output->EndAnalog())
        {
          output->AppendAnalog(*it);
          break;
        }
        suffix = "_" + ToString(inc);
        ++inc;
      }
    }
  };
  
  void MergeAcquisitionFilter::RemoveDeprecatedMetaData(MetaData::Pointer in, bool pointScreenToRemove) const
  {
    // List of metadata which have to be removed.
    int num = 0;
    MetaData::Iterator it;
    // POINT
    it = in->FindChild("POINT");
    if (it != in->End())
    {
      // List for the metadata POINT
      const char* point[] = {"USED", "SCALE", "RATE", "DATA_START", "FRAMES", "UNITS", "INITIAL_COMMAND",
                             "MOVIE_DELAY", "TYPE_GROUPS", "ANGLE_UNITS", "SCALAR_UNITS", "POWER_UNITS", "FORCE_UNITS", 
                             "MOMENT_UNITS", "LABELS", "DESCRIPTIONS", "ANGLES", "SCALARS", "POWERS", 
                             "FORCES", "MOMENTS", "REACTIONS"};
      // The following list remove the metadata which can be repeatable (for example, LABELS, LABELS2, etc.)
      const char* pointRepeatable[] = {"LABELS", "DESCRIPTIONS" "ANGLES", "SCALARS", "POWERS", "FORCES", 
                                       "MOMENTS", "REACTIONS"};
                                       
      num = sizeof(point) / sizeof(char*);
      for (int i = 0 ; i < num ; ++i)
        (*it)->RemoveChild(point[i]);
      if (pointScreenToRemove)
      {
        (*it)->RemoveChild("X_SCREEN");
        (*it)->RemoveChild("Y_SCREEN");
      }
      num = sizeof(pointRepeatable) / sizeof(char*);
      for (int i = 0 ; i < num ; ++i)
      {
        int inc = 2;
        while (1)
        {
          MetaData::Iterator toRemove = (*it)->FindChild(pointRepeatable[i] + ToString(inc));
          if (toRemove == (*it)->End())
            break;
          (*it)->RemoveChild(toRemove);
          ++inc;
        }
      }
    }
      
    // ANALOG
    it = in->FindChild("ANALOG");
    if (it != in->End())
    {
      // List for the metadata ANALOG
      const char* analog[] = {"USED", "GEN_SCALE", "RATE", "FORMAT", "BITS", "LABELS", "DESCRIPTIONS", 
                              "SCALE", "OFFSET", "GAIN", "UNITS"};
      // Test metadata are repeatable: LABELS, DESCRIPTIONS, SCALE, OFFSET, GAIN, UNITS
      const char* analogRepeatable[] = {"LABELS", "DESCRIPTIONS", "SCALE", "OFFSET", "GAIN", "UNITS"};
      
      num = sizeof(analog) / sizeof(char*);
      for (int i = 0 ; i < num ; ++i)
        (*it)->RemoveChild(analog[i]);
      num = sizeof(analogRepeatable) / sizeof(char*);
      for (int i = 0 ; i < num ; ++i)
      {
        int inc = 2;
        while (1)
        {
          MetaData::Iterator toRemove = (*it)->FindChild(analogRepeatable[i] + ToString(inc));
          if (toRemove == (*it)->End())
            break;
          (*it)->RemoveChild(toRemove);
          ++inc;
        }
      }
    }
    
    // EVENT
    in->RemoveChild("EVENT");
  };
  
  /**
   * Remove empty metadata (no child, no info)
   */
  void MergeAcquisitionFilter::CleanMetaData(MetaData::Pointer in) const
  {
    Acquisition::MetaDataIterator it = in->Begin();
    while (it != in->End())
    {
      this->CleanMetaData(*it);
      if (!(*it)->HasChildren() && !(*it)->HasInfo())
        it = in->RemoveChild(it);
      else
        ++it;
    }
  };

  void MergeAcquisitionFilter::UpdateKnownMetaData(Acquisition::Pointer out, MetaData::Pointer in, bool mergeData) const
  {
    // TRIAL
    MetaData::Iterator it = in->FindChild("TRIAL");
    if (it != in->End())
    {
      if (this->UpdateTrialMetaData(out, *it, mergeData))
        in->RemoveChild(it);
    }
    
    if (!mergeData)
    {
      // FORCE_PLATFORM
      it = in->FindChild("FORCE_PLATFORM");
      if (it != in->End())
      {
        if (this->UpdateForcePlatformMetaData(out->GetMetaData(), *it))
          in->RemoveChild(it);
      }
      // EVENT_CONTEXT
      it = in->FindChild("EVENT_CONTEXT");
      if (it != in->End())
      {
        if (this->UpdateEventContextMetaData(out->GetMetaData(), *it))
          in->RemoveChild(it);
      }
      // MANUFACTURER
      it = in->FindChild("MANUFACTURER");
      if (it != in->End())
      {
        MetaData::Iterator itOut = out->GetMetaData()->FindChild("MANUFACTURER");
        if (*(*it) != *(*itOut))
          out->GetMetaData()->RemoveChild(itOut);
        in->RemoveChild(it);
      }
    }
  };

  void MergeAcquisitionFilter::UpdatePartialForcePlateMetaData(MetaData::Pointer out) const
  {
    // Check for all metadata starting by BTK_PARTIAL_FP_CONFIG* and BTK_PARTIAL_FP_CHAN*
    std::list<MetaData::Pointer> fpConfigList;
    std::string fpConfigString = "BTK_PARTIAL_FP_CONFIG";
    std::list<MetaData::Pointer> fpChanList;
    std::string fpChanString = "BTK_PARTIAL_FP_CHAN";
    
    for (Acquisition::MetaDataIterator it = out->Begin() ; it != out->End() ; ++it)
    {
      if ((*it)->GetLabel().substr(0, fpChanString.length()).compare(fpChanString) == 0)
        fpChanList.push_back(*it);
      else if ((*it)->GetLabel().substr(0, fpConfigString.length()).compare(fpConfigString) == 0)
        fpConfigList.push_back(*it);
    }
    
    if (!fpChanList.empty() && !fpConfigString.empty())
    {
      for (std::list<MetaData::Pointer>::iterator itConfig = fpConfigList.begin() ; itConfig != fpConfigList.end() ; ++itConfig)
      {
        int chanNumber;
        int usedNumber = this->GetMaxInfoForcePlatform(*itConfig, &chanNumber);
        if (usedNumber == -1)
          continue;
        std::list<MetaData::Pointer>::iterator itChan = fpChanList.begin();
        while (itChan != fpChanList.end())
        {
          MetaDataInfo::Pointer channel = (*itChan)->ExtractChildInfo("CHANNEL", MetaDataInfo::Integer, 2);
          if (!channel)
          {
            btkWarningMacro("Metadata '" + (*itChan)->GetLabel() + "' is corrupted. No child labeled 'CHANNEL'. Impossible to detect partial force platform configuration.");
            itChan = fpChanList.erase(itChan);
            continue;
          }
          if (static_cast<int>(channel->GetValues().size()) == (chanNumber * usedNumber))
          {
            (*itConfig)->AppendChild((*itChan)->GetChild("CHANNEL"));
            this->UpdateForcePlatformMetaData(out, *itConfig);
            if ((*itConfig)->GetLabel().compare("FORCE_PLATFORM") == 0)
              out->RemoveChild((*itChan)->GetLabel());
            fpChanList.erase(itChan);
            break;
          }
          ++itChan;
        }
      }
    }
  };
  
  bool MergeAcquisitionFilter::UpdateForcePlatformMetaData(MetaData::Pointer out, MetaData::Pointer forcePlatform) const
  {
    int usedOut, chanOut, calRowOut;
    MetaData::Iterator fpIt = out->FindChild("FORCE_PLATFORM");
    if (fpIt != out->End())
    {
      usedOut = this->GetMaxInfoForcePlatform(*fpIt, &chanOut, &calRowOut);
      if (!this->CheckForcePlatform(*fpIt) || (usedOut == -1))
      {
        btkWarningMacro("Existing metadata for defining force platforms configuration are corrupted. Only valid metadata will be kept in the merged acquisition.");
        fpIt = out->RemoveChild(fpIt);
      }
    }
    if (this->CheckForcePlatform(forcePlatform))
    {
      if ((fpIt == out->End()) || ((*fpIt)->GetLabel().compare("FORCE_PLATFORM") != 0))
      {
        forcePlatform->SetLabel("FORCE_PLATFORM");
        if (out->FindChild("FORCE_PLATFORM") == out->End()) // Come from a partial configuration?
          out->AppendChild(forcePlatform);
      }
      else
      {
        int chanIn, calRowIn;
        int usedIn = this->GetMaxInfoForcePlatform(forcePlatform, &chanIn, &calRowIn);
        if (usedIn == -1)
          return false;
        int usedNum = usedOut + usedIn;
        int chanNum, calRowNum;
        if (chanIn >= chanOut)
        {
          chanNum = chanIn;
          calRowNum = calRowIn;
        }
        else
        {
          chanNum = chanOut;
          calRowNum = calRowOut;
        }
        std::vector<int16_t> typeValOut, chanValOut, typeValIn, chanValIn;
        std::vector<float> cornersValOut, originValOut, cornersValIn, originValIn, calMatrixValIn, calMatrixValOut;
        MetaDataInfo::Pointer typeInfoOut = (*fpIt)->GetChild("TYPE")->GetInfo();
        typeInfoOut->ToInt16(typeValOut);
        MetaDataInfo::Pointer channelInfoOut = (*fpIt)->GetChild("CHANNEL")->GetInfo();
        channelInfoOut->ToInt16(chanValOut);
        MetaDataInfo::Pointer cornersInfoOut = (*fpIt)->GetChild("CORNERS")->GetInfo();
        cornersInfoOut->ToFloat(cornersValOut);
        MetaDataInfo::Pointer originInfoOut = (*fpIt)->GetChild("ORIGIN")->GetInfo();
        originInfoOut->ToFloat(originValOut);
        forcePlatform->GetChild("TYPE")->GetInfo()->ToInt16(typeValIn);
        forcePlatform->GetChild("CHANNEL")->GetInfo()->ToInt16(chanValIn);
        forcePlatform->GetChild("CORNERS")->GetInfo()->ToFloat(cornersValIn);
        forcePlatform->GetChild("ORIGIN")->GetInfo()->ToFloat(originValIn);
        MetaData::Iterator calMatrixOutIt = (*fpIt)->FindChild("CAL_MATRIX");
        MetaData::Iterator calMatrixInIt = forcePlatform->FindChild("CAL_MATRIX");
        if (calMatrixOutIt != (*fpIt)->End())
          (*calMatrixOutIt)->GetInfo()->ToFloat(calMatrixValOut);
        if (calMatrixInIt != forcePlatform->End())
          (*calMatrixInIt)->GetInfo()->ToFloat(calMatrixValIn);
        // Resize
        typeValOut.resize(usedNum);
        cornersValOut.resize(usedNum * 4 * 3);
        originValOut.resize(usedNum * 3);
        std::vector<int16_t> chanVal(usedNum * chanNum, 65535);
        std::vector<float> calMatrixVal(usedNum * chanNum * calRowNum, 0.0);
        for (int i = 0 ; i < usedOut ; ++i)
        {
          for (int j = (i * chanNum) ; j < (chanOut + i * chanNum) ; ++j)
            chanVal[j] = chanValOut[j - i * (chanNum - chanOut)];
          if (static_cast<int>(calMatrixValOut.size()) >= ((i + 1) * chanOut * calRowOut))
          {
            int diff = 0, inc = 0;
            for (int j = (i * chanNum * calRowNum) ; j < (chanOut * calRowOut + (i * chanNum * calRowNum)) ; ++j)
              calMatrixVal[j + diff] = calMatrixValOut[j - i * (chanNum * calRowNum - chanOut * calRowOut)];
            if (inc >= calRowIn)
            {
              diff += (calRowNum - calRowOut);
              inc = 0;
            }
          }
        }
        for (int i = usedOut ; i < usedNum ; ++i)
        {
          typeValOut[i] = typeValIn[i - usedOut];
          for (int j = (i * 4 * 3) ; j < ((i + 1) * 4 * 3) ; ++j)
            cornersValOut[j] = cornersValIn[j - (usedOut * 4 * 3)];
          for (int j = (i * 3) ; j < ((i + 1) * 3) ; ++j)
            originValOut[j] = originValIn[j - (usedOut * 3)];
          for (int j = (i * chanNum) ; j < (chanIn + i * chanNum) ; ++j)
            chanVal[j] = chanValIn[j - usedOut * chanIn - i *(chanNum - chanIn)] + usedOut * chanNum;
          if (static_cast<int>(calMatrixValIn.size()) >= ((i + 1 - usedOut) * chanIn * calRowIn))
          {
            int diff = 0, inc = 0;
            for (int j = (i * chanNum * calRowNum) ; j < (chanIn * calRowIn + (i * chanNum * calRowNum)) ; ++j)
            {
              calMatrixVal[j + diff] = calMatrixValIn[j - usedOut * (chanIn * calRowIn) - i * (chanNum * calRowNum - chanIn * calRowIn)];
              ++inc;
              if (inc >= calRowIn)
              {
                diff += (calRowNum - calRowIn);
                inc = 0;
              }
            }
          }
        }
        // Adapt metadata
        (*fpIt)->GetChild("USED")->GetInfo()->SetValues(static_cast<int16_t>(usedNum));
        std::vector<uint8_t> dims = typeInfoOut->GetDimensions();
        dims[0] = usedNum;
        typeInfoOut->SetValues(dims, typeValOut);
        dims = channelInfoOut->GetDimensions();
        dims[0] = chanNum; dims[1] = usedNum;
        channelInfoOut->SetValues(dims, chanVal);
        dims = cornersInfoOut->GetDimensions();
        dims[2] = usedNum;
        cornersInfoOut->SetValues(dims, cornersValOut);
        dims = originInfoOut->GetDimensions();
        dims[1] = usedNum;
        originInfoOut->SetValues(dims, originValOut);
        if (!calMatrixValOut.empty() || !calMatrixValIn.empty())
        {
          MetaData::Pointer calMatrix;
          if (calMatrixOutIt == (*fpIt)->End())
          {
            calMatrix = MetaData::New("CAL_MATRIX");
            (*fpIt)->AppendChild(calMatrix);
          }
          else
            calMatrix = (*calMatrixOutIt);
          dims.resize(3);
          dims[0] = calRowNum; dims[1] = chanNum; dims[2] = usedNum;
          calMatrix->SetInfo(MetaDataInfo::New(dims, calMatrixVal));
        }
      }
    }
    else
    {
      btkErrorMacro("Invalid metadata to merge a new configuration of force platforms");
      return false;
    }
    return true;
  };
  
  bool MergeAcquisitionFilter::CheckForcePlatform(MetaData::Pointer forcePlatform) const
  {
    // Check the format of each FORCE_PLATFORM child
    MetaDataInfo::Pointer used = forcePlatform->ExtractChildInfo("USED", MetaDataInfo::Integer, 0);
    MetaDataInfo::Pointer type = forcePlatform->ExtractChildInfo("TYPE", MetaDataInfo::Integer, 1, false);
    // Not necessary to check FORCE_PLATFORM:ZERO
    MetaDataInfo::Pointer corners = forcePlatform->ExtractChildInfo("CORNERS", MetaDataInfo::Real, 3, false);
    MetaDataInfo::Pointer origin = forcePlatform->ExtractChildInfo("ORIGIN", MetaDataInfo::Real, 2, false);
    MetaDataInfo::Pointer channel = forcePlatform->ExtractChildInfo("CHANNEL", MetaDataInfo::Integer, 2, false);
    MetaDataInfo::Pointer calMatrix = forcePlatform->ExtractChildInfo("CAL_MATRIX", MetaDataInfo::Real, 3, false);
    
    bool valid = used && type && corners && origin && channel;
    
    if (valid)
    {
      int usedVal = used->ToInt(0);
      int typeUsedVal = type->GetDimensions()[0];
      int cornersUsedVal = corners->GetDimensions()[2];
      int originUsedVal = origin->GetDimensions()[1];
      int chanUsedVal = channel->GetDimensions()[1];
      
      if ((typeUsedVal < usedVal) || (cornersUsedVal < usedVal) || (originUsedVal < usedVal) || (chanUsedVal < usedVal))
        return false;
      if (calMatrix)
      {
        int calMatrixUsedVal = calMatrix->GetDimensions()[2];
        if (calMatrixUsedVal < usedVal)
          return false;
      }
    }
    else
      return false;
    return true;
  }
  
  int MergeAcquisitionFilter::GetMaxInfoForcePlatform(MetaData::Pointer fp, int* rows, int* cols) const
  {
    MetaDataInfo::Pointer used = fp->ExtractChildInfo("USED", MetaDataInfo::Integer, 0);
    MetaDataInfo::Pointer type = fp->ExtractChildInfo("TYPE", MetaDataInfo::Integer, 1);
    if (!used || !type)
    {
      btkErrorMacro("Metadata '" + fp->GetLabel() + "' seems corrupted. No child labeled 'USED' or 'TYPE'. Impossible to detect partial force platform configuration.");
      return -1;
    }
    size_t usedValue = used->ToInt(0);
    std::vector<int> typeValues = type->ToInt();
    if (usedValue < typeValues.size())
    {
      btkErrorMacro("Metadata '" + fp->GetLabel() + "' seems corrupted. The number of force platform used is greater than the number of force platform.");
      return -1;
    }
    int chanInNumber = 0, chanOutNumber = 0;
    for (size_t i = 0 ; i < usedValue ; ++i)
    {
      switch (typeValues[i])
      {
      case 1:
      case 2:
      case 4:
      case 21:
        if (chanInNumber < 6)
        {
          chanInNumber = 6;
          chanOutNumber = 6;
        }
        break;
      case 3:
      case 7:
      case 11:
      case 12:
        if (chanInNumber < 8)
        {
          chanInNumber = 8;
          chanOutNumber = 8;
        }
        break;
      case 5:
        if (chanInNumber < 8)
        {
          chanInNumber = 8;
          chanOutNumber = 6;
        }
        break;
      case 6:
        if (chanInNumber < 12)
        {
          chanInNumber = 12;
          chanOutNumber = 12;
        }
        break;
      default:
        {
        btkErrorMacro("Metadata '" + fp->GetLabel() + "' is corrupted. Unknown type of force platform.");
        return -1;
        }
      }
    }
    if (rows)
      *rows = chanInNumber;
    if (cols)
      *cols = chanOutNumber;
    return static_cast<int>(usedValue);
  };
  
  bool MergeAcquisitionFilter::UpdateTrialMetaData(Acquisition::Pointer out, MetaData::Pointer trial, bool mergeData) const
  {
    MetaData::Iterator it = out->GetMetaData()->FindChild("TRIAL");
    if (it == out->GetMetaData()->End())
    {
      trial->RemoveChild("DATE");
      trial->RemoveChild("TIME");
      out->GetMetaData()->AppendChild(trial);
    }
    else
    {
      const char* labels[] = {"DATE", "TIME", "VIDEO_RATE_DIVIDER", "CAMERA_RATE"};
      int num = sizeof(labels) / sizeof(char*);
      for (int i = 0 ; i < num ; ++i)
      {
        MetaData::Iterator itIn = trial->FindChild(labels[i]);
        MetaData::Iterator itOut = (*it)->FindChild(labels[i]);
        if ((itIn == trial->End()) || (itOut == (*it)->End()) || (*(*itIn) != *(*(itOut))))
          (*it)->RemoveChild(itOut);
      }
    }
    if (mergeData)
    {
      (*it)->RemoveChild("DATE");
      (*it)->RemoveChild("TIME");
    }
    
    std::vector<int16_t> val(2, 0);
    val[0] = static_cast<int16_t>(out->GetFirstFrame() % 65535);
    val[1] = static_cast<int16_t>(out->GetFirstFrame() / 65535);
    MetaDataCreateChild(*it, "ACTUAL_START_FIELD", val);
    val[0] = static_cast<int16_t>(out->GetLastFrame() % 65535);
    val[1] = static_cast<int16_t>(out->GetLastFrame() / 65535);
    MetaDataCreateChild(*it, "ACTUAL_END_FIELD", val);

    return true;
  };
  
  bool MergeAcquisitionFilter::UpdateEventContextMetaData(MetaData::Pointer out, MetaData::Pointer eventContextIn) const
  {
    MetaData::Iterator itOut = out->FindChild("EVENT_CONTEXT");
    if (itOut != out->End())
    {
      if (!this->CheckEventContext(*itOut))
      {
        btkWarningMacro("Existing metadata for defining events context are corrupted. Only valid metadata will be kept in the merged acquisition.");
        itOut = out->RemoveChild(itOut);
      }
    }
    if (this->CheckEventContext(eventContextIn))
    {
      if (*(*itOut) != *eventContextIn)
      {
        MetaDataInfo::Pointer usedInfo = (*itOut)->GetChild("USED")->GetInfo();
        int usedOut = usedInfo->ToInt(0);
        int usedIn = eventContextIn->GetChild("USED")->GetInfo()->ToInt(0);
        std::vector<std::string> labelsOut, descOut, labelsIn, descIn;
        std::vector<int16_t> iconsOut, coloursOut, iconsIn, coloursIn;
        MetaDataInfo::Pointer labelsInfo = (*itOut)->GetChild("LABELS")->GetInfo();
        labelsInfo->ToString(labelsOut);
        MetaDataInfo::Pointer descInfo = (*itOut)->GetChild("DESCRIPTIONS")->GetInfo();
        descInfo->ToString(descOut);
        MetaDataInfo::Pointer iconsInfo = (*itOut)->GetChild("ICON_IDS")->GetInfo();
        iconsInfo->ToInt16(iconsOut);
        MetaDataInfo::Pointer coloursInfo = (*itOut)->GetChild("COLOURS")->GetInfo();
        coloursInfo->ToInt16(coloursOut);
        eventContextIn->GetChild("LABELS")->GetInfo()->ToString(labelsIn);
        eventContextIn->GetChild("DESCRIPTIONS")->GetInfo()->ToString(descIn);
        eventContextIn->GetChild("ICON_IDS")->GetInfo()->ToInt16(iconsIn);
        eventContextIn->GetChild("COLOURS")->GetInfo()->ToInt16(coloursIn);
        
        bool modified = false;
        for (int i = 0 ; i < usedOut ; ++i)
        {
          bool found = false;
          int j = 0;
          for (j = 0 ; j < usedIn ; ++j)
          {
            if (labelsOut[i].compare(labelsIn[j]) == 0)
            {
              found = true;
              break;
            }
          }
          if (!found)
          {
            usedOut += 1;
            labelsOut.push_back(labelsIn[j]);
            descOut.push_back(descIn[j]);
            iconsOut.push_back(iconsIn[j]);
            coloursOut.push_back(coloursIn[j*3]);
            coloursOut.push_back(coloursIn[j*3+1]);
            coloursOut.push_back(coloursIn[j*3+2]);
            modified = true;
          }
        }
        if (modified)
        {
          usedInfo->SetValues(static_cast<int16_t>(usedOut));
          labelsInfo->SetValues(labelsOut);
          descInfo->SetValues(descOut);
          iconsInfo->SetValues(iconsOut);
          std::vector<uint8_t> dims(2,0); dims[0] = 3; dims[1] = usedOut;
          coloursInfo->SetValues(dims, coloursOut);
        }
      }
    }
    else
    {
      btkErrorMacro("Invalid metadata to merge a new configuration of force platforms");
      return false;
    }
    return true;
  };
  
  bool MergeAcquisitionFilter::CheckEventContext(MetaData::Pointer eventContext) const
  {
    MetaDataInfo::Pointer used = eventContext->ExtractChildInfo("USED", MetaDataInfo::Integer, 0);
    MetaDataInfo::Pointer icon = eventContext->ExtractChildInfo("ICON_IDS", MetaDataInfo::Integer, 1, false);
    MetaDataInfo::Pointer label = eventContext->ExtractChildInfo("LABELS", MetaDataInfo::Char, 2, false);
    MetaDataInfo::Pointer desc = eventContext->ExtractChildInfo("DESCRIPTIONS", MetaDataInfo::Char, 2, false);
    MetaDataInfo::Pointer colours = eventContext->ExtractChildInfo("COLOURS", MetaDataInfo::Integer, 2, false);

    bool valid = used && icon && label && desc && colours;
    if (valid)
    {
      int usedVal = used->ToInt(0);
      int iconUsedVal = icon->GetDimensions()[0];
      int labelUsedVal = label->GetDimensions()[1];
      int descUsedVal = desc->GetDimensions()[1];
      int coloursUsedVal = colours->GetDimensions()[1];
      
      if ((iconUsedVal < usedVal) || (labelUsedVal < usedVal) || (descUsedVal < usedVal) || (coloursUsedVal < usedVal))
        return false;
    }
    else
      return false;
    return true;
  };
};
