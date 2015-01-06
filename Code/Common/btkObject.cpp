/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include "btkObject.h"
#include "btkObject_p.h"

#include <atomic>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  ObjectPrivate::ObjectPrivate()
  : Timestamp(0)
  {};
  
  ObjectPrivate::~ObjectPrivate() = default; // Cannot be inlined
}

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class Object btkObject.h
   * @brief Base for all objects which need to keep track of modified time.
   *
   * This class uses the private implementation (pimpl) idiom to keep an ABI compatibility
   * between the different release of the project.
   *
   * Two macros are provided to simplify the implementation of the pimpl idiom.
   * Both of them are defined in the file "btkOpaque.h".
   *  - BTK_DECLARE_PIMPL_ACCESSOR: Creates pimpl() methods in the public interface to access to the private part (opaque pointer)
   *  - BTK_DECLARE_PINT_ACCESSOR: Creates pint() methods in the private implementation to access to the public part
   *
   * For example, the following header file create a new class which contains an accessor and mutator for a member 'Version' in the (opaque) private implementation.
   * The macro BTK_DECLARE_PIMPL_ACCESSOR declares private pimpl() methods used to access to the mp_Pimpl member.
   * The methods are able to downcast to the given classname. This is usefull when the implemented class inherits from a class which uses already the pimpl idiom.
   *
   * To use the macros for custom development you can follow the next examples. The first and second introduce the macros while the third introduce their use to inherit from the class btk::Object
   *
   * First the public interface is defined in a header file.
   *
   * @code
   * class MyObject
   * {
   *   BTK_DECLARE_PIMPL_ACCESSOR(MyObject) // pimpl() methods
   *   
   * public:
   *   MyObject();
   *   int version() const noexcept;
   *   void setVersion() const noexcept:
   *
   *   void print() const;
   *  
   * private:
   *   struct Private; // Forward declaration
   *   std::unique_ptr<Private> mp_Pimpl; // RAII storage
   * };
   * @endcode 
   *
   * The associated implementation file can be presented as in the following example. The MyObject::Private data structure is defined as well as the needed methods for the class MyObject.
   *
   * @code
   * struct MyObject::Private
   * {
   *   Private();
   *   int Version;
   * };
   *
   * MyObject::Private::Private()
   * : Version(1)
   * {};
   *
   * MyObject::MyObject()
   * : mp_Pimpl(new Private(this))
   * {};
   * 
   * int MyObject::version() const
   * {
   *   auto optr = this->pimpl();
   *   return optr->Version;
   * };
   * 
   * void MyObject::setVersion(int value)
   * {
   *   auto optr = this->pimpl();
   *   optr->Version = value;
   * };
   *
   * void MyObject::print() const
   * {
   *   std::cout << this->version() << std::endl;
   * };
   * @endcode
   *
   * In the following example, the macro defining the accessors of the public interface in the private implementation is used. It is also important to declare a member @a mp_Pint and a constructor with a pointer to the public interface to be useable.
   *
   * @code
   * struct MyObject::Private
   * {
   *   BTK_DECLARE_PINT_ACCESSOR(MyObject) // pint() methods
   *   Private(MyObject* pint); // Constructor with a pointer associated to the public interface object
   *   MyObject* mp_Pint;
   *   int Version;
   *   void foo(); // internal method
   * };
   *
   * MyObject::Private::Private(MyObject* pint)
   * : mp_Pint(pint), Version(1)
   * {};
   * 
   * MyObject::Private::foo()
   * {
   *   this->pint()->print(); // Call a method of the public interface.
   * }
   *
   * MyObject::MyObject()
   * : mp_Pimpl(new Private(this))
   * {};
   * 
   * int MyObject::version() const
   * {
   *   auto optr = this->pimpl();
   *   return optr->Version;
   * };
   * 
   * void MyObject::setVersion(int value)
   * {
   *   auto optr = this->pimpl();
   *   optr->Version = value;
   * };
   *
   * void MyObject::print() const
   * {
   *   std::cout << this->version() << std::endl;
   * };
   * @endcode
   *
   * Finally, to extand BTK with a new class, you can inherit from the class btk::Object (or other inherited classes). Thus it is even simpler to use the pimpl idiom. Indeed, the member mp_Pimpl is already declared in the class btk::Object.
   * You only need to inherit from the public and private part. Note, to inherit from the private part, this one is defined in its own private header file (with the suffix _p) to not have it visible and possible break the ABI compatibilty.
   *
   * @code
   * class MyObjectPrivate; // Defined outside in case it can be reused
   *
   * class MyObject : public Object
   * {
   *   BTK_DECLARE_PIMPL_ACCESSOR(MyObject)
   *   
   * public:
   *   MyObject();
   *   int version() const noexcept;
   *   void setVersion() const noexcept:
   *
   * protected:
   *   MyObject(MyObjectPrivate& pimpl) noexcept; // Only needed if this class could be inherited
   * };
   * @endcode
  
   * The private header file (e.g. MyObject_p.h) could be like this:
   *
   * @code
   * struct MyObjectPrivate : public ObjectPrivate
   * {
   *   MyObjectPrivate()
   *   int Version;
   * };
   * @endcode
   *
   * And the implementation file:
   *
   * @code
   * MyObject::MyObject()
   * : btk::Object(*new MyObjectPrivate())
   * {};
   * 
   * int MyObject::version() const
   * {
   *   auto optr = this->pimpl();
   *   return optr->Version;
   * };
   * 
   * void MyObject::setVersion(int val)
   * {
   *   auto optr = this->pimpl();
   *   if (optr->Version != val)
   *   {
   *     optr->Version = val;
   *     this->modified();
   *   }
   * };
   *
   * MyObject::MyObject(MyObjectPrivate& pimpl) noexcept
   * : Object(pimpl)
   * {};
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * Destructor
   * @note The opaque pointer representing the private implementation is automatically deleted as it is contained in a std::unique_ptr object.
   */
  Object::~Object() noexcept = default;
  
  /**
   * Returns the timestamp of the object.
   */
  unsigned long Object::timestamp() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Timestamp;
  };
  
  /**
   * Sets the object as modified (its timestamp is updated).
   * It is important to use this method each time a member of the object is modified.
   */
  void Object::modified() noexcept
  {
    auto optr = this->pimpl();
    static std::atomic<unsigned long> _btk_atomic_time{0};
    optr->Timestamp = ++_btk_atomic_time;
  };
  
  /**
   * Constructor.
   * Initialize the timestamp to 0
   */
  Object::Object()
  : mp_Pimpl(new ObjectPrivate)
  {};
  
  Object::Object(ObjectPrivate& impl) noexcept
  : mp_Pimpl(&impl)
  {};
};
