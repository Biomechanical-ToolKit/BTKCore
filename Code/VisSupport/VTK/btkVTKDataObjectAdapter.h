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

#ifndef __btkVTKDataObjectAdapter_h
#define __btkVTKDataObjectAdapter_h

#include "btkDataObject.h"

#include <vtkDataObject.h>

namespace btk
{
  class VTKDataObjectAdapter : public vtkDataObject
  {
  public:
    BTK_VTK_EXPORT static VTKDataObjectAdapter* New();
    vtkExportedTypeRevisionMacro(VTKDataObjectAdapter, vtkDataObject, BTK_VTK_EXPORT);
    BTK_VTK_EXPORT void PrintSelf(ostream& os, vtkIndent indent);
    
    BTK_VTK_EXPORT virtual unsigned long int GetMTime();
    
    DataObject::Pointer GetBTKDataObject() {return this->mp_Data;};
    BTK_VTK_EXPORT void SetBTKDataObject(DataObject::Pointer data);
    
  protected:
    BTK_VTK_EXPORT VTKDataObjectAdapter();
    ~VTKDataObjectAdapter() {};
    
  private:    
    VTKDataObjectAdapter(const VTKDataObjectAdapter& );  // Not implemented.
    VTKDataObjectAdapter& operator=(const VTKDataObjectAdapter& );  // Not implemented.

    DataObject::Pointer mp_Data;
    unsigned long int m_LastKnownTimestamp;

  };
};

#endif // __btkVTKDataObjectAdapter_h
