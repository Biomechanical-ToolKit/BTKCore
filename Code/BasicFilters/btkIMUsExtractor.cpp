/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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
#include "btkIMUTypes.h"
#include "btkConvert.h"

namespace btk
{
 /**
  * @class IMUsExtractor btkIMUsExtractor.h
  * @brief Extracts inertial measurement units (IMUs) from a btk::Acquisition object. 
  *
  * Based on the content of the metadata IMU and the analog channels stored in the given acquisition, this filter creates a collection of btk::IMU returned in the output.
  *
  * The metadata IMU must be structured as the following:
  *  - IMU:USED: single integer storing the number of IMUs ;
  *  - IMU:TYPE: 1D array of integer containing the type of IMU contained in the acquisition (see below for the supported types) ;
  *  - IMU:CHANNEL: 2D array of integer containing 1-based indices of the analog channels used ;
  *  - IMU:LABELS: 1D array of strings containing the force plates' label ;
  *  - IMU:DESCRIPTIONS: 1D array of strings containing the force plates' description ;
  *  - IMU:CAL_MATRIX (optional): 2D array (max_num_channels x max_num_channels, num_imus) of float to construct matrices used to correct alignment between sensors
  *  - IMU:EXTRA (optional): 1D array of integer containing the number of extra channels for each IMU (e.g. ECG, altimeter, etc) ;
  *
  * The supported types are the following:
  *  - Type 1: 6D IMU with a 3D accelerometer and 3D gyroscope.
  *  - Type 2: 6D IMU + a calibration matrix to align sensors together.
  *
  * Note: In case your inertial sensor contain only a subset of the proposed type (for example 1D accelerometer + 2D gyroscope), you don't need to add 3 channels with 0 values. Only the metadata IMU:CHANNEL and IMU:CAL_MATRIX must be adapted in consequence. For example the value of IMU:CHANNEL could be [0 0 1 2 3 0]  (Z accelerometer on channel #1, X and Y gyroscope on #2 and #3 respectively). The metadata IMU:CAL_MATRIX will be then a matrix with 3x3 value (e.g. [1 0 0 0 1 0 0 0 1]).
  *
  * @note This class is still experimental and could be modified in the next release.
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
   * @fn Acquisition::Pointer IMUsExtractor::GetInput(int idx)
   * Gets the input registered with this process at the index @a idx.
   */
   
  /**
   * @fn void IMUsExtractor::SetInput(Acquisition::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn void IMUsExtractor::SetInput(int idx, Acquisition::Pointer input)
   * Sets the input required with this process at the index @a idx.
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
      if (input != Acquisition::Null)
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
              btkWarningMacro("IMU:USED and IMU:TYPE do not indicate the same number of units. The lower is kept: IMU:TYPE");
              numUnits = types.size();
            }
            else if (types.size() > numUnits)
            {
              btkWarningMacro("IMU:USED and IMU:TYPE do not indicate the same number of units. The lower is kept: IMU:USED");
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
          
          MetaData::Iterator itCalMatrix = (*itIMU)->FindChild("CAL_MATRIX");
          if ((itCalMatrix != (*itIMU)->End()) && (!(*itChannels)->HasInfo() || (*itChannels)->GetInfo()->GetDimensions().size() != 2))
          {
            btkErrorMacro("Wrong format for the IMU::CAL_MATRIX entry. Impossible to extract analog channels associated with the unit(s). IMUs' data are empty.");
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
          
          std::vector<int> extra;
          info = (*itIMU)->ExtractChildInfo("EXTRA", MetaDataInfo::Integer, 1);
          if (info)
            info->ToInt(extra);
          extra.resize(numUnits,0);

          AnalogCollection::Pointer analogs = input->GetAnalogs();
          std::vector<int> channelsIndex;
          (*itChannels)->GetInfo()->ToInt(channelsIndex);
          int channelStep = (*itChannels)->GetInfo()->GetDimension(0);
          int channelNumberAlreadyExtracted = 0;
          channelsIndex.resize(numUnits*channelStep,-1);
          std::vector<double> calibration;
          if (itCalMatrix != (*itIMU)->End())
          {  
            (*itCalMatrix)->GetInfo()->ToDouble(calibration);
            calibration.resize(numUnits*channelStep*channelStep,0.0);
          }
          IMU::Pointer imu;

          for(size_t i = 0 ; i < numUnits ; ++i)
          {
            switch(types[i])
            {
            case 1:
              imu = IMUType1::New(labels[i], descs[i], false);
              this->ExtractData(imu.get(), input->GetAnalogs(), channelsIndex, 6+extra[i], channelNumberAlreadyExtracted);
              break;
            case 2:
              imu = IMUType2::New(labels[i], descs[i], false);
              this->ExtractDataWithCalibrationMatrix(imu.get(), input->GetAnalogs(), &(calibration[channelNumberAlreadyExtracted*channelNumberAlreadyExtracted]), channelsIndex, 6+extra[i], channelStep, channelNumberAlreadyExtracted);
              break;
            // TODO: Implement IMMU
            default:
              btkErrorMacro("Unsupported IMU type. Impossible to extract corresponding data");
              continue;
              break;
            }
            channelNumberAlreadyExtracted += channelStep;
            imu->SetFrequency(input->GetAnalogFrequency());
            imu->SetFrameNumber(input->GetAnalogFrameNumber());
            output->InsertItem(imu);
          }
          numTotalUnits += numUnits;
        }
      }
    }
  };
  
  void IMUsExtractor::ExtractData(IMU* imu, AnalogCollection::Pointer channels, std::vector<int> channelsIndex, int numberOfChannelsToExtract, int alreadyExtracted)
  {
    int numberOfChannels = channels->GetItemNumber();
    for (int i = 0 ; i < numberOfChannelsToExtract ; ++i)
    {
      int index = channelsIndex[i + alreadyExtracted];
      if ((index >= 1) && (index <= numberOfChannels))
        imu->SetChannel(i, channels->GetItem(index - 1));
    }
  };
  
  void IMUsExtractor::ExtractDataWithCalibrationMatrix(IMU* imu, AnalogCollection::Pointer channels, double* pCalib, std::vector<int> channelsIndex, int numberOfChannelsToExtract, int channelStep, int alreadyExtracted)
  {
    int numberOfChannels = channels->GetItemNumber();
    int numChannelsExtracted = 0;
    int numberOfFrames = channels->GetItem(0)->GetFrameNumber();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> data = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(numberOfFrames, numberOfChannelsToExtract);
    for (int i = 0 ; i < numberOfChannelsToExtract ; ++i)
    {
      int index = channelsIndex[i + alreadyExtracted];
      if ((index >= 1) && (index <= numberOfChannels))
      {
        Analog::Pointer channel = Analog::New();
        Analog::Pointer channelToCopy = channels->GetItem(index - 1);
        channel->SetLabel(channelToCopy->GetLabel());
        channel->SetDescription(channelToCopy->GetDescription());
        channel->SetUnit(channelToCopy->GetUnit());
        imu->SetChannel(i, channel);
        data.col(numChannelsExtracted) = channelToCopy->GetValues();
        ++numChannelsExtracted;
      }
    }
    data.conservativeResize(numberOfFrames,numChannelsExtracted);
    imu->SetCalMatrix(Eigen::Map< Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> >(pCalib, channelStep, channelStep).block(0,0,numChannelsExtracted,numChannelsExtracted));
    data *= imu->GetCalMatrix().transpose();
    numChannelsExtracted = 0;
    for (int i = 0 ; i < numberOfChannelsToExtract ; ++i)
    {
      Analog::Pointer channel = imu->GetChannel(i);
      if (channel != Analog::Null)
      {
        channel->SetValues(data.col(numChannelsExtracted));
        ++numChannelsExtracted;
      }
    }
  };
};
