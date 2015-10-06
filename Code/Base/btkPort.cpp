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

#include "btkPort.h"
#include "btkPort_p.h"
#include "btkAlgorithm.h"
#include "btkNode.h"
#include "btkLogger.h"

#include <algorithm> // std::find

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  PortPrivate::PortPrivate()
  : ObjectPrivate(),
    Index(0), Information(), Data(nullptr), Parent(nullptr), Connections()
  {};
    
  PortPrivate::~PortPrivate() = default;
    
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Port::Port(unsigned index, Algorithm* parent)
  : Object(*new PortPrivate)
  {
    auto optr = this->pimpl();
    optr->Index = index;
    optr->Parent = parent;
  };
  
  Port::~Port() _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    delete optr->Data;
    for (auto& cnx: optr->Connections)
      cnx->removeConnection(this);
  };
  
  Any Port::information(const char* key, const btk::Any& defaultValue) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Information.value(key,std::move(defaultValue));
  };

  void Port::setInformation(const char* key, const btk::Any& value)
  {
    auto optr = this->pimpl();
    if (value.isValid() ? optr->Information.add(key,std::move(value)) : optr->Information.remove(key))
      this->modified();
  };
  
  Algorithm* Port::algorithm() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Parent;
  };
  
  Port* Port::connection(unsigned index) const  _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkConnectionIndex(index))
      return nullptr;
    auto it = optr->Connections.begin();
    std::advance(it, index);
    return *it;
  };
  
  const std::list<Port*>& Port::connections() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Connections;
  };
  
  void Port::setConnection(Port* port)
  {
    auto optr = this->pimpl();
    if ((optr->Connections.size() == 1) && (optr->Connections.front() == port))
      return;
    optr->Connections.clear();
    if (port != nullptr)
      optr->Connections.push_back(port);
    this->modified();
  };
  
  void Port::addConnection(Port* port)
  {
    if (port == nullptr)
    {
      Logger::warning("Impossible to connect a null port");
      return;
    }
    auto optr = this->pimpl();
    auto it = std::find(optr->Connections.begin(), optr->Connections.end(), port);
    if (it != optr->Connections.end())
    {
      optr->Connections.insert(it, port);
      this->modified();
    }
  };
  
  void Port::removeConnection(Port* port)
  {
    auto optr = this->pimpl();
    auto it = std::find(optr->Connections.begin(), optr->Connections.end(), port);
    if (it != optr->Connections.end())
    {
      optr->Connections.erase(it);
      this->modified();
    }
  };
  
  void Port::clearConnections()
  {
    auto optr = this->pimpl();
    if (!optr->Connections.empty())
    {
      optr->Connections.clear();
      this->modified();
    }
  };

  /** 
   * Create a new Node object on the heap and link it to the port data object. 
   * With this way, it is, for example, possible to export data computed by a pipeline created in a function.
   * You can access to the object data
   * @note The returned node must be deleted afterwards. In case you do not want to manage the deletion of the node, you can use the function make_shared_node().
   */
  Node* Port::exportData() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    auto temp = new Node("_BTK_PORT_SHARED");
    if (optr->Data != nullptr)
    {
      for (auto& child: optr->Data->children())
        child->addParent(temp);
    }
    return temp;
  };
  
  /*
   * Retruns the data associated to the port.
   *
   * @warning This method should be used only inside Algorithm methods. Or at least it is important to remind that data is owned by the Port. In case, you want to keep the data outside the port, use the method exportData(). Never try to delete/free the pointer returned by this method.
   */
  Node* Port::data() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Replace the current data object (if any) and attach the given @a data to this port.
   * The replaced data will be deleted if it was not shared previously.
   */
  void Port::holdData(Node* data)
  {
    this->holdData(std::list<Node*>{data});
  };
  
  void Port::holdData(const std::list<Node*>& data)
  {
    auto optr = this->pimpl();
    if (optr->Data == nullptr)
      optr->Data = new Node("_BTK_PORT_ROOT");
    auto current = optr->Data->children();
    bool identical = false;
    if (current.size() == data.size())
    {
      identical = true;
      for (const auto& c: current)
      {
        if (std::find(data.cbegin(), data.cend(), c) == data.cend())
        {
          identical = false;
          break;
        }
      }
    }
    if (identical)
      return;
    for (auto& c: current)
    {
      c->removeParent(optr->Data);
      if (!c->hasParents())
        delete c;
    }
    for (auto& d: data)
    {
      if (d != nullptr)
        d->addParent(optr->Data);
    }
    this->modified();
  };
  
  bool Port::checkConnectionIndex(unsigned connection) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (connection >= optr->Connections.size())
    {
      Logger::error("Requested connection outside of the current range.");
      return false;
    }
    return true;
  };
};