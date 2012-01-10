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
 
#ifndef pcl_UnlabeledOpt_h
#define pcl_UnlabeledOpt_h

#include "ValueOpt.h"

namespace pcl
{
  template <typename T>
  class UnlabeledOpt : public ValueOpt<T>
  {
  public:
    UnlabeledOpt(const std::string& desc, bool req);
    // ~UnlabeledOpt(); // Implicit destructor used.
    
    virtual void SetValue(const std::string& val);
    virtual std::string PrintDescription(const std::string& /* indent */) const {return "";};
    
  private:
    UnlabeledOpt(const UnlabeledOpt&); // Not implemented.
    UnlabeledOpt& operator=(const UnlabeledOpt&);  // Not implemented.
  };

  template <typename T>
  inline UnlabeledOpt<T>::UnlabeledOpt(const std::string& desc, bool req = true)
  : ValueOpt<T>("", "", desc, "", req)
  {
    this->m_Properties |= Opt::Unlabeled;
  };
  
  template <typename T>
  inline void UnlabeledOpt<T>::SetValue(const std::string& val)
  {
    if (this->IsAssigned())
      throw(OptionError("Only one input is supported. Did you put the options after the input?"));
    this->m_Value = this->Convert(val);
    this->m_Properties |= Opt::Assigned;
  };
};

#endif // pcl_UnlabeledOpt_h