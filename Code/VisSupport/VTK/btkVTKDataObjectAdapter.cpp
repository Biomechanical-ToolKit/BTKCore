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

#include "btkVTKDataObjectAdapter.h"

#include <vtkObjectFactory.h>

namespace btk
{ 
  /**
   * @class VTKDataObjectAdapter btkVTKDataObjectAdapter.h
   * @brief Adapter to use a btk::DataObject in a VTK pipeline.
   *
   * Use the method SetBTKDataObject() to set the btk::DataObject object linked
   * with this adpater.
   * 
   * @ingroup BTKVTK
   */ 
  
  /**
   * @fn static VTKDataObjectAdapter* VTKDataObjectAdapter::New();
   * Creates a VTKDataObjectAdapter object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKDataObjectAdapter);
  vtkCxxRevisionMacro(VTKDataObjectAdapter, "$Revision: 0.1 $");

  /**
   * Prints only the Superclass information.
   */
  void VTKDataObjectAdapter::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  };
  
  /**
   * Method overloaded to use the BTK pipeline update mechanism inside the VTK 
   * update mechanism.
   */
  unsigned long int VTKDataObjectAdapter::GetMTime()
  {
    if (this->mp_Data.get() != 0)
    {
      if (this->m_LastKnownTimestamp != this->mp_Data->GetTimestamp())
      {
        this->m_LastKnownTimestamp = this->mp_Data->GetTimestamp();
        this->Modified();
      }
      else
      {
        unsigned long int result = this->mp_Data->GetTimestamp();
        this->mp_Data->Update();
        this->m_LastKnownTimestamp = this->mp_Data->GetTimestamp();
        if (result != this->m_LastKnownTimestamp)
          this->Modified();
      }
    }
    return Superclass::GetMTime();
  };
  
  /**
   * @fn DataObject::Pointer VTKDataObjectAdapter::GetBTKDataObject()
   * Returns the btk::DataObject associated.
   */
  
  /**
   * Sets the btk::DataObject associated with this adapter.
   */
  void VTKDataObjectAdapter::SetBTKDataObject(DataObject::Pointer data)
  {
    if (this->mp_Data == data)
      return;
    this->mp_Data = data;
    this->m_LastKnownTimestamp = data->GetTimestamp();
    this->Modified();
  };
  
  /**
   * Constructor.
   */
  VTKDataObjectAdapter::VTKDataObjectAdapter()
  : vtkDataObject(), mp_Data()
  {
    this->m_LastKnownTimestamp = 0;
  };
  
  /**
   * @fn VTKDataObjectAdapter::~VTKDataObjectAdapter()
   * Destructor (empty).
   */
};
