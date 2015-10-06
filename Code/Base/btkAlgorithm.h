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

#ifndef __btkAlgorithm_h
#define __btkAlgorithm_h

#include "btkObject.h"
#include "btkInformation.h" // BTK_DECLARE_METAINFO
#include "btkAny.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

#include <vector>
#include <list>

namespace btk
{
  class Node;
  class Port;
  class Information;
  class Executive;
  
  BTK_DECLARE_METAINFO(CreateOutputData, bool)
  BTK_DECLARE_METAINFO(CheckInputConsistency, bool)
  BTK_DECLARE_METAINFO(UpdateOutputData, bool)
  BTK_DECLARE_METAINFO(ManageInputDataPerSlice, bool)
  
  class AlgorithmPrivate;
  
  enum class Result : short {Error = -2,  Aborted = -1, Success = 0};
  
  class BTK_BASE_EXPORT Algorithm : public Object
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Algorithm)
    
  public:
    // static void setDefaultExecutiveFactory(const std::function<Executive*()>& factory);
    
    virtual ~Algorithm() _BTK_NOEXCEPT;
    
    Algorithm(const Algorithm& ) = delete;
    Algorithm(Algorithm&& ) _BTK_NOEXCEPT = delete;
    Algorithm& operator=(const Algorithm& ) = delete;
    Algorithm& operator=(Algorithm&& ) _BTK_NOEXCEPT = delete;
    
    template <typename U> typename U::ValueType information(typename U::ValueType&& defaultValue = typename U::ValueType()) const _BTK_NOEXCEPT;
    btk::Any information(const char* key, const btk::Any& defaultValue = btk::Any()) const _BTK_NOEXCEPT;
    template <typename U> void setInformation(typename U::ValueType&& value);
    void setInformation(const char* key, const btk::Any& value);
    
    unsigned inputsNumber() const _BTK_NOEXCEPT;
    unsigned outputsNumber() const _BTK_NOEXCEPT;
    
    Port* input(unsigned port, unsigned connection) const _BTK_NOEXCEPT;
    std::list<Port*> inputs(unsigned port) const _BTK_NOEXCEPT;
    
    void setInput(unsigned port, Port* input) _BTK_NOEXCEPT;
    void setInput(Port* input) _BTK_NOEXCEPT;
    
    void addInput(unsigned port, Port* input) _BTK_NOEXCEPT;
    void addInput(Port* input) _BTK_NOEXCEPT;
    
    void removeInput(unsigned port, Port* input) _BTK_NOEXCEPT;
    void removeInput(unsigned port, unsigned connection) _BTK_NOEXCEPT;
    
    void clearInputs() _BTK_NOEXCEPT;
    
    Port* output(unsigned port) _BTK_NOEXCEPT;
    Port* output() _BTK_NOEXCEPT;
    
    Executive* executive();
    
    virtual Result processRequest(const Information& info);
    
    Result update();
    
  protected:
    Algorithm();
    Algorithm(AlgorithmPrivate& pimpl) _BTK_NOEXCEPT;
    
    void setInputsNumber(unsigned num);
    void setOutputsNumber(unsigned num);
    
    bool checkInputIndex(unsigned port) const _BTK_NOEXCEPT;
    bool checkOutputIndex(unsigned port) const _BTK_NOEXCEPT;
    virtual bool checkInputConsistency(unsigned port) const _BTK_NOEXCEPT;
    
    virtual bool fillInputInformation(Port* port) const;
    virtual bool fillOutputInformation(Port* port) const;
    virtual Node* createOutputData(unsigned port) const;
    virtual bool updateOutputData(const std::vector<std::vector<const Node*>>& in, const std::vector<std::vector<Node*>>& out);
    virtual bool updateOutputData(const std::vector<const Node*>& in, const std::vector<Node*>& out);
  };
  
  template <typename U>
  typename U::ValueType Algorithm::information(typename U::ValueType&& defaultValue) const _BTK_NOEXCEPT
  {
    return this->information(U::key(),std::forward<typename U::ValueType>(defaultValue)).template cast<typename U::ValueType>();
  };

  template <typename U>
  void Algorithm::setInformation(typename U::ValueType&& value)
  {
    this->setInformation(U::key(),std::forward<typename U::ValueType>(value));
  };
};

#endif // __btkAlgorithm_h