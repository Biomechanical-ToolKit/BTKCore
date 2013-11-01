/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef __btkASCIIFileWriter_h
#define __btkASCIIFileWriter_h

#include "btkException.h"
#include "btkProcessObject.h"
#include "btkAcquisition.h"

#include <fstream>

namespace btk
{
  class ASCIIFileWriterException : public Exception
  {
  public:
    explicit ASCIIFileWriterException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~ASCIIFileWriterException() throw() {};
  };
  
  class ASCIIFileWriter : public ProcessObject
  {
  public:
    typedef btkSharedPtr<ASCIIFileWriter> Pointer;
    typedef btkSharedPtr<const ASCIIFileWriter> ConstPointer;
    
    virtual ~ASCIIFileWriter() {};
    
    static Pointer New() {return Pointer(new ASCIIFileWriter());};
    
    Acquisition::Pointer GetInput() {return static_pointer_cast<Acquisition>(this->GetNthInput(0));};
    void SetInput(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    
    const std::string& GetFilename() const {return this->m_Filename;};
    BTK_IO_EXPORT void SetFilename(const std::string& filename);
    
    const std::string& GetSeparator() const {return this->m_Separator;};
    BTK_IO_EXPORT void SetSeparator(const std::string& sep);
    
    const int* GetFramesOfInterest() const {return this->m_FOI;};
    void GetFramesOfInterest(int& ff, int& lf) const {ff = this->m_FOI[0]; lf = this->m_FOI[1];};
    BTK_IO_EXPORT void SetFramesOfInterest(int ff = -1, int lf = -1);
  
  protected:
    BTK_IO_EXPORT ASCIIFileWriter();
    
    BTK_IO_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_IO_EXPORT virtual void GenerateData();
    
  private:
    void WritePoints(std::ofstream* ofs, int ff, int lf, Acquisition::Pointer acq, PointCollection::Pointer points);
    
    ASCIIFileWriter(const ASCIIFileWriter& ); // Not implemented.
    ASCIIFileWriter& operator=(const ASCIIFileWriter& ); // Not implemented.
    
    std::string m_Filename;
    std::string m_Separator;
    int m_FOI[2];
  };
};

#endif // __btkASCIIFileWriter_h
