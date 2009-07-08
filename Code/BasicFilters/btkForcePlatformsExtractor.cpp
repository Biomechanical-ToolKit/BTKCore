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

#include "btkForcePlatformsExtractor.h"
#include "btkMetaDataEntry.h"
#include "btkConvert.h"

namespace btk
{
 /**
   * @class ForcePlatformsExtractor
   * @brief Extracts force platform data from a btk::Acquisition object. 
   *
   * Analog channels' data are transformed (if required) to express force and moment at the origin of the force plateform in its frame.
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
   * Creates a PointCollection:Pointer object and return it as a DataObject::Pointer.
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
    output->Clear(); 
    Acquisition::Pointer input = this->GetInput();
    if (input.get() != 0)
    {
      MetaDataEntry::Iterator itForcePlatformGr = input->GetMetaData()->FindChild("FORCE_PLATFORM");
      if (itForcePlatformGr != input->GetMetaData()->End())
      {
        MetaDataEntry::Iterator itUsed = (*itForcePlatformGr)->FindChild("USED");
        int numberOfForcePlatforms = 0;
        if (itUsed != (*itForcePlatformGr)->End())
          FromString((*itUsed)->GetMetaDataEntryValue()->GetValues().front(), numberOfForcePlatforms);

        MetaDataEntry::Iterator itType = (*itForcePlatformGr)->FindChild("TYPE");
        std::vector<int> types = std::vector<int>(numberOfForcePlatforms, 1);
        if (itType != (*itForcePlatformGr)->End())
        {
          FromString((*itType)->GetMetaDataEntryValue()->GetValues(), types);
          if (static_cast<int>(types.size()) < numberOfForcePlatforms)
          {
            btkErrorMacro("FORCE_PLATFORM:USED and FORCE_PLATFORM:TYPE don't indicate the same number of force platforms. The lower is kept: FORCE_PLATFORM:TYPE");
            numberOfForcePlatforms = types.size();
          }
          else if (static_cast<int>(types.size()) > numberOfForcePlatforms)
          {
            btkErrorMacro("FORCE_PLATFORM:USED and FORCE_PLATFORM:TYPE don't indicate the same number of force platforms. The lower is kept: FORCE_PLATFORM:USED");
          }
        }

        MetaDataEntry::Iterator itChannels = (*itForcePlatformGr)->FindChild("CHANNEL");
        if (itChannels == (*itForcePlatformGr)->End())
        {
          btkErrorMacro("No FORCE_PLATFORM::CHANNEL entry. Impossible to extract analog channels associated with the force plateform(s). Force plateforms' data are empty.");
          return;
        }

        output->SetItemNumber(numberOfForcePlatforms);

        MetaDataEntry::Pointer pOrigin;
        MetaDataEntry::Iterator itOrigin = (*itForcePlatformGr)->FindChild("ORIGIN");
        if (itOrigin == (*itForcePlatformGr)->End())
        {
          btkErrorMacro("No FORCE_PLATFORM::ORIGIN entry. Default values are used.");
        }
        else
          pOrigin = (*itOrigin);
        
        MetaDataEntry::Pointer pCorners;
        MetaDataEntry::Iterator itCorners = (*itForcePlatformGr)->FindChild("CORNERS");
        if (itCorners == (*itForcePlatformGr)->End())
        {
          btkErrorMacro("No FORCE_PLATFORM::CORNERS entry. Default values are used.");
        }
        else
          pCorners = (*itCorners);
        MetaDataEntry::Pointer pCalMatrix;
        MetaDataEntry::Iterator itCalMatrix = (*itForcePlatformGr)->FindChild("CAL_MATRIX");
        if (itCalMatrix == (*itForcePlatformGr)->End())
        {
          for(int i = 0 ; i < numberOfForcePlatforms ; ++i)
          {
            if (types[i] > 3)
            {
              btkErrorMacro("No FORCE_PLATFORM::CAL_MATRIX entry. Force platform which requires a calibration matrix to convert volts in newtons won't be scaled.");
              break;
            }
          }
        }
        else
          pCalMatrix = (*itCalMatrix);

        ForcePlatformCollection::Iterator itFP = output->Begin();
        AnalogCollection::Pointer analogs = input->GetAnalogs();
        std::vector<int> channelsIndex;
        FromString((*itChannels)->GetMetaDataEntryValue()->GetValues(), channelsIndex);
        int channelNumberAlreadyExtracted = 0;
        int calMatrixCoefficentNumberAleadyExtracted = 0;
        bool noError = true;

        for(int i = 0 ; i < numberOfForcePlatforms ; ++i)
        {
          switch(types[i])
          {
          case 1:
            (*itFP) = ForcePlatformType1::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, &calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            //calMatrixCoefficentNumberAleadyExtracted *= (6 * 6);
            noError = this->ExtractForcePlatformData((*itFP), analogs, &channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 2:
            (*itFP) = ForcePlatformType2::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, &calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            //calMatrixCoefficentNumberAleadyExtracted *= (6 * 6);
            noError = this->ExtractForcePlatformData((*itFP), analogs, &channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 3:
            (*itFP) = ForcePlatformType3::New();
            this->ExtractForcePlatformDataCommon((*itFP), i, &calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            //calMatrixCoefficentNumberAleadyExtracted *= (8 * 8);
            noError = this->ExtractForcePlatformData((*itFP), analogs, &channelNumberAlreadyExtracted, channelsIndex);
            break;
          case 4:
            {
            ForcePlatformType4::Pointer fp4 = ForcePlatformType4::New();
            this->ExtractForcePlatformDataCommon(fp4, i, &calMatrixCoefficentNumberAleadyExtracted, pOrigin, pCorners, pCalMatrix);
            //ForcePlatformType4::CalMatrix cal = fp4->GetCalMatrix();
            //fp4->SetCalMatrix(cal);
            //this->ExtractForcePlatformDataCalibrationMatrix(fp4, i, &calMatrixCoefficentNumberAleadyExtracted, (*itCalMatrix));
            noError = this->ExtractForcePlatformData(fp4, analogs, &channelNumberAlreadyExtracted, channelsIndex);
            (*itFP) = fp4;
            }
            break;
          case 5:
            {
            btkErrorMacro("Force Platform type 5 is not yet supported. Please, report this to the developers");
            //ForcePlatformType5::Pointer fp5 = ForcePlatformType5::New();
            //this->ExtractForcePlatformData(fp5, analogs, (*itForcePlatformGr));
            //(*itFP) = fp5;
            }
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
          // Fill emty force platform channel if necessary
          if (!noError)
          {
            btkErrorMacro("Error(s) occured during channel extraction for force platform #" + ToString(i + 1) + ". Replacement by vector of zeros.")
            int inc = 0;
            for (ForcePlatform::Iterator it = (*itFP)->Begin() ; it != (*itFP)->End() ; ++it )
            {
              (*it) = Analog::New("FP" + ToString(i + 1) + "C" + ToString(inc + 1), input->GetAnalogFrameNumber());
              ++inc;
            } 

          }
          ++itFP;
        }
      }
    }
  };
  
  void ForcePlatformsExtractor::ExtractForcePlatformDataCommon(ForcePlatform::Pointer fp, int idx, int* coefficientsAlreadyExtracted, MetaDataEntry::Pointer pOrigin, MetaDataEntry::Pointer pCorners, MetaDataEntry::Pointer pCalMatrix)
  {
    MetaDataEntryValue::Pointer pValue;
    if (pOrigin.get() != 0)
    {
      pValue = pOrigin->GetMetaDataEntryValue();
      if (static_cast<int>(pValue->GetValues().size()) >= 3 * (idx + 1))
      {
        fp->SetOrigin(FromString<double>(pValue->GetValue(3 * idx)),
                      FromString<double>(pValue->GetValue(3 * idx + 1)),
                      FromString<double>(pValue->GetValue(3 * idx + 2)));
      }
    }
    if (pCorners.get() != 0)
    {
      pValue = pCorners->GetMetaDataEntryValue();
      if (static_cast<int>(pValue->GetValues().size()) >= 12 * (idx + 1))
      {
        for (int i = 0 ; i < 4 ; ++i)
          for (int j = 0 ; j < 3 ; ++j)
            fp->SetCorner(j, i, FromString<double>(pValue->GetValue(j + i * 3 + idx * 12)));
      }
    }
    if (pCalMatrix.get() != 0)
    {
      pValue = pCalMatrix->GetMetaDataEntryValue();
      ForcePlatform::CalMatrix cal = fp->GetCalMatrix();
      if (static_cast<int>(pValue->GetValues().size()) >= (*coefficientsAlreadyExtracted + cal.size()))
      {
        for (int i = 0 ; i < cal.cols() ; ++i)
          for (int j = 0 ; j < cal.rows() ; ++j)
            cal.coeffRef(j,i) = FromString<double>(pValue->GetValue(j + i * cal.rows() + *coefficientsAlreadyExtracted));
        fp->SetCalMatrix(cal);
      }
      (*coefficientsAlreadyExtracted) += cal.size();
    }
    else if (fp->GetType() > 3)
      btkErrorMacro("No calibration matrix for force platform #" + ToString(idx) + ". Its data won't be scaled.");
  };

  /**
   *
   */
  bool ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatform::Pointer fp, AnalogCollection::Pointer channels, int* alreadyExtracted, std::vector<int> channelsIndex)
  {
    int numberOfChannelToExtract = fp->GetChannelNumber();
    int numberOfChannels = channels->GetItemNumber();
    bool noError = true;
    // Test on the number of analog channels
    if ((numberOfChannelToExtract + *alreadyExtracted) > numberOfChannels)
      noError = false;
    else if ((numberOfChannelToExtract + *alreadyExtracted) > static_cast<int>(channelsIndex.size()))
      noError = false;
    else
    {
      // Index check
      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
      {
        if (channelsIndex[i + *alreadyExtracted] > numberOfChannels)
        {
          noError = false;
          break;
        }
      }
    }
    // Assignment
    if (noError)
    {
      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
        fp->SetChannel(i, channels->GetItem(channelsIndex[i + *alreadyExtracted] - 1)->Clone());
    }
    (*alreadyExtracted) += fp->GetChannelNumber();

    return noError;
  };

  bool ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType4::Pointer fp, AnalogCollection::Pointer channels, int* alreadyExtracted, std::vector<int> channelsIndex)
  {
    int numberOfChannelToExtract = fp->GetChannelNumber();
    bool noError = true;
    if ((numberOfChannelToExtract + *alreadyExtracted) > static_cast<int>(channelsIndex.size()))
     noError = false;
    if (noError)
    {
      int numberOfFrame = channels->GetItem(0)->GetFrameNumber();
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> data = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(numberOfFrame, numberOfChannelToExtract);
      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
      {
        Analog::Pointer channel = Analog::New();
        Analog::Pointer channelToCopy = channels->GetItem(channelsIndex[i + *alreadyExtracted] - 1);
        channel->SetLabel(channelToCopy->GetLabel());
        channel->SetDescription(channelToCopy->GetDescription());
        fp->SetChannel(i, channel);
        data.col(i) = channelToCopy->GetValues();
      }
      data *= fp->GetCalMatrix().transpose();

      for (int i = 0 ; i < numberOfChannelToExtract ; ++i)
        fp->GetChannel(i)->SetValues(data.col(i));
    }

    (*alreadyExtracted) += fp->GetChannelNumber();
    return noError;
  };
/*
  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType5::Pointer fp, AnalogCollection::Pointer channels, MetaDataEntry::Pointer fpGr)
  {
  };

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType6::Pointer fp, AnalogCollection::Pointer channels, MetaDataEntry::Pointer fpGr)
  {

  };
  */
/*
  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType7::Pointer fp, AnalogCollection::Pointer channels, MetaDataEntry::Pointer fpGr)
  {};

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType11::Pointer fp, AnalogCollection::Pointer channels, MetaDataEntry::Pointer fpGr)
  {};

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType12::Pointer fp, AnalogCollection::Pointer channels, MetaDataEntry::Pointer fpGr)
  {};

  void ForcePlatformsExtractor::ExtractForcePlatformData(ForcePlatformType21::Pointer fp, AnalogCollection::Pointer channels, MetaDataEntry::Pointer fpGr)
  {};
  */
};
