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

#ifndef __btkMEXObjectHandle_h
#define __btkMEXObjectHandle_h

#include "btkMEXClassID.h"

#include <btkSharedPtr.h>
#include <list>

#include "btkMex.h" // Must be the last included header

namespace btk
{
  template<typename T> class MEXHandleCollector;

  template <typename T>
  class MEXObjectHandle 
  {
  public:
    static MEXObjectHandle* FromMEXHandle(const mxArray* ma);

    MEXObjectHandle(btkSharedPtr<T> ptr)
    : m_Object(ptr)
    {
      // mexPrintf("MEXObjectHandle::MEXObjectHandle\n");
      this->m_ClassID = MEXClassID<T>();
      this->mp_Signature = this; 
      MEXHandleCollector<T>::RegisterHandle(this);
    };

    ~MEXObjectHandle()
    {
      // mexPrintf("MEXObjectHandle::~MEXObjectHandle\n");
      this->mp_Signature = 0;
    };

    mxArray* ToMEXHandle(); 
    btkSharedPtr<T> GetObject() const {return this->m_Object;};  

  private:
    MEXObjectHandle* mp_Signature;
    int m_ClassID;
    btkSharedPtr<T> m_Object;

    friend class MEXHandleCollector<T>;
  };

  template <typename T>
  class MEXHandleCollector
  {
  public:
    static MEXHandleCollector& Singleton() 
    {
      static MEXHandleCollector<T> singleton;
      return singleton;
    };
    
    static void RegisterHandle(MEXObjectHandle<T>* obj)
    {
      MEXHandleCollector::Singleton().m_Objects.push_back(obj);
    };
    
    static void RemoveHandle(MEXObjectHandle<T>* obj)
    {
      MEXHandleCollector& c = MEXHandleCollector::Singleton();
      for(typename std::list<btk::MEXObjectHandle<T>*>::iterator it = c.m_Objects.begin() ; it != c.m_Objects.end() ; ++it)
      {
        if (*it == obj)
        {
          c.m_Objects.erase(it);
          break;
        }
      }
    };
    
    static void ManualClear()
    {
      MEXHandleCollector& singleton = MEXHandleCollector::Singleton();
      MEXHandleCollector::Clear(&singleton);
    };
    
    ~MEXHandleCollector()
    {
      MEXHandleCollector::Clear(this);
    };

  private:
    static inline void Clear(MEXHandleCollector* c)
    {
      for(typename std::list<MEXObjectHandle<T>*>::iterator it = c->m_Objects.begin() ; it != c->m_Objects.end() ; ++it)
      {
        if ((*it)->mp_Signature == *it) // check for valid signature
          delete *it;
      }
      c->m_Objects.clear();
    };
    
    MEXHandleCollector() : m_Objects() {};
    MEXHandleCollector(const MEXHandleCollector& ); // Not implemented;

    std::list<MEXObjectHandle<T>*> m_Objects;
  };

  /**
   * @class MEXObjectHandle
   * @brief Handle class for MEX-Files functions.
   *
   * This class (code and documentation) is largely inspired by the 
   * %ObjectHandle class of Tim Bailey (2004). The btk::MEXObjectHandle 
   * class is designed to handle object embedded in a btkSharedPtr smart pointer.
   *
   * @ingroup BTKWrappingMatlab
   */

  /**
   * Convert the btk::MEXObjectHandle<T> to a mxArray handle (to pass back from mex-function).
   */
  template <typename T>
  mxArray* MEXObjectHandle<T>::ToMEXHandle() 
  {
	  mxArray* handle  = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);

	  *static_cast<MEXObjectHandle<T>**>(mxGetData(handle)) = this;
	  return handle;
  };
  
  /**
   * @fn SharedPtr<T> MEXObjectHandle::GetObject() const
   * Returns the object handled.
   */

  /**
   * @fn MEXObjectHandle::MEXObjectHandle(SharedPtr<T> ptr)
   * Constructor. Handle takes a copy of the object @a ptr as it is a shared pointer.
   */

  /**
   * @fn MEXObjectHandle::~MEXObjectHandle()
   * Destructor.
   */

  /**
   * Convert mxArray (passed to mex-function) to a btk::MEXObjectHandle<T>.
   */
  template <typename T>
  MEXObjectHandle<T>* MEXObjectHandle<T>::FromMEXHandle(const mxArray* handle) 
  {
    if (mxGetClassID(handle) != mxDOUBLE_CLASS
        || mxIsComplex(handle) || mxGetM(handle)!=1 || mxGetN(handle)!=1)
      mexErrMsgTxt("Parameter is not a MEXObjectHandle type.");
	  
    MEXObjectHandle* obj = *static_cast<MEXObjectHandle<T>**>(mxGetData(handle));

    if (!obj) // gross check to see we don't have an invalid pointer
      mexErrMsgTxt("Parameter is NULL. It does not represent a MEXObjectHandle object.");
	  
    if (obj->mp_Signature != obj) // check memory has correct signature
      mexErrMsgTxt("Parameter does not represent a MEXObjectHandle object.");
	  
    if (obj->m_ClassID != MEXClassID<T>()) // check type 
    {
      mexPrintf("Given: <%i>, Required: <%i>.\n", obj->m_ClassID, MEXClassID<T>());
      mexErrMsgTxt("Given MEXObjectHandle does not represent the correct type.");
    }

    return obj;
  };

  /**
   * @class MEXHandleCollector
   * @brief Garbage collector.
   *
   * Garbage collection singleton (one collector object for each type T).
   * Ensures that registered handles are deleted when the ressources are 
   * released (they may also be deleted previously without problem).
   * The MEXHandleCollector provides protection against resource leaks in the 
   * case where 'clear all' is called in Matlab (This is because Matlab will
   * call the destructors of statically allocated objects but not free-store
   * allocated objects).
   *
   * @ingroup BTKWrappingMatlab
   */

  /**
   * @fn static void MEXHandleCollector::RegisterHandle(MEXObjectHandle<T>* obj)
   * Push back the handle @a obj.
   */

  /**
   * @fn MEXHandleCollector::~MEXHandleCollector()
   * Destructor. Delete the handles and clear the list.
   */
};

/**
 * Helper function to create mex handle to object @a ptr. 
 * This function remove the need to deal with MEXObjectHandle<T> 
 * class directly for most common operations.
 *
 * @ingroup BTKWrappingMatlab
 */
template <typename T>
mxArray* btk_MOH_create_handle(btkSharedPtr<T> ptr)
{
  btk::MEXObjectHandle<T>* handle = new btk::MEXObjectHandle<T>(ptr);
  return handle->ToMEXHandle();
};

/**
 * Helper function to obtain object represented by handle. 
 * This function remove the need to deal with MEXObjectHandle<T> 
 * class directly for most common operations.
 *
 * @ingroup BTKWrappingMatlab
 */

template <typename T>
btkSharedPtr<T> btk_MOH_get_object(const mxArray* mxh)
{
  btk::MEXObjectHandle<T>* handle = btk::MEXObjectHandle<T>::FromMEXHandle(mxh);
  return handle->GetObject();
};

/**
 * Helper function to delete handle and its object. 
 * This function can be used when you want to delete manualy the 
 * handle rather than leaving it to garbage collection.
 * This function remove the need to deal with MEXObjectHandle<T> 
 * class directly for most common operations.
 *
 * @ingroup BTKWrappingMatlab
 */
template <typename T>
void btk_MOH_destroy_handle(const mxArray* mxh)
{
  btk::MEXObjectHandle<T>* handle = btk::MEXObjectHandle<T>::FromMEXHandle(mxh);
  btk::MEXHandleCollector<T>::RemoveHandle(handle);
  delete handle;
};

#endif  // __btkMEXObjectHandle_h
