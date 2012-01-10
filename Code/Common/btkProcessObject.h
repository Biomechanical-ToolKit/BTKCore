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

#ifndef __btkProcessObject_h
#define __btkProcessObject_h

#include "btkMacro.h"
#include "btkObject.h"
#include "btkDataObject.h"

#include <vector>

namespace btk
{
  class ProcessObject : public Object
  {
  public:
    typedef SharedPtr<ProcessObject> Pointer;
    typedef SharedPtr<const ProcessObject> ConstPointer;
    
    int GetInputNumber() const {return static_cast<int>(this->m_Inputs.size());};
    BTK_COMMON_EXPORT int GetValidInputNumber() const;
    
    int GetOutputNumber() const {return static_cast<int>(m_Outputs.size());};
    
    BTK_COMMON_EXPORT void Update();
    BTK_COMMON_EXPORT void ResetState();
    
  protected:
    BTK_COMMON_EXPORT ProcessObject();
    BTK_COMMON_EXPORT virtual ~ProcessObject();
    
    BTK_COMMON_EXPORT DataObject::Pointer GetNthInput(int idx);
    BTK_COMMON_EXPORT DataObject::ConstPointer GetNthInput(int idx) const;
    BTK_COMMON_EXPORT int GetInputIndex(DataObject::Pointer input);
    BTK_COMMON_EXPORT virtual void SetNthInput(int idx, DataObject::Pointer input);
    BTK_COMMON_EXPORT void SetInputNumber(int num);
    
    BTK_COMMON_EXPORT DataObject::Pointer GetNthOutput(int idx);
    BTK_COMMON_EXPORT DataObject::ConstPointer GetNthOutput(int idx) const;
    BTK_COMMON_EXPORT int GetOutputIndex(DataObject::Pointer output);
    BTK_COMMON_EXPORT virtual void SetNthOutput(int idx, DataObject::Pointer output);
    BTK_COMMON_EXPORT void SetOutputNumber(int num);
    
    BTK_COMMON_EXPORT void Modified();
    bool IsModified() const {return this->m_Modified;};
    BTK_COMMON_EXPORT virtual void GenerateData() = 0;
    BTK_COMMON_EXPORT virtual DataObject::Pointer MakeOutput(int idx) = 0;
    
  private:
    ProcessObject(const ProcessObject& ); // Not implemented.
    ProcessObject& operator=(const ProcessObject& ); // Not implemented.
    
    std::vector<DataObject::Pointer> m_Inputs;
    std::vector<DataObject::Pointer> m_Outputs;
    bool m_Modified;
    bool m_Updating;
  };
};

#endif // __btkProcessObject_h
