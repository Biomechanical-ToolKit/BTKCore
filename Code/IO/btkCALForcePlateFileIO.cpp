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

#include "btkCALForcePlateFileIO.h"
#include "btkMotionAnalysisFileIOUtils_p.h"
#include "btkConvert.h"
#include "btkMetaDataUtils.h"

#include <Eigen/Array>
#include <Eigen/Geometry>

#include <algorithm>
#include <cctype>
#include <sstream>

namespace btk
{
  /**
   * @class CALForcePlateFileIOException btkCALForcePlateFileIO.h
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
   * @class CALForcePlateFileIO btkCALForcePlateFileIO.h
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
    std::ifstream ifs(filename.c_str(), std::ios_base::in);
    int index = 0;
    bool ok = true;
    if (!(ifs >> index))
      ok = false;
    ifs.close();
    
    if (ok && (index != 1))
      ok = false;
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
      int numLine = 0;
      while (!std::getline(ifs, line).eof())
      {
        // Feed the string stream
        iss.str(line);
        iss.clear();
      
        // Check for forceplate index
        if (!(iss >> index))
          throw(CALForcePlateFileIOException("Unable to extract the index of the force platform #"  + ToString(inc) + "."));
        if (index != inc)
          throw(CALForcePlateFileIOException("Incorrect index for the force platform #"  + ToString(inc) + "."));
        iss >> line;
        if (!iss.eof())
          throw(CALForcePlateFileIOException("Too many values for the line which should contain the index of the force platform #"  + ToString(inc) + "."));
        
        fpConfigurationComplete = false;
        ForcePlateInfo fp = ForcePlateInfo();
        
        // Dimension (h x w x l)
        if (!this->ExtractValues(fp.dimensions, 3, &ifs))
          throw(CALForcePlateFileIOException("The dimensions of the force platform #" + ToString(inc) + " can't be extracted."));
        
        // Calibration matrix
        // - Count the number of cols
        numLine += 2;
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
        ifs.seekg(0, std::ios::beg);
        for (int l = 0 ; l < numLine ; ++l)
          std::getline(ifs, line);
        // - Extract values
        fp.calMatrix.resize(rows, cols);
        for (int i = 0 ; i < rows ; ++i)
          this->ExtractValues(fp.calMatrix.data() + i * cols, cols, &ifs);
        fp.calMatrix.transposeInPlace();
        // - Detect the type of the matrix:
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
        numLine += rows;
        
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
        
        numLine += 5;
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
      size_t used = forcePlates.size();
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
        origins[inc1] = static_cast<float>(it->origin[0] * -10.0);
        origins[inc1+1] = static_cast<float>(it->origin[1] * -10.0);
        origins[inc1+2] = static_cast<float>(it->origin[2] * -10.0);
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
        temp.row(0).cwise() += (it->position[0] * 10.0);
        temp.row(1).cwise() += (it->position[1] * 10.0);
        temp.row(2).cwise() += (it->position[2] * 10.0);
        double* c = temp.data();
        for (int i = 0 ; i < 12 ; ++i)
          corners[inc2 + i] = static_cast<float>(c[i]);
        inc2 += 12;
        
        // Convert calibration matrix factor in Nmm instead of Nm (if applicable)
        if ((it->type == 4) || (it->type == 5))
        {
          it->calMatrix.row(3) *= 1000.0;
          it->calMatrix.row(4) *= 1000.0;
          it->calMatrix.row(5) *= 1000.0;
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
      std::vector<uint8_t> dims(2,3); dims[1] = static_cast<uint8_t>(used);
      partial->AppendChild(btk::MetaData::New("ORIGIN", dims, origins));
      // - BTK_PARTIAL_FP_CONFIG:CORNERS
      dims.resize(3); dims[0] = 3; dims[1] = 4; dims[2] = static_cast<uint8_t>(used);
      partial->AppendChild(btk::MetaData::New("CORNERS", dims, corners));
      // - BTK_PARTIAL_FP_CONFIG:CAL_MATRIX
      dims[0] = rows; dims[1] = chans;
      partial->AppendChild(btk::MetaData::New("CAL_MATRIX", dims, calMatrix));
      // Add a metadata to notify that the first frame was not set.
      MetaData::Pointer btkPointConfig = MetaDataCreateChild(output->GetMetaData(), "BTK_POINT_CONFIG");
      MetaDataCreateChild(btkPointConfig, "NO_FIRST_FRAME", static_cast<int8_t>(1));
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
      throw(CALForcePlateFileIOException("Unexpected exception occurred: " + std::string(e.what())));
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
      
    // Look for the metadata FORCE_PLATFORM or BTK_PARTIAL_FP_CONFIG
    MetaData::Iterator itFP;
    MetaData::Pointer forcePlatform;
    bool partial = false;
    if ((itFP = input->GetMetaData()->FindChild("FORCE_PLATFORM")) != input->EndMetaData())
      forcePlatform = *itFP;
    else if ((itFP = input->GetMetaData()->FindChild("BTK_PARTIAL_FP_CONFIG")) != input->EndMetaData())
    {
      forcePlatform = *itFP;
      partial = true;
    }
    else
    {
      // Nothing to do
      ofs.close();
      return;
    }
    
    MetaDataInfo::Pointer fpUsed = forcePlatform->ExtractChildInfo("USED", MetaDataInfo::Integer, 0, true);
    MetaDataInfo::Pointer fpType = forcePlatform->ExtractChildInfo("TYPE", MetaDataInfo::Integer, 1, true);
    MetaDataInfo::Pointer fpOrigin = forcePlatform->ExtractChildInfo("ORIGIN", MetaDataInfo::Real, 2, true);
    MetaDataInfo::Pointer fpCorners = forcePlatform->ExtractChildInfo("CORNERS", MetaDataInfo::Real, 3, true);
    MetaDataInfo::Pointer fpCalMatrix = forcePlatform->ExtractChildInfo("CAL_MATRIX", MetaDataInfo::Real, 3, false);
    MetaDataInfo::Pointer fpChannel = forcePlatform->ExtractChildInfo("CHANNEL", MetaDataInfo::Integer, 2, true);
    
    if (fpUsed && fpType && fpOrigin && fpCorners)
    {
      ofs.setf(std::ios::fixed, std::ios::floatfield);
      for (int i = 0 ; i < fpUsed->ToInt(0) ; ++i)
      {
        // Pre
        // - Rotation matrix used for the orientation
        Eigen::Matrix<double, 3, 1> col0, col1, col3;
        col0 << fpCorners->ToDouble(0 + 12 * i), fpCorners->ToDouble(1 + 12 * i), fpCorners->ToDouble(2 + 12 * i);
        col1 << fpCorners->ToDouble(3 + 12 * i), fpCorners->ToDouble(4 + 12 * i), fpCorners->ToDouble(5 + 12 * i);
        col3 << fpCorners->ToDouble(9 + 12 * i), fpCorners->ToDouble(10 + 12 * i), fpCorners->ToDouble(11 + 12 * i);
        Eigen::Matrix<double, 3, 3> R;
        R.col(0) = col0 - col1;
        R.col(0).normalize();
        R.col(2) = R.col(0).cross(col0 - col3);
        R.col(2).normalize();
        R.col(1) = R.col(2).cross(R.col(0));
        // - Express the corners in the local frame
        col0 = R * col0;
        col1 = R * col1;
        col3 = R * col3;
        // - Prepare the data for the calibration matrix
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> cal;
        int type = fpType->ToInt(i);
        int numChannels = 0;
        switch(type)
        {
        //   * No calibration matrix for the type 1, 2 & 3.
        case 1:
        case 2:
          numChannels = 6;
          cal.setIdentity(6,6);
          break;
        case 3:
          numChannels = 8;
          cal.setIdentity(8,8);
          break;
        case 4:
          numChannels = 6;
          cal.setIdentity(6,6);
          this->ExtractCalibrationMatrix(&cal,fpCalMatrix,i);
          break;
        case 5:
          numChannels = 8;
          cal.setIdentity(6,8);
          this->ExtractCalibrationMatrix(&cal,fpCalMatrix,i);
          break;
        default:
          {
          btkErrorMacro("Force platform type " + ToString(fpType->ToInt(i)) + " no yet supported. Please report this problem to the developers.");
          break;
          }
        }
        
        // Write the data into the file
        // Platform's index
        ofs << i+1 << "\n";
        // Platform's dimensions
        ofs.precision(3);
        // - Use the the coordinate of the corners
        double width = (col0.x() - col1.x()) / 10.0; // x1 - x2 (unit: centimeter)
        double length = (col0.y() - col3.y()) / 10.0; // y1 - y4 (unit: centimeter)
        // - No guess on the height of the platform => set as 1cm
        ofs << 1.0 << " " << width << " " << length << "\n";
        // Calibration matrix
        ofs.precision(6);
        // - Its content can adapted to be able to scale correctly the analog channels
        // - It is due in some case case to the incompatibility between the scale factor in C3D and ANC/ANB file format
        if (partial)
        {
          // - Because the CAL reader forced the values to be displayed in Nmm instead of Nm.
          if ((type == 4) || (type == 5))
          {
            cal.row(3) /= 1000.0;
            cal.row(4) /= 1000.0;
            cal.row(5) /= 1000.0;
          }
        }
        else if (fpChannel && (input->GetAnalogResolution() == Acquisition::Bit16) && (numChannels != 0))
        {
          // - Adapt the scaling factor if necessary
          int maxChannelPerPlatform = fpChannel->GetDimension(0);
          for (int j = 0 ; j < numChannels ; ++j)
          {
            int idxChannel = j + maxChannelPerPlatform * i;
            if ((idxChannel >= static_cast<int>(fpChannel->GetValues().size())) || (idxChannel >= input->GetAnalogNumber()))
            {
              btkErrorMacro("Index for the analog channel used by the force platform #" + ToString(i+1) + " is out of range.");
            }
            else
            {
              Analog::Pointer analog = input->GetAnalog(idxChannel);
              uint16_t range = AnxFileIOExtractAnalogRangeFromGain(idxChannel, analog->GetGain(), analog->GetScale(), input->GetAnalogResolution());
              double scale = ANxFileIOComputeScaleFactor(range, input->GetAnalogResolution());
              if (fabs(analog->GetScale()) != fabs(scale))
              {
                cal.col(j) *= -1.0 * analog->GetScale() / scale;
              }
            }
          }
          if (input->GetPointUnit(Point::Moment).compare("Nmm") == 0)
          {
            // - The calibration matrix seems to be for Nm not Nmm
            if (type != 3)
            {
              cal.row(3) /= 1000.0;
              cal.row(4) /= 1000.0;
              cal.row(5) /= 1000.0;
            }
          }
        }
        for (int m = 0 ; m < cal.rows() ; ++m)
        {
          for (int n = 0 ; n < cal.cols() ; ++n)
            ofs << cal.coeff(m,n) << ((n < cal.cols()-1) ? " " : "");
          ofs << "\n";
        }
        // Origin in centimeters and from the surfacic origin to the physical origin
        ofs << fpOrigin->ToDouble(0+i*3) / -10.0 << " " << fpOrigin->ToDouble(1+i*3) / -10.0 << " " << fpOrigin->ToDouble(2+i*3) / -10.0 << "\n";
        // Position of the force plate center in the global frame (in centimeters)
        ofs << (fpCorners->ToDouble(0 + 12 * i) + fpCorners->ToDouble(6 + 12 * i)) / 2.0 / 10.0 << " "
            << (fpCorners->ToDouble(1 + 12 * i) + fpCorners->ToDouble(7 + 12 * i)) / 2.0 / 10.0 << " "
            << (fpCorners->ToDouble(2 + 12 * i) + fpCorners->ToDouble(8 + 12 * i)) / 2.0 / 10.0 << "\n";
        // Orientation
        for (int m = 0 ; m < R.rows() ; ++m)
        {
          for (int n = 0 ; n < R.cols() ; ++n)
            ofs << R.coeff(m,n) << ((n < R.cols()-1) ? " " : "");
          ofs << "\n";
        }
      }
    }
    else
    {
      btkErrorMacro("Missing metadata informations to create the force platform calibration file.");
    }
    ofs.close();
  };
  
  /**
   * Constructor.
   */
  CALForcePlateFileIO::CALForcePlateFileIO()
  : AcquisitionFileIO()
  {
    this->SetFileType(AcquisitionFileIO::ASCII);
  };

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
  
  void CALForcePlateFileIO::ExtractCalibrationMatrix(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>* cal, MetaDataInfo::Pointer data, int idx)
  {
    int coefficientsAlreadyExtracted = data->GetDimension(0) * data->GetDimension(1) * idx;
    if (static_cast<int>(data->GetValues().size()) >= (coefficientsAlreadyExtracted + cal->size()))
    {
      for (int i = 0 ; i < cal->cols() ; ++i)
        for (int j = 0 ; j < cal->rows() ; ++j)
          cal->coeffRef(j,i) = data->ToDouble(j + i * cal->rows() + coefficientsAlreadyExtracted);
    }
    else
    {
      btkErrorMacro("Missing coefficients to build the calibration matrix #" + ToString(idx+1) + ". Its content might be corrupted.");
    }
  }
};
