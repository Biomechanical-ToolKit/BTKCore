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
 
#ifndef __btkSwigMacro_h
#define __btkSwigMacro_h

// ------------------------------------------------------------------------- //
//                        Macros for btk<class>                              //
// ------------------------------------------------------------------------- //

#ifdef BTK_SWIG_HEADER_DECLARATION
  #define BTK_SWIG_DECLARE_CLASS(classname) \
    typedef btk::classname btk##classname##_impl; \
    typedef btk::classname::Pointer btk##classname##_shared; \
    class btk##classname : public btk##classname##_shared
    
  #define BTK_SWIG_DECLARE_CLASS_TEMPLATE(classname, template, identifier) \
    typedef btk::template< identifier > btk##classname##_impl; \
    typedef btk::template< identifier >::Pointer btk##classname##_shared; \
    class btk##classname : public btk##classname##_shared
  
  #define BTK_SWIG_DECLARE_CLASS_INHERIT(classname, inheritname) \
    BTK_SWIG_DECLARE_CLASS(classname)
    
  #define BTK_SWIG_DECLARE_DEFAULT_CTOR(classname) \
    btk##classname()
  
  #define BTK_SWIG_DECLARE_DEFAULT_INHERIT_CTOR(classname, inheritname) \
    BTK_SWIG_DECLARE_DEFAULT_CTOR(classname); \
    btk##classname(const btk##inheritname toCopy)
  
  #define BTK_SWIG_DECLARE_DEFAULT_IO_CTOR(classname) \
    BTK_SWIG_DECLARE_DEFAULT_INHERIT_CTOR(classname, AcquisitionFileIO)

  #define BTK_SWIG_DECLARE_CLONE(classname) \
    btk##classname Clone(); \
    btk##classname(const btk##classname##_shared toCopy)
  
  #define BTK_SWIG_DECLARE_POINTER_OPERATOR(classname)
#else
  #define BTK_SWIG_DECLARE_CLASS(classname) \
    class btk##classname
  
  #define BTK_SWIG_DECLARE_CLASS_TEMPLATE(classname, template, identifier) \
    BTK_SWIG_DECLARE_CLASS(classname)
  
  #define BTK_SWIG_DECLARE_CLASS_INHERIT(classname, inheritname) \
    class btk##classname : public btk##inheritname
    
  #define BTK_SWIG_DECLARE_DEFAULT_CTOR(classname) \
    btk##classname()
    
  #define BTK_SWIG_DECLARE_DEFAULT_INHERIT_CTOR(classname, inheritname) \
    BTK_SWIG_DECLARE_DEFAULT_CTOR(classname); \
    btk##classname(const btk##inheritname toCopy)

  #define BTK_SWIG_DECLARE_DEFAULT_IO_CTOR(classname) \
    BTK_SWIG_DECLARE_DEFAULT_INHERIT_CTOR(classname, AcquisitionFileIO)

  #define BTK_SWIG_DECLARE_CLONE(classname) \
    btk##classname Clone();

  #define BTK_SWIG_DECLARE_POINTER_OPERATOR(classname) \
    btk##classname##_impl* operator->() const;
#endif

#define BTK_SWIG_DECLARE_CLASS_IO(classname) \
  BTK_SWIG_DECLARE_CLASS_INHERIT(classname, AcquisitionFileIO) \
  { \
  public: \
    BTK_SWIG_DECLARE_DEFAULT_IO_CTOR(classname); \
    BTK_SWIG_DECLARE_POINTER_OPERATOR(classname); \
  }; 
  
#define BTK_SWIG_DEFINE_DEFAULT_CTOR(classname) \
  btk##classname::btk##classname() \
  : btk##classname##_shared(btk::classname::New()) \
  {};

#define BTK_SWIG_DEFINE_DEFAULT_CTOR_TEMPLATE(classname, template, identifier) \
  btk##classname::btk##classname() \
  : btk##classname##_shared(btk::template< identifier >::New()) \
  {};
  
#define BTK_SWIG_DEFINE_DEFAULT_INHERIT_CTOR(classname, inheritname) \
  BTK_SWIG_DEFINE_DEFAULT_CTOR(classname); \
  btk##classname::btk##classname(const btk##inheritname toCopy) \
  : btk##classname##_shared(static_pointer_cast<btk::classname>(toCopy)) \
  {};

#define BTK_SWIG_DEFINE_DEFAULT_IO_CTOR(classname) \
  BTK_SWIG_DEFINE_DEFAULT_INHERIT_CTOR(classname, AcquisitionFileIO)

#define BTK_SWIG_DEFINE_CLONE(classname) \
  btk##classname btk##classname::Clone() \
  { \
    return btk##classname((*this)->btk##classname##_impl::Clone()); \
  }; \
  \
  btk##classname::btk##classname(btk##classname##_shared toCopy) \
  : btk##classname##_shared(toCopy) \
  {};

#define BTK_SWIG_EXTEND_CLASS_GETSET_VECTOR(classname, method) \
  %extend btk##classname \
  { \
    double Get##method(int idx) const \
    { \
      if ((idx < 0) || (idx >= (*$self)->GetFrameNumber())) \
        throw(btk::OutOfRangeException("Index out of bounds.")); \
      return (*$self)->Get##method##s().coeff(idx); \
    }; \
    void Set##method(int idx, double v) \
    { \
      if ((idx < 0) || (idx >= (*$self)->GetFrameNumber())) \
        throw(btk::OutOfRangeException("Index out of bounds.")); \
      (*$self)->Get##method##s().coeffRef(idx) = v; \
    }; \
  }
  
#define BTK_SWIG_EXTEND_CLASS_GETSET_MATRIX(classname, method) \
  %extend btk##classname \
  { \
    double Get##method(int row, int col) const \
    { \
      if ((row < 0) || (row >= (*$self)->Get##method##s().rows()) || (col < 0) || (col >= (*$self)->Get##method##s().cols())) \
        throw(btk::OutOfRangeException("Index out of bounds.")); \
      return (*$self)->Get##method##s().coeff(row, col); \
    }; \
    void Set##method(int row, int col, double v) \
    { \
      if ((row < 0) || (row >= (*$self)->Get##method##s().rows()) || (col < 0) || (col >= (*$self)->Get##method##s().cols())) \
        throw(btk::OutOfRangeException("Index out of bounds.")); \
      (*$self)->Get##method##s().coeffRef(row, col) = v; \
    }; \
  }
  
// ------------------------------------------------------------------------- //
//                      Macros for btk<class>Iterator                        //
// ------------------------------------------------------------------------- //
  
#ifdef BTK_SWIG_HEADER_DECLARATION
  #define BTK_SWIG_DECLARE_ITERATOR(classname, elt) \
    class btk##classname##Iterator : public std::list<btk##elt##_shared>::iterator \
    { \
    public: \
      btk##classname##Iterator() : std::list<btk##elt##_shared>::iterator() {}; \
      btk##classname##Iterator(const std::list<btk##elt##_shared>::iterator& toCopy) : std::list<btk##elt##_shared>::iterator(toCopy) {}; \
      void incr() {this->operator++();}; \
      void decr() {this->operator--();}; \
      btk##elt value() {return this->operator*();}; \
      bool operator==(const btk##classname##Iterator& rhs) {return this->std::list<btk##elt##_shared>::iterator::operator==(rhs);}; \
      bool operator!=(const btk##classname##Iterator& rhs) {return !(*this == rhs);}; \
    };
#else
  #define BTK_SWIG_DECLARE_ITERATOR(classname, elt) \
    class btk##classname##Iterator \
    { \
    public: \
      btk##classname##Iterator(); \
      void incr(); \
      void decr(); \
      btk##elt value(); \
      bool operator==(const btk##classname##Iterator& rhs); \
      bool operator!=(const btk##classname##Iterator& rhs); \
    };
#endif

// ------------------------------------------------------------------------- //
//                     Macros for btk<class>Collection                       //
// ------------------------------------------------------------------------- //

#define BTK_SWIG_DECLARE_COLLECTION(elt) \
  BTK_SWIG_DECLARE_ITERATOR(elt##Collection,elt) \
  BTK_SWIG_DECLARE_CLASS(elt##Collection) \
  { \
  public: \
    BTK_SWIG_DECLARE_DEFAULT_CTOR(elt##Collection); \
    BTK_SWIG_DECLARE_CLONE(elt##Collection); \
    BTK_SWIG_DECLARE_POINTER_OPERATOR(elt##Collection); \
  };
  
// ------------------------------------------------------------------------- //
//                       Macros for btk<class>_impl                          //
// ------------------------------------------------------------------------- //

#define BTK_SWIG_DECLARE_IMPL_CLASS(classname) \
  %ignore btk##classname##_impl; \
  class btk##classname##_impl
    
#define BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(classname) \
  btk##classname##_impl();

#define BTK_SWIG_DECLARE_IMPL_GETSET(member, type) \
  const type Get##member##() const; \
  void Set##member##(const type );
  
#define BTK_SWIG_DECLARE_IMPL_CLASS_IO(classname) \
  BTK_SWIG_DECLARE_IMPL_CLASS(classname) : public btkAcquisitionFileIO_impl \
  {};
  
#define BTK_SWIG_DECLARE_IMPL_CLASS_DATA(classname) \
  BTK_SWIG_DECLARE_IMPL_CLASS(classname) : public btkDataObject_impl
  
#define BTK_SWIG_DECLARE_IMPL_CLASS_PROCESS(classname) \
  BTK_SWIG_DECLARE_IMPL_CLASS(classname) : public btkProcessObject_impl

// ------------------------------------------------------------------------- //
//                  Macros for btk<class>Collection_impl                     //
// ------------------------------------------------------------------------- //

#define BTK_SWIG_DECLARE_IMPL_COLLECTION(elt) \
  BTK_SWIG_DECLARE_IMPL_CLASS_DATA(elt##Collection) \
  { \
  public: \
    typedef btk##elt ItemPointer; \
    typedef btk##elt##CollectionIterator Iterator; \
    Iterator Begin(); \
    Iterator End(); \
    bool IsEmpty() const; \
    int GetItemNumber() const; \
    void SetItemNumber(int ); \
    int GetIndexOf(ItemPointer ) const; \
    ItemPointer GetItem(int ); \
    bool InsertItem(Iterator , ItemPointer ); \
    bool InsertItem(int , ItemPointer ); \
    bool InsertItem(ItemPointer ); \
    bool SetItem(int , ItemPointer ); \
    Iterator RemoveItem(Iterator ); \
    void RemoveItem(int ); \
    ItemPointer TakeItem(Iterator ); \
    ItemPointer TakeItem(int ); \
    void Clear(); \
  protected: \
    BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(elt##Collection); \
  };
  
// ------------------------------------------------------------------------- //
//                  Macros for extra documentation
// ------------------------------------------------------------------------- //
#define BTK_SWIG_AUTODOC(classname, method, text) \
  %feature("autodoc", text) btk##classname##::method;

#define BTK_SWIG_DOCSTRING(classname, method, text); \
  %feature("docstring")  btk##classname::method text;
  
#define BTK_SWIG_AUTODOC_IMPL(classname, method, text) \
  %feature("autodoc", text) btk##classname##_impl::method;
  
#define BTK_SWIG_DOCSTRING_IMPL(classname, method, text); \
  %feature("docstring")  btk##classname##_impl::method text;

#endif // __btkSwigMacro_h