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

#ifndef __btkDataFactory_h
#define __btkDataFactory_h

#include "btkAlgorithm.h"
#include "btkPort.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

#include <functional> // std::function

namespace btk
{
  class Node;
  
  class DataFactoryBasePrivate;
  
  class DataFactoryBase : public Algorithm
  {
    BTK_DECLARE_PIMPL_ACCESSOR(DataFactoryBase)
  
  public:
    using GeneratorFunctor = std::function<Node*()>;
    
    DataFactoryBase(GeneratorFunctor&& func);
    ~DataFactoryBase() _BTK_NOEXCEPT;
    
  protected:
    Node* generateNode() const;
  };
  
  template <typename T>
  class DataFactory : public DataFactoryBase
  {
  public:
    template <typename... Args> DataFactory(Args&&... args);
    ~DataFactory() _BTK_NOEXCEPT = default;
    
  protected:
    virtual bool fillOutputInformation(Port* port) const override;
    virtual Node* createOutputData(unsigned port) const override;
  };
};

// ------------------------------------------------------------------------- //

namespace btk
{ 
  /**
   *
   */
  template <typename T>
  template <typename... Args>
  DataFactory<T>::DataFactory(Args&&... args)
  : DataFactoryBase([&]() -> Node* {return new T(std::forward<Args>(args)...);})
  {
    static_assert(std::is_pointer<T*>::value, "The generated type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node, T>::value, "The generated type must derive from btk::Node.");
  };

  /**
   *
   */
  template <typename T>
  bool DataFactory<T>::fillOutputInformation(Port* port) const
  {
    port->setInformation<OutputDataTypeid>(static_typeid<T>());
    return true;
  };

  /**
   *
   */
  template <typename T>
  Node* DataFactory<T>::createOutputData(unsigned port) const
  {
    _BTK_UNUSED(port)
    return this->generateNode();
  };
}

#endif // __btkDataFactory_h