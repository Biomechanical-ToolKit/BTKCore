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
 
#ifndef pcl_MultiValueOpt_h
#define pcl_MultiValueOpt_h

#include "Opt.h"

#include <list>

namespace pcl
{
  template <typename T>
  class MultiValueOpt : public ValueOpt< std::list<T> >
  {
  public:
    MultiValueOpt(const std::string& name, const std::string& abbr, const std::string& desc, 
                  const std::string& valDesc, bool req = true);
    // ~MultiValueOpt(); // Implicit destructor used.
    
    virtual void SetValue(const std::string& val);
  
  private:
    MultiValueOpt(const MultiValueOpt&); // Not implemented.
    MultiValueOpt& operator=(const MultiValueOpt&); // Not implemented.
  };

  template <typename T>
  inline MultiValueOpt<T>::MultiValueOpt(const std::string& name, const std::string& abbr, 
                                      const std::string& valDesc, 
                                      const std::string& desc, bool req)
  : ValueOpt< std::list<T> >(name, abbr, desc, valDesc, req)
  {
    this->m_Properties |= Opt::MultipleValues;
  };

  template <typename T>
  void MultiValueOpt<T>::SetValue(const std::string& val)
  {
    if (this->IsValueRequired() && (val.compare("") == 0))
      throw(OptionError("A value is required for the argument '" + this->name() + "'."));
    this->m_Value.push_back(this->Convert(val));
    this->m_Properties |= Opt::Assigned;
  };
};

#endif // pcl_MultiValueOpt_h