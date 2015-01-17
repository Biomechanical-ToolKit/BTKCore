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

#ifndef __btkMotionAnalysisFileIOUtils_p_h
#define __btkMotionAnalysisFileIOUtils_p_h

#include "btkAcquisition.h"
#include <vector>

namespace btk
{
  // For ANB/ANC files
  
  class ANxFileIOException : public Exception
  {
  public:
    explicit ANxFileIOException(const std::string& msg)
    : Exception(msg)
    {};

    virtual ~ANxFileIOException() throw() {};
  };
  
  void ANxFileIOCheckHeader_p(double preciseRate, size_t channelNumber, 
                            const std::vector<uint16_t>& channelRate, 
                            const std::vector<uint16_t>& channelRange);
  void ANxFileIOStoreHeader_p(Acquisition::Pointer output, const std::string& filename,
                            double preciseRate, size_t frameNumber, size_t channelNumber,
                            const std::vector<std::string>& channelLabel,
                            const std::vector<uint16_t>& channelRate,
                            const std::vector<uint16_t>& channelRange,
                            const std::string& boardType, int bitDepth, int gen = 2);
  void ANxFileIOExtractForcePlatformChannel_p(std::vector< std::vector<int16_t> >& fpChan, 
                            Acquisition::Pointer output, const char** labels, int num);
  void ANxFileIOExtractForcePlatformChannel_p(std::vector< std::vector<int16_t> >& fpChan, 
                            Acquisition::Pointer output, const std::string& prefix, const char** labels, int num);
  void ANxFileIOExtractForcePlatformChannel_p(std::vector< std::vector<int16_t> >& fpChan, 
                            Acquisition::Pointer output, const std::vector<std::string>& labels);
  int ANxFileIOFindAnalogLabeCaselInsensitive_p(const std::string& label, Acquisition::Pointer output);
  uint16_t ANxFileIODetectAnalogRange_p(double s, int bitDepth);
  inline double ANxFileIOComputeScaleFactor_p(int range, int bitDepth) {return 2.0 * static_cast<double>(range) / 1000.0 / pow(2.0, bitDepth);};
  uint16_t AnxFileIOExtractAnalogRangeFromGain_p(int idx, int gain, double scale, int bitDepth);
};

#endif // __btkMotionAnalysisFileIOUtils_p_h
