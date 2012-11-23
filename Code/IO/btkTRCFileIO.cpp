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

#include "btkTRCFileIO.h"
#include "btkConvert.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>

namespace btk
{
  /**
   * @class TRCFileIOException btkTRCFileIO.h
   * @brief Exception class for the TRCFileIO class.
   */
  
  /**
   * @fn TRCFileIOException::TRCFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual TRCFileIOException::~TRCFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class TRCFileIO btkTRCFileIO.h
   * @brief Interface to read/write TRC files.
   *
   * The TRC file format is created by Motion Analysis Corp.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef TRCFileIO::Pointer
   * Smart pointer associated with a TRCFileIO object.
   */
  
  /**
   * @typedef TRCFileIO::ConstPointer
   * Smart pointer associated with a const TRCFileIO object.
   */
  
  /**
   * @fn static TRCFileIO::Pointer TRCFileIO::New()
   * Create a TRCFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first word in the file corresponds to "PathFileType".
   */
  bool TRCFileIO::CanReadFile(const std::string& filename)
  {
    std::ifstream ifs(filename.c_str());
    char c[13] = {0};
    ifs.read(c, 12); c[12] = '\0';
    ifs.close();
    if (strcmp(c,"PathFileType") != 0)
      return false;
    return true;
  };
  
  /**
   * Checks if the suffix of @a filename is TRC.
   */
  bool TRCFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type TRCPos = lowercase.rfind(".trc");
    if ((TRCPos != std::string::npos) && (TRCPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void TRCFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    // Open the stream
    std::ifstream ifs;
    ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      std::string line;
      ifs.open(filename.c_str());
    // Check the first header keyword: "PathFileType"
      std::getline(ifs, line);
      if (line.substr(0,12).compare("PathFileType") != 0)
        throw(TRCFileIOException("Invalid TRC file."));
    // Extract header data
      // Required TRC keywords : DataRate, NumFrames, NumMarkers, Units, OrigDataStartFrame
      std::map<std::string, std::string> keywords;
      std::string k, v;
      std::getline(ifs, k); // keywords
      std::getline(ifs, v); // corresponding values
      size_t kf2 = -1, kf1 = 0, vf2 = -1, vf1 = 0;
      char sep = '\t';
      while(1)
      {
        kf2 = k.find(sep, kf2 + 1);
        vf2 = v.find(sep, vf2 + 1);
        if (((kf2 == std::string::npos) && (vf2 != std::string::npos))
            || ((kf2 != std::string::npos) && (vf2 == std::string::npos)))
          throw(TRCFileIOException("Error between TRC header keywords and values."));
        if ((kf2 == std::string::npos) && (vf2 == std::string::npos))
          break;
        keywords.insert(std::make_pair(k.substr(kf1, kf2 - kf1), v.substr(vf1, vf2 - vf1)));
        kf1 = kf2 + 1;
        vf1 = vf2 + 1;
      }
      std::string num;
      int numberOfPoints = 0;
      if (!(num = keywords["NumMarkers"]).empty())
        numberOfPoints = FromString<int>(num);
      int numberOfFrames = 0;
      if (!(num = keywords["NumFrames"]).empty())
        numberOfFrames = FromString<int>(num);
      if (!(num = keywords["DataRate"]).empty())
        output->SetPointFrequency(FromString<double>(num));
      if (!(num = keywords["OrigDataStartFrame"]).empty())
        output->SetFirstFrame(FromString<int>(num));
      if (!(num = keywords["Units"]).empty())
        output->SetPointUnit(num);
      else
      {
        btkIOErrorMacro(filename, "No 'Units' keyword. Default unit is millimeter (mm)");
        output->SetPointUnit("mm");
      }
      if (numberOfPoints != 0)
      {
        std::getline(ifs, line);
        std::istringstream iss(line.substr(0, line.length() - 1), std::istringstream::in); // No need of the carriage return character
        std::string buf;
        std::list<std::string> labels;
        iss >> buf; // Frame#
        iss >> buf; // Time
        while (iss)
        {
          std::getline(iss, buf, '\t');
          if (!buf.empty())
          {
            buf = buf.erase(buf.find_last_not_of(' ') + 1);
            buf = buf.erase(0, buf.find_first_not_of(' '));
            labels.push_back(buf);
          }
        }
        int numberOfLabels = static_cast<int>(labels.size());
        if (numberOfPoints != numberOfLabels)
        {
          btkIOErrorMacro(filename, "Mismatch between the number of points and the number of labels extracted. Final number of points corresponds to the number of labels extracted.");
          numberOfPoints = numberOfLabels;
        }
        std::getline(ifs, line); // Coordinate's label (X1, Y1, Z1, ...)
        output->Init(numberOfPoints, numberOfFrames);
        std::list<std::string>::const_iterator itLabel = labels.begin();
        for (PointCollection::Iterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
        {
          (*it)->SetLabel(*itLabel);
          ++itLabel;
        }
        ifs.exceptions(std::ios_base::goodbit); // Remove exceptions
        for(int i = 0 ; i < numberOfFrames ; ++i)
        {
          if (ifs.eof())
            throw(TRCFileIOException("Unexpected end of file."));
          
          ifs >> buf; // Frame#
          ifs >> buf; // Time
          std::getline(ifs, line);
          line[0] = ' '; // was '\t'
          line[line.length() - 1] = '\t'; // was '\n'
          std::istringstream iss2(line, std::istringstream::in);
          for (PointCollection::Iterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
          {
            std::string x="", y="", z="";
            std::getline(iss2, x, '\t');
            std::getline(iss2, y, '\t');
            std::getline(iss2, z, '\t');
            
            if (x.empty() || y.empty() || z.empty()) // occlusion
            {
              (*it)->GetValues().coeffRef(i, 0) = 0.0;
              (*it)->GetValues().coeffRef(i, 1) = 0.0;
              (*it)->GetValues().coeffRef(i, 2) = 0.0;
              (*it)->GetResiduals().coeffRef(i) = -1.0;
            }
            else
            {
              FromString(x, (*it)->GetValues().coeffRef(i, 0));
              FromString(y, (*it)->GetValues().coeffRef(i, 1));
              FromString(z, (*it)->GetValues().coeffRef(i, 2));
              (*it)->GetResiduals().coeffRef(i) = 0.0;
            }
          }
        }
      }
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (!ifs.is_open())
        excmsg = "Invalid file path.";
      else if (ifs.eof())
        excmsg = "Unexpected end of file.";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the filestream.";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the filestream.";
      
      if (ifs.is_open()) ifs.close();    
      throw(TRCFileIOException(excmsg));
    }
    catch (TRCFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(TRCFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(TRCFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void TRCFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    if (input.get() == 0)
    {
      btkIOErrorMacro(filename, "Empty input. Impossible to write an empty file.");
      return;
    }
    std::ofstream ofs(filename.c_str());
    if (!ofs) 
      throw(TRCFileIOException("Invalid file path."));
    PointCollection::Pointer markers = PointCollection::New();
    for (PointCollection::ConstIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
    {
      Point::Pointer pt = *it;
      if (pt->GetType() != Point::Marker)
        continue;
      else if (pt->GetFrameNumber() != input->GetPointFrameNumber())
      {
        btkIOErrorMacro(filename, "Marker '" + pt->GetLabel() + "' was cloned and its number of frames is resized to the general number of frames.");
        pt = (*it)->Clone();
        pt->SetFrameNumber(input->GetPointFrameNumber());
      }
      markers->InsertItem(pt);
    }
    double freq = 100.0;
    if (input->GetPointFrequency() != 0)
       freq = input->GetPointFrequency();
    else
      btkIOErrorMacro(filename, "Points' frequency is not set. Default frequency is set to 100Hz");
    double stepTime = 1.0 / freq;
    ofs << static_cast<std::string>("PathFileType\t4\t(X/Y/Z)\t") << btkStripPathMacro(filename.c_str());
    ofs << static_cast<std::string>("\t\nDataRate\tCameraRate\tNumFrames\tNumMarkers\tUnits\tOrigDataRate\tOrigDataStartFrame\tOrigNumFrames\t\n");
    ofs.setf(std::ios::fixed, std::ios::floatfield);
    ofs.precision(2);
    ofs << input->GetPointFrequency() /* DataRate */ << "\t"
        << input->GetPointFrequency() /* CameraRate */ << "\t"
        << input->GetPointFrameNumber() /* NumFrames */ << "\t"
        << markers->GetItemNumber() /* NumMarkers */ << "\t"
        << input->GetPointUnit() /* Units */ << "\t" 
        << freq /* OrigDataRate */ << "\t"
        << input->GetFirstFrame() /* OrigDataStartFrame */ << "\t"
        << input->GetPointFrameNumber() /* OrigNumFrames */<< "\t\n";
    ofs << "Frame#\tTime\t";
    for (PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
      ofs << (*it)->GetLabel() << "\t\t\t";
    ofs << "\n\t\t";
    int idx = 1;
    for (PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
    {
      ofs << "X" << idx << "\t" << "Y" << idx << "\t" << "Z" << idx << "\t";
      ++idx;
    }
    ofs << "\n";
    double time = 0.0;
    for (int frame = 0 ; frame < input->GetPointFrameNumber() ; ++frame)
    {
      ofs.precision(3);
      ofs << std::endl << frame + 1 << "\t" << time;
      ofs.precision(5);

      for (PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
      {
        if ((*it)->GetValues().row(frame).isZero() && ((*it)->GetResiduals().coeff(frame) == -1))
          ofs << "\t\t\t";
        else
           ofs << "\t" << (*it)->GetValues().coeff(frame, 0)
               << "\t" << (*it)->GetValues().coeff(frame, 1)
               << "\t" << (*it)->GetValues().coeff(frame, 2);
      };
      ofs << " ";
      time += stepTime;
    };
    ofs << std::endl;
    ofs.close();
  };
  
  /**
   * Constructor.
   */
  TRCFileIO::TRCFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::ASCII)
  {};
};
