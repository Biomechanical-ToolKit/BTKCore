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

#ifndef __btkConvert_h
#define __btkConvert_h

#include "btkException.h"

#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

#ifdef _MSC_VER
  #include "Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

// This code is largely inspired of http://www.parashift.com/c++-faq-lite/misc-technical-issues.html

namespace btk
{
  /**
   * @brief Bad conversion exception class.
   */
  class ConversionError : public Exception 
  {
  public:
    /** Constructor. */
    explicit ConversionError(const std::string& msg)
    : Exception(msg)
    {}
    
    /** Destructor. */
    virtual ~ConversionError() throw() {}
  };
  
  
  // --------------- FromString methods ---------------
  
  /**
   * General method to convert the string @a source to the wanted @a target.
   */
  template<typename T> // T: Target
  inline void FromString(const std::string& source, T& target)
  {
    std::istringstream iss(source);
    if (!(iss >> target))
      throw ConversionError("Error during type conversion from a string");
  };
  
  /**
   * Convenient method to convert a vector of string to the vector of wanted type T.
   */
  template <typename T>
  inline void FromString(const std::vector<std::string>& source, std::vector<T>& target)
  {
    target.clear();
    target.resize(source.size());
    size_t inc = 0;
    while (inc < source.size())
    {
      FromString(source[inc], target[inc]);
      ++inc;
    }
  };
  
  /**
   * Convenient method to convert the string @a source to the wanted type T.
   */
  template<typename T> // T: Target
  inline T FromString(const std::string& source)
  {
    T target;
    FromString(source, target);
    return target;
  };
  
  // --------------- ToString methods ---------------
  
  /**
   * Stringify the @a source.
   */
  template<typename S> // S: Source
  inline std::string ToString(const S& source)
  {
    std::ostringstream oss;
    if (!(oss << source))
      throw ConversionError("Error during stringification");
    return oss.str();
  };
  
  /**
   * Conveniant method to stringify the vector @a source to a vector of string.
   */
  template <typename S>
  inline void ToString(const std::vector<S>& source, std::vector<std::string>& target)
  {
    target.clear();
    target.resize(source.size());
    size_t inc = 0;
    while (inc < source.size())
    {
      target[inc] = ToString(source[inc]);
      ++inc;  
    }
  };
  
  // --------------- Explicit specialization ---------------
  
  /**
   * Explicit method to stringify a string This method only return @a toConvert.
   */
  template<>
  inline std::string ToString<std::string>(const std::string& toConvert)
  {
    return toConvert;
  }
  
  /**
   * Explicit method to stringify a boolean.
   */
  template<>
  inline std::string ToString<bool>(const bool& toConvert)
  {
    std::ostringstream oss;
    if (!(oss << std::boolalpha << toConvert))
      throw ConversionError("Error during stringification");
    return oss.str();
  }
  
  /**
   * Explicit method to stringify an int8_t (string the decimal number and not the hex value).
   */
  template<>
  inline std::string ToString<int8_t>(const int8_t& toConvert)
  {
    std::ostringstream oss;
    if (!(oss << static_cast<int16_t>(toConvert)))
      throw ConversionError("Error during stringification");
    return oss.str();
  }
  
  /**
   * Explicit method to stringify a float. The precision of the stringification 
   * is std::numeric_limits<float>::digits10.
   */
  template<>
  inline std::string ToString<float>(const float& toConvert)
  {
    const int precision = std::numeric_limits<float>::digits10;
    std::ostringstream oss;
    if (!(oss << std::setprecision(precision) << toConvert))
      throw ConversionError("Error during stringification");
    return oss.str();
  }
  
  /**
   * Explicit method to stringify a double. The precision of the stringification 
   * is std::numeric_limits<double>::digits10.
   */
  template<>
  inline std::string ToString<double>(const double& toConvert)
  {
    const int precision = std::numeric_limits<double>::digits10;
    std::ostringstream oss;
    if (!(oss << std::setprecision(precision) << toConvert))
      throw ConversionError("Error during stringification");
    return oss.str();
  }
  
  /**
   * Explicit method to stringify a long double. The precision of the stringification 
   * is std::numeric_limits<long double>::digits10.
   */
  template<>
  inline std::string ToString<long double>(const long double& toConvert)
  {
    const int precision = std::numeric_limits<long double>::digits10;
    std::ostringstream oss;
    if (!(oss << std::setprecision(precision) << toConvert))
      throw ConversionError("Error during stringification");
    return oss.str();
  }
}

#endif // __btkConvert_h
