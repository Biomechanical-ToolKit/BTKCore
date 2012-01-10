/* 
 * Parser for the Command Line (PCL)
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
 
#ifndef pcl_ValueOpt_h
#define pcl_ValueOpt_h

#include "Opt.h"

#include <sstream>

namespace pcl
{
  template <typename T>
  class ValueOpt : public Opt
  {
  public:
    ValueOpt(const std::string& name, const std::string& abbr, const std::string& desc, 
             const std::string& valDesc, bool req = true);
    // ~ValueOpt(); // Implicit destructor used
    
    const T& GetValue() const {return this->m_Value;};
    virtual void SetValue(const std::string& val);
    
  protected:
    T Convert(const std::string& source) const;
    T m_Value;
  
  private:
    ValueOpt(const ValueOpt&); // Not implemented.
    ValueOpt& operator=(const ValueOpt&); // Not implemented.
  };

  template <typename T>
  inline ValueOpt<T>::ValueOpt(const std::string& name, const std::string& abbr,
                            const std::string& desc, const std::string& valDesc, bool req)
  : Opt(name, abbr, desc, valDesc, req, true), m_Value(T())
  {};

  template <typename T>
  inline void ValueOpt<T>::SetValue(const std::string& val)
  {
    if (this->IsValueRequired() && (val.compare("") == 0))
      throw(OptionError("A value is required for the argument '" + this->m_Name + "'."));
    if (this->IsAssigned())
      throw(OptionError("Option '" + this->m_Name + "' already assigned."));
    this->m_Value = this->Convert(val);
    this->m_Properties |= Opt::Assigned;
  };
  
  template<typename T>
  inline T ValueOpt<T>::Convert(const std::string& source) const
  {
    T target;
    std::istringstream iss(source);
    if (!(iss >> target))
      throw ConversionError("Error during type conversion from a string.");
    return target;
  };
  
  template<>
  inline std::string ValueOpt<std::string>::Convert(const std::string& source) const
  {
    return source;
  };
};

#endif // pcl_ValueOpt_h