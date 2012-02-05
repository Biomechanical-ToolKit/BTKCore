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

#include "btkVTKContextScene.h"

#include <vtkObjectFactory.h>
#include <vtkContextItem.h>

#include <vtkstd/vector>

#if (VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION < 8)
// No choice to add this code as VTK define it in the CXX file.
//! @cond
class vtkContextScene::Private
{
public:
  Private()
  {
   this->itemMousePressCurrent = -1;
   this->Event.Button = -1;
   this->IsDirty = true;
  };
  ~Private()
  {
    size_t size = this->items.size();
    for (size_t i = 0; i < size; ++i)
    {
      this->items[i]->Delete();
      this->items[i] = NULL;
    }
  };
 vtkstd::vector<vtkContextItem*> items;
 vtkstd::vector<bool> itemState;
 int itemMousePressCurrent;
 vtkContextMouseEvent Event;
 bool IsDirty;
};
//! @endcond
#endif

namespace btk
{
#if (VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION < 8)
  /**
   * @class VTKContextScene
   * @brief Simple inheriting of vtkContextScene to be able to remove item from a scene in VTK 5.6
   *
   * This class is only defined as a typedef for VTK 5.8 and greater.
   */

  /**
   * @fn static VTKContextScene* VTKContextScene::New()
   * Constructs a VTKContextScene object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKContextScene);
  vtkCxxRevisionMacro(VTKContextScene, "$Revision: 0.1 $");

  /**
   * @fn virtual VTKContextScene::~VTKContextScene();
   * Destructor.
   */

  /**
   * Remove and unregister (delete) an item from the list
   * 
   * An invalid item returns false, true otherwise.
   */
  bool VTKContextScene::RemoveItem(vtkContextItem* item)
  {
    for(vtkstd::vector<vtkContextItem*>::iterator it = this->Storage->items.begin(); it != this->Storage->items.end(); ++it)
    {
      if (item == *it)
      {
        item->SetScene(NULL);
        (*it)->Delete();
        this->Storage->items.erase(it);
        return true;
      }
    }
    return false;
  };

  /**
   * Remove and unregister (delete) an item from the list by using the given index.
   * 
   * An invalid index returns false, true otherwise.
   */
  bool VTKContextScene::RemoveItem(unsigned int index)
  {
    if (index < this->Storage->items.size())
      return this->RemoveItem(this->Storage->items.at(index));
    return false;
  };

  /**
   * Clear all items from the list and unregister them.
   */
  void VTKContextScene::Clear()
  {
    for(vtkstd::vector<vtkContextItem*>::iterator it = this->Storage->items.begin(); it != this->Storage->items.end(); ++it)
      (*it)->Delete();
    this->Storage->items.clear();
  }
  
  /**
   * Constructor
   */
  VTKContextScene::VTKContextScene()
  : vtkContextScene()
  {};
  
#endif
 };