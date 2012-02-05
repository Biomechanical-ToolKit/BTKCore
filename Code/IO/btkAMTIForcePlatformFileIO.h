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

#ifndef __btkAMTIForcePlatformFileIO_h
#define __btkAMTIForcePlatformFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkForcePlatform.h"
#include "btkException.h"

#include <sstream>

namespace btk
{
  class AMTIForcePlatformFileIOException : public Exception
  {
  public:
    explicit AMTIForcePlatformFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~AMTIForcePlatformFileIOException() throw() {};
  };
  
  class AMTIForcePlatformFileIO : public AcquisitionFileIO
  {
  public:
    typedef SharedPtr<AMTIForcePlatformFileIO> Pointer;
    typedef SharedPtr<const AMTIForcePlatformFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new AMTIForcePlatformFileIO());};
    
    // ~AMTIForcePlatformFileIO(); // Implicit.
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual bool CanWriteFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    BTK_IO_EXPORT virtual void Write(const std::string& filename, Acquisition::Pointer input);
    
    const std::vector<float>& GetDimensions() const {return this->m_Dimensions;};
    void SetDimensions(float width, float length, float height);
    
    const std::vector<float>& GetCorners() const {return this->m_Corners;};
    const std::vector<float>& GetOrigin() const {return this->m_Origin;};
    void SetGeometry(const std::vector<float>& corners, const std::vector<float>& origin);
    
  protected:
    BTK_IO_EXPORT AMTIForcePlatformFileIO();
    
  private:
    void ExtractLineData(std::ifstream* pifs, double* d);
    void computeGeometryFromDimensions(const std::vector<float>& d, std::vector<float>& c, std::vector<float>& o);
    
    AMTIForcePlatformFileIO(const AMTIForcePlatformFileIO& ); // Not implemented.
    AMTIForcePlatformFileIO& operator=(const AMTIForcePlatformFileIO& ); // Not implemented. 
    
    bool m_UseDimensions;
    std::vector<float> m_Dimensions;
    std::vector<float> m_Corners;
    std::vector<float> m_Origin;
  };
};

#endif // __btkAMTIForcePlatformFileIO_h
