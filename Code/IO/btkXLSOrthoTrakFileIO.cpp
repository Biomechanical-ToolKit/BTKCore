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

#include "btkXLSOrthoTrakFileIO.h"
#include "btkMetaDataUtils.h"
#include "btkConvert.h"
#include "btkLogger.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>

namespace btk
{
  /**
   * @class XLSOrthoTrakFileIOException btkXLSOrthoTrakFileIO.h
   * @brief Exception class for the XLSOrthoTrakFileIO class.
   */
  
  /**
   * @fn XLSOrthoTrakFileIOException::XLSOrthoTrakFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual XLSOrthoTrakFileIOException::~XLSOrthoTrakFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class XLSOrthoTrakFileIO btkXLSOrthoTrakFileIO.h
   * @brief Class to read XLSOrthoTrak files.
   *
   * The XLSOrthoTrak file format is created by Motion Analysis Corp.
   *
   * @warning The events stocked in XLS files are only set by their frame. It is not possible to determine the time associated with it due to the lack of information.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef XLSOrthoTrakFileIO::Pointer
   * Smart pointer associated with a XLSOrthoTrakFileIO object.
   */
  
  /**
   * @typedef XLSOrthoTrakFileIO::ConstPointer
   * Smart pointer associated with a const XLSOrthoTrakFileIO object.
   */
  
  /**
   * @fn static XLSOrthoTrakFileIO::Pointer XLSOrthoTrakFileIO::New()
   * Create a XLSOrthoTrakFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first word in the file corresponds to "PathFileType".
   */
  bool XLSOrthoTrakFileIO::CanReadFile(const std::string& filename)
  {
    std::ifstream ifs(filename.c_str());
    bool canBeRead = true;
    std::string line;
    std::getline(ifs, line);
    if (line.substr(0,8).compare("Version\t") != 0)
      canBeRead = false;
    std::getline(ifs, line);
    if (line.substr(0,15).compare("Starting Frame\t") != 0)
      canBeRead = false;
    ifs.close();
    return canBeRead;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void XLSOrthoTrakFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    double* values = 0;
    // Open the stream
    std::ifstream ifs;
    try
    {
      std::string line;
      ifs.open(filename.c_str());
    // Check the first header keyword: "Version"
      std::getline(ifs, line);
      if (line.substr(0,8).compare("Version\t") != 0)
        throw(XLSOrthoTrakFileIOException("Invalid XLS (OrthoTrak) file."));
    // Check the second line to be sure.
      std::getline(ifs, line);
      if (line.substr(0,15).compare("Starting Frame\t") != 0)
        throw(XLSOrthoTrakFileIOException("Invalid XLS (OrthoTrak) file."));
    // Extract data
      std::istringstream iss(line, std::istringstream::in);
      std::string buf;
      std::getline(iss, buf, '\t'); // Starting Frame
      iss >> buf; 
      output->SetFirstFrame(FromString<int>(buf)+1);
      
      // SPATIOTEMP metadata
      MetaData::Pointer spatiotemp = MetaDataCreateChild(output->GetMetaData(), "SPATIOTEMP");
      // Parameters
      // Avg Step Width
      std::getline(ifs, line);
      line = "Avg_Step_Width" + line.substr(20, std::string::npos);
      iss.str(line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // R_Velocity
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // R_Stride_Len
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // R_Cadence
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // L_Velocity
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // L_Stride_Len
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // L_Cadence
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // R_Support_Time
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // L_Support_Time
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // R_Non_Support
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // L_Non_Support
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // R_Step_Len
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // L_Step_Len
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss, 10.0);
      // R_Dbl_Support
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      // L_Dbl_Support
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendSpatiotemparalParameter(spatiotemp, &iss);
      
      // Events 
      // Only the frame index is set. There is no information on the acquisition frequency
      // RHS
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendEvent(output, &iss, "Foot Strike", "Right", 1);
      // LHS
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendEvent(output, &iss, "Foot Strike", "Left", 1);
      // RTO
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendEvent(output, &iss, "Foot Off", "Right", 2);
      // LTO
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->AppendEvent(output, &iss, "Foot Off", "Left", 2);
      
      // Determine the event detected from the force platforms' data
      // RHS FP
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->ExtractEventDetectionFlag(output, &iss, "Foot Strike", "Right");
      // LHS FP
      std::getline(ifs, line); this->ConvertCRLFToCF(&iss, line);
      this->ExtractEventDetectionFlag(output, &iss, "Foot Strike", "Left");
      
      // Extract labels
      std::list<std::string> labels;
      std::getline(ifs, line);
      iss.str(line); iss.clear();
      int colNumber = 0;
      while (1)
      {
        std::getline(iss, buf, '\t');
        if (!iss.eof())
        {
          labels.push_back(buf);
          ++colNumber;
        }
        else
          break;
      }
      
      // Extract values
      int frameNumber = 0;
      std::streampos pos = ifs.tellg();
      while (1)
      {
        std::getline(ifs, line);
        if (ifs.eof() || line.empty())
          break;
        ++frameNumber;
      }
      ifs.clear();
      ifs.seekg(pos, std::ios::beg);
      int valueNumber = colNumber * frameNumber;
      double* values = new double[valueNumber];
      std::string strVal;
      int i = 0;
      for (i = 0 ; i < frameNumber ; ++i)
      {
        std::getline(ifs, line);
        if (ifs.eof())
        {
          btkWarningMacro(filename, "File corrupted. There is not enough values to fill data. Others values will be set to 0.");
          break;
        }
        iss.str(line); iss.clear();
        for (int j = 0 ; j < colNumber ; ++j)
          iss >> values[i*colNumber+j];
      }
      i *= colNumber;
      for (int j = i ; j < valueNumber ; ++j)
        values[j] = 0.0;
        
      output->Init(0, frameNumber);
      
      int inc = 0;
      for (std::list<std::string>::const_iterator itl = labels.begin() ; itl != labels.end() ; ++itl)
      {
        std::string label = *itl;
        std::string strtype = label.substr(label.length() - 3, std::string::npos);
        std::string suffix = "";
        Point::Type type = Point::Scalar;
        // Type
        if (strtype.compare("ANG") == 0)
        {
          type = Point::Angle;
          suffix = "_ANGLE";
        }
        else if (strtype.compare("FRC") == 0)
        {
          type = Point::Force;
          suffix = "_FORCE";
        }
        else if (strtype.compare("MOM") == 0)
        {
          type = Point::Moment;
          suffix = "_MOMENT";
        }
        else if (strtype.compare("PWR") == 0)
        {
          type = Point::Power;
          suffix = "_POWER";
        }
        // axis and label
        int axis = 0;
        if (type != Point::Scalar)
        {
          bool hasAxis = true;
          label = label.substr(0, label.length() - 4);
          std::string::size_type pos = label.rfind(' ');
          std::string straxis = "";
          if (pos != std::string::npos)
             straxis = label.substr(pos, std::string::npos);
          if (straxis.compare(" Rot") == 0)
            axis = 2;
          else if (straxis.compare(" Abd") == 0)
            axis = 1;
          else if (straxis.compare(" Flex") == 0)
            axis = 0;
          else
          {
            axis = 0;
            hasAxis = false;
          }
          if (hasAxis)
            label = label.substr(0, label.length() - straxis.length());
          btkTrimString(&label);
          label += suffix;
        }
        else
        {
          // Special case for some angles
          if (this->ExtractSpecialAngleLabel(label, "_Add_Ang"))
          {
            axis = 1;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Flex_Ang"))
          {
            axis = 0;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Ang"))
          {
            axis = 0;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Lat_Tilt"))
          {
            axis = 0;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Fwd_Tilt"))
          {
            axis = 1;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Rotation"))
          {
            axis = 2;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Orientation"))
          {
            axis = 0;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Elevation"))
          {
            axis = 1;
            type = Point::Angle;
          }
          else if (this->ExtractSpecialAngleLabel(label, "_Null"))
          {
            axis = 2;
            type = Point::Angle;
          }
          // Special case for GRF
          else if (this->ExtractSpecialForceLabel(label, "_GRF_FWD"))
          {
            axis = 0;
            type = Point::Force;
          }
          else if (this->ExtractSpecialForceLabel(label, "_GRF_LAT"))
          {
            axis = 1;
            type = Point::Force;
          }
          else if (this->ExtractSpecialForceLabel(label, "_GRF_VRT"))
          {
            axis = 2;
            type = Point::Force;
          }
        }
        Acquisition::PointIterator it = output->FindPoint(label);
        Point::Pointer point;
        if (it == output->EndPoint())
        {
          point = Point::New(label, frameNumber, type);
          output->AppendPoint(point);
        }
        else
          point = *it;
        double* data = point->GetValues().data();
        for (int j = 0 ; j < frameNumber ; ++j)
          data[j + axis * frameNumber] = values[j * colNumber + inc];
        ++inc;
      }
      
      delete[] values;
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (!ifs.is_open())
        excmsg = "Invalid file path.";
      else if (ifs.eof())
        excmsg = "Unexpected end of file.";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the file stream.";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the file stream.";
      
      if (ifs.is_open()) ifs.close();
      if (values) delete[] values;
      throw(XLSOrthoTrakFileIOException(excmsg));
    }
    catch (XLSOrthoTrakFileIOException& )
    {
      if (ifs.is_open()) ifs.close();
      if (values) delete[] values;
      throw;
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close();
      if (values) delete[] values;
      throw(XLSOrthoTrakFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close();
      if (values) delete[] values;
      throw(XLSOrthoTrakFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  XLSOrthoTrakFileIO::XLSOrthoTrakFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::ASCII)
  {};
  
  void XLSOrthoTrakFileIO::AppendEvent(Acquisition::Pointer output, std::istringstream* iss, const std::string& label, const std::string& context, int id)
  {
    iss->clear();
    std::string buf;
    *iss >> buf; // label
    double frame;
    while(!iss->eof() && (*iss >> frame))
      output->AppendEvent(Event::New(label, static_cast<int>(frame) + output->GetFirstFrame(), context, Event::Unknown, "", "", id));
  };
  
  void XLSOrthoTrakFileIO::ExtractEventDetectionFlag(Acquisition::Pointer output, std::istringstream* iss, const std::string& label, const std::string& context)
  {
    iss->clear();
    std::string buf;
    *iss >> buf; // label
    *iss >> buf; // FP
    double frame;
    while(!iss->eof() && (*iss >> frame))
    { 
      for (Acquisition::EventIterator it = output->BeginEvent() ; it != output->EndEvent() ; ++it)
      {
        if (((*it)->GetLabel().compare(label) == 0) 
            && ((*it)->GetContext().compare(context) == 0)
            && ((*it)->GetFrame() == static_cast<int>(frame) + output->GetFirstFrame()))
        {
          (*it)->SetDetectionFlags(Event::FromForcePlatform);
          break;
        }
      }
    }
  };
  
  void XLSOrthoTrakFileIO::AppendSpatiotemparalParameter(MetaData::Pointer st, std::istringstream* iss, double scale)
  {
    iss->clear();
    std::string name;
    float val;
    std::vector<float> values;
    *iss >> name;
    float s = static_cast<float>(scale);
    while (!iss->eof() && (*iss >> val))
      values.push_back(val * s);
    MetaDataCreateChild(st, name, values);
  };
  
  bool XLSOrthoTrakFileIO::ExtractSpecialAngleLabel(std::string& label, const std::string& str) const
  {
    std::string::size_type pos = label.rfind(str);
    if (pos != std::string::npos)
    {
      label = label.substr(0, pos) + "_ANGLE";
      return true;
    }
    return false;
  };
  
  bool XLSOrthoTrakFileIO::ExtractSpecialForceLabel(std::string& label, const std::string& str) const
  {
    std::string::size_type pos = label.rfind(str);
    if (pos != std::string::npos)
    {
      label = label.substr(0, pos + 4) + "_FORCE";
      return true;
    }
    return false;
  };
};
