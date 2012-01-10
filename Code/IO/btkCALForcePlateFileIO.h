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

#ifndef __btkCALForcePlateFileIO_h
#define __btkCALForcePlateFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkException.h"

#include <Eigen/Core>
#include <fstream>

namespace btk
{
  class CALForcePlateFileIOException : public Exception
  {
  public:
    explicit CALForcePlateFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~CALForcePlateFileIOException() throw() {};
  };
  
  class CALForcePlateFileIO : public AcquisitionFileIO
  {
  public:
    typedef SharedPtr<CALForcePlateFileIO> Pointer;
    typedef SharedPtr<const CALForcePlateFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new CALForcePlateFileIO());};
    
    // ~CALForcePlateFileIO(); // Implicit.
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual bool CanWriteFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    BTK_IO_EXPORT virtual void Write(const std::string& filename, Acquisition::Pointer input);
    
  protected:
    BTK_IO_EXPORT CALForcePlateFileIO();
    
  private:
    bool ExtractValues(double* values, int num, std::ifstream* ifs);
    void ExtractCalibrationMatrix(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>* cal, MetaDataInfo::Pointer data, int i);
    
    CALForcePlateFileIO(const CALForcePlateFileIO& ); // Not implemented.
    CALForcePlateFileIO& operator=(const CALForcePlateFileIO& ); // Not implemented. 
    
    struct ForcePlateInfo
    {
      int type;
      double dimensions[3];
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> calMatrix;
      double origin[3];
      double position[3];
      Eigen::Matrix<double, 3, 3> orientation;
    };
  };
};

#endif // __btkCALForcePlateFileIO_h
