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

#ifndef __btkPort_h
#define __btkPort_h

#include "btkObject.h"
#include "btkAny.h"
#include "btkInformation.h"
#include "btkNode.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

#include <list>

namespace btk
{
  class Algorithm;
  class Node;
  
  class PortPrivate;
  
  BTK_DECLARE_METAINFO(InformationFilled, bool)
  BTK_DECLARE_METAINFO(InputIsRepeatable, bool)
  BTK_DECLARE_METAINFO(InputIsOptional, bool)
  BTK_DECLARE_METAINFO(InputDataRequiredTypeid, typeid_t)
  BTK_DECLARE_METAINFO(InputDataPossiblyMultinode, bool)
  BTK_DECLARE_METAINFO(OutputDataTypeid, typeid_t)
  BTK_DECLARE_METAINFO(OutputDataCreated, bool)
  
  class BTK_BASE_EXPORT Port : public Object
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Port)
    
  public:  
    Port(unsigned index, Algorithm* parent);
    ~Port() _BTK_NOEXCEPT;
    
    template <typename U> typename U::ValueType information(const typename U::ValueType& defaultValue = typename U::ValueType()) const _BTK_NOEXCEPT;
    btk::Any information(const char* key, const btk::Any& defaultValue = btk::Any()) const _BTK_NOEXCEPT;
    template <typename U> void setInformation(const typename U::ValueType& value);
    void setInformation(const char* key, const btk::Any& value);
    
    unsigned index() const _BTK_NOEXCEPT;
    Algorithm* algorithm() const _BTK_NOEXCEPT;
    
    Port* connection(unsigned index) const  _BTK_NOEXCEPT;
    const std::list<Port*>& connections() const _BTK_NOEXCEPT;
    void setConnection(Port* port);
    void addConnection(Port* port);
    void removeConnection(Port* port);
    void clearConnections();
    
    Node* data() const _BTK_NOEXCEPT;
    template <typename U = Node*> U data(unsigned index) const _BTK_NOEXCEPT;
    Node* exportData() const _BTK_NOEXCEPT;
    void holdData(Node* data);
    void holdData(const std::list<Node*>& data);
    
  protected:
    bool checkConnectionIndex(unsigned connection) const _BTK_NOEXCEPT;
  };
  
  template <typename U>
  typename U::ValueType Port::information(const typename U::ValueType& defaultValue) const _BTK_NOEXCEPT
  {
    return this->information(U::key(),defaultValue).template cast<typename U::ValueType>();
  };

  template <typename U>
  void Port::setInformation(const typename U::ValueType& value)
  {
    this->setInformation(U::key(),value);
  };
  
  template <typename U>
  U Port::data(unsigned index) const _BTK_NOEXCEPT
  {
    static_assert(std::is_pointer<U>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<U>::type>::value, "The casted type must derive from btk::Node.");
    auto root = this->data();
    if (root == nullptr)
      return nullptr;
    return root->child<U>(index);
  };
};

#endif // __btkPort_h