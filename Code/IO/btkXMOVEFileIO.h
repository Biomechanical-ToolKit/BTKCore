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

#ifndef __btkXMOVEFileIO_h
#define __btkXMOVEFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkException.h"

namespace btk
{
  class XMOVEFileIOException : public Exception
  {
  public:
    explicit XMOVEFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~XMOVEFileIOException() throw() {};
  };
  
  class XMOVEFileIO : public AcquisitionFileIO
  {
    BTK_IO_FILE_SUPPORTED_EXTENSIONS("XMOVE");
    BTK_IO_FILE_ONLY_READ_OPERATION;
    
  public:
    typedef SharedPtr<XMOVEFileIO> Pointer;
    typedef SharedPtr<const XMOVEFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new XMOVEFileIO());};
    
    // ~XMOVEFileIO(); // Implicit.
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    
  protected:
    BTK_IO_EXPORT XMOVEFileIO();
    
  private:
    XMOVEFileIO(const XMOVEFileIO& ); // Not implemented.
    XMOVEFileIO& operator=(const XMOVEFileIO& ); // Not implemented. 
   };
};

#endif // __btkXMOVEFileIO_h
