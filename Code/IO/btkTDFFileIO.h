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

#ifndef __btkTDFFileIO_h
#define __btkTDFFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkException.h"

#include <list>

namespace btk
{
  class IEEELittleEndianBinaryFileStream;
  
  class TDFFileIOException : public Exception
  {
  public:
    explicit TDFFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~TDFFileIOException() throw() {};
  };
  
  class TDFFileIO : public AcquisitionFileIO
  {
    BTK_FILE_IO_SUPPORTED_EXTENSIONS("TDF");
    BTK_FILE_IO_ONLY_READ_OPERATION;
    
  public:
    typedef btkSharedPtr<TDFFileIO> Pointer;
    typedef btkSharedPtr<const TDFFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new TDFFileIO());};
    
    // ~TDFFileIO(); // Implicit.
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    
  protected:
    BTK_IO_EXPORT TDFFileIO();
    
  private:
    struct BlockEntry
    {
      uint32_t type;
      uint32_t format;
      int32_t offset;
      int32_t size;
    };
    
    const BlockEntry* SeekToBlock(IEEELittleEndianBinaryFileStream* bifs, const std::list<BlockEntry>* blockEntries, unsigned int id) const;
    std::string& CleanLabel(std::string& label) const;
    
    TDFFileIO(const TDFFileIO& ); // Not implemented.
    TDFFileIO& operator=(const TDFFileIO& ); // Not implemented. 
   };
   
   inline std::string& TDFFileIO::CleanLabel(std::string& label) const
   {
     size_t nullpos = label.find_first_of('\0');
     if (nullpos != std::string::npos)
       label = label.substr(0, nullpos);
     btkTrimString(&label);
     return label;
   }
};

#endif // __btkTDFFileIO_h
