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

#include "btkAny.h"

namespace btk
{
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
   * The internal behaviour of the class is mostly composed of three elements:
   *  - A type-erasure storage containing the data
   *  - A cast() method implemented differently depending the type requested (using the SFINAE technique)
   *  - A static table (std::unordered_map) containing all the custom conversions between registered types.
   *
   * By default, all the arithmetic types (bool, integers, floating points) and 
   * the std::string type are convertible between them
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
   * For this, the type must answer two requirements:
   * - Have a valid default constructor
   * - Have an non-member equal operator
   *
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
   * It is then possible to register it using the btk::Any::Register command.
   *
   * @code
   * // Must be in a function. For example the main(). The scope of the registration
   * // is valid until the end of the program.
   * btk::Any::Register<Date, btk::converter<>, btk::converter<>>();
   * //                 ^^^^
   * //      Registered type
   * //                       ^^^^^^^^^^^^^^^^
   * // A Date stored in a btk::Any object can be
   * //                  converted to these types
   * //          (none available in this example)
   * //                                         ^^^^^^^^^^^^^^^^
   * //           A Date object can be created from another type
   * //                         (none available in this example)
   * @endcode
   *
   * When conversions are intended. the internal converter must be 
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
   * // You can still store a Date object in btk::Any, but only a conversion to
   * // it its own type will be valid
   * btk::Any::Unregister<Date>();
   * btk::Any a(Date{2009,05,02});
   * btk::Date b = a; // 2009,05,02
   * std::string c = a; // empty string
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * Default constructor.
   * This kind of Any object is defined as null (method Any::isValid() returns true).
   */
  Any::Any() _BTK_NOEXCEPT
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
  Any::Any(Any&& other) _BTK_NOEXCEPT
  : mp_Storage(std::move(other.mp_Storage))
  {};

  /**
   * @fn template <typename U> Any::Any(const U& value);
   * Constructor which store the given @a value.
   *
   * You can store any kind of type. But the conversion between types is only available if the stored type correspoinds to an arithmetic type, or a std::string, or if you register some custom conversion.
   */

  /**
   * Destrutor
   * Delete the internal data.
   */
  Any::~Any()
  {
    delete this->mp_Storage;
  };
  
  std::vector<size_t> Any::dimensions() const _BTK_NOEXCEPT
  {
    return this->mp_Storage->dimensions();
  };

  size_t Any::size() const _BTK_NOEXCEPT
  {
    return this->mp_Storage->size();
  };
  
  /**
   * Return true if the object as no content, otherwise true.
   */
  bool Any::isValid() const _BTK_NOEXCEPT
  {
    return (this->mp_Storage != nullptr);
  };
 
  /**
   * Swap the content of two Any object.
   */
  void Any::swap(Any& other) _BTK_NOEXCEPT
  {
    std::swap(this->mp_Storage, other.mp_Storage);
  };
  
  /**
   * template <typename U, typename> inline bool Any::isEqual(U&& value) const _BTK_NOEXCEPT
   * Convenient method to compare the content of an Any object with the given value.
   */
  
  /**
   * template <typename U, typename> inline bool Any::assign(U&& value) const _BTK_NOEXCEPT
   * Convenient method to assign a value to an Any object.
   */
  
  /** 
   * @fn template <typename U, typename> U Any::cast() const _BTK_NOEXCEPT
   * Method to explicitely convert the content of this object to the given type.
   *
   * @code
   * btk::Any a("5");
   * std::string str = a.cast<std::string>();
   * @endcode
   *
   * @see Any::operator U()
   */
  
  /**
   * @fn template<class U> Any::operator U() const _BTK_NOEXCEPT
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
  Any& Any::operator=(Any&& other) _BTK_NOEXCEPT
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
   * Equal operator. Compare the content of two Any objects.
   */
  bool operator==(const Any& lhs, const Any& rhs) _BTK_NOEXCEPT
  {
    return ((lhs.mp_Storage != nullptr) && (rhs.mp_Storage != nullptr) && lhs.mp_Storage->compare(rhs.mp_Storage));
  };
  
 /**
  * Inequal operator. Returns the opposite of equal operator.
  */
  bool operator!=(const Any& lhs, const Any& rhs) _BTK_NOEXCEPT
  {
    return !(lhs == rhs);
  };
  
  /**
   * @fn template <typename U> friend inline bool operator==(const Any& lhs, const U& rhs)
   * Convenient equal operator to compare an Any object (@c lhs) with another object of type @c U (@c rhs).
   */  
    
  /**
   * template <typename U> friend inline bool operator==(const U& lhs, const Any& rhs)
   * Convenient equal operator to compare an object of type @c U (@c lhs) with an Any object (@c rhs). Internally this operator does (rhs == lhs).
   */
  
  /**
   * template <typename U> friend inline bool operator!=(const Any& lhs, const U& rhs)
   * Convenient inequal operator to compare an Any object (@c lhs) with another object of type @c U (@c rhs). Internally this operator take the opposite of the equal operator.
   */
  
  /**
   * template <typename U> friend inline bool operator!=(const U& lhs, const Any& rhs)
   * Convenient inequal operator to compare an object of type @c U (@c lhs) with an Any object (@c rhs). Internally this operator take the opposite of the equal operator.
   */

  // ----------------------------------------------------------------------- //

  namespace __details
  {
    /**
     * Returns the conversion table used for all the Any object.
     *
     * @note The returned object is a singleton as proposed by Scott Meyers in C++11.
     */
    _Any_converter_map& _any_converter_map() _BTK_NOEXCEPT
    {
      static _Any_converter_map table;
      return table;
    };
  
    /**
     * Extract the convertion function pointer based on the ID used from the type source (@c sid) and the returned type (@c rid)
     * In case no function pointer was found, the returned value is set to nullptr.
     */
    _Any_convert_t _any_extract_converter(typeid_t sid, typeid_t rid) _BTK_NOEXCEPT
    {
      auto it = _any_converter_map().find(_any_hash(static_cast<size_t>(sid),static_cast<size_t>(rid)));
      return (it != _any_converter_map().end()) ? it->second : nullptr;
    };
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct Any::Conversion btkAny.h
   * @brief Utilitary to define conversion from/to a Any object.
   * The role of this class is to facilitate the registration of a type using Any::Register. 
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