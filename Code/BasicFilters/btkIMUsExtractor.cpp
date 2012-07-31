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

#include "btkIMUsExtractor.h"
#include "btkConvert.h"

namespace btk
{
 /**
   * @class IMUsExtractor btkIMUsExtractor.h
   * @brief Extracts inertial measurement units (IMUs) from a btk::Acquisition object. 
   *
   * The choice of the channel to set correctly accelerometers and gyroscopes must be done using the methods SetSensorsChannelIndices() or AppendSensorChannelIndices().
   * By default, it is assumed that the first 6 analog channels correspond to first IMU (and so on) and are ordered. The channels #1, #2 and #3 correspond the axis (X,Y,Z) of the 3D accelerometer, while the channels #4, #5 and #6 correspond the axis of the 3D gyroscope.
   * If you don't use 3D sensors, then set the unused index to the value -1 (e.g. AppendSensorChannelIndices(0,-1,1,-1,2,-1). Only the axis X and Z of the accelerometer are extracted and the axis Y of the gyroscope.)
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef IMUsExtractor::Pointer
   * Smart pointer associated with a IMUsExtractor object.
   */
  
  /**
   * @typedef IMUsExtractor::ConstPointer
   * Smart pointer associated with a const IMUsExtractor object.
   */
    
  /**
   * @fn static Pointer IMUsExtractor::New();
   * Creates a smart pointer associated with a IMUsExtractor object.
   */

  /**
   * @fn Acquisition::Pointer IMUsExtractor::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn void IMUsExtractor::SetInput(Acquisition::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer IMUsExtractor::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  IMUsExtractor::IMUsExtractor()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
  };

  /**
   * @fn Acquisition::Pointer IMUsExtractor::GetInput(int idx)
   * Gets the input registered with this process at the index @a idx.
   */

  /**
   * @fn PointCollection::Pointer IMUsExtractor::GetOutput(int idx)
   * Gets the output created with this process at the index @a idx.
   */
  
  /**
   * Creates a PointCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer IMUsExtractor::MakeOutput(int /* idx */)
  {
    return IMUCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void IMUsExtractor::GenerateData()
  {
    IMUCollection::Pointer output = this->GetOutput();
    output->Clear();
    size_t numTotalUnits = 0;
    for (int i = 0 ; i < this->GetInputNumber() ; ++i)
    {
      Acquisition::Pointer input = this->GetInput(i);
      if (input.get() != 0)
      {
        MetaData::Iterator itIMU = input->GetMetaData()->FindChild("IMU");
        if (itIMU != input->GetMetaData()->End())
        {
          MetaData::Iterator itUsed = (*itIMU)->FindChild("USED");
          size_t numUnits = 0;
          if (itUsed != (*itIMU)->End())
            numUnits = (*itUsed)->GetInfo()->ToInt(0);

          MetaData::Iterator itType = (*itIMU)->FindChild("TYPE");
          std::vector<int> types = std::vector<int>(numUnits, 1);
          if (itType != (*itIMU)->End())
          {
            (*itType)->GetInfo()->ToInt(types);
            if (types.size() < numUnits)
            {
              btkErrorMacro("IMU:USED and IMU:TYPE don't indicate the same number of units. The lower is kept: IMU:TYPE");
              numUnits = types.size();
            }
            else if (types.size() > numUnits)
            {
              btkErrorMacro("IMU:USED and IMU:TYPE don't indicate the same number of units. The lower is kept: IMU:USED");
            }
          }

          MetaData::Iterator itChannels = (*itIMU)->FindChild("CHANNEL");
          if (itChannels == (*itIMU)->End())
          {
            btkErrorMacro("No IMU::CHANNEL entry. Impossible to extract analog channels associated with the unit(s). IMUs' data are empty.");
            return;
          }
          else if (!(*itChannels)->HasInfo() || (*itChannels)->GetInfo()->GetDimensions().size() != 2)
          {
            btkErrorMacro("Wrong format for the IMU::CHANNEL entry. Impossible to extract analog channels associated with the unit(s). IMUs' data are empty.");
            return;
          }

          MetaDataInfo::Pointer info;
          
          std::vector<std::string> labels;
          info = (*itIMU)->ExtractChildInfo("LABELS", MetaDataInfo::Char, 2);
          if (info)
            info->ToString(labels);
          labels.resize(numUnits);
          for (size_t j = 0 ; j < numUnits ; ++j)
          {
            if (labels[j].empty())
              labels[j] = "IMU #" + ToString(numTotalUnits+j+1);
          }
          
          std::vector<std::string> descs;
          info = (*itIMU)->ExtractChildInfo("DESCRIPTIONS", MetaDataInfo::Char, 2);
          if (info)
            info->ToString(descs);
          descs.resize(numUnits);

          AnalogCollection::Pointer analogs = input->GetAnalogs();
          std::vector<int> channelsIndex;
          (*itChannels)->GetInfo()->ToInt(channelsIndex);
          int channelStep = (*itChannels)->GetInfo()->GetDimension(0);
          int channelNumberAlreadyExtracted = 0;
          channelsIndex.resize(numUnits*channelStep,-1);
          IMU::Pointer imu;

          for(size_t i = 0 ; i < numUnits ; ++i)
          {
            bool newIMU = true;
            switch(types[i])
            {
            case 1:
              imu = IMU::New();
              this->ExtractData(imu.get(), input->GetAnalogs(), labels[i], descs[i], channelsIndex, channelStep, channelNumberAlreadyExtracted);
              break;
            /* TODO: Implement IMMU
            case 2:
              imu = IMMU::New();
              this->ExtractForcePlatformDataCommon((*itFP), i, calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
              break;
            */
            default:
              newIMU = false;
              btkErrorMacro("Unsupported IMU type. Impossible to extract corresponding data");
              break;
            }
            channelNumberAlreadyExtracted += channelStep;
            if (newIMU)
            {
              imu->SetFrequency(input->GetAnalogFrequency());
              output->InsertItem(imu);
            }
          }
          numTotalUnits += numUnits;
        }
      }
    }
  };
  
  void IMUsExtractor::ExtractData(IMU* imu, AnalogCollection::Pointer channels, const std::string& label, const std::string desc, std::vector<int> channelsIndex, int numberOfChannelsToExtract, int alreadyExtracted)
  {
    int numberOfChannels = channels->GetItemNumber();
    imu->SetLabel(label);
    imu->SetDescription(desc);
    
    for (int i = 0 ; i < numberOfChannelsToExtract ; ++i)
    {
      int index = channelsIndex[i + alreadyExtracted];
      if ((index >= 1) && (index <= numberOfChannels))
        imu->SetChannel(i, channels->GetItem(index - 1));
      else
        imu->SetChannel(i, Analog::Pointer());
    }
  }
};
