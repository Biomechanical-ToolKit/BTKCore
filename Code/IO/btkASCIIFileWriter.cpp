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

#include "btkASCIIFileWriter.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class ASCIIFileWriterException btkASCIIFileWriter.h
   * @brief Exception class for the ASCIIFileWriter class.
   */
  
  /**
   * @fn ASCIIFileWriterException::ASCIIFileWriterException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual ASCIIFileWriterException::~ASCIIFileWriterException()
   * Empty destructor.
   */
  
  /**
   * @class ASCIIFileWriter
   * @brief Writer of acquisition data into ASCII/Text file.
   *
   * Export data of events, points and analog channels to ASCII/Text file.
   *
   * The format in the file is the following
   *  - Header (First frame, number of frames, etc.): Optional: see option BTK_ASCII_EXPORT_OPTIONS:NO_HEADER ;
   *  - Events (grouped by label with times stored in row) ;
   *  - Points (X/Y/Z values. Invalid values set to 0): Optional: see option BTK_ASCII_EXPORT_OPTIONS:FORCEPLATE_POINTS_SEPARATELY to separate point's data coming from force plates.
   *  - Analogs.
   * 
   * The first column for the points and the analogs correspond to the time of the acquisition relative to the frame one.
   *
   * You can specify the separator between values with the method SetSeparator(). By default, the separator is set to comma (,).
   *
   * You can export only a subset of the acquisition by specifying the frames of interest using the method SetFramesOfInterest().
   *
   * There is some options in this class enabled/disabled by using the metadata of the given input.
   * This writer check if the metadata BTK_ASCII_EXPORT_OPTIONS exists and the check for its children. The used children metadata are:
   * - NO_HEADER: Disable the writing of the header if the metadata is set to 1.
   * - NO_EVENT: Disable the writing of events if the metadata is set to 1.
   * - NO_POINT: Disable the writing of the points if the metadata is set to 1.
   * - NO_ANALOG: Disable the writing of the analog channels if the metadata is set to 1.
   * - FORCEPLATE_POINTS_SEPARATELY: Write the data computed from the force plates in an other section. The points used for that are given to this option as a vector of integer (int16_t) containg their index (starting from 0).
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef ASCIIFileWriter::Pointer
   * Smart pointer associated with an ASCIIFileWriter object.
   */
  
  /**
   * @typedef ASCIIFileWriter::ConstPointer
   * Smart pointer associated with a const ASCIIFileWriter object.
   */
  
  /**
   * @fn ASCIIFileWriter::~ASCIIFileWriter()
   * Empty destructor.
   */
  
  /**
   * @fn static ASCIIFileWriter::Pointer ASCIIFileWriter::New()
   * Creates a ASCIIFileWriter process
   */
  
  /**
   * @fn Acquisition::Pointer ASCIIFileWriter::GetInput()
   * Returns the Acquisition used to create the text files.
   */

  /**
   * @fn void ASCIIFileWriter::SetInput(Acquisition::Pointer input)
   * Sets the Acquisition used to create the text files.
   */
    
  /**
   * @fn const std::string& ASCIIFileWriter::GetFilename() const
   * Gets the filename of the file to read.
   */
  
  /**
   * Specifies the file to read. This is forwarded to the IO instance.
   */
  void ASCIIFileWriter::SetFilename(const std::string& filename)
  {
    if (this->m_Filename.compare(filename) != 0)
    {
      this->m_Filename = filename;
      this->Modified();
    }
  };
  
  /**
   * @fn Acquisition::Pointer ASCIIFileWriter::GetInput()
   * Returns the Acquisition to export to an ASCII file.
   */

  /**
   * @fn void ASCIIFileWriter::SetInput(Acquisition::Pointer input)
   * Sets the Acquisition to export to an ASCII file.
   */
  
  /**
   * @fn const std::string& ASCIIFileWriter::GetSeparator() const
   * Gets the string used to separate the values between them.
   */
  
  /**
   * Sets the string used to separate the values between them.
   */
  void ASCIIFileWriter::SetSeparator(const std::string& sep)
  {
    if (this->m_Separator.compare(sep) != 0)
    {
      this->m_Separator = sep;
      this->Modified();
    }
  };
  
  /**
   * @fn const int* ASCIIFileWriter::GetFramesOfInterest() const
   * Returns the frames of interest.
   */
  
  /**
   * @fn void ASCIIFileWriter::GetFramesOfInterest(int& ff, int& lf) const
   * Returns the frames of interest.
   */
   
  /**
   * Sets the frames of interest to export.
   * Setting @a ff to -1 means you start from the first frame of the acquisition.
   * Setting @a lf to -1 means you use the last frame of the acquisition.
   */
  void ASCIIFileWriter::SetFramesOfInterest(int ff, int lf)
  {
    if ((this->m_FOI[0] != ff) || (this->m_FOI[1] != lf))
    {
      this->m_FOI[0] = ff;
      this->m_FOI[1] = lf;
      this->Modified();
    }
  };
  
  /**
   * Constructor. Sets the number of outputs equal to one. No input. Separator set to common (,).
   */
  ASCIIFileWriter::ASCIIFileWriter()
  : m_Filename(), m_Separator(",")
  {
    this->m_FOI[0] = -1;
    this->m_FOI[1] = -1;
    this->SetInputNumber(1);
  };
  
  /**
   * Whatever the specified index, this method creates an Acquisition object
   */
  DataObject::Pointer ASCIIFileWriter::MakeOutput(int idx)
  {
    btkNotUsed(idx);
    throw(RuntimeError("btk::ASCIIFileWriter has not output."));
  };
  
  /**
   * Create STL files based on the given file prefix (see SetFilePrefix()) and fill them with the given acquisition (see SetInputAcquisition()) and the given mesh (see SetInputMesh()). You can also set the frames to extract using the method SetFramesOfInterest().
   */
  void ASCIIFileWriter::GenerateData()
  {
    if (this->m_Filename.empty())
      throw ASCIIFileWriterException("Filename must be specified.");
    
    Acquisition::Pointer input = this->GetInput();
    if (!input)
    {
      btkErrorMacro("Impossible to write a null input into a file.");
      return;
    }
      
    int ff = (this->m_FOI[0] == -1 ? input->GetFirstFrame() : this->m_FOI[0]);
    int lf = (this->m_FOI[1] == -1 ? input->GetLastFrame() : this->m_FOI[1]);
    
    if (ff < input->GetFirstFrame())
      throw ASCIIFileWriterException("First frame out of range.");
    else if (lf > input->GetLastFrame())
      throw ASCIIFileWriterException("Last frame out of range.");
    
    MetaData::ConstIterator options = input->GetMetaData()->FindChild("BTK_ASCII_EXPORT_OPTIONS");
    bool writeHeader = true, writeEvent = true, writePoint = true, writeAnalog = true;
    std::vector<int16_t> forcePlatePointIndices;
    
    if (options != input->EndMetaData())
    {
      MetaDataInfo::Pointer noHeaderOption = (*options)->ExtractChildInfo("NO_HEADER", MetaDataInfo::Byte, 0);
      if (noHeaderOption)
        writeHeader = noHeaderOption->ToInt8(0) == 0;
      MetaDataInfo::Pointer noEventOption = (*options)->ExtractChildInfo("NO_EVENT", MetaDataInfo::Byte, 0);
      if (noEventOption)
        writeEvent = noEventOption->ToInt8(0) == 0;
      MetaDataInfo::Pointer noPointOption = (*options)->ExtractChildInfo("NO_POINT", MetaDataInfo::Byte, 0);
      if (noPointOption)
        writePoint = noPointOption->ToInt8(0) == 0;
      MetaDataInfo::Pointer noAnalogOption = (*options)->ExtractChildInfo("NO_ANALOG", MetaDataInfo::Byte, 0);
      if (noAnalogOption)
        writeAnalog = noAnalogOption->ToInt8(0) == 0;
      MetaDataInfo::Pointer forcePlatePointsSeparatelyOption = (*options)->ExtractChildInfo("FORCEPLATE_POINTS_SEPARATELY", MetaDataInfo::Integer, 1);
      if (forcePlatePointsSeparatelyOption)
        forcePlatePointIndices = forcePlatePointsSeparatelyOption->ToInt16();
    }
    
    if (input->GetPointFrequency() == 0.0)
    {
      btkWarningMacro("The frequency is not set! The column for the time will contain only zero value.");
    }
    
    try
    {
      std::ofstream ofs(this->m_Filename.c_str());
      if (!ofs)
        throw ASCIIFileWriterException("File can't be opened. Have you the permission to write this file?\nFilename: " + this->m_Filename);
      
      if (writeHeader)
      {
        ofs << "Frame number" << this->m_Separator << lf - ff + 1 << "\n";
        ofs << "First frame" << this->m_Separator << ff << "\n";
        ofs << "Point frequency" << this->m_Separator << input->GetPointFrequency() << "\n";
        ofs << "Analog frequency" << this->m_Separator << input->GetAnalogFrequency() << "\n";
        ofs << std::endl;
      }
      
      if (writeEvent)
      {
        // Keep only the events between the frames of intereset
        btk::EventCollection::Pointer events = btk::EventCollection::New();
        for(btk::EventCollection::ConstIterator it = input->GetEvents()->Begin() ; it != input->GetEvents()->End() ; ++it)
        {
          if (((*it)->GetFrame() >= ff) && ((*it)->GetFrame() <= lf))
            events->InsertItem(*it);
        }
        if (!events->IsEmpty())
        {
          std::vector<std::string> labels;
          std::vector< std::vector<double> > times;
          bool onlyOneSubject = true;
        
          btk::EventCollection::ConstIterator it = events->Begin();
          std::string subject = (*it)->GetSubject();
          ++it;
          while (it != events->End())
          {
            if (subject.compare((*it)->GetSubject()) != 0)
            {
              onlyOneSubject = false;
              break;
            }
            ++it;
          }
          for(btk::EventCollection::ConstIterator it = events->Begin() ; it != events->End() ; ++it)
          {
            std::string label = "";
            if (!onlyOneSubject)
            {
              if (!((*it)->GetSubject().empty()))
                label = (*it)->GetSubject() + "_";
            }
            if (!((*it)->GetContext().empty()))
              label += (*it)->GetContext() + "_" + (*it)->GetLabel();
            else
              label += (*it)->GetLabel();
            bool labelAddition = true;
            for(size_t i = 0 ; i < labels.size() ; ++i)
            {
              if (labels[i].compare(label) == 0)
              {
                times[i].push_back((*it)->GetTime());
                labelAddition = false;
                break;
              }
            }
            if (labelAddition)
            {
              labels.push_back(label);
              times.resize(times.size() + 1);
              times.back().push_back((*it)->GetTime());
            }
          }
          // sort event's times.
          for (size_t i = 0 ; i < times.size() ; ++i)
          {
            for (size_t j = 0 ; j < times[i].size() - 1 ; ++j)
            {
              if (times[i][j] > times[i][j + 1])
              {
                double t = times[i][j];
                times[i][j] = times[i][j + 1];
                times[i][j + 1] = t;
              }
              for (size_t k = j ; k > 0 ; --k)
              {
                if (times[i][k] < times[i][k - 1])
                {
                  double t = times[i][k];
                  times[i][k] = times[i][k - 1];
                  times[i][k - 1] = t;
                }
                else
                  break;
              }
            }
          }
          // sort event depending the first time.
          std::vector<int> indexes = std::vector<int>(times.size(),0);
          std::vector<double> firstTimes = std::vector<double>(times.size(),0);
          for (size_t i = 0 ; i < times.size() ; ++i)
          {
            indexes[i] = (int)i;
            firstTimes[i] = times[i][0];
          }
          for (size_t i = 0 ; i < times.size() - 1 ; ++i)
          {
            if (firstTimes[i] > firstTimes[i+1])
            {
              int idx = indexes[i];
              indexes[i] = indexes[i+1];
              indexes[i+1] = idx;
              double t = firstTimes[i];
              firstTimes[i] = firstTimes[i+1];
              firstTimes[i + 1] = t;
            }
            for (size_t k = i ; k > 0 ; --k)
            {
              if (firstTimes[k] < firstTimes[k - 1])
              {
                int idx = indexes[k];
                indexes[k] = indexes[k-1];
                indexes[k-1] = idx;
                double t = firstTimes[k];
                firstTimes[k] = firstTimes[k-1];
                firstTimes[k - 1] = t;
              }
              else
                break;
            }
          }
          // Times and labels sorted
          std::vector< std::vector<double> > times_sorted = std::vector< std::vector<double> >(times.size());
          std::vector<std::string> labels_sorted = std::vector<std::string>(labels.size());
          for (size_t i = 0 ; i < indexes.size() ; ++i)
          {
            times_sorted[i] = times[indexes[i]];
            labels_sorted[i] = labels[indexes[i]];
          }
          // Export the events
          for (size_t i = 0 ; i < labels_sorted.size() ; ++i)
          {
            ofs << labels_sorted[i];
            for (size_t j = 0 ; j < times_sorted[i].size() ; ++j)
              ofs << this->m_Separator << times_sorted[i][j];
            ofs << "\n";
          }
          ofs << std::endl;
        }
      }
      
      if (writePoint)
      {
        btk::PointCollection::Pointer points = btk::PointCollection::New();
        btk::PointCollection::Pointer forceplates = btk::PointCollection::New();
        int inc = 0;
        for (btk::PointCollection::ConstIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
        {
          bool found = false;
          for (std::vector<int16_t>::iterator itI = forcePlatePointIndices.begin() ; itI != forcePlatePointIndices.end() ; ++itI)
          {
            if (*itI == inc)
            {
              found = true;
              forcePlatePointIndices.erase(itI);
              break;
            }
          }
          ++inc;
          if (found)
            forceplates->InsertItem(*it);
          else
            points->InsertItem(*it);
        }
        if (!points->IsEmpty())
          this->WritePoints(&ofs, ff, lf, input, points);
        if (!forceplates->IsEmpty())
          this->WritePoints(&ofs, ff, lf, input, forceplates);
      }
      
      if (writeAnalog && !input->IsEmptyAnalog())
      {
        ofs << "Time";
        // Label
        for (btk::AnalogCollection::ConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
          ofs << this->m_Separator << (*it)->GetLabel();
        ofs << std::endl;
        // Unit
        ofs << "s";
        for (btk::AnalogCollection::ConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
          ofs << this->m_Separator << (*it)->GetUnit();
        ofs << std::endl;
        // Data
        int ffi = (ff - input->GetFirstFrame()) * input->GetNumberAnalogSamplePerFrame();
        int lfi = (lf - input->GetFirstFrame() + 1) * input->GetNumberAnalogSamplePerFrame();
        double t = 0.0;
        if (input->GetAnalogFrequency() != 0.0)
          t = 1.0 / input->GetAnalogFrequency();
        for (int i = ffi ; i < lfi ; ++i)
        {
          ofs << static_cast<double>(i + (input->GetFirstFrame()-1) * input->GetNumberAnalogSamplePerFrame()) * t;
          for (btk::AnalogCollection::ConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
            ofs << this->m_Separator << (*it)->GetValues().coeff(i);
          ofs << std::endl;
        }
        ofs << std::endl;
      }
      
      ofs.close();
    }
    catch (ASCIIFileWriterException& )
    {
      throw;
    }
    catch (std::exception& e)
    {
      throw(ASCIIFileWriterException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      throw(ASCIIFileWriterException("Unknown exception"));
    }
  };
  
  void ASCIIFileWriter::WritePoints(std::ofstream* ofs, int ff, int lf, Acquisition::Pointer acq, PointCollection::Pointer points)
  {
    *ofs << "Time";
    // Label
    for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
      *ofs << this->m_Separator << (*it)->GetLabel() << this->m_Separator << this->m_Separator;
    *ofs << std::endl;
    // Unit
    *ofs << "s";
    for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
    {
      std::string unit = acq->GetPointUnit((*it)->GetType());
      *ofs << this->m_Separator << unit << this->m_Separator << unit << this->m_Separator << unit;
    }
    *ofs << std::endl;
    // X/Y/Z
    for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
      *ofs << this->m_Separator << "X" << this->m_Separator << "Y" << this->m_Separator << "Z";
    *ofs << std::endl;
    // Data
    double t = 0.0;
    if (acq->GetPointFrequency() != 0.0)
      t = 1.0 / acq->GetPointFrequency();
    int ffi = ff - acq->GetFirstFrame();
    int lfi = lf - acq->GetFirstFrame();
    for (int i = ffi ; i <= lfi ; ++i)
    {
      *ofs << static_cast<double>(i + acq->GetFirstFrame() - 1) * t;
      for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
      {
        if ((*it)->GetResiduals().coeff(i) >= 0.0)
          *ofs << this->m_Separator << (*it)->GetValues().coeff(i,0) << this->m_Separator << (*it)->GetValues().coeff(i,1) << this->m_Separator << (*it)->GetValues().coeff(i,2);
        else
          *ofs << this->m_Separator << 0 << this->m_Separator << 0 << this->m_Separator << 0;
      }
      *ofs << std::endl;
    }
    *ofs << std::endl;
  };
};
