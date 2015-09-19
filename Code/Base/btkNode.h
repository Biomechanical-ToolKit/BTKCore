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
#include "btkMacros.h" // _BTK_NOEXCEPT
#include "btkNodeid.h" // Macro BTK_DECLARE_NODEID used by inheriting classes.

#include <list>
#include <string>
#include <regex>

#define BTK_NODE(exportname,classname) \
  classname; \
  BTK_EXPORT_STATIC_TYPEID(classname,exportname) \
  class exportname classname

namespace btk
{
  template <typename T, typename N> T node_cast(N* node) _BTK_NOEXCEPT;
  
  class NodePrivate;
  
  class BTK_NODE(BTK_BASE_EXPORT, Node) : public Object
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Node)
    
  public:
    Node(const std::string& name, Node* parent = nullptr);
    virtual ~Node() _BTK_NOEXCEPT;
    
    Node(const Node& ) = delete;
    Node(Node&& ) _BTK_NOEXCEPT = delete;
    Node& operator=(const Node& ) = delete;
    Node& operator=(Node&& ) _BTK_NOEXCEPT = delete;
    
    const std::string& name() const _BTK_NOEXCEPT;
    void setName(const std::string& value) _BTK_NOEXCEPT;
    
    const std::string& description() const _BTK_NOEXCEPT;
    void setDescription(const std::string& value) _BTK_NOEXCEPT;
    
    Any property(const std::string& key) const _BTK_NOEXCEPT;
    void setProperty(const std::string& key, const Any& value);
    
    template <typename U = Node*> U child(unsigned index) const _BTK_NOEXCEPT;
    const std::list<Node*>& children() const _BTK_NOEXCEPT;
    bool hasChildren() const _BTK_NOEXCEPT;
    
    const std::list<Node*>& parents() const _BTK_NOEXCEPT;
    bool hasParents() const _BTK_NOEXCEPT;
    void addParent(Node* node) _BTK_NOEXCEPT;
    void removeParent(Node* node) _BTK_NOEXCEPT;
    
    template <typename U = Node*> U findChild(const std::string& name = std::string{}, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const _BTK_NOEXCEPT;
    template <typename U = Node*> std::list<U> findChildren(const std::string& name = std::string{}, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const _BTK_NOEXCEPT;
    template <typename U = Node*, typename V, typename = typename std::enable_if<std::is_same<std::regex, V>::value>::type> std::list<U> findChildren(const V& regexp, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const _BTK_NOEXCEPT;
    
    std::list<const Node*> retrievePath(const Node* node) const _BTK_NOEXCEPT;
    
    virtual void modified() _BTK_NOEXCEPT;
    
    void clear() _BTK_NOEXCEPT;
    Node* clone(Node* parent = nullptr) const;
    // void copy(Node* other);
    
    virtual bool isCastable(typeid_t id) const _BTK_NOEXCEPT;
    
  protected:
    Node(NodePrivate& pimpl, Node* parent) _BTK_NOEXCEPT;
    
    void replaceChild(Node* current, Node* substitute);
    
  private:
    Node* findNode(typeid_t id, const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const _BTK_NOEXCEPT;
    void findNodes(std::list<void*>* list, typeid_t id, const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const _BTK_NOEXCEPT;
    void findNodes(std::list<void*>* list, typeid_t id, const std::regex& regexp, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const _BTK_NOEXCEPT;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename U>
  U Node::child(unsigned index) const _BTK_NOEXCEPT
  {
    static_assert(std::is_pointer<U>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<U>::type>::value, "The casted type must derive from btk::Node.");
    if (index < this->children().size())
    {
      auto it = this->children().begin();
      std::advance(it,index);
      return node_cast<U>(*it);
    }
    return nullptr;
  };

  template <typename U>
  U Node::findChild(const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const _BTK_NOEXCEPT
  {
    static_assert(std::is_pointer<U>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<U>::type>::value, "The casted type must derive from btk::Node.");
    return static_cast<U>(this->findNode(static_typeid<typename std::remove_cv<typename std::remove_pointer<U>::type>::type>(),name,std::move(properties),recursiveSearch));
  };

  template <typename U>
  std::list<U> Node::findChildren(const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const _BTK_NOEXCEPT
  {
    static_assert(std::is_pointer<U>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<U>::type>::value, "The casted type must derive from btk::Node.");
    std::list<U> children;
    this->findNodes(reinterpret_cast<std::list<void*>*>(&children),static_typeid<typename std::remove_cv<typename std::remove_pointer<U>::type>::type>(),name,std::move(properties),recursiveSearch);
    return children;
  };
  
  template <typename U, typename V, typename>
  std::list<U> Node::findChildren(const V& regexp, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const _BTK_NOEXCEPT
  {
    static_assert(std::is_pointer<U>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<U>::type>::value, "The casted type must derive from btk::Node.");
    std::list<U> children;
    this->findNodes(reinterpret_cast<std::list<void*>*>(&children), static_typeid<typename std::remove_cv<typename std::remove_pointer<U>::type>::type>(),regexp,std::move(properties),recursiveSearch);
    return children;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename T, typename N>
  inline T node_cast(N* node) _BTK_NOEXCEPT
  {
    static_assert(std::is_pointer<T>::value, "The casted type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node,typename std::remove_pointer<T>::type>::value, "The casted type must derive from btk::Node.");
    static_assert(std::is_base_of<Node,typename std::decay<N>::type>::value, "The type of the given object must derive from btk::Node.");
    if (node->isCastable(static_typeid<typename std::remove_pointer<T>::type>()))
      return static_cast<T>(node);
    return nullptr;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename T, typename... Args> 
  inline std::vector<T> make_nodes(size_t num, Args&&... args)
  {
    using N = typename std::remove_pointer<T>::type;
    static_assert(std::is_pointer<T>::value, "The generated type must be a (const) pointer type.");
    static_assert(std::is_base_of<Node, N>::value, "The generated type must derive from btk::Node.");
    std::vector<T> nodes(num,nullptr);
    for (size_t i = 0 ; i < num ; ++i)
      nodes[i] = new N("uname*"+std::to_string(i+1), std::forward<Args>(args)...);
    return nodes;
  }
};

#endif // __btkNode_h