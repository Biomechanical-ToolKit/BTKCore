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

#ifndef __btkANCFileIO_h
#define __btkANCFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkException.h"

namespace btk
{
  class ANCFileIOException : public Exception
  {
  public:
    explicit ANCFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~ANCFileIOException() throw() {};
  };
  
  class ANCFileIO : public AcquisitionFileIO
  {
  public:
    typedef SharedPtr<ANCFileIO> Pointer;
    typedef SharedPtr<const ANCFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new ANCFileIO());};
    
    // ~ANCFileIO(); // Implicit.
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual bool CanWriteFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    BTK_IO_EXPORT virtual void Write(const std::string& filename, Acquisition::Pointer input);
    
    int GetFileGeneration() const {return this->m_Generation;};
    BTK_IO_EXPORT void SetFileGeneration(int gen) {this->m_Generation = gen;};
    
  protected:
    BTK_IO_EXPORT ANCFileIO();
    
  private:
    std::string ExtractKeywordValue(const std::string& line, const std::string& keyword) const;
    void ExtractDataInfo(const std::string& line, const std::string& keyword, std::list<std::string>& info) const;
    
    ANCFileIO(const ANCFileIO& ); // Not implemented.
    ANCFileIO& operator=(const ANCFileIO& ); // Not implemented. 
    
    int m_Generation;
   };
};

#endif // __btkANCFileIO_h
