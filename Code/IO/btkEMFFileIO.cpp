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

#include "btkEMFFileIO.h"
#include "btkConvert.h"
#include "btkMetaDataUtils.h"
#include "btkLogger.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <list>
#include <vector>
#include <sstream>

namespace btk
{
  /**
   * @class EMFFileIOException btkEMFFileIO.h
   * @brief Exception class for the EMFFileIO class.
   */
  
  /**
   * @fn EMFFileIOException::EMFFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual EMFFileIOException::~EMFFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class EMFFileIO btkEMFFileIO.h
   * @brief Interface to read/write EMF files.
   *
   * The EMF file format is used in the software FusionCore from Ascension Technology Corporation.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef EMFFileIO::Pointer
   * Smart pointer associated with a EMFFileIO object.
   */
  
  /**
   * @typedef EMFFileIO::ConstPointer
   * Smart pointer associated with a const EMFFileIO object.
   */
  
  /**
   * @fn static EMFFileIO::Pointer EMFFileIO::New()
   * Create a EMFFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to EMF.
   */
  bool EMFFileIO::CanReadFile(const std::string& filename)
  {
    std::ifstream ifs(filename.c_str());
    char c[43] = {0};
    ifs.read(c, 42); c[42] = '\0';
    ifs.close();
    if (strcmp(c,"EMF1.0     ## HyperVision EMF ASCII Format") != 0)
      return false;
    return true;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void EMFFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    // Open the stream
    std::ifstream ifs;
    ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      std::string line;
      ifs.open(filename.c_str());
    // Check the first line: "EMF1.0     ## HyperVision EMF ASCII Format"
      std::getline(ifs, line);
     if (line.substr(0,42).compare("EMF1.0     ## HyperVision EMF ASCII Format") != 0)
        throw(EMFFileIOException("Invalid EMF file."));
    // Extract header: 
      const char* keywords[8] = {"Filename", "Frames", "Time", "StartTimeOffset", "OpticalMarkers", "FrameRate", "Date", "Nodes"};
      std::vector<std::string> values(8);
      for (int i = 0 ; i < 8 ; ++i)
      {
        std::getline(ifs, line); // CR
        std::getline(ifs, line);
        size_t len = strlen(keywords[i]);
        if (line.substr(1,len).compare(keywords[i]) != 0)
          throw(EMFFileIOException("Wrong header."));
        values[i] = line.substr(len+2);
      }
      // - Nodes' values
      std::getline(ifs, line);
      values[7] = line;
    // Configure the acquisition
      MetaData::Pointer trial = MetaDataCreateChild(output->GetMetaData(), "TRIAL");
      // - Number of frames
      int numFrames = FromString<int>(values[1]);
      // - Time
      std::vector<int16_t> time = std::vector<int16_t>(3);  // Hours, minutes and seconds, using a 24 hour clock.
      time[0] = FromString<int16_t>(values[2].substr(0,2)); // hours
      time[1] = FromString<int16_t>(values[2].substr(3,2)); // minutes
      time[2] = FromString<int16_t>(values[2].substr(6,2)); // seconds
      MetaDataCreateChild(trial, "TIME", time);
      // - StartTimeOffset
      int startTimeOffset = FromString<int16_t>(values[3].substr(0,2)) * 3600 * 1000 // Hours converted in ms
                            + FromString<int16_t>(values[3].substr(3,2)) * 60 * 1000 // Minutes converted in ms
                            + FromString<int16_t>(values[3].substr(6,2)) * 1000 // Minutes converted in ms
                            + FromString<int16_t>(values[3].substr(9));
      // - OpticalMarkers
      int numMarkers = FromString<int>(values[4]);
      // - FrameRate
      double frameRate = FromString<double>(values[5]);
      // - Date
      std::vector<int16_t> date = std::vector<int16_t>(3); // The date is converted from DD/MM/(YY)YY to (YY)YY/MM/DD (Western calendar system as proposed by the C3D format).
      date[2] = FromString<int16_t>(values[6].substr(0,2)); // day
      date[1] = FromString<int16_t>(values[6].substr(3,2)); // month
      date[0] = FromString<int16_t>(values[6].substr(6,2)); // year
      MetaDataCreateChild(trial, "DATE", date);
      // - Nodes
      btkTrimString(&(values[7]));
      size_t pos = values[7].find(' ');
      size_t oldPos = 0;
      std::list<std::string> labels;
      int numLabels = 0;
      while (1)
      {
        labels.push_back(values[7].substr(oldPos,pos-oldPos));
        ++numLabels;
        if (pos == std::string::npos)
          break;
        oldPos = pos+1;
        pos = values[7].find(' ', pos+1);
      }
      if (numLabels != numMarkers)
        btkWarningMacro(filename, "Number of nodes is not the same than the number of markers. The number of markers is kept.")
      output->Init(numMarkers,numFrames);
      output->SetFirstFrame(1 + static_cast<int>(startTimeOffset * frameRate / 1000.0));
      output->SetPointFrequency(frameRate);
      std::list<std::string>::const_iterator it = labels.begin();
      int inc = 0;
      while ((it != labels.end()) && (inc != numMarkers))
      {
        output->GetPoint(inc++)->SetLabel(*it);
        ++it;
      }
    // Extract data
      std::getline(ifs, line); // CR
      std::getline(ifs, line);
      if (line.substr(0,5).compare(":Data") != 0)
        throw(EMFFileIOException("Corrupted EMF file. No :Data section."));

      ifs.exceptions(std::ios_base::goodbit); // Remove exceptions
      int indexMarker;
      double x,y,z;
      for(int i = 0 ; i < numFrames ; ++i)
      {
        if (ifs.eof())
          throw(EMFFileIOException("Unexpected end of file."));
        
        std::getline(ifs, line); // :Sample frame_index timestamp
        for (PointCollection::Iterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
        {
          std::getline(ifs, line);
          std::istringstream iss(line, std::istringstream::in);
          iss >> indexMarker ; iss >> x; iss >> y; iss >> z;
          if ((x == 0.0) && (y == 0.0) && (z == 0.0)) // occlusion
          {
            (*it)->GetValues().coeffRef(i, 0) = 0.0;
            (*it)->GetValues().coeffRef(i, 1) = 0.0;
            (*it)->GetValues().coeffRef(i, 2) = 0.0;
            (*it)->GetResiduals().coeffRef(i) = -1.0;
          }
          else
          {
            (*it)->GetValues().coeffRef(i, 0) = x;
            (*it)->GetValues().coeffRef(i, 1) = y;
            (*it)->GetValues().coeffRef(i, 2) = z;
            // (*it)->GetResiduals().coeffRef(i) = 0.0; // Set to 0 by default
          }
        }
        std::getline(ifs, line); // CR
      }
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (ifs.eof())
        excmsg = "Unexpected end of file.";
      else if (!ifs.is_open())
        excmsg = "Invalid file path.";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the file stream.";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the file stream.";
      
      if (ifs.is_open()) ifs.close();    
      throw(EMFFileIOException(excmsg));
    }
    catch (EMFFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(EMFFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(EMFFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  EMFFileIO::EMFFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::ASCII)
  {};
};
