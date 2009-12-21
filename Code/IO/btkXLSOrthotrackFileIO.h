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

#ifndef __btkXLSOrthotrackFileIO_h
#define __btkXLSOrthotrackFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkException.h"

namespace btk
{
  class XLSOrthotrackFileIOException : public Exception
  {
  public:
    explicit XLSOrthotrackFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~XLSOrthotrackFileIOException() throw() {};
  };
  
  class XLSOrthotrackFileIO : public AcquisitionFileIO
  {
  public:
    typedef SharedPtr<XLSOrthotrackFileIO> Pointer;
    typedef SharedPtr<const XLSOrthotrackFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new XLSOrthotrackFileIO());};
    
    // ~XLSOrthotrackFileIO(); // Implicit.
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual bool CanWriteFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    BTK_IO_EXPORT virtual void Write(const std::string& filename, Acquisition::Pointer input);
    
  protected:
    BTK_IO_EXPORT XLSOrthotrackFileIO();
    
  private:
    void AppendEvent(Acquisition::Pointer output, std::istringstream* iss, const std::string& label, const std::string& context, int id);
    void ExtractEventDetectionFlag(Acquisition::Pointer output, std::istringstream* iss, const std::string& label, const std::string& context);
    void AppendSpatiotemparalParameter(MetaData::Pointer st, std::istringstream* iss, double scale = 1.0);
    bool ExtractSpecialAngleLabel(std::string& label, const std::string& str) const;
    bool ExtractSpecialForceLabel(std::string& label, const std::string& str) const;
    
    XLSOrthotrackFileIO(const XLSOrthotrackFileIO& ); // Not implemented.
    XLSOrthotrackFileIO& operator=(const XLSOrthotrackFileIO& ); // Not implemented.
   };
};

#endif // __btkXLSOrthotrackFileIO_h
