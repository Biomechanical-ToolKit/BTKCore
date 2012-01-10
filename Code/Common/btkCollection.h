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

#ifndef __btkCollection_h
#define __btkCollection_h

#include "btkDataObject.h"
#include "btkException.h"
#include "btkMacro.h"

#include <list>

namespace btk
{
  template <class T>
  class Collection : public DataObject
  {
  public:
    typedef typename SharedPtr<Collection> Pointer;
    typedef typename SharedPtr<const Collection> ConstPointer;
    
    typedef typename T::Pointer ItemPointer;
    typedef typename T::ConstPointer ItemConstPointer;    
    
    typedef typename std::list<ItemPointer>::iterator Iterator;
    typedef typename std::list<ItemPointer>::const_iterator ConstIterator;
    
    static Pointer New() {return Pointer(new Collection());};
    
    virtual ~Collection() {};
    
    Iterator Begin() {return this->m_Items.begin();};
    ConstIterator Begin() const {return this->m_Items.begin();};
    Iterator End() {return this->m_Items.end();};
    ConstIterator End() const {return this->m_Items.end();};
   
    bool IsEmpty() const {return this->m_Items.empty();}; 
    int GetItemNumber() const {return static_cast<int>(this->m_Items.size());};
    void SetItemNumber(int num);

    int GetIndexOf(ItemPointer elt) const;
    ItemPointer GetItem(int idx);
    ItemConstPointer GetItem(int idx) const;
    bool InsertItem(Iterator loc, ItemPointer elt);
    bool InsertItem(int idx, ItemPointer elt);
    bool InsertItem(ItemPointer elt) {return this->InsertItem(this->End(), elt);};
    bool SetItem(int idx, ItemPointer elt);
    Iterator RemoveItem(Iterator loc);
    void RemoveItem(int idx);
    ItemPointer TakeItem(Iterator loc);
    ItemPointer TakeItem(int idx);
    void Clear() {this->m_Items.clear();};
    Pointer Clone() const;
    
  protected:
    Collection()
    : DataObject(), m_Items()
    {};
    
  private:
    Collection(const Collection& ); // Not implemented.
    Collection& operator=(const Collection& ); // Not implemented.
    
    std::list<ItemPointer> m_Items;
  };
  
  /**
   * @class Collection btkCollection.h
   * @brief List of objects.
   *  
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef Collection<T>::Pointer
   * Smart pointer associated with a Collection object.
   */
  
  /**
   * @typedef Collection<T>::ConstPointer
   * Smart pointer associated with a const Collection object.
   */
  
  /**
   * @typedef Collection<T>::ItemPointer
   * Smart pointer associated with a T object.
   */
  
  /**
   * @typedef Collection<T>::ItemConstPointer
   * Smart pointer associated with a const T object.
   */
  
  /**
   * @typedef Collection<T>::Iterator
   * Iterator for items contained in the Collection object.
   */
  
  /**
   * @typedef Collection<T>::ConstIterator
   * Const iterator for items contained in the Collection object.
   */
  
  /**
   * @fn template <class T> static Pointer Collection<T>::New();
   * Creates a smart pointer associated with a Collection<T> object.
   */
  
  /**
   * @fn template <class T> virtual Collection<T>::~Collection()
   * Empty destructor.
   */
  
  /**
   * @fn template <class T> Collection<T>::Iterator Collection<T>::Begin()
   *  Returns an iterator to the beginning of the list of items.
   */
  
  /**
   * @fn template <class T> Collection<T>::ConstIterator Collection<T>::Begin() const
   *  Returns a const iterator to the beginning of the list of items.
   */
  
  /**
   * @fn template <class T> Collection<T>::Iterator Collection<T>::End()
   * Returns an iterator just past the last item.
   */
  
  /**
   * @fn template <class T> Collection<T>::ConstIterator Collection<T>::End() const
   * Returns a const iterator just past the last item.
   */

  /**
   * @fn template <class T> bool Collection<T>::IsEmpty() const
   * Returns true if the collection is empty.
   */
  
  /**
   * @fn template <class T> int Collection<T>::GetItemNumber() const
   * Returns the number of items in the list.
   */
  
  /**
   * Sets the number of items in the list.
   */
  template <class T>
  void Collection<T>::SetItemNumber(int num)
  {
    if (num == this->GetItemNumber())
      return;
    this->m_Items.resize(num);
    this->Modified();
  };
  
  /**
   * Return the index associated with the item @c elt or -1 if not found
   */
  template <class T>
  int Collection<T>::GetIndexOf(ItemPointer elt) const
  {
    int idx = 0;
    for (ConstIterator it = this->Begin() ; it != this->End() ; ++it)
    {
      if (it->get() == elt.get())
        return idx;
      ++idx;
    }
    return -1;
  };
  
  /**
   * Returns a smart pointer of the object located at the index @a idx.
   */
  template <class T>
  typename T::Pointer Collection<T>::GetItem(int idx)
  {
    if (idx >= this->GetItemNumber())
      throw(OutOfRangeException("Collection<T>::GetItem"));
    Iterator it = this->Begin();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Returns a const smart pointer of the object located at the index @a idx.
   */
  template <class T>
  typename T::ConstPointer Collection<T>::GetItem(int idx) const
  {
    if (idx >= this->GetItemNumber())
      throw(OutOfRangeException("Collection<T>::GetItem"));
    ConstIterator it = this->Begin();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Inserts the smart pointer @a elt in the list at the location @a loc.
   * @return True if @a elt has been inserted or false otherwise.
   */
  template <class T>
  bool Collection<T>::InsertItem(Iterator loc, ItemPointer elt)
  {
    if (elt.get() == 0)
    {
      btkErrorMacro("Impossible to insert an empty entry");
      return false;
    }
    this->m_Items.insert(loc, elt);
    this->Modified();
    return true;
  };
  
  /**
   * Inserts the smart pointer @a elt in the list at the index @a idx.
   * @return True if @a elt has been inserted or false otherwise.
   */
  template <class T>
  bool Collection<T>::InsertItem(int idx, ItemPointer elt)
  {
    Iterator it = this->Begin();
    if (idx > static_cast<int>(this->m_Items.size()))
    {
      btkErrorMacro("Out of range, the entry is appended");
      it = this->End();
    }
    else
      std::advance(it, idx);
    return this->InsertItem(it, elt);
  };
  
  /**
   * @fn template <class T> bool Collection<T>::InsertItem(ItemPointer elt)
   * Inserts the smart pointer @a elt at the end of the list.
   * @return True if @a elt has been inserted or false otherwise.
   */
  
  
  /**
   * Sets the smart pointer @a elt in the list at the index @a idx.
   * @return True if @a elt has been assigned or false otherwise.
   */
  template <class T>
  bool Collection<T>::SetItem(int idx, ItemPointer elt)
  {
    if (elt.get() == 0)
    {
      btkErrorMacro("Impossible to set an empty entry");
      return false;
    }
    if (idx >= static_cast<int>(this->m_Items.size()))
    {
      btkErrorMacro("Out of range");
      return false;
    }
    Iterator it = this->Begin();
    std::advance(it, idx);
    *it = elt;
    this->Modified();
    return true;
  };
  
  /**
   * Removes the item at the location @a loc.
   */
  template <class T>
  typename std::list<typename T::Pointer>::iterator Collection<T>::RemoveItem(Iterator loc)
  {
    if (loc == this->End())
    {
      btkErrorMacro("Out of range");
      return loc;
    }
    Iterator it = this->m_Items.erase(loc);
    this->Modified();
    return it;
  };
  
  /**
   * Removes the item at the index @a idx.
   */
  template <class T>
  void Collection<T>::RemoveItem(int idx)
  {
    if (idx >= static_cast<int>(this->m_Items.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    Iterator it = this->Begin();
    std::advance(it, idx);
    this->m_Items.erase(it);
    this->Modified();
  };
  
  /**
   * Removes the item at the location @a loc and return it.
   */
  template <class T>
  typename T::Pointer Collection<T>::TakeItem(Iterator loc)
  {
    if (loc == this->End())
    {
      btkErrorMacro("Out of range");
      return ItemPointer();
    }
    ItemPointer p = *loc;
    this->m_Items.erase(loc);
    this->Modified();
    return p;
  };
  
  /**
   * Removes the item at the index @a idx and return it.
   */
  template <class T>
  typename T::Pointer Collection<T>::TakeItem(int idx)
  {
    if (idx >= static_cast<int>(this->m_Items.size()))
    {
      btkErrorMacro("Out of range");
      return ItemPointer();
    }
    Iterator it = this->Begin();
    std::advance(it, idx);
    ItemPointer p = *it;
    this->m_Items.erase(it);
    this->Modified();
    return p;
  };
  
  /**
   * @fn template <class T> void Collection<T>::Clear()
   * Clear the contents of the list.
   */
   
  /**
   * Deep copy.
   */
  template <class T>
  typename SharedPtr< Collection<T> > Collection<T>::Clone() const
  {
    Pointer p = Pointer(new Collection());
    for (ConstIterator it = this->Begin() ; it != this->End() ; ++it)
      p->m_Items.push_back((*it)->Clone());
    return p;
  };
};

#endif // __btkCollection_h
