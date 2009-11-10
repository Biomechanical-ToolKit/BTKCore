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

#include "btkCALForcePlateFileIO.h"
#include "btkConvert.h"

#include <Eigen/Array>

#include <algorithm>
#include <cctype>
#include <sstream>

namespace btk
{
  /**
   * @class CALForcePlateFileIOException
   * @brief Exception class for the CALForcePlateFileIO class.
   */
  
  /**
   * @fn CALForcePlateFileIOException::CALForcePlateFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual CALForcePlateFileIOException::~CALForcePlateFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class CALForcePlateFileIO
   * @brief Interface to read/write CAL files containing calibration of force platform.
   *
   * The CAL file format is created by Motion Analysis Corp.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef CALForcePlateFileIO::Pointer
   * Smart pointer associated with a CALForcePlateFileIO object.
   */
  
  /**
   * @typedef CALForcePlateFileIO::ConstPointer
   * Smart pointer associated with a const CALForcePlateFileIO object.
   */
  
  /**
   * @fn static CALForcePlateFileIO::Pointer CALForcePlateFileIO::New()
   * Create a CALForcePlateFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first word in the file corresponds to "PathFileType".
   */
  bool CALForcePlateFileIO::CanReadFile(const std::string& filename)
  {
    std::ifstream ifs(filename.c_str());
    int index = 0;
    bool ok = true;
    if (!(ifs >> index))
      ok = false;
    ifs.close();
    
    if (ok && (index != 1))
      ok = false;
    /*
    char c[2];
    ifs.read(c, 1); c[1] = '\0';
    ifs.close();
    if (strcmp(c,"1") != 0)
      return false;
    */
    return ok;
  };
  
  /**
   * Checks if the suffix of @a filename is CAL.
   */
  bool CALForcePlateFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type CALPos = lowercase.rfind(".cal");
    if ((CALPos != std::string::npos) && (CALPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void CALForcePlateFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    // Open the stream
    std::ifstream ifs;
    //ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      int inc = 1;
      int index = 0;
      std::list<ForcePlateInfo> forcePlates;
      std::string line;
      std::istringstream iss;
      ifs.open(filename.c_str());
      bool fpConfigurationComplete = false;
      while (!ifs.eof())
      {
        // Check for forceplate index
        std::getline(ifs, line);
        if (ifs.eof())
          break;
        iss.str(line); iss.clear();
        if (!(iss >> index))
          throw(CALForcePlateFileIOException("Unable to extract the index of the force platform #"  + ToString(inc) + "."));
        if (index != inc)
          throw(CALForcePlateFileIOException("Incorrect index for the force platform #"  + ToString(inc) + "."));
        iss >> line;
        if (!iss.eof())
          throw(CALForcePlateFileIOException("Too many values for the line which should contain the index of the force platform #"  + ToString(inc) + "."));
        
        fpConfigurationComplete = false;
        ForcePlateInfo fp = ForcePlateInfo();
        
        /*
        std::getline(ifs, line);
        this->RemoveBlank(line);
        std::cout << "'" << line.length() << "'" << std::endl;
        if ((line.substr(0,1).compare(ToString(inc)) != 0) || line.length() > 1)
          throw(CALForcePlateFileIOException("Unable to extract the index of the force platform #"  + ToString(inc) + "."));
          */
        // Dimension (h x w x l)
        if (!this->ExtractValues(fp.dimensions, 3, &ifs))
          throw(CALForcePlateFileIOException("The dimensions of the force platform #" + ToString(inc) + " can't be extracted."));
        // Calibration matrix
        // - Count the number of cols
        std::streampos pos = ifs.tellg();
        std::getline(ifs, line);
        iss.str(line); iss.clear();
        int cols = 0;
        double buff;
        while(!iss.eof())
        {
          if (iss >> buff)
            ++cols;
          else
            break;
        }
        if (!iss.eof())
          throw(CALForcePlateFileIOException("The calibration matrix of the force platform #" + ToString(inc) + " can't be extracted."));
        if (cols <= 3)
          throw(CALForcePlateFileIOException("The calibration matrix for the force platform #" + ToString(inc) + " has too few number of values."));
        // - Count the number of rows
        int rows = 1;
        double* temp = new double[cols];
        while (1)
        {
          if (this->ExtractValues(temp, cols, &ifs))
            ++rows;
          else
            break;
        }
        delete[] temp;
        // - Back to the first line of the calibration matrix
        ifs.seekg(pos, std::ios::beg);
        // - Extract values
        fp.calMatrix.resize(rows, cols);
        for (int i = 0 ; i < rows ; ++i)
          this->ExtractValues(fp.calMatrix.data() + i * cols, cols, &ifs);
        fp.calMatrix.transposeInPlace();
        
        // Detect the type of the matrix:
        if ((rows == 6) || (cols ==  6))
          fp.type = 4;
        else if ((rows == 6) || (cols ==  8))
          fp.type = 5;
        else if ((rows == 8) || (cols ==  8))
          fp.type = 7;
        else if ((rows == 12) || (cols ==  12))
          fp.type = 6;
        else
          throw(CALForcePlateFileIOException("Unknown type of for the force platform #" + ToString(inc) + "."));
        // Origin
        if (!this->ExtractValues(fp.origin, 3, &ifs))
          throw(CALForcePlateFileIOException("The origin of the force platform #" + ToString(inc) + " can't be extracted."));
        // Position of the force plate center in the global frame
        if (!this->ExtractValues(fp.position, 3, &ifs))
          throw(CALForcePlateFileIOException("The position of the force platform #" + ToString(inc) + " can't be extracted."));
        // Orientation;
        if (!this->ExtractValues(fp.orientation.data(), 3, &ifs))
          throw(CALForcePlateFileIOException("The orientation (row #1) of the force platform #" + ToString(inc) + " can't be extracted."));
        if (!this->ExtractValues(fp.orientation.data()+3, 3, &ifs))
          throw(CALForcePlateFileIOException("The orientation (row #2) of the force platform #" + ToString(inc) + " can't be extracted."));
        if (!this->ExtractValues(fp.orientation.data()+6, 3, &ifs))
          throw(CALForcePlateFileIOException("The orientation (row #3) of the force platform #" + ToString(inc) + " can't be extracted."));
        ++inc;
        fpConfigurationComplete = true;
        forcePlates.push_back(fp);
        
      }
      if (!fpConfigurationComplete)
        throw(CALForcePlateFileIOException("The configuration of the force platform #" + ToString(inc) + " is incomplete."));
      
      // Check for the maximum number of chanel;
      int chans = 0, rows = 0;
      for (std::list<ForcePlateInfo>::const_iterator it = forcePlates.begin() ; it != forcePlates.end() ; ++it)
      {
        if (it->calMatrix.cols() > chans)
          chans = it->calMatrix.cols();
        if (it->calMatrix.rows() > rows)
          rows = it->calMatrix.rows();
      }
      // Adapt dimensions of calibration matrices
      for (std::list<ForcePlateInfo>::iterator it = forcePlates.begin() ; it != forcePlates.end() ; ++it)
      {
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> cal = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Zero(rows, chans);
        cal.block(0,0,it->calMatrix.rows(),it->calMatrix.cols()) = it->calMatrix;
        it->calMatrix = cal;
      }
      // Format data
      int used = forcePlates.size();
      std::vector<int16_t> types(used);
      std::vector<float> origins(3 * used);
      std::vector<float> corners(3 * 4 * used);
      std::vector<float> calMatrix(rows * chans * used);
      inc = 0; int inc1 = 0, inc2 = 0, inc3 = 0;
      for (std::list<ForcePlateInfo>::iterator it = forcePlates.begin() ; it != forcePlates.end() ; ++it)
      {
        types[inc] = it->type;
        inc += 1;
        
        // Origin in a CAL file is defined in centimeters and from the center of the platform
        // The data are converted in the global frame with the default unit: mm
        origins[inc1] = static_cast<float>(it->origin[0] * -10);
        origins[inc1+1] = static_cast<float>(it->origin[1] * -10);
        origins[inc1+2] = static_cast<float>(it->origin[2] * -10);
        inc1 += 3;
        
        // Corners must be in millimeters
        Eigen::Matrix<double, 3, 4> temp = Eigen::Matrix<double, 3, 4>::Zero();
        temp.coeffRef(0) = it->dimensions[1] / 2.0;
        temp.coeffRef(1) = it->dimensions[2] / 2.0;
        temp.coeffRef(3) = -it->dimensions[1] / 2.0;
        temp.coeffRef(4) = it->dimensions[2] / 2.0;
        temp.coeffRef(6) = -it->dimensions[1] / 2.0;
        temp.coeffRef(7) = -it->dimensions[2] / 2.0;
        temp.coeffRef(9) = it->dimensions[1] / 2.0;
        temp.coeffRef(10) = -it->dimensions[2] / 2.0;
        temp *= 10.0;
        temp = it->orientation * temp;
        //Eigen::Matrix<double, 3, 1> origin(it->position);
        temp.row(0).cwise() += (it->position[0] * 10);
        temp.row(1).cwise() += (it->position[1] * 10);
        temp.row(2).cwise() += (it->position[2] * 10);
        double* c = temp.data();
        for (int i = 0 ; i < 12 ; ++i)
          corners[inc2 + i] = static_cast<float>(c[i]);
        inc2 += 12;
        
        // Convert calibration matrix factor in Nmm instead of Nm (if applicable)
        if ((it->type == 4) || (it->type == 5))
        {
          it->calMatrix.row(3) *= 1000;
          it->calMatrix.row(4) *= 1000;
          it->calMatrix.row(5) *= 1000;
        }
        
        
        
        const double* cm = it->calMatrix.data();
        for (int i = 0 ; i < rows * chans ; ++i)
          calMatrix[inc3 + i] = static_cast<float>(cm[i]);
        inc3 += (rows * chans);
      }
      // Create the metadata BTK_PARTIAL_FP_CONFIG
      btk::MetaData::Pointer partial = btk::MetaData::New("BTK_PARTIAL_FP_CONFIG");
      output->GetMetaData()->AppendChild(partial);
      // - BTK_PARTIAL_FP_CONFIG:USED
      partial->AppendChild(btk::MetaData::New("USED", static_cast<int16_t>(used)));
      // - BTK_PARTIAL_FP_CONFIG:TYPE
      partial->AppendChild(btk::MetaData::New("TYPE", types));
      // - BTK_PARTIAL_FP_CONFIG:ORIGIN
      std::vector<uint8_t> dims(2,3); dims[1] = used;
      partial->AppendChild(btk::MetaData::New("ORIGIN", dims, origins));
      // - BTK_PARTIAL_FP_CONFIG:CORNERS
      dims.resize(3); dims[0] = 3; dims[1] = 4; dims[2] = used;
      partial->AppendChild(btk::MetaData::New("CORNERS", dims, corners));
      // - BTK_PARTIAL_FP_CONFIG:CAL_MATRIX
      dims[0] = rows; dims[1] = chans;
      partial->AppendChild(btk::MetaData::New("CAL_MATRIX", dims, calMatrix));
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
      throw(CALForcePlateFileIOException(excmsg));
    }
    catch (CALForcePlateFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(CALForcePlateFileIOException("Unexpected exception occured: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(CALForcePlateFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void CALForcePlateFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    if (input.get() == 0)
    {
      btkIOErrorMacro(filename, "Empty input. Impossible to write an empty file.");
      return;
    }
    std::ofstream ofs(filename.c_str());
    if (!ofs) 
      throw(CALForcePlateFileIOException("Invalid file path."));
    
    ofs.close();
  };
  
  /**
   * Constructor.
   */
  CALForcePlateFileIO::CALForcePlateFileIO()
  : AcquisitionFileIO()
  {
    this->SetFileType(AcquisitionFileIO::Binary);
  };
  /*
  void CALForcePlateFileIO::RemoveBlank(std::string& str)
  {
    str = str.erase(str.find_last_not_of(' ') + 1);
    str = str.erase(0, str.find_first_not_of(' '));
    str = str.erase(str.find_last_not_of('\t') + 1);
    str = str.erase(0, str.find_first_not_of('\t'));
  };
  */
  bool CALForcePlateFileIO::ExtractValues(double* values, int num, std::ifstream* ifs)
  {
    std::string line;
    std::getline(*ifs, line);
    std::istringstream iss(line, std::istringstream::in);
    for (int i = 0 ; i < num ; ++i)
    {
      if (!(iss >> values[i]))
        return false;
    }
    return true;
  };
};
