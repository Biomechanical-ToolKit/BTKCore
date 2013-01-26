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

#ifndef __btkMultiSTLFileWriter_h
#define __btkMultiSTLFileWriter_h

#include "btkException.h"
#include "btkProcessObject.h"
#include "btkAcquisition.h"
#include "btkTriangleMesh.h"

namespace btk
{
  class MultiSTLFileWriterException : public Exception
  {
  public:
    explicit MultiSTLFileWriterException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~MultiSTLFileWriterException() throw() {};
  };
  
  class MultiSTLFileWriter : public ProcessObject
  {
  public:
    typedef SharedPtr<MultiSTLFileWriter> Pointer;
    typedef SharedPtr<const MultiSTLFileWriter> ConstPointer;
    
    virtual ~MultiSTLFileWriter() {};
    
    static Pointer New() {return Pointer(new MultiSTLFileWriter());};
    
    Acquisition::Pointer GetInputAcquisition() {return static_pointer_cast<Acquisition>(this->GetNthInput(0));};
    void SetInputAcquisition(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    
    TriangleMesh::Pointer GetInputMesh() {return static_pointer_cast<TriangleMesh>(this->GetNthInput(1));};
    void SetInputMesh(TriangleMesh::Pointer input) {this->SetNthInput(1, input);};   
     
    const std::string& GetFilePrefix() const {return this->m_FilePrefix;};
    BTK_IO_EXPORT void SetFilePrefix(const std::string& prefix);
    
    const int* GetFramesOfInterest() const {return this->m_FOI;};
    void GetFramesOfInterest(int& ff, int& lf) const {ff = this->m_FOI[0]; lf = this->m_FOI[1];};
    BTK_IO_EXPORT void SetFramesOfInterest(int ff = -1, int lf = -1);
  
  protected:
    BTK_IO_EXPORT MultiSTLFileWriter();
    
    BTK_IO_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_IO_EXPORT virtual void GenerateData();
    
  private:
    MultiSTLFileWriter(const MultiSTLFileWriter& ); // Not implemented.
    MultiSTLFileWriter& operator=(const MultiSTLFileWriter& ); // Not implemented.
    
    std::string m_FilePrefix;
    int m_FOI[2];
  };
};

#endif // __btkMultiSTLFileWriter_h
