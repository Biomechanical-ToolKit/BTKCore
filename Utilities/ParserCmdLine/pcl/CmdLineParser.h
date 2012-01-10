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

#ifndef pcl_CmdLineParser_h
#define pcl_CmdLineParser_h

#include "Opt.h"
#include "FlagOpt.h"
#include "Exception.h"

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <cctype>

namespace pcl
{
  typedef enum {XOR} ConstraintType;
  typedef enum {None = 0x00, Help = 0x01, Version = 0x02} DefaultOption;
  
  typedef std::pair<ConstraintType, std::list<Opt*> > ConstraintOption;
  
  class CmdLineParser
  {
  public:
    CmdLineParser(const std::string& description, const std::string& version);
    // ~CmdLineParser();  // Implict destructor used.
    CmdLineParser& Add(const Opt& o);
    void AddConstraint(ConstraintType c, Opt& o1, Opt& o2);
    void AddConstraint(const ConstraintOption& c);
    void CheckConstraint();
    void CheckRequiredOption();
    int GetStatus() const {return this->m_Status;};
    void Parse(int argc, char * const argv[]);
    void PrintBrief(std::ostream& os);
    void PrintHelp(std::ostream& os);
    void PrintVersion(std::ostream& os);
    int Run(int argc, char * const argv[]);
    void UseDefaultOption(int opt) {this->m_DefaultOption = opt;};
    
  private:
    Opt* FindOptName(const std::string& name);
    Opt* FindOptAbbreviation(const std::string& abbr);
    void GenerateUsage(std::string& usage) const;
    void GenerateUsage(std::string& usage, Opt* o) const;
    
    std::string m_Name;
    std::string m_Description;
    std::string m_Version;
    std::list<Opt*> m_OptList;
    std::list<ConstraintOption> m_ConstraintList;
    size_t m_RequiredOption;
    int m_DefaultOption;
    bool m_UnlabeledAdded;
    int m_Status;
    
    CmdLineParser(const CmdLineParser& ); // Not implemented.
    CmdLineParser& operator=(const CmdLineParser& ); // Not implemented.
  };
  
  inline CmdLineParser::CmdLineParser(const std::string& description, const std::string& version)
  : m_Name(""), m_Description(description), m_Version(version), m_OptList()
  {
    this->m_RequiredOption = 0;
    this->m_UnlabeledAdded = false;
    this->m_DefaultOption = 0; // None
    this->m_Status = 0;
  };
  
  CmdLineParser& CmdLineParser::Add(const Opt& o)
  {
    if (o.HasProperty(Opt::Unlabeled))
    {
      if (this->m_UnlabeledAdded)
        std::cerr << "Option error: Unlabeled argument is already added." << std::endl;
      else
      {
        this->m_UnlabeledAdded = true;
        this->m_OptList.push_back(const_cast<Opt*>(&o));
        if (o.IsRequired())
          ++this->m_RequiredOption;
      }
    }
    else
    {
      if (o.GetName().empty() && o.GetAbbreviation().empty())
        std::cerr << "Option error: Name and abbreviation are both empty." << std::endl;
      else if (o.GetName().substr(0,2).compare("--") == 0)
        std::cerr << "Option error: Its name cannot start by '--'." << std::endl;
      else if (o.GetAbbreviation().length() > 1)
        std::cerr << "Option error: The abbreviation must be set only by one character." << std::endl;
      else if (o.GetAbbreviation().substr(0,1).compare("-") == 0)
        std::cerr << "Option error: Its abbreviation cannot start by '-'." << std::endl;
      else
      {
        std::list<Opt*>::const_iterator it = this->m_OptList.begin();
        while (it != this->m_OptList.end())
        {
          if ( (o.GetName().compare("") != 0) && ((*it)->GetName().compare(o.GetName()) == 0) )
          {  
            std::cerr << "Option error: Name '" << (*it)->GetName() << "' is already used." << std::endl;
            break;
          }
          if ( (o.GetAbbreviation().compare("") != 0) && ((*it)->GetAbbreviation().compare(o.GetAbbreviation()) == 0))
          {
            std::cerr << "Option error: Abbreviation '" << (*it)->GetAbbreviation() << "' is already used." << std::endl;
            break;
          }
          ++it;
        }
        if (it == this->m_OptList.end()) // No error
        {
          this->m_OptList.push_back(const_cast<Opt*>(&o));
          if (o.IsRequired())
            ++this->m_RequiredOption;
        }
      }
    }
    return *this;
  };
  
  inline void CmdLineParser::AddConstraint(ConstraintType c, Opt& o1, Opt& o2)
  {
      std::list<Opt*> constrainedOption = std::list<Opt*>();
      constrainedOption.push_back(&o1);
      constrainedOption.push_back(&o2);
      ConstraintOption newConstraint = ConstraintOption(c, constrainedOption);
      this->AddConstraint(newConstraint);
  };
    
  inline void CmdLineParser::AddConstraint(const ConstraintOption& c)
  {
    std::list<Opt*>::const_iterator it = c.second.begin();
    bool requiredOpt = (*it)->IsRequired();
    while (it != c.second.end())
    {
      if (requiredOpt != (*it)->IsRequired())
      {
        std::cerr << "Option Error: Impossible to have a constraint with required and unrequired argument." << std::endl;
        return;
      }
      ++it;
    }
    this->m_ConstraintList.push_back(c);
    if (requiredOpt)
      this->m_RequiredOption -= c.second.size() + 1;
  };
  
  inline void CmdLineParser::CheckConstraint()
  {
    std::list<ConstraintOption>::iterator itConstraint = this->m_ConstraintList.begin();
    std::list<Opt*>::iterator itOpt;
    while (itConstraint != this->m_ConstraintList.end())
    {
      itOpt = itConstraint->second.begin();
      bool checkConstraint = false;
      while (itOpt != itConstraint->second.end())
      {
        if ( (*itOpt)->IsAssigned() && checkConstraint )
          throw(OptionError("Incorrect options use together or not."));
        checkConstraint = (*itOpt)->IsAssigned();
        ++itOpt;
      }
      ++itConstraint;
    }
  };
  
  inline void CmdLineParser::CheckRequiredOption()
  {
    size_t requiredOpt = 0;
    std::list<Opt*>::iterator itOpt = this->m_OptList.begin();
    while (itOpt != this->m_OptList.end())
    {
      if ((*itOpt)->IsRequired() && (*itOpt)->IsAssigned())
        ++requiredOpt;
      ++itOpt;
    }
    if (requiredOpt != this->m_RequiredOption)
      throw(OptionError("Incorrect number of required argument."));
  };
  
  void CmdLineParser::Parse(int argc, char * const argv[])
  {
    this->m_Name = argv[0];
    std::string::size_type slashIdx = this->m_Name.find_last_of('/');
    std::string::size_type backslashIdx = this->m_Name.find_last_of('\\');
    if ((slashIdx == std::string::npos) && (backslashIdx == std::string::npos))
      this->m_Name = this->m_Name;
    else if (slashIdx == std::string::npos)
      this->m_Name = this->m_Name.substr(backslashIdx + 1);
    else
      this->m_Name = this->m_Name.substr(slashIdx + 1);
#if defined (_MSC_VER)
    // Remove the .exe
    std::string exe = this->m_Name.substr(this->m_Name.length()-4);
    std::transform(exe.begin(), exe.end(), exe.begin(), tolower);
    if (exe.compare(".exe") == 0)
      this->m_Name = this->m_Name.substr(0,this->m_Name.length()-4);
#endif
    
    if (argc == 1)
      return;
    std::list<std::string> argList;
    int inc = 1;
    while (inc < argc)
      argList.push_back(argv[inc++]);
    std::list<std::string>::iterator it = argList.begin();
    Opt* currentOpt = NULL;
    // Parse section
    while(it != argList.end())
    {
      if (it->compare("") == 0)
        throw(ParserError("Unexpected empty value"));
#if defined(__APPLE__)
      if (it->substr(0,7).compare("-psn_0_") == 0) // Process Serial Number given by MacOS X
      {
        it = argList.erase(it);
        continue;
      }
#endif
      if (it->substr(0,1).compare("-") == 0) // Option
      {
        if (currentOpt)
          currentOpt->SetValue("");
        if (it->substr(1,1).compare("-") == 0) // Name
        {
          std::string::size_type idxOpt = it->find_first_of("=");
          if (idxOpt == std::string::npos)
          {
            currentOpt = this->FindOptName(it->substr(2));
            if (!currentOpt)
              throw(ParserError("Illegal option with name: '"+ it->substr(2) +"'"));
            currentOpt->SetValue("");
          }
          else
          {
            currentOpt = this->FindOptName(it->substr(2,idxOpt-2));
            if (!currentOpt)
              throw(ParserError("Illegal option with name: '"+ it->substr(2,idxOpt-2) +"'"));
            currentOpt->SetValue(it->substr(idxOpt+1));
          }
          currentOpt = NULL;
        }
        else // Abbreviation
        {
          currentOpt = this->FindOptAbbreviation(it->substr(1,1));
          if (!currentOpt)
            throw(ParserError("Illegal option with abbreviation: '"+ it->substr(1,1) +"'"));
          size_t idxAbbr = 2;
          while(idxAbbr < it->size())
          {
            Opt* otheo = this->FindOptAbbreviation(it->substr(idxAbbr,1));
            if (otheo)
            {
              currentOpt->SetValue("");
              currentOpt = otheo;
            }
            else
            {
              currentOpt->SetValue(it->substr(idxAbbr));
              currentOpt = NULL;
              break;
            }
            ++idxAbbr;
          }
        }
        it = argList.erase(it);
      }
      else // Value
      {
        if (currentOpt)
        {
          if (currentOpt->IsValueRequired())
          {
            currentOpt->SetValue(*it);
            it = argList.erase(it);
          }
          else
            currentOpt->SetValue(""); // Flag option
        }
        else
        {
          if (this->m_UnlabeledAdded)
          {
            std::list<Opt*>::iterator itOpt = this->m_OptList.begin();
            while (itOpt != this->m_OptList.end())
            {
              if ((*itOpt)->HasProperty(Opt::Unlabeled))
              {
                currentOpt = *itOpt;
                break;
              }
              ++itOpt;
            }
            while(it != argList.end())
            {
              currentOpt->SetValue(*it);
              it = argList.erase(it);
            }
          }
          else
          {
            throw(ParserError("Unexpected value '" + *it +"'"));
          }
        }
        currentOpt = NULL;
      }
    }
    if (currentOpt)
    {
      currentOpt->SetValue("");
      currentOpt = NULL;
    }
  };
  
  int CmdLineParser::Run(int argc, char * const argv[])
  {
    FlagOpt help("help","h","Display this message", false);
    FlagOpt version("version","v","Display version information", false);
    if ((this->m_DefaultOption & Help) == Help)
      this->Add(help);
    if ((this->m_DefaultOption & Version) == Version)
      this->Add(version);

    try
    {
      this->Parse(argc, argv);
      this->CheckConstraint();
      this->CheckRequiredOption();
    }
    catch (pcl::Exception& e)
    {
#if defined(_MSC_VER) && defined(_WINDOWS_) // Only when using WinMain application
      std::cerr << "\n";
#endif
      std::cerr << e.what() << std::endl;
      this->m_Status = -1;
    }
    catch (std::exception& e)
    {
#if defined(_MSC_VER) && defined(_WINDOWS_) // Only when using WinMain application
      std::cerr << "\n";
#endif
      std::cerr << "Unexpected error: " << e.what() << std::endl;
      this->m_Status = -2;
    }
    catch (...)
    {
#if defined(_MSC_VER) && defined(_WINDOWS_) // Only when using WinMain application
      std::cerr << "\n";
#endif
      std::cerr << "Unknown error: Please contact the developer." << std::endl;
      this->m_Status = -3;
    }
    
    if (this->m_Status != 0) // Exception triggered
    {
      this->PrintBrief(std::cerr);
      return this->m_Status;
    }
    else
    {
      if(help.IsAssigned())
      {
        this->PrintHelp(std::cout);
        this->m_Status = 0;
        return Help;
      }
      if(version.IsAssigned())
      {
        this->PrintVersion(std::cout);
        this->m_Status = 0;
        return Version;
      }
    }
    
    return 0;
  };
  
  inline Opt* CmdLineParser::FindOptName(const std::string& name)
  {
    Opt* arg = NULL;
    std::list<Opt*>::iterator it = this->m_OptList.begin();
    while (it != this->m_OptList.end())
    {
      if ( (*it)->GetName().compare(name) == 0)
      {
        arg = *it;
        break;
      }
      ++it;
    }
    return arg;
  };
  
  inline void CmdLineParser::PrintBrief(std::ostream& os)
  {
#if defined(_MSC_VER) && defined(_WINDOWS_) // Only when using WinMain application
    os << "\n";
#endif
    std::string usage; this->GenerateUsage(usage);
    os << "Brief USAGE:\n";
    os << "  " << usage << "\n\n";
    os << "For complete USAGE and HELP type:\n";
    os << "  " << this->m_Name << " --help" << std::endl;
  };
  
  inline void CmdLineParser::PrintHelp(std::ostream& os)
  {
#if defined(_MSC_VER) && defined(_WINDOWS_) // Only when using WinMain application
    os << "\n";
#endif
    std::string usage; this->GenerateUsage(usage);
    os << "USAGE:\n";
    os << "  " << usage << "\n\n";
    os << "DESCRIPTION:" << std::endl;
    os << "  " << this->m_Description << " - " << this->m_Version << "\n\n";
    os << "OPTIONS:" << std::endl;
    for (std::list<Opt*>::iterator it = this->m_OptList.begin() ; it != this->m_OptList.end() ; ++it)
      os << (*it)->PrintDescription("  ");
    os.flush();
  };
  
  inline void CmdLineParser::PrintVersion(std::ostream& os)
  {
#if defined(_MSC_VER) && defined(_WINDOWS_) // Only when using WinMain application
    os << "\n";
#endif
    os << this->m_Name << ": " << this->m_Description << " - " << this->m_Version << std::endl;
  };
  
  inline Opt* CmdLineParser::FindOptAbbreviation(const std::string& abbr)
  {
    Opt* arg = NULL;
    std::list<Opt*>::iterator it = this->m_OptList.begin();
    while (it != this->m_OptList.end())
    {
      if ((*it)->GetAbbreviation().compare(abbr) == 0)
      {
        arg = *it;
        break;
      }
      ++it;
    }
    return arg;
  };
  
  void CmdLineParser::GenerateUsage(std::string& usage) const
  {
    usage.clear();
    usage = this->m_Name;
    
    std::string u;
    std::list<Opt*> alreadyProcessed;
    Opt* unlabeled = 0;
    
    for (std::list<ConstraintOption>::const_iterator it = this->m_ConstraintList.begin() ; it != this->m_ConstraintList.end() ; ++it)
    {
      if (it->first == XOR)
      {
        for (std::list<Opt*>::const_iterator it2 = it->second.begin() ; it2 != it->second.end() ; ++it2)
        {
          this->GenerateUsage(u, *it2);
          u += "|";
          alreadyProcessed.push_back(*it2);
        }
        if (static_cast<Opt*>(*(it->second.begin()))->IsRequired())
          usage += " (" + u + ")";
        else
          usage += " [" + u + "]";
      }
    }
    
    for (std::list<Opt*>::const_iterator it = this->m_OptList.begin() ; it != this->m_OptList.end() ; ++it)
    {
      if ((*it)->HasProperty(Opt::Unlabeled))
      {
        unlabeled = *it;
        continue;
      }
      bool processed = false;
      for (std::list<Opt*>::const_iterator it2 = alreadyProcessed.begin() ; it2 != alreadyProcessed.end() ; ++it2)
      {
        if (*it == *it2)
        {
          processed = true;
          break;
        }
      }
      if (!processed)
      {
        this->GenerateUsage(u, *it);
        if ((*it)->IsRequired())
          usage += " u";
        else
          usage += " [" + u + "]";
      }
    }
    
    if (unlabeled)
    {
      std::string file = " file";
      if (unlabeled->HasProperty(Opt::MultipleValues))
        file += "...";
      usage += file;
    }
  };
  
  inline void CmdLineParser::GenerateUsage(std::string& usage, Opt* o) const
  {
    usage.clear();
    if (!o->GetAbbreviation().empty())
    {
      usage += "-" + o->GetAbbreviation();
      if (o->IsValueRequired())
        usage += " <" + o->GetValueDescription() + ">";
    }
    else
    {
      usage += "-" + o->GetName();
      if (o->IsValueRequired())
        usage += "=<" + o->GetValueDescription() + ">";
    }
  };
};

#endif // pcl_CmdLineParser_h