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

#include "btkAny.h"

namespace btk
{
  // Known conversion table defined at compile time.
  
  Any::Converter::Converter()
  : Table{std::forward<Map>(merge(
      Register<bool,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<int8_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<uint8_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<int8_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<int16_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<uint16_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<int32_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<uint32_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<int64_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<uint64_t,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<float,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<double,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{},
      Register<std::string,bool,int8_t,uint8_t,int16_t,uint16_t,int32_t,uint32_t,int64_t,uint64_t,float,double,std::string>{}
    ).Table)}
  {};
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class Any btkAny.h
   * @brief Generic type to store and convert C++'s normal strict and static type.
   *
   * As the C++ language is a strong typed language it is not possible to use the 
   * same variable to hold an integer, a real, or a string.
   * In most of the cases, this is not a problem as this is a feature of the C++ language.
   * However, some applications would require to store different kind of data in a same
   * variable (like a database's fields). There are already existing solutions like
   * union keyword in the C++ language, Boost::Variant, Boost::Any, or QVariant solution.
   * However, some of them are only definable at compile time (union, Boost::Variant) 
   * or cannot freely convert stored value between different types (e.g. string to integer)
   * like Boost::Any. In case the stored type is not known at run-time and the use of the
   * run-time type information (RTTI) mechanism is discouraged, only the Qt class 
   * QVariant gives the possibity to store a generic content and convert it (if possible)
   * in the desired type. However, the use of QVariant means you need to link your code
   * the QtCore module which could be very heavy if you have a lightweight project.
   * Moreover, QVariant could support more type than desired (QColor, QFont, QByteArray, etc.)
   *
   * The class btk::Any was implemented to have a similiar behaviour than QVariant using
   * the Boost naming convention. Indeed, the purpose of btk::Any is closer of 
   * Boost::Any than Boost::Variant.
   *
   * The implementation of the class btk::Any relies heavily on the features introduced
   * in the C++11 revision of the C++ standard (constexpr, variadic templates, 
   * initializer lists, static assertions, rvalue, move, etc.).
   *
   * The internal behaviour of the class is mostly composed of two elements:
   *  - A type-erasure storage containing the data
   *  - A static table (std::unordered_map) containing all the known conversions between registered types.
   *
   * By default, the table gives the possibility to convert boolean, integers (int*_t),
   * reals (float, double) and string (std::string) between them. For example.
   *
   * @code
   * btk::Any a(5); // store an integer with the value 5
   * int ifive = a; // will contain the value 5.
   * std::string str = a; // will contain the string "5".
   * bool active = a; // will be set to true.
   * float ffive = a; // will store the value 5.0f.
   * a = "something else"; // And now 'a' contains a string
   * ifive = a; // invalid conversion, the stored value is now 0.
   * str = a; // Will contain the string "something else".
   * active = a; // Still the value is equal to true.
   * ffive = a; // Invalid conversion, the stored value is set to 0.0f
   * @endcode
   *
   * It is possible to register new types verified at compile time.
   * For this, the type must answer to two requirements:
   * - Have a valid default constructor
   * - Have an non-member equal operator
   *
   * One template specialization is at least required to define the type ID associated.
   * For example, if you want to register the class @c Date defined as:
   *
   * @code
   * struct Date
   * {
   *   int Year, Month, Day;
   *   friend bool operator==(const Date& lhs,const Date& rhs) {return ((lhs.Year == rhs.Year) && (lhs.Month == rhs.Month) && (lhs.Day == rhs.Day));};
   * };
   * @endcode
   *
   * The private class Any::Traits must be specialized to define the type ID.
   * Note: The use of the enum value Any::TraitsBase::User is important as it is used 
   * to distinguish builtin and user types. This value is used in case you want to 
   * unregister a type. Only user's type can be unregistered. The ID values available 
   * for user type are ranged between 1024 and 65535.
   *
   * @code
   * template<> struct btk::Any::Traits<Date> : btk::Any::TraitsBase
   * {
   *   enum {ID = Any::TraitsBase::User + 1}; // 1025
   * };
   * @endcode
   *
   * It is then possible to register a type using the btk::Any::Register command.
   *
   * @code
   * // Must be in a function. For example the main(). The scope of the registration
   * // is valid until the end of the program.
   * btk::Any::Register<Date, btk::converter<Date>, btk::converter<>>();
   * //                  ^^^^
   * //      Registered type
   * //                                 ^^^^^^^^^^^^^^^^
   * //        A Date stored in a btk::Any object can be
   * //      converted to these types (In this example, a
   * //  stored Date is convertible only to a Date object)
   * //                                                             ^^^^^^^^^^^^
   * //                           A Date object can be created from another type
   * //                                         (none available in this example)
   * @endcode
   *
   *
   * In case other conversions are intended. the internal converter must be 
   * specialized in consequence. For example, if a @c Date can be converted to a
   * @c std::string or created from a @c std::string, two specializations are required.
   *
   * @code
   * template<>
   * // From std::string to Date.
   * struct btk::Any::Converter::Helper<std::string,Date> : btk::Any::Converter::HelperBase<std::string,Date>
   * {
   *   // Fake conversion. The goal is only the show the specialization of the converter. 
   *   static inline Date convert(const std::string& ) {return Date{2009,05,02};};
   * };
   *
   * // From Date to std::string.
   * template<>
   * struct btk::Any::Converter::Helper<Date,std::string> : btk::Any::Converter::HelperBase<Date,std::string>
   * {
   *   static inline std::string convert(const Date& val) {return std::to_string(val.Year) + "-" + std::to_string(val.Month) + "-" + std::to_string(val.Day);};
   * };
   * @endcode 
   * 
   * The registration in the conversion table would be then:
   * @code
   * btk::Any::Register<Date, btk::converter<Date,std::string>, btk::converter<std::string>>();
   * @endcode
   *
   * In case a user type has to be unregistered (e.g. plugin unloaded), the command 
   * btk::Any::Unregister must be called.
   * @code
   * // The next line unregisters all the conversions related to a Date object. 
   * // You can still store a Date object in btk::Any, but all conversions will be 
   * // not valid.
   * btk::Any::Unregister<Date>();
   * btk::Any a(Date{2009,05,02});
   * btk::Date empty = a; // No conversion is known, the returned Date is empty.
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * Default constructor.
   * This kind of Any object is defined as null (method Any::isValid() returns true).
   */
  Any::Any() noexcept
  : mp_Storage(nullptr)
  {};

  /**
   * Copy constructor.
   * If the content of the copied object (@c other) is not null, it will be cloned (deep copy) in the copy
   */
  Any::Any(const Any& other)
  : mp_Storage(other.mp_Storage ? other.mp_Storage->clone() : nullptr)
  {};

  /**
   * Move constructor
   * The content of @c other is moved to this object. The content of @c other is then defined as null (method Any::isValid() returns true).
   */
  Any::Any(Any&& other) noexcept
  : mp_Storage(std::move(other.mp_Storage))
  {};

  /**
   * @fn template <typename U> Any::Any(const U& value);
   * Constructor which store the given @a value.
   * Internally, the type @c U is tested to verify if an ID is associated with.
   * If it is not the case an error is triggered (static assertion) at compile time.
   *
   * By default, all the following types are registered:
   *  - bool
   *  - int8_t / uint8_t
   *  - int16_t / uint16_t (short / unsigned short)
   *  - int32_t / uint32_t (integer / unsigned integer for 32-bit system)  
   *  - int64_t / uint64_t (integer / unsigned integer for 64-bit system)  
   *  - float
   *  - double
   *  - std::string
   */

  /**
   * Destrutor
   * Delete the internal data.
   */
  Any::~Any()
  {
    delete this->mp_Storage;
  };
  
  /**
   * Swap the content of two Any object.
   */
  void Any::swap(Any& other) noexcept
  {
    std::swap(this->mp_Storage, other.mp_Storage);
  };
  
  /**
   * Return true if the object as no content, otherwise true.
   */
  bool Any::isValid() const noexcept
  {
    return (this->mp_Storage != nullptr);
  };
  
  /** 
   * @fn template <typename U, typename> U Any::cast() const noexcept
   * Method to explicitely convert the content of this object to the given type.
   *
   * @code
   * btk::Any a("5");
   * std::string str = a.cast<std::string>();
   * @endcode
   *
   * @note The casted type must be registered otherwise a compiler error will be thrown (unvalid static assertion).
   *
   * @see Any::operator U()
   */
  
  /**
   * @fn template<class U> Any::operator U() const noexcept
   * Type conversion operator. Internally this operator uses the Any::cast() method.
   */

  /**
   * Copy assignement operator.
   * In case the assigned object is not this one, the previous content is deleted and replaced by a deep copy of the content of the @c other object.
   */
  Any& Any::operator=(const Any& other)
  {
    if (this != &other)
    {
      delete this->mp_Storage;
      this->mp_Storage = other.mp_Storage ? other.mp_Storage->clone() : nullptr;
    }
    return *this;
  };

  /**
   * Move assignement operator.
   * In case the assigned object is not this one, the previous content is deleted and replaced by the content of the @c other object. The @c other object is then defined as null (method Any::isValid() returns true).
   */
  Any& Any::operator=(Any&& other) noexcept
  {
    if (this != &other)
    {
      delete this->mp_Storage;
      this->mp_Storage = other.mp_Storage;
      other.mp_Storage = nullptr;
    }
    return *this;
  };
  
  /**
   * @fn template<typename U> Any& Any::operator=(const U& other);
   * Convenient assignment operator. This reduces the number of steps (and memory allocation) to assign a new value (compared to the use a constructor plus a copy constructor).
   * The same rules applies than for the constructor (i.e. the assigned type must be registered).
   */
  
  /**
   * Equal operator. Compare the content of two Any objects.
   */
  bool operator==(const Any& lhs, const Any& rhs) noexcept
  {
    return lhs.mp_Storage->compare(rhs.mp_Storage);
  };
  
 /**
  * Inequal operator. Returns the opposite of equal operator.
  */
  bool operator!=(const Any& lhs, const Any& rhs) noexcept
  {
    return !(lhs == rhs);
  };
  
  /**
   * @fn template <typename U, typename> friend inline bool operator==(const Any& lhs, const U& rhs)
   * Convenient equal operator to compare an Any object (@c lhs) with another object of type @c U (@c rhs).
   */  
    
  /**
   * template <typename U, typename> friend inline bool operator==(const U& lhs, const Any& rhs)
   * Convenient equal operator to compare an object of type @c U (@c lhs) with an Any object (@c rhs). Internally this operator does (rhs == lhs).
   */
  
  /**
   * template <typename U, typename> friend inline bool operator!=(const Any& lhs, const U& rhs)
   * Convenient inequal operator to compare an Any object (@c lhs) with another object of type @c U (@c rhs). Internally this operator take the opposite of the equal operator.
   */
  
  /**
   * template <typename U, typename> friend inline bool operator!=(const U& lhs, const Any& rhs)
   * Convenient inequal operator to compare an object of type @c U (@c lhs) with an Any object (@c rhs). Internally this operator take the opposite of the equal operator.
   */

  /**
   * Returns the conversion table used for all the Any object.
   *
   * @note The returned object is a singleton as proposed by Scott Meyers in C++11.
   */
  Any::Converter& Any::converter() noexcept
  {
    static Converter helper;
    return helper;
  };
  
  /**
   * Extract the convertion function pointer (convertoid) based on the ID used from the type source (@c sid) and the returned type (@c rid)
   * In case no function pointer was found, the returned valud is set to nullptr.
   */
  Any::Convertoid Any::extractConvertoid(short sid, short rid) noexcept
  {
    auto it = Any::converter().Table.find(Any::Converter::hash(sid,rid));
    return (it != Any::converter().Table.end()) ? it->second : nullptr;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct converter btkAny.h
   * @brief Utilitary to define conversion from/to a Any object.
   * The role of this class is to facilitate the registration a type using Any::Register. 
   */
  
  /**
   * @struct Any::Register btkAny.h
   * @brief Utilitary to register a new type in an Any object.
   */
  
 /**
  * @struct Any::Unregister btkAny.h
  * @brief Utilitary to unregister a new type in an Any object.
  */
};