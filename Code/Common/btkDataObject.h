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

#ifndef __btkDataObject_h
#define __btkDataObject_h

#include "btkObject.h"

namespace btk
{
  class ProcessObject;
  
  class DataObject : public Object
  {
  public:
    typedef SharedPtr<DataObject> Pointer;
    typedef SharedPtr<const DataObject> ConstPointer;
    
    bool HasParent() const {return this->mp_Parent != 0;};
    DataObject* GetParent() const {return this->mp_Parent;};
    BTK_COMMON_EXPORT void SetParent(DataObject* parent);
    
    BTK_COMMON_EXPORT void Update();
    
  protected:
    DataObject()
    : Object()
    {
      this->mp_Parent = 0;
      this->mp_Source = 0;
    };
    virtual ~DataObject() {};

    BTK_COMMON_EXPORT void Modified();
    
  private:
    DataObject(const DataObject& ); // Not implemented.
    DataObject& operator=(const DataObject& ); // Not implemented.
    
    DataObject* mp_Parent;
    ProcessObject* mp_Source;
    
    friend class ProcessObject;
  };
};

#endif // __btkDataObject_h
