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
 
#ifndef pcl_MultiUnlabeledOpt_h
#define pcl_MultiUnlabeledOpt_h

#include "MultiValueOpt.h"

namespace pcl
{
  template <typename T>
  class MultiUnlabeledOpt : public MultiValueOpt<T>
  {
  public:
    MultiUnlabeledOpt(const std::string& desc, bool req);
    // ~MultiUnlabeledOpt(); // Implicit destructor used.
    
    virtual std::string PrintDescription(const std::string& indent) const {return "";};
    
  private:
    MultiUnlabeledOpt(const MultiUnlabeledOpt&); // Not implemented.
    MultiUnlabeledOpt& operator=(const MultiUnlabeledOpt&); // Not implemented.
  };

  template <typename T>
  inline MultiUnlabeledOpt<T>::MultiUnlabeledOpt(const std::string& desc, bool req = true)
  : MultiValueOpt<T>("", "", desc, "", req, true)
  {
    this->m_Properties |= (Opt::MultipleValues | Opt::Unlabeled);
  };
};

#endif // pcl_MultiUnlabeledOpt_h