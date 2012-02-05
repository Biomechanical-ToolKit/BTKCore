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
 
#ifndef pcl_Exception_h
#define pcl_Exception_h

#include <stdexcept>

namespace pcl
{
  /**
   * @brief General class for exception
   */
  class Exception : public std::exception
  {
  public:
    /** Constructor */
    explicit Exception(const std::string& msg)
    : mWhat(msg)
    {};
  
    /** Destructor */
    virtual ~Exception() throw() {};
  
    /**
     * Gets the exception's message
     */
    virtual const char* what() const throw()
    {
      return mWhat.c_str();
    };
  
  private:
    std::string mWhat;
  };
  
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


  /**
   * @brief Error related to the option
   */
  class OptionError : public Exception
  {
  public:
  
    /** Constructor */
    explicit OptionError(const std::string& msg)
    : Exception(msg)
    {};
  
    /** Destructor */
    virtual ~OptionError() throw() {}
  };
  
  /**
   * @brief Error during the parsing
   */
  class ParserError : public Exception
  {
  public:
  
    /** Constructor */
    explicit ParserError(const std::string& msg)
    : Exception(msg)
    {};
  
    /** Destructor */
    virtual ~ParserError() throw() {}
  };
}
#endif // pcl_Exception_h