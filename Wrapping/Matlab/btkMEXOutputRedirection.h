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

#ifndef __btkMEXOutputRedirection_h
#define __btkMEXOutputRedirection_h

#include "btkMEXStreambufToPrintf.h"
#include "btkMEXStreambufToWarnMsgTxt.h"

#include "btkMex.h"

namespace btk
{
  /**
   * @class MEXOutputRedirection btkMEXOutputRedirection.h
   * @brief Template class to redirect an output stream.
   *
   * This is a generic class which gives the possibility to redirect any
   * output stream to another stream by setting a new stream buffer.
   *
   * @ingroup BTKWrappingMatlab
   * @sa MEXCoutToPrintf, MEXCerrToWarnMsgTxt
   */
  template <class T>
  class MEXOutputRedirection
  {
  public:
    MEXOutputRedirection(std::ostream* output);
    MEXOutputRedirection(std::ostream* output, const std::string& id);
    virtual ~MEXOutputRedirection();
  private:
    std::ostream* mp_Stream;
    T* mp_NewBuffer;
    std::streambuf* mp_OldBuffer;
  };
  
  /**
   * @class MEXCoutToPrintf btkMEXOutputRedirection.h
   * @brief Class to redirect to standard C++ output (std::cout) to the MEX function mexPrintf.
   *
   * The use of this class gives you the possibility in one line to redirect std::cout during the execution
   * of the MEX function. Automaticaly at the end of the function, the redirection is reseted.
   * @code
   * void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
   * {
   *   // std::cout redirection to the mexPrintf function.
   *   btk::MEXCoutToPrintf coutRedir = btk::MEXCoutToPrintf(); // It is important to construct an object otherise the redirection is not done.
   *   ... // Do what you want
   *   std::cout << "Simple example to show you that this line will be printed in the Matlab console."
   *   return;
   *   // The redirection is automaticaly reseted at the end of the function
   * }
   * @endcode
   *
   * @sa MEXStreambufToPrintf
   * @ingroup BTKWrappingMatlab
   */
  class MEXCoutToPrintf : public MEXOutputRedirection<MEXStreambufToPrintf>
  {
  public:
    MEXCoutToPrintf() : MEXOutputRedirection<MEXStreambufToPrintf>(&std::cout)
    {};
  };
  
  /**
   * @class MEXCerrToWarnMsgTxt btkMEXOutputRedirection.h
   * @brief Class to redirect to standard C++ error (std::cerr) to the MEX function mexWarnMsgIdAndTxt.
   *
   * As the new stream buffer is wrapped around the MEX function mexWarnMsgIdAndTxt, it is important
   * to give to the constructor a specific ID which will can be used to enable or disable this warning in Matlab.
   * @code
   * void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
   * {
   *   // std::cerr redirection to the mexWarnMsgTxt function.
   *   btk::MEXCerrToWarnMsgTxt cerrRedir = btk::MEXCerrToWarnMsgTxt("myProgram:MyID"); // It is important to construct an object otherise the redirection is not done.
   *   ... // Do what you want
   *   std::cerr << "Simple example to show you that this line will be printed in the Matlab console as a warning."
   *   return;
   *   // The redirection is automaticaly reseted at the end of the function
   * }
   * @endcode
   *
   * @sa MEXStreambufToPrintf
   * @ingroup BTKWrappingMatlab
   */
  class MEXCerrToWarnMsgTxt : public MEXOutputRedirection<MEXStreambufToWarnMsgTxt>
  {
  public:
    MEXCerrToWarnMsgTxt(const std::string& id) : MEXOutputRedirection<MEXStreambufToWarnMsgTxt>(&std::cerr, id)
    {};
  };
  
  /**
   * Constructor setting the new buffer by using the default constructor of the template class @a T
   */
  template <class T>
  MEXOutputRedirection<T>::MEXOutputRedirection(std::ostream* output)
  {
    this->mp_Stream = output;
    this->mp_NewBuffer = new T();
    this->mp_OldBuffer = this->mp_Stream->rdbuf(this->mp_NewBuffer);
  }
  
  /**
   * Constructor setting the new buffer by using a constructor of the template class @a T requiring an id.
   */
  template <class T>
  MEXOutputRedirection<T>::MEXOutputRedirection(std::ostream* output, const std::string& id)
  {
    this->mp_Stream = output;
    this->mp_NewBuffer = new T(id);
    this->mp_OldBuffer = this->mp_Stream->rdbuf(this->mp_NewBuffer);
  }
  
  /**
   * Destructor
   * Reset the redirection.
   */
  template <class T>
  MEXOutputRedirection<T>::~MEXOutputRedirection()
  {
    this->mp_Stream->rdbuf(this->mp_OldBuffer);
    delete this->mp_NewBuffer;
  };
  
  /**
   * @fn template <class T> MEXCoutToPrintf::MEXCoutToPrintf()
   * Constructor
   */
   
  /**
   * @fn template <class T> MEXCerrToWarnMsgTxt::MEXCerrToWarnMsgTxt(const std::string& id)
   * Constructor
   */
};
#endif // __btkMEXStreambufToPrintf_h
