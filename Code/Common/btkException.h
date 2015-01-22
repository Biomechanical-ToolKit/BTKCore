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

#ifndef __btkException_h
#define __btkException_h

#include <stdexcept>
#include <string>

namespace btk
{
  /**
   * @brief General class for exception
   */
  class Exception : public std::exception
  {
  public:
    /**
     * Constructor
     */
    explicit Exception(const std::string& msg) noexcept
    : m_What(msg)
    {};
    
    /**
     * Destructor
     */
    virtual ~Exception() noexcept = default;
    
    /**
     * Gets the exception's message
     */
    virtual const char* what() const noexcept
    {
      return this->m_What.c_str();
    };
    
  private:
    std::string m_What;
  };


  /**
   * @brief Logic error exception
   */
  class LogicError : public Exception
  {
  public:
    /**
     * Constructor
     */
    explicit LogicError(const std::string& msg)
    : Exception(msg)
    {};
  };

  /**
   * @brief Logic error exception
   */
  class InvalidArgument : public LogicError
  {
  public:
    /**
     * Constructor
     */
    explicit InvalidArgument(const std::string& msg)
    : LogicError(msg)
    {};
  };

  /**
   * @brief Out of range exception
   */
  class OutOfRangeException : public Exception
  {
  public:
    /**
     * Constructor
     */
    explicit OutOfRangeException(const std::string& msg)
    : Exception(msg)
    {};
  };
  
  /**
   * @brief Domain error exception
   */
  class DomainError : public Exception
  {
  public:
    /**
     * Constructor
     */
    explicit DomainError(const std::string& msg)
    : Exception(msg)
    {};
  };
  
  /**
   * @brief Runtime error exception
   */
  class RuntimeError : public Exception
  {
  public:
    /**
     * Constructor
     */
    explicit RuntimeError(const std::string& msg)
    : Exception(msg)
    {};
  };
};

#endif // __btkException_h
