/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include "btkNode.h"
#include "btkNode_p.h"
#include "btkProperty.h"
#include "btkLogger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  _BTK_STATIC_PROPERTIES(
    Property<Node,const std::string&>{"name", &Node::name, &Node::setName},
    Property<Node,const std::string&>{"description", &Node::description, &Node::setDescription}
  );

  // ------------------------------------------------------------------------ //
  
  NodePrivate::NodePrivate(Node* pint, const std::string& name)
  : ObjectPrivate(), mp_Pint(pint), Name(name), Description(), Properties(), Parents(), Children()
  {};
  
  NodePrivate::~NodePrivate() noexcept
  {};
  
  bool NodePrivate::castable(nodeid_t id) const noexcept
  {
    return (nodeid<Node>() == id);
  };
  
  bool NodePrivate::staticProperty(const char* key, btk::Any* value) const noexcept
  {
    return StaticProperties::visit(this->pint(),key,value);
  };
  
  bool NodePrivate::setStaticProperty(const char* key, const btk::Any* value) noexcept
  {
    return StaticProperties::visit(this->pint(),key,value);
  };
}

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class Node btkNode.h
   * @brief Base class for the data structure
   *
   * The data struture in BTK is mostly a tree-like structure (more exactly it is a graph as a node can have several parents).
   * The general idea is to store data in a dynamic structure without the need to modify the internal storage each time a new category of data is added (e.g. pressure, GPS, etc.).
   * Thus, it would be simpler to integrate new kind of file formats as well as new models.
   *
   * Children nodes are owned by their parent. Thus only the first parent(s) (e.g. the root' tree) has to be deleted. 
   * This one can be stored in a smart pointer (shared, unique pointer) to not manage its deletion.
   * For example:
   *
   * @code
   * btk::Node* root = new btk::Node("root");
   * btk::Node* leafA = new btk::Node("leafA",&root); // Owned by the root
   * btk::Node* leafB = new btk::Node("leafB",&root); // Owned by the root
   * // ...
   * delete leafB; // Early deletion. LeafB is removed from the root's children.
   * // ...
   * delete root; // End of the program/function, the root is deleted and leafA at the same time.
   * @endcode
   *
   * It is not necessary to create nodes on the heap (using the new operator).
   * If the nodes are created on the stack (using regular constructor), they must follow a specific order: the parent must be created before the children.
   * For example:
   *
   * @code
   * btk::Node root("root");
   * btk::Node leafA("leafA",&root); // Owned by the root
   * btk::Node leafB("leafB",&root); // Owned by the root
   * // ...
   * // end of the program/function. Nodes leafB, leafA, and root are destroyed in thid order.
   * @endcode
   *
   * However, in case this order is not respected, a runtime error (or crash) should occur like in the next example:
   *
   * @code
   * btk::Node leafA("leafA");
   * btk::Node root("root");
   * btk::Node leafB("leafB",&root); // Owned by the root
   * leafA.appendParent(&root); // Owned by the root
   * // ...
   * // End of the program/function, leafB is destroyed, then root which destroys also its children. What about leafA?
   * @endcode
   *
   * In the previous example, the remaining child of root (pointer to leafA) is a local variable and calling its destructor is incorrect. Thus, when the variable leafA goes out of scope, its destructor is called again. The same memory is freed two times that should crash the program.
   */
  
  /*
   * Constructor. The @a parent node takes ownership of this object.
   * @note It is adviced to have a unique @a name. This would simplify the research of specific nodes with the methods findChild() and findChildren().
   */
  Node::Node(const std::string& name, Node* parent)
  : Node::Node(*new NodePrivate(this,name),parent)
  {};
  
  /**
   * Destructor.
   * Detach this object of these children. In case a child has no more parent, it is deleted.
   * Detach also this object of these parents
   */
  Node::~Node()
  {
    auto optr = this->pimpl();
    for (std::list<Node*>::iterator it = optr->Children.begin() ; it != optr->Children.end() ; ++it)
    {
      (*it)->detachParent(this);
      if (!(*it)->hasParents())
        delete *it;
    }
    for (std::list<Node*>::iterator it = optr->Parents.begin() ; it != optr->Parents.end() ; ++it)
      (*it)->detachChild(this);
  };

  /*
   * Returns the name of the node.
   * You can also access to this information using the property 'name'.
   */
  const std::string& Node::name() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Name;
  };
  
 /*
  * Sets the name of the node.
  * You can also modify this information using the property 'name'.
  * In case the value is different, the state of the node is set to modified.
  */
  void Node::setName(const std::string& name) noexcept
  {
    auto optr = this->pimpl();
    if (name == optr->Name)
      return;
    optr->Name = name;
    this->modified();
  };
  
  /*
   * Returns the description of the node. By default the description is empty.
   * You can also access to this information using the property 'description'.
   */
  const std::string& Node::description() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Description;
  };
  
  /*
   * Sets the description of the node. By default the description is empty.
   * You can also modify this information using the property 'description'.
   * @note The state of the node is not modified if the description is different.
   */
  void Node::setDescription(const std::string& value) noexcept
  {
    auto optr = this->pimpl();
    optr->Description = value;
  };
  
  /**
   * Returns the value's property associated to the given @a key.
   * The value is a Any object and can be converted to several types implicity.
   * For example, the following code shows two ways to extract property's value.
   *
   * @code
   * btk::Node node("node");
   * node.setProperty("count","1");
   * // First way: use a Any object
   * btk::Any value = node.property("count");
   * int count1 = value.cast<int>();
   *  // Second way: implicit type conversion operator
   * int count2 = node.property("count");
   * @endcode
   */ 
  Any Node::property(const std::string& key) const noexcept
  {
    auto optr = this->pimpl();
    Any value;
    bool caught = optr->staticProperty(key.c_str(),&value);
    if (!caught)
    {
      std::unordered_map<std::string,Any>::const_iterator it = optr->Properties.find(key);
      if (it != optr->Properties.end())
        value = it->second;
    }
    return value;
  };
  
  /**
   * Sets the property @a key with the given @a value.
   * The @a value can be an Any object or a type supported by it (e.g. int, double, std::string,).
   * In case the property does not exist, or its value is different, or it is removed, the state of the node is set to modified.
   */
  void Node::setProperty(const std::string& key, const Any& value) noexcept
  {
    auto optr = this->pimpl();
    bool caught = optr->setStaticProperty(key.c_str(),&value);
    if (!caught)
    {
      auto it = optr->Properties.find(key);
      // Existing property
      if (it != optr->Properties.end())
      {
        // Modify its value
        if (value.isValid())
        {
          if (value != it->second)
          {
            it->second = value;
            this->modified();
          }
        }
        // Or remove it
        else
        {
          optr->Properties.erase(it);
          this->modified();
        }
      }
      // In case it does not exist add it
      else if (value.isValid())
      {
        optr->Properties[key] = value;
        this->modified();
      }
    }
  };
  
  /**
   * Returns the list of children attached with this node.
   */ 
  const std::list<Node*>& Node::children() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Children;
  };
  
  /**
   * Returns true if the node has at least one child, false otherwise
   */
  bool Node::hasChildren() const noexcept
  {
    auto optr = this->pimpl();
    return !optr->Children.empty();
  };
  
  /**
   * Append a node if this one is not already a child.
   * In case the child is appended, this node is attached as parent and its state is set to modified.
   */
  void Node::appendChild(Node* node) noexcept
  {
    if (this->attachChild(node))
    {
      node->attachParent(this);
      this->modified();
    }
  };  
  
  /**
   * Returns the list of parents attached with this node.
   */ 
  const std::list<Node*>& Node::parents() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Parents;
  };
  
  /**
   * Returns true if the node has at least one parent, false otherwise.
   */
  bool Node::hasParents() const noexcept
  {
    auto optr = this->pimpl();
    return !optr->Parents.empty();
  };
  
  /**
   * Appends a node if this one is not already a parent.
   * In case this node is appended, @a node is attached as parent and its state is set to modified.
   */
  void Node::appendParent(Node* node) noexcept
  {
    node->appendChild(this);
  };  
  
  /**
   * Checks if @a node is already a parent and add it if it is not the case.
   * In case @a node is already a parent a warning message is send to the message logger.
   */
  void Node::attachParent(Node* node) noexcept
  {
    auto optr = this->pimpl();
    for (std::list<Node*>::const_iterator it = optr->Parents.begin() ; it != optr->Parents.end() ; ++it)
    {
      if (*it == node)
      {
        Logger::warning("The parent '%s' was already attached to the node '%s'", node->name().c_str(), optr->Name.c_str());
        return;
      }
    }
    optr->Parents.push_back(node);
  };
  
  /**
   * Removes the given @a node if it is a parent of the current object.
   */
  void Node::detachParent(Node* node) noexcept
  {
    auto optr = this->pimpl();
    optr->Parents.remove(node);
  };
  
  /**
   * Checks if @a node is already a child and add it if it is not the case.
   * In case @a node is already a child, this method returns false, true otherwise.
   */
  bool Node::attachChild(Node* node) noexcept
  {
    auto optr = this->pimpl();
    for (std::list<Node*>::const_iterator it = optr->Children.begin() ; it != optr->Children.end() ; ++it)
    {
      if (*it == node)
        return false;
    }
    optr->Children.push_back(node);
    return true;
  };
  
  /**
   * Removes the given @a node if it is a child of the current object.
   */
  void Node::detachChild(Node* node) noexcept
  {
    auto optr = this->pimpl();
    optr->Children.remove(node);
  };

  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  Node::Node(NodePrivate& pimpl, Node* parent) noexcept
  : Object(pimpl)
  {
    if (parent != nullptr)
      parent->appendChild(this);
  };
  
  /**
   * @fn template <typename T = Node*> T findChild(const std::string& name = std::string(), bool recursiveSearch = true) const noexcept
   * Returns the child with the given @a name and which can be casted to the type T. The search can be done recursively (by default) or only in direct children. The latter is available by setting the second argument @a recursiveSearch to false.
   * There are three ways to use this methods.
   *
   * You can explicitely define the type @a T and the @a name.
   * @code
   * btk::Event* = root.findChild<btk::Event*>("RHS2");
   * @endcode
   *
   * You can also only give the type @a T. Then the first node found with this type will be returned. In this case, no name is given as it is set to a null string (not empty)
   * @code
   * btk::ForcePlatform* = root.findChild<btk::ForcePlatform*>();
   * @endcode
   *
   * By default, the default type is set to btk::Node*, so you can even search for a "node" without giving the type..
   * @code
   * btk::Node* foo = root.findChild(); // First child
   * btk::Node* bar = root.findChild("bar"); // A node with the name "bar"
   * @endcode
   *
   * In any case you can set the second argument to false for a search in the direct children only.
   * @code
   * btk::Event* rhs2 = root.findChild<btk::Event*>("RHS2",false);
   * btk::ForcePlatform* fp = root.findChild<btk::ForcePlatform*>(std::string(),false); // std::string() and "" are the same.
   * btk::Node* foo = root.findChild(std::string(),false);
   * btk::Node* bar = root.findChild("bar",false);
   * @endcode
   */
  
  /**
   * @fn template <typename T = Node*> std::list<T> findChildren(const std::string& name = std::string(), bool recursiveSearch = true) const noexcept
   * Returns the children with the given @a name and which can be casted to the type T. The search can be done recursively (by default) or only in direct children. The latter is available by setting the second argument @a recursiveSearch to false.
   * As with the method findChild(), you can explicitely or implicitely give the type and/or the name of the children. For example:
   * @code
   * std::list<btk::Event*> events = root.findChildren<btk::Event*>(); // Find all events
   * std::list<btk::Event*> footstrikes = root.findChildren<btk::Event*>("Foot Strike"); // Find all foot strike events
   * std::list<btk::Node*> nodes = root.findChildren("Foot Strike"); // Find all node with the name "Foot Strike"
   * @endcode
   */
  
  /**
   * @fn template <typename T = Node*> std::list<T> findChildren(const std::regex& regexp, bool recursiveSearch = true) const noexcept
   * Conveniant method to find children using a regular expression.
   */
  
  /**
   * Implementation of the findChild method.
   */
  Node* Node::findNode(nodeid_t id, const std::string& name, bool recursiveSearch) const noexcept
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
    {
      Node* node = *it;
      if (node->pimpl()->castable(id) && (name.empty() || (node->name() == name)))
        return node;
    }
    // In case no corresponding child was found and the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
      {
        Node* node = (*it)->findNode(id,name,recursiveSearch);
        if (node != nullptr)
          return node;
      }
    }
    return nullptr;
  };
  
  /**
   * Implementation of the findChildren method.
   */
  void Node::findNodes(std::list<void*>* list, nodeid_t id, const std::string& name, bool recursiveSearch) const noexcept
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
    {
      Node* node = *it;
      if (node->pimpl()->castable(id) && (name.empty() || (node->name() == name)))
        list->emplace_back(node);
    }
    // In case the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
        (*it)->findNodes(list,id,name,recursiveSearch);
    }
  };
  
  /**
   * Implementation of the findChildren method.
   */
  void Node::findNodes(std::list<void*>* list, nodeid_t id, const std::regex& regexp, bool recursiveSearch) const noexcept
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
    {
      Node* node = *it;
      if (node->pimpl()->castable(id) && std::regex_match(node->name(),regexp))
        list->emplace_back(node);
    }
    // In case the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
        (*it)->findNodes(list,id,regexp,recursiveSearch);
    }
  };
  
  /**
   * Returns true if the current object is castable to another with the given @a nodeid_t value, false otherwise.
   */
  bool Node::castable(nodeid_t id)
  {
    return this->pimpl()->castable(id);
  };
  
  /**
   * @fn template <typename U> U node_cast(Node* node) noexcept
   * Conveniant function to cast a pointer to a Node object to another type U. If it is not possible, this method returns nullptr.
   *
   * @code
   * btk::Node root("root");
   * btk::Node* event = new btk::Event("RHS1",&root);
   * btk::Event* evt = node_cast<btk::Event*>(event); // Ok
   * btk::TimeSequence* tsq = node_cast<btk::TimeSequence*>(event); // nullptr
   * @endcode
   *
   * @note The type U must be a pointer type which inherit of the Node class. If this is not the case a compilation error will be thrown.
   */
};