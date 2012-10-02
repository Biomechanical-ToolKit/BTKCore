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

#include "btkForcePlatformsExtractor.h"
#include "btkMetaData.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class ForcePlatformsExtractor btkForcePlatformsExtractor.h
   * @brief Extracts force platform data from a btk::Acquisition object. 
   *
   * Based on the content of the metadata FORCE_PLATFORM and the analog channels stored in the given acquisition, this filter creates a collection of btk::ForcePlatform returned in the output.
   *
   * Depending of the type of the force platform, the analog channels' data are transformed to take into account a calibration matrix.
   *
   * The metadata FORCE_PLATFORM must be structured as the following:
   *  - FORCE_PLATFORM:USED: single integer storing the number of force plates ;
   *  - FORCE_PLATFORM:TYPE: 1D array of integer containing the type of force plates contained in the acquisition (see below for the supported types) ;
   *  - FORCE_PLATFORM:CHANNEL: 2D array of integer containing of 1-based indices of the analog channels used ;
   *  - FORCE_PLATFORM:LABELS: 1D array of strings containing the force plates' label ;
   *  - FORCE_PLATFORM:DESCRIPTIONS: 1D array of strings containing the force plates' description ;
   *  - FORCE_PLATFORM:ORIGIN: 2D array of reals (float) containing the coordinates of the origin of the surface of each force platform expressed in their frame ;
   *  - FORCE_PLATFORM:CORNERS: 3D array of reals containing the coordinates of the corners. The third dimensions corresponds to the number of force platforms ;
   *  - FORCE_PLATFORM:CAL_MATRIX: (could be optional, depend on the FP type). 3D array containing the calibration to transform the volts into forces and moments ;
   * You can have more details on each metadata by following this link: http://www.c3d.org/HTML/theforceplatformgroup.htm
   *
   * This filter is able to manage the following force platform's type:
   *  - Type 1: 6 channels with Forces X,Y,Z Position X,Y of the COP, and Moment Z around the COP
   *  - Type 2: 6 channels (FX, FY, FZ, MX, MY, MZ);
   *  - Type 3: 8 channels (FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23);
   *  - Type 4: Same as Type-2 + calibration matrix 6 (columns) by 6 (rows);
   *  - Type 5: Same as Type-3 + calibration matrix 6 (columns) by 8 (rows).
   *
   * However, some other types are not yet supported due to the lack of information and data on them:
   *  - Type 6: 12 channels (FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4]) + calibration matrix 12 by 12;
   *  - Type 7: 8 channels (FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23) + calibration matrix 8 by 8;
   *  - Type 11: Kistler Split Belt Treadmill: 8 channels + calibration matrix 8X8 + polynomial correction matrix 2x6 + COP translation + COP rotation;
   *  - Type 12: Gaitway treadmill: 8 channels (Fz11, Fz12, Fz13, Fz14, Fz21, Fz22, Fz23, and Fz24) + calibration matrix 8X8.
   *  - Type 21: AMTI-Stairs: 2 force plates with 6 channels + a calibration matrix 6x6 + data to locate the corners of the 4 steps.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef ForcePlatformsExtractor::Pointer
   * Smart pointer associated with a ForcePlatformsExtractor object.
   */
  
  /**
   * @typedef ForcePlatformsExtractor::ConstPointer
   * Smart pointer associated with a const ForcePlatformsExtractor object.
   */
    
  /**
   * @fn static Pointer ForcePlatformsExtractor::New();
   * Creates a smart pointer associated with a ForcePlatformsExtractor object.
   */

  /**
   * @fn Acquisition::Pointer ForcePlatformsExtractor::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn void ForcePlatformsExtractor::SetInput(Acquisition::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer ForcePlatformsExtractor::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  ForcePlatformsExtractor::ForcePlatformsExtractor()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
  };

  /**
   * @fn Acquisition::Pointer ForcePlatformsExtractor::GetInput(int idx)
   * Gets the input registered with this process at the index @a idx.
   */

  /**
   * @fn PointCollection::Pointer ForcePlatformsExtractor::GetOutput(int idx)
   * Gets the output created with this process at the index @a idx.
   */
  
  /**
   * Creates a ForcePlatformCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer ForcePlatformsExtractor::MakeOutput(int /* idx */)
  {
    return ForcePlatformCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void ForcePlatformsExtractor::GenerateData()
  {
    ForcePlatformCollection::Pointer output = this->GetOutput();
    Acquisition::Pointer input = this->GetInput();
    if (!input)
      output->Clear(); 
    else
    {
      MetaData::Iterator itForcePlatformGr = input->GetMetaData()->FindChild("FORCE_PLATFORM");
      if (itForcePlatformGr != input->GetMetaData()->End())
      {
        MetaData::Iterator itUsed = (*itForcePlatformGr)->FindChild("USED");
        size_t numberOfForcePlatforms = 0;
        if (itUsed != (*itForcePlatformGr)->End())
          numberOfForcePlatforms = (*itUsed)->GetInfo()->ToInt(0);

        MetaData::Iterator itType = (*itForcePlatformGr)->FindChild("TYPE");
        std::vector<int> types = std::vector<int>(numberOfForcePlatforms, 1);
        if (itType != (*itForcePlatformGr)->End())
        {
          (*itType)->GetInfo()->ToInt(types);
          if (types.size() < numberOfForcePlatforms)
          {
            btkErrorMacro("FORCE_PLATFORM:USED and FORCE_PLATFORM:TYPE don't indicate the same number of force platforms. The lower is kept: FORCE_PLATFORM:TYPE");
            numberOfForcePlatforms = types.size();
          }
          else if (types.size() > numberOfForcePlatforms)
          {
            btkErrorMacro("FORCE_PLATFORM:USED and FORCE_PLATFORM:TYPE don't indicate the same number of force platforms. The lower is kept: FORCE_PLATFORM:USED");
          }
        }

        MetaData::Iterator itChannels = (*itForcePlatformGr)->FindChild("CHANNEL");
        if (itChannels == (*itForcePlatformGr)->End())
        {
          btkErrorMacro("No FORCE_PLATFORM::CHANNEL entry. Impossible to extract analog channels associated with the force platform(s). Force platforms' data are empty.");
          return;
        }
        else if (!(*itChannels)->HasInfo() || (*itChannels)->GetInfo()->GetDimensions().size() != 2)
        {
          btkErrorMacro("Wrong format for the FORCE_PLATFORM::CHANNEL entry. Impossible to extract analog channels associated with the force platform(s). Force platforms' data are empty.");
          return;
        }

        MetaData::Pointer pOrigin;
        MetaData::Iterator itOrigin = (*itForcePlatformGr)->FindChild("ORIGIN");
        if (itOrigin == (*itForcePlatformGr)->End())
        {
          btkErrorMacro("No FORCE_PLATFORM::ORIGIN entry. Default values are used.");
        }
        else
          pOrigin = (*itOrigin);
        
        MetaData::Pointer pCorners;
        MetaData::Iterator itCorners = (*itForcePlatformGr)->FindChild("CORNERS");
        if (itCorners == (*itForcePlatformGr)->End())
        {
          btkErrorMacro("No FORCE_PLATFORM::CORNERS entry. Default values are used.");
        }
        else
          pCorners = (*itCorners);
        MetaData::Pointer pCalMatrix;
        MetaData::Iterator itCalMatrix = (*itForcePlatformGr)->FindChild("CAL_MATRIX");
        int calMatrixStep = 0;
        if (itCalMatrix == (*itForcePlatformGr)->End())
        {
          for(size_t i = 0 ; i < numberOfForcePlatforms ; ++i)
          {
            if (types[i] > 3)
            {
              btkErrorMacro("No FORCE_PLATFORM::CAL_MATRIX entry. Force platform which requires a calibration matrix to convert volts in newtons won't be scaled.");
              break;
            }
          }
        }
        else
        {
          pCalMatrix = (*itCalMatrix);
          calMatrixStep = pCalMatrix->GetInfo()->GetDimension(0) * pCalMatrix->GetInfo()->GetDimension(1);
        }
        
        AnalogCollection::Pointer analogs = input->GetAnalogs();
        std::vector<int> channelsIndex;
        (*itChannels)->GetInfo()->ToInt(channelsIndex);
        
        // Check if the content of the input used by this filter changed since the last update
        if (static_cast<int>(numberOfForcePlatforms) == output->GetItemNumber())
        {
          int numAnalogs = analogs->GetItemNumber();
          bool dataModified = false;
          for (size_t i = 0 ; i < channelsIndex.size() ; ++i)
          {
            if ((channelsIndex[i] < 1) || (channelsIndex[i] > numAnalogs))
              continue;
            AnalogCollection::ConstIterator it = analogs->Begin();
            std::advance(it, channelsIndex[i]-1);
            if ((*it)->GetTimestamp() >= this->GetTimestamp())
            {
              dataModified = true;
              break;
            }
          }
          if (!dataModified && ((*itForcePlatformGr)->GetTimestamp() < this->GetTimestamp()))
            return;
        }
        
        output->Clear();
        output->SetItemNumber(static_cast<int>(numberOfForcePlatforms));
        ForcePlatformCollection::Iterator itFP = output->Begin();
        int channelStep = (*itChannels)->GetInfo()->GetDimension(0);
        int channelNumberAlreadyExtracted = 0;
        int calMatrixCoefficentNumberAleadyExtracted = 0;
        bool noError = true;

        for(size_t i = 0 ; i < numberOfForcePlatforms ; ++i)
        {
          switch(types[i])
          {
          case 1:
            (*itFP) = ForcePlatformType1::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            noError = this->ExtractForcePlatformData((*itFP), analogs, channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 2:
            (*itFP) = ForcePlatformType2::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            noError = this->ExtractForcePlatformData((*itFP), analogs, channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 3:
            (*itFP) = ForcePlatformType3::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            noError = this->ExtractForcePlatformData((*itFP), analogs, channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 4:
            (*itFP) = ForcePlatformType4::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            noError = this->ExtractForcePlatformDataWithCalibrationMatrix((*itFP), analogs, channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 5:
            (*itFP) = ForcePlatformType5::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            noError = this->ExtractForcePlatformDataWithCalibrationMatrix((*itFP), analogs, channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 6:
            {
            btkErrorMacro("Force Platform type 6 is not yet supported. Please, report this to the developers");
            //ForcePlatformType6::Pointer fp6 = ForcePlatformType6::New();
            //this->ExtractForcePlatformData(fp6, analogs, (*itForcePlatformGr));
            //(*itFP) = fp6;
            }
            break;
          case 7:
            {
            btkErrorMacro("Force Platform type 7 is not yet supported. Please, report this to the developers");
            //ForcePlatformType7::Pointer fp7 = ForcePlatformType7::New();
            //this->ExtractForcePlatformData(fp7, analogs, (*itForcePlatformGr));
            //(*itFP) = fp7;
            }
            break;
          case 11:
            {
            btkErrorMacro("Force Platform type 11 is not yet supported. Please, report this to the developers");
            //ForcePlatformType11::Pointer fp11 = ForcePlatformType11::New();
            //this->ExtractForcePlatformData(fp11, analogs, (*itForcePlatformGr));
            //(*itFP) = fp11;
            }
            break;
          case 12:
            {
            btkErrorMacro("Force Platform type 12 is not yet supported. Please, report this to the developers");
            //ForcePlatformType12::Pointer fp12 = ForcePlatformType12::New();
            //this->ExtractForcePlatformData(fp12, analogs, (*itForcePlatformGr));
            //(*itFP) = fp12;
            }
            break;
          case 21:
            {
            btkErrorMacro("Force Platform type 21 is not yet supported. Please, report this to the developers");
            //ForcePlatformType21::Pointer fp21 = ForcePlatformType21::New();
            //this->ExtractForcePlatformData(fp21, analogs, (*itForcePlatformGr));
            //(*itFP) = fp21;
            }
            break;
          default:
            btkErrorMacro("Unsupported force platform type. Impossible to extract corresponding data");
            break;
          }
          // Fill empty force platform channel if necessary
          if (!noError)
          {
            btkErrorMacro("Error(s) occurred during channel extraction for force platform #" + ToString(i + 1) + ". Replacement by vector of zeros.")
            int inc = 0;
            for (ForcePlatform::Iterator it = (*itFP)->Begin() ; it != (*itFP)->End() ; ++it )
            {
              (*it) = Analog::New("FP" + ToString(i + 1) + "C" + ToString(inc + 1), input->GetAnalogFrameNumber());
              ++inc;
            } 

          }
          ++itFP;
          calMatrixCoefficentNumberAleadyExtracted += calMatrixStep;
          channelNumberAlreadyExtracted += channelStep;
        }
      }
    }
  };
  
  void ForcePlatformsExtractor::ExtractForcePlatformDataCommon(ForcePlatform::Pointer fp, size_t idx, int coefficientsAlreadyExtracted, MetaData::Pointer pOrigin, MetaData::Pointer pCorners, MetaData::Pointer pCalMatrix)
  {
    MetaDataInfo::Pointer pValue;
    if (pOrigin.get() != 0)
    {
      pValue = pOrigin->GetInfo();
      if (pValue->GetValues().size() >= 3 * (idx + 1))
      {
        fp->SetOrigin(pValue->ToDouble(3 * static_cast<int>(idx)),
                      pValue->ToDouble(3 * static_cast<int>(idx) + 1),
                      pValue->ToDouble(3 * static_cast<int>(idx) + 2));
      }
    }
    if (pCorners.get() != 0)
    {
      pValue = pCorners->GetInfo();
      if (pValue->GetValues().size() >= 12 * (idx + 1))
      {
        for (int i = 0 ; i < 4 ; ++i)
          for (int j = 0 ; j < 3 ; ++j)
            fp->SetCorner(j, i, pValue->ToDouble(j + i * 3 + static_cast<int>(idx) * 12));
      }
    }
    if (pCalMatrix.get() != 0)
    {
      pValue = pCalMatrix->GetInfo();
      ForcePlatform::CalMatrix cal = fp->GetCalMatrix();
      if (static_cast<int>(pValue->GetValues().size()) >= (coefficientsAlreadyExtracted + cal.size()))
      {
        for (int i = 0 ; i < cal.cols() ; ++i)
          for (int j = 0 ; j < cal.rows() ; ++j)
            cal.coeffRef(j,i) = pValue->ToDouble(j + i * cal.rows() + coefficientsAlreadyExtracted);
        fp->SetCalMatrix(cal);
      }
    }
    else if (fp->GetType() > 3)
      btkErrorMacro("No calibration matrix for force platform #" + ToString(idx) + ". Its data won't be scaled.");
  };

  /**
   *
   */
  bool ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatform::Pointer fp, AnalogCollection::Pointer channels, int alreadyExtracted, std::vector<int> channelsIndex)
  {
    int numberOfChannelToExtract = fp->GetChannelNumber();
    int numberOfChannels = channels->GetItemNumber();
    bool noError = this->CheckAnalogIndicesForForcePlatform(channelsIndex, alreadyExtracted, numberOfChannelToExtract, numberOfChannels);
    // Assignment
    if (noError)
    {
      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
        fp->SetChannel(i, channels->GetItem(channelsIndex[i + alreadyExtracted] - 1)->Clone());
    }
    return noError;
  };

  bool ForcePlatformsExtractor::ExtractForcePlatformDataWithCalibrationMatrix(ForcePlatform::Pointer fp, AnalogCollection::Pointer channels, int alreadyExtracted, std::vector<int> channelsIndex)
  {
    int numberOfChannelToExtract = fp->GetChannelNumber();
    int numberOfChannels = channels->GetItemNumber();
    bool noError = this->CheckAnalogIndicesForForcePlatform(channelsIndex, alreadyExtracted, numberOfChannelToExtract, numberOfChannels);
    // Assignment
    if (noError)
    {
      int numberOfFrame = channels->GetItem(0)->GetFrameNumber();
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> data = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(numberOfFrame, numberOfChannelToExtract);
      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
      {
        Analog::Pointer channel = Analog::New();
        Analog::Pointer channelToCopy = channels->GetItem(channelsIndex[i + alreadyExtracted] - 1);
        channel->SetLabel(channelToCopy->GetLabel());
        channel->SetDescription(channelToCopy->GetDescription());
        fp->SetChannel(i, channel);
        data.col(i) = channelToCopy->GetValues();
      }
      data *= fp->GetCalMatrix().transpose();

      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
        fp->GetChannel(i)->SetValues(data.col(i));
    }
    return noError;
  };
/*
  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType5::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr)
  {
  };

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType6::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr)
  {

  };
  */
/*
  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType7::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr)
  {};

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType11::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr)
  {};

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType12::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr)
  {};

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType21::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr)
  {};
  */
  
  bool ForcePlatformsExtractor::CheckAnalogIndicesForForcePlatform(std::vector<int> channelsIndex, int alreadyExtracted, int numberOfChannelToExtract, int numberOfChannels) const
  {
    // Test on the number of analog channels
    if ((numberOfChannelToExtract + alreadyExtracted) > static_cast<int>(channelsIndex.size()))
      return false;
    else
    {
      // Index check
      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
      {
        int index = channelsIndex[i + alreadyExtracted];
        if ((index < 1) || (index > numberOfChannels))
          return false;
      }
    }
    return true;
  };
};
