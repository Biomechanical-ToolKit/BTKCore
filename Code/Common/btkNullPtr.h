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

#ifndef __btkNullPtr_h
#define __btkNullPtr_h

#include "btkSharedPtr.h"
#include "btkMacro.h"

/**
 * @def btkNullPtr
 * Macro to be consistent with the API and more specificaly with btkSharedPtr.
 *
 * @ingroup BTKCommon
 */ 
#define btkNullPtr btk::NullPtr

namespace btk
{
  /**
   * @class NullPtr btkNullPtr.h
   * @brief To simplify the API when testing a shared pointer to know if it is null or not
   *
   * The intent of this class is facilitate the validity test of a shared pointer to know if it is null or not.
   * Each class inherithing of the class btk::DataObject will be able to test shared pointer nullity by using a simple condition as the following.
   *
   * @b Example #1
   * @code
   * // Equality operator
   * if (pX == btk::_classname_::Null)
   * {
   *   std::cerr << "Invalid data" << std::endl;
   * }
   * @endcode
   *
   * @b Example #2
   * @code
   * // Inequality operator
   * if (pX != btk::_classname_::Null)
   * {
   *   std::cout << "Valid data" << std::endl;
   * }
   * @endcode
   * 
   * The use of this class replaces the previous code which needed to know the API of the class used behind the macro btkSharedPtr.
   *
   * @ingroup BTKCommon
   */
  template <typename T>
  class NullPtr
  {
  public:
    /**
     * Empty constructor
     */
    NullPtr() {};
    /**
     * Empty copy constructor
     */
    NullPtr(const NullPtr& ) {};
    // ~NullPtr(); // Implicit
  private:
    NullPtr& operator=(const NullPtr& ); // Not implemented.
  };
}

/**
 * Equality operator to test if a (const) shared pointer is null or not.
 * @note It is not advised to use this implementation of the operator. In pratice it will requires the instantiante a NullPtr object which will be then copied by reference.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // A btk::NullPtr object is instanciated in the following condition.
 * if (an == btk::Analog::Null())
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator==(btkSharedPtr<T> lhs, const btkNullPtr<T>& rhs)
{
  btkNotUsed(rhs);
  return (lhs.get() == 0);
};

template<typename T>
inline bool operator==(btkSharedPtr<const T> lhs, const btkNullPtr<T>& rhs)
{
  btkNotUsed(rhs);
  return (lhs.get() == 0);
};

/**
 * Equality operator to test if a (const) shared pointer is null or not.
 * @note It is not advised to use this implementation of the operator. In pratice it will requires the instantiante a NullPtr object which will be then copied by reference.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // A btk::NullPtr object is instanciated in the following condition.
 * if (btk::Analog::Null() == an)
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator==(const btkNullPtr<T>& lhs, btkSharedPtr<T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() == 0);
};

template<typename T>
inline bool operator==(const btkNullPtr<T>& lhs, btkSharedPtr<const T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() == 0);
};

/**
 * Inequality operator to test if a (const) shared pointer is null or not.
 * @note It is not advised to use this implementation of the operator. In pratice it will requires the instantiante a NullPtr object which will be then copied by reference.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // A btk::NullPtr object is instanciated in the following condition.
 * if (an != btk::Analog::Null())
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator!=(btkSharedPtr<T> lhs, const btkNullPtr<T>& rhs)
{
  btkNotUsed(rhs);
  return (lhs.get() != 0);
};

template<typename T>
inline bool operator!=(btkSharedPtr<const T> lhs, const btkNullPtr<T>& rhs)
{
  btkNotUsed(rhs);
  return (lhs.get() != 0);
};

/**
 * Inequality operator to test if a (const) shared pointer is null or not.
 * @note It is not advised to use this implementation of the operator. In pratice it will requires the instantiante a NullPtr object which will be then copied by reference.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // A btk::NullPtr object is instanciated in the following condition.
 * if (btk::Analog::Null() != an)
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator!=(const btkNullPtr<T>& lhs, btkSharedPtr<T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() != 0);
};

template<typename T>
inline bool operator!=(const btkNullPtr<T>& lhs, btkSharedPtr<const T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() != 0);
};

/**
 * Equality operator to test if a (const) shared pointer is null or not.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // No btk::NullPtr object is instanciated in the following condition.
 * if (an == btk::Analog::Null)
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator==(btkSharedPtr<T> lhs, btkNullPtr<T> (*rhs)())
{
  btkNotUsed(rhs);
  return (lhs.get() == 0);
};

template<typename T>
inline bool operator==(btkSharedPtr<const T> lhs, btkNullPtr<T> (*rhs)())
{
  btkNotUsed(rhs);
  return (lhs.get() == 0);
}

/**
 * Equality operator to test if a (const) shared pointer is null or not.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // No btk::NullPtr object is instanciated in the following condition.
 * if (btk::Analog::Null == an)
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator==(btkNullPtr<T> (*lhs)(), btkSharedPtr<T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() == 0);
}

template<typename T>
inline bool operator==(btkNullPtr<T> (*lhs)(), btkSharedPtr<const T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() == 0);
}

/**
 * Inequality operator to test if a (const) shared pointer is null or not.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // No btk::NullPtr object is instanciated in the following condition.
 * if (an != btk::Analog::Null)
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator!=(btkSharedPtr<T> lhs, btkNullPtr<T> (*rhs)())
{
  btkNotUsed(rhs);
  return (lhs.get() != 0);
};

template<typename T>
inline bool operator!=(btkSharedPtr<const T> lhs, btkNullPtr<T> (*rhs)())
{
  btkNotUsed(rhs);
  return (lhs.get() != 0);
}

/**
 * Inequality operator to test if a (const) shared pointer is null or not.
 *
 * @b Example
 * @code
 * // Variable an is a btk::Analog::(Const)Pointer object.
 * // No btk::NullPtr object is instanciated in the following condition.
 * if (btk::Analog::Null != an)
 * {
 *   // Do whatever
 * }
 * @endcode
 *
 * @ingroup BTKCommon
 */
template<typename T>
inline bool operator!=(btkNullPtr<T> (*lhs)(), btkSharedPtr<T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() != 0);
}

template<typename T>
inline bool operator!=(btkNullPtr<T> (*lhs)(), btkSharedPtr<const T> rhs)
{
  btkNotUsed(lhs);
  return (rhs.get() != 0);
}

#endif // __btkNullPtr_h