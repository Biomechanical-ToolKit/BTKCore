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

#ifndef __btkNode_h
#define __btkNode_h

#include "btkObject.h"
#include "btkAny.h"
#include "btkTypeid.h"

#include <list>
#include <string>
#include <regex>

namespace btk
{
  class Node;
  
  template <typename U> U node_cast(Node* node) noexcept;
  
  class NodePrivate;
  
  class BTK_COMMON_EXPORT Node : public Object
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Node)
    
  public:
    Node(const std::string& name, Node* parent = nullptr);
    virtual ~Node() noexcept;
    
    Node(const Node& ) = delete;
    Node(Node&& ) noexcept = delete;
    Node& operator=(const Node& ) = delete;
    Node& operator=(Node&& ) noexcept = delete;
    
    const std::string& name() const noexcept;
    void setName(const std::string& value) noexcept;
    
    const std::string& description() const noexcept;
    void setDescription(const std::string& value) noexcept;
    
    Any property(const std::string& key) const noexcept;
    void setProperty(const std::string& key, const Any& value) noexcept;
    
    const std::list<Node*>& children() const noexcept;
    bool hasChildren() const noexcept;
    void appendChild(Node* node) noexcept;
    void removeChild(Node* node) noexcept;
    
    const std::list<Node*>& parents() const noexcept;
    bool hasParents() const noexcept;
    void appendParent(Node* node) noexcept;
    void removeParent(Node* node) noexcept;
    
    template <typename T = Node*> T findChild(const std::string& name = {}, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const noexcept;
    template <typename T = Node*> std::list<T> findChildren(const std::string& name = {}, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const noexcept;
    template <typename T = Node*, typename U, typename = typename std::enable_if<std::is_same<std::regex, U>::value>::type> std::list<T> findChildren(const U& regexp, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const noexcept;
    
    virtual void modified() noexcept;
    
    
  protected:
    Node(NodePrivate& pimpl, Node* parent) noexcept;
    
  private:
    Node* findNode(typeid_t id, const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept;
    void findNodes(std::list<void*>* list, typeid_t id, const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept;
    void findNodes(std::list<void*>* list, typeid_t id, const std::regex& regexp, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept;
    
    template <typename U> friend U node_cast(Node* node) noexcept;
    bool castable(typeid_t id);
  };

  template <typename T>
  T Node::findChild(const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept
  {
    static_assert(std::is_pointer<T>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<T>::type>::value, "The casted type must derive from btk::Node.");
    return static_cast<T>(this->findNode(static_typeid<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>(),name,std::move(properties),recursiveSearch));
  };

  template <typename T>
  std::list<T> Node::findChildren(const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept
  {
    static_assert(std::is_pointer<T>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<T>::type>::value, "The casted type must derive from btk::Node.");
    std::list<T> children;
    this->findNodes(reinterpret_cast<std::list<void*>*>(&children),static_typeid<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>(),name,std::move(properties),recursiveSearch);
    return children;
  };
  
  template <typename T, typename U, typename>
  std::list<T> Node::findChildren(const U& regexp, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept
  {
    static_assert(std::is_pointer<T>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<T>::type>::value, "The casted type must derive from btk::Node.");
    std::list<T> children;
    this->findNodes(reinterpret_cast<std::list<void*>*>(&children), static_typeid<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>(),regexp,std::move(properties),recursiveSearch);
    return children;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename U>
  U node_cast(Node* node) noexcept
  {
    static_assert(std::is_pointer<U>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<U>::type>::value, "The casted type must derive from btk::Node.");
    if (node->castable(static_typeid<typename std::remove_pointer<U>::type>()))
      return static_cast<U>(node);
    return nullptr;
  };
};

using btk::node_cast;

#endif // __btkNode_h