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
 
#ifndef pcl_Opt_h
#define pcl_Opt_h

#include "Exception.h" // For the inherited classes.

#include <string>

namespace pcl
{
  class Opt
  {
  public:
    enum Property {None = 0x00, Required = 0x01, Assigned = 0x02, ValueRequired = 0x04, MultipleValues = 0x08, Unlabeled = 0x10};
    
    virtual ~Opt() {};
    const std::string& GetName() const {return this->m_Name;};
    const std::string& GetAbbreviation() const {return this->m_Abbreviation;};
    const std::string& GetDescription() const {return this->m_Description;};
    const std::string& GetValueDescription() const {return this->m_ValueDescription;};
    bool HasProperty(Property p) const {return ((this->m_Properties & p) == p);};
    bool IsRequired() const {return this->HasProperty(Required);};
    bool IsValueRequired() const {return this->HasProperty(ValueRequired);};
    bool IsAssigned() const {return this->HasProperty(Assigned);};
    virtual void SetValue(const std::string& val) = 0;
    virtual std::string PrintDescription(const std::string& indent) const;
    
  protected:
    Opt(const std::string& name, const std::string& abbr, const std::string& desc,
       const std::string& valDesc, bool req, bool valReq);
    
    std::string m_Name;   
    int m_Properties;
       
  private:
    std::string m_Abbreviation;
    std::string m_Description;
    std::string m_ValueDescription;

    Opt(const Opt&); // Not implemented.
    Opt& operator=(const Opt&); // Not implemented.
  };
  
  inline Opt::Opt(const std::string& name, 
                  const std::string& abbr,
                  const std::string& desc,
                  const std::string& valDesc,
                  bool req, 
                  bool valReq)
  : m_Name(name), m_Abbreviation(abbr), m_Description(desc), m_ValueDescription(valDesc)
  {
    this->m_Properties = 0; // None
    if (req)
      this->m_Properties |= Required;
    if (valReq)
      this->m_Properties |= ValueRequired;
  };
  
  inline std::string Opt::PrintDescription(const std::string& indent) const
  {
    std::string msg = "";
    if (!this->m_Abbreviation.empty())
    {
      msg += "-" + this->m_Abbreviation;
      if (this->IsValueRequired())
        msg += " <" + this->m_ValueDescription + ">";
    }
    if (!msg.empty() && !this->m_Name.empty())
      msg += ",  ";
    if (!this->m_Name.empty())
    {
      msg += "--" + this->m_Name;
      if (this->IsValueRequired())
        msg += "=<" + this->m_ValueDescription + ">";
    }
    msg += "\n  ";
    msg += indent + indent + this->m_Description + "\n";
    msg = indent + msg;
    
    return msg;
  };
};

#endif // pcl_Opt_h