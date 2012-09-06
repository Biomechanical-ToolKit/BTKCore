/** 
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

#include "btkMotionAnalysisFileIOUtils_p.h"
#include "btkMetaDataUtils.h"

#include <algorithm>
#include <cctype>

namespace btk
{  
  /**
   * @class ANxFileIOException
   * @brief Exception class for the ANxFileIO class.
   */

  /**
   * @fn ANxFileIOException::ANxFileIOException(const std::string& msg)
   * Constructor.
   */

  /**
   * @fn virtual ANxFileIOException::~ANxFileIOException()
   * Empty destructor.
   */
  
  // -------------------------------------------------------------------- // 
  //                        ANB/ANC util functions                        //
  // -------------------------------------------------------------------- //
   
  /**
   * Check the header's informations for the (ANB|ANC)FileIO reader.
   */
  void ANxFileIOCheckHeader_p(double preciseRate, size_t channelNumber, 
                            const std::vector<uint16_t>& channelRate, 
                            const std::vector<uint16_t>& channelRange)
  {
    // Analog channels' rate
    if (channelNumber > channelRate.size())
      throw(ANxFileIOException("Incorrect number of analog rates."));
    // Analog channels' range
    if (channelNumber > channelRange.size())
      throw(ANxFileIOException("Incorrect number of analog ranges."));
    // Check that all analog channels' rate are the same. Only equal rates are supported for the moment.
    for (std::vector<uint16_t>::const_iterator it = channelRate.begin() ; it != channelRate.end() ; ++it)
    {
      if (fabs(preciseRate - static_cast<double>(*it)) > 0.1)
        throw(ANxFileIOException("Only analog channels with equal rates are supported for the moment. Please, send an email to the developers to explain the problem."));
    }
  };
  
  /**
   * Store the header's informations for the (ANB|ANC)FileIO reader into a btk::Acquisition.
   */
  void ANxFileIOStoreHeader_p(Acquisition::Pointer output,
                              double preciseRate, size_t frameNumber, size_t channelNumber, 
                              const std::vector<std::string>& channelLabel, 
                              const std::vector<uint16_t>& channelRate, 
                              const std::vector<uint16_t>& channelRange, 
                              const std::string& boardType, int bitDepth, int gen)
  {
    btkNotUsed(channelRate);
    output->Init(0, static_cast<int>(frameNumber), static_cast<int>(channelNumber));
    output->SetPointFrequency(preciseRate);
    Acquisition::MetaDataIterator itAnalog = output->GetMetaData()->FindChild("ANALOG");
    MetaData::Pointer analog;
    if (itAnalog == output->EndMetaData())
      analog = MetaDataCreateChild(output->GetMetaData(), "ANALOG");
    else
      analog = *itAnalog;
    MetaDataCreateChild(analog, "BOARD", boardType);
    switch(bitDepth)
    {
      case 8:
        output->SetAnalogResolution(Acquisition::Bit8);
        break;
      case 10:
        output->SetAnalogResolution(Acquisition::Bit10);
        break;
      case 12:
        output->SetAnalogResolution(Acquisition::Bit12);
        break;
      case 14:
        output->SetAnalogResolution(Acquisition::Bit14);
        break;
      case 16:
        output->SetAnalogResolution(Acquisition::Bit16);
        break;
      default:
        btkErrorMacro("Unknown bit depth resolution. Default value used: 12 bits.");
        output->SetAnalogResolution(Acquisition::Bit12);
        break;
    }
    int inc = 0;
    for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
    {
      (*it)->SetLabel(channelLabel[inc]);
      if (gen == 1)
        (*it)->SetScale(pow(2.0, bitDepth) / static_cast<double>(channelRange[inc]));
      else
      {
        switch(channelRange[inc]) // range is in mV
        {
          case 10000:
            (*it)->SetGain(Analog::PlusMinus10);
            break;
          case 5000:
            (*it)->SetGain(Analog::PlusMinus5);
            break;
          case 2500:
            (*it)->SetGain(Analog::PlusMinus2Dot5);
            break;
          case 1250:
            (*it)->SetGain(Analog::PlusMinus1Dot25);
            break;
          case 1000:
            (*it)->SetGain(Analog::PlusMinus1);
            break;
          case 500:
            (*it)->SetGain(Analog::PlusMinus0Dot5);
            break;
          case 250:
            (*it)->SetGain(Analog::PlusMinus0Dot25);
            break;
          case 100:
            (*it)->SetGain(Analog::PlusMinus0Dot1);
            break;
          case 50:
            (*it)->SetGain(Analog::PlusMinus0Dot05);
            break;
          default:
            btkErrorMacro("Unknown range: '" + channelLabel[inc] + "'. Default value used: +/- 10 V");
            (*it)->SetGain(Analog::PlusMinus10);
            break;
        }
        (*it)->SetScale(2.0 * static_cast<double>(channelRange[inc]) / 1000.0 / pow(2.0, bitDepth));
      }
      ++inc;
    }
    // Extract force platform channel.
    // WARNING: This part only supports some format. For example:
    //  - FX, FY, FZ, MX, MY, MZ
    //  - F<idx>X, F<idx>Y, F<idx>Z, M<idx>X, M<idx>Y, M<idx>Z
    //  - FX<idx>, FY<idx>, FZ<idx>, MX<idx>, MY<idx>, MZ<idx>
    //  - The prefix FP or FP<idx> can also be added.
    std::vector< std::vector<int16_t> > fpChan;
    //const char* labelsTypeI[] = {"FX", "FY", "FZ", "PX", "PY", "MZ"};
    const char* labelsTypeII[] = {"FX", "FY", "FZ", "MX", "MY", "MZ"};
    //const char* labelsTypeIII[] = {"FX12", "FX34", "FY14", "FY23", "FZ1", "FZ2", "FZ3", "FZ4"};
    // FP type II start as the initial CAL format is composed of this type of force platform
    // The scale adataption is realized in the next lines.
    ANxFileIOExtractForcePlatformChannel_p(fpChan, output, labelsTypeII, sizeof(labelsTypeII) / sizeof(char*));
    ANxFileIOExtractForcePlatformChannel_p(fpChan, output, "FP", labelsTypeII, sizeof(labelsTypeII) / sizeof(char*));
    //ANxFileIOExtractForcePlatformChannel_p(fpChan, output, labelsTypeI, sizeof(labelsTypeI) / sizeof(char*));
    //ANxFileIOExtractForcePlatformChannel_p(fpChan, output, "FP", labelsTypeI, sizeof(labelsTypeI) / sizeof(char*));
    //ANxFileIOExtractForcePlatformChannel_p(fpChan, output, "FP", labelsTypeIII, sizeof(labelsTypeIII) / sizeof(char*));
    
    if (fpChan.size() != 0)
    {  
      // Create the metadata BTK_PARTIAL_FP_CHAN
      btk::MetaData::Pointer partial = btk::MetaData::New("BTK_PARTIAL_FP_CHAN");
      output->GetMetaData()->AppendChild(partial);
      // - BTK_PARTIAL_FP_CHAN:CHANNEL
      size_t numFp = fpChan.size();
      size_t numMaxChannel = 0;
      for (std::vector< std::vector<int16_t> >::const_iterator it = fpChan.begin() ; it != fpChan.end() ; ++it)
      {
        if (numMaxChannel < it->size())
          numMaxChannel = it->size();
      }
      std::vector<int16_t> channel(numFp * numMaxChannel, -32768); // 65535: default value for unknown channel.
      for (size_t i = 0 ; i < numFp ; ++i)
      {
        for (size_t j = 0 ; j < fpChan[i].size() ; ++j)
          channel[i * numMaxChannel + j] = fpChan[i][j];
      }
      std::vector<uint8_t> dims(2, 0); dims[0] = static_cast<uint8_t>(numMaxChannel); dims[1] = static_cast<uint8_t>(numFp);
      partial->AppendChild(btk::MetaData::New("CHANNEL", dims, channel));
    }
  };
  
  /**
   * Extract force platform channels.
   */
  void ANxFileIOExtractForcePlatformChannel_p(std::vector< std::vector<int16_t> >& fpChan, Acquisition::Pointer output, const char** labels, int num)
  {
    if ((output->GetAnalogNumber() - static_cast<int>(fpChan.size())) < num)
      return;
    std::vector<std::string> labels2(num, "");
    std::string suffix = "";
    // Looking for label with suffix "",0,1,2,3,4,5,6,7,8,9
    for (int i = 0 ; i < 10 ; ++i)
    {
      for (int j = 0 ; j < num ; ++j)
        labels2[j] = std::string(labels[j]) + suffix;
      ANxFileIOExtractForcePlatformChannel_p(fpChan, output, labels2);
      suffix = ToString(i);
    }
    // Looking for label with index 0,1,2,3,4,5,6,7,8,9 after the first letter.
    for (int i = 0 ; i < 10 ; ++i)
    {
      suffix = ToString(i);
      for (int j = 0 ; j < num ; ++j)
      {
        labels2[j] = std::string(labels[j]);
        labels2[j].insert(1, suffix);
      }
      ANxFileIOExtractForcePlatformChannel_p(fpChan, output, labels2);
    }
  };
  
  /**
   * Extract force platform channels.
   */
  void ANxFileIOExtractForcePlatformChannel_p(std::vector< std::vector<int16_t> >& fpChan, Acquisition::Pointer output, const std::string& prefix, const char** labels, int num)
  {
    if ((output->GetAnalogNumber() - fpChan.size()) < 6)
      return;
    std::vector<std::string> labels2(num, "");
    std::string suffix = "";
    // Looking for label with prefix and a possible index between 0 and 9 (included).
    for (int i = 0 ; i < 10 ; ++i)
    {
      for (int j = 0 ; j < num ; ++j)
        labels2[j] =  prefix + suffix + std::string(labels[j]);
      ANxFileIOExtractForcePlatformChannel_p(fpChan, output, labels2);
      suffix = ToString(i);
    }
  };
  
  /**
   * Extract force platform channels.
   * Adapt also the scale of the force platform channels.
   */
  void ANxFileIOExtractForcePlatformChannel_p(std::vector< std::vector<int16_t> >& fpChan, Acquisition::Pointer output, const std::vector<std::string>& labels)
  {
    std::vector<int16_t> fp;
    for (size_t i = 0 ; i < labels.size() ; ++i)
    {
      int idx = ANxFileIOFindAnalogLabeCaselInsensitive_p(labels[i], output);
      if (idx <= output->GetAnalogNumber())
        fp.push_back(idx);
    }
    if (fp.size() == labels.size())
    {
      fpChan.push_back(fp);
      for (size_t i = 0 ; i < fp.size() ; ++i)
      {
        Analog::Pointer a = output->GetAnalog(fp[i]-1);
        if (output->GetAnalogResolution() == Acquisition::Bit16)
          a->SetScale(a->GetScale() * -1.0);
        else
          a->SetScale(a->GetScale() / 4000.0 / 10.0 * 1000000.0 * -1.0);
      }
    }
  };
  
  /**
   * Find analog channel lobel.
   */
  int ANxFileIOFindAnalogLabeCaselInsensitive_p(const std::string& label, Acquisition::Pointer output)
  {
    int idx = 1;
    for (Acquisition::AnalogConstIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
    {
      std::string l = (*it)->GetLabel();
      std::transform(l.begin(), l.end(), l.begin(), toupper);
      if (label.compare(l) == 0)
        break;
      ++idx;
    }
    return idx;
  };
  
  /**
   * Detect analog channel's range by its scale and the analog resolution used.
   */
  uint16_t ANxFileIODetectAnalogRange_p(double s, int bitDepth)
  {
    uint16_t gain = static_cast<uint16_t>(fabs(s) / 2.0 * 1000.0 * pow(2.0, static_cast<double>(bitDepth)));
    if (gain <= Analog::PlusMinus0Dot05)
      gain = Analog::PlusMinus0Dot05;
    else if (gain <= Analog::PlusMinus0Dot1)
      gain = Analog::PlusMinus0Dot1;
    else if (gain <= Analog::PlusMinus0Dot25)
      gain = Analog::PlusMinus0Dot25;
    else if (gain <= Analog::PlusMinus0Dot5)
      gain = Analog::PlusMinus0Dot5;
    else if (gain <= Analog::PlusMinus1)
      gain = Analog::PlusMinus1;
    else if (gain <= Analog::PlusMinus1Dot25)
      gain = Analog::PlusMinus1Dot25;
    else if (gain <= Analog::PlusMinus2Dot5)
      gain = Analog::PlusMinus2Dot5;
    else if (gain <= Analog::PlusMinus5)
      gain = Analog::PlusMinus5;
    else
      gain = Analog::PlusMinus10;;
    return gain;
  };
  
  uint16_t AnxFileIOExtractAnalogRangeFromGain_p(int idx, int gain, double scale, int bitDepth)
  {
    uint16_t range;
    switch(gain) // range is in mV
    {
      case Analog::PlusMinus10:
        range = 10000;
        break;
      case Analog::PlusMinus5:
        range = 5000;
        break;
      case Analog::PlusMinus2Dot5:
        range = 2500;
        break;
      case Analog::PlusMinus1Dot25:
        range = 1250;
        break;
      case Analog::PlusMinus1:
        range = 1000;
        break;
      case Analog::PlusMinus0Dot5:
        range = 500;
        break;
      case Analog::PlusMinus0Dot25:
        range = 250;
        break;
      case Analog::PlusMinus0Dot1:
        range = 100;
        break;
      case Analog::PlusMinus0Dot05:
        range = 50;
        break;
      case Analog::Unknown:
        range = ANxFileIODetectAnalogRange_p(scale, bitDepth);
        btkErrorMacro("Unknown gain for channel #" + ToString(idx+1) + ". Automatically replaced by +/- " + ToString(static_cast<double>(range) / 1000)  + " volts in the file. Could corrupt the data in the written file!");
        break;
    }
    return range;
  };
};
