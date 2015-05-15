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

#include "btkNode.h"
#include "btkNode_p.h"
#include "btkProperty.h"
#include "btkLogger.h"
#include "btkMacros.h" // _BTK_UNUSED

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
  : ObjectPrivate(),
    Name(name), Description(), Properties(), Parents(), Children(),
    mp_Pint(pint)
  {};
  
  NodePrivate::~NodePrivate() noexcept = default;
  
  bool NodePrivate::retrievePath(std::list<const Node*>& path, const Node* current, const Node* stop)
  {
    const auto& children = current->children();
    // Direct child search
    for (auto it = children.cbegin() ; it != children.cend() ; ++it)
    {
      if (*it == stop)
      {
        path.push_back(current);
        path.push_back(*it);
        return true;
      };
    }
    // Deep child search
    for (auto it = children.cbegin() ; it != children.cend() ; ++it)
    {
      std::list<const Node*> temp;
      if (NodePrivate::retrievePath(temp,*it,stop))
      {
        path.push_back(current);
        path.splice(path.end(),temp);
        return true;
      }
    }
    return false;
  };
  
  bool NodePrivate::castable(typeid_t id) const noexcept
  {
    return (static_typeid<Node>() == id);
  };
  
  bool NodePrivate::staticProperty(const char* key, btk::Any* value) const noexcept
  {
    return StaticProperties::visit(this->pint(),key,value);
  };
  
  bool NodePrivate::setStaticProperty(const char* key, const btk::Any* value) noexcept
  {
    return StaticProperties::visit(this->pint(),key,value);
  };
  
  /**
   * Checks if @a node is already a parent and add it if it is not the case.
   * In case @a node is already a parent a warning message is send to the message logger.
   * This method returns also false if the given @a node is null.
   */
  bool NodePrivate::attachParent(Node* node) noexcept
  {
    if (node == nullptr)
      return false;
    for (auto it = this->Parents.cbegin() ; it != this->Parents.cend() ; ++it)
    {
      if (*it == node)
      {
        Logger::warning("The parent '%s' was already attached to the node '%s'", node->name().c_str(), this->Name.c_str());
        return false;
      }
    }
    this->Parents.push_back(node);
    return true;
  };
  
  /**
   * Removes the given @a node if it is a parent of the current object.
   * In case @a node is not a parent or is null, this method returns false.
   */
  bool NodePrivate::detachParent(Node* node) noexcept
  {
    if (node == nullptr)
      return false;
    for (auto it = this->Parents.begin() ; it != this->Parents.end() ; ++it)
    {
      if (*it == node)
      {
        this->Parents.erase(it);
        return true;
      }
    }
    return false;
  };
  
  /**
   * Checks if @a node is already a child and add it if it is not the case.
   * In case @a node is already a child, this method returns false, true otherwise.
   * This method returns also false if the given @a node is null.
   */
  bool NodePrivate::attachChild(Node* node) noexcept
  {
    if (node == nullptr)
      return false;
    for (auto it = this->Children.cbegin() ; it != this->Children.cend() ; ++it)
    {
      if (*it == node)
        return false;
    }
    this->Children.push_back(node);
    return true;
  };
  
  /**
   * Removes the given @a node if it is a child of the current object.
   */
  bool NodePrivate::detachChild(Node* node) noexcept
  {
    if (node == nullptr)
      return false;
    for (auto it = this->Children.begin() ; it != this->Children.end() ; ++it)
    {
      if (*it == node)
      {
        this->Children.erase(it);
        return true;
      }
    }
    return false;
  };
  
  Node* NodePrivate::makePint() const
  {
    return new Node(this->Name);
  };
  
  
  void NodePrivate::finalizePint(Node* pint) const
  {
    _BTK_UNUSED(pint)
  };
};

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
  Node::~Node() noexcept
  {
    this->clear();
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
  void Node::setName(const std::string& value) noexcept
  {
    auto optr = this->pimpl();
    if (value == optr->Name)
      return;
    optr->Name = value;
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
  void Node::setProperty(const std::string& key, const Any& value)
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
    if (this->pimpl()->attachChild(node))
    {
      node->pimpl()->attachParent(this);
      this->modified();
    }
  };
  
  /**
   * Remove the given @a node from the list of the children.
   * @note It is the responsability to the developer to delete the given node if this one has no more parent.
   */
  void Node::removeChild(Node* node) noexcept
  {
    if (this->pimpl()->detachChild(node))
    {
      node->pimpl()->detachParent(this);
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
    if (this->pimpl()->attachParent(node))
    {
      node->pimpl()->attachChild(this);
      node->modified();
    }
  };
  
  /**
   * Remove the given @a node from the list of the parent.
   * @note It is the responsability to the developer to delete this node if this one has no more parent.
   */
  void Node::removeParent(Node* node) noexcept
  {
    if (this->pimpl()->detachParent(node))
    {
      node->pimpl()->detachChild(this);
      node->modified();
    }
  };
  
  /**
   * Overload method which modify this object as well as all these parents.
   */
  void Node::modified() noexcept
  {
    auto optr = this->pimpl();
    this->Object::modified();
    for (auto& parent : optr->Parents)
      parent->modified();
  };
  
  /**
   * Removes all parents, children and properties
   * If a child has no more parent, this one is deleted.
   */
  void Node::clear() noexcept
  {
    auto optr = this->pimpl();
    if (optr->Parents.empty() && optr->Children.empty() && optr->Properties.empty())
      return;
    optr->Properties.clear();
    for (auto it = optr->Children.begin() ; it != optr->Children.end() ; ++it)
    {
      (*it)->pimpl()->detachParent(this);
      if (!(*it)->hasParents())
        delete *it;
    }
    for (auto it = optr->Parents.begin() ; it != optr->Parents.end() ; ++it)
      (*it)->pimpl()->detachChild(this);
    this->modified();
  };
  
  /**
   * 
   */
  Node* Node::clone(Node* parent) const
  {
    auto optr = this->pimpl();
    auto node = optr->makePint();
    auto node_pimpl = node->pimpl();
    node_pimpl->Description = optr->Description;
    node_pimpl->Properties = optr->Properties;
    for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
      (*it)->clone(node);
    node->appendParent(parent);
    return node;
  };
  
  // /**
  //  *
  //  */
  // void Node::copy(Node* other)
  // {
  //   if (other == nullptr)
  //     return;
  //   auto optr = this->pimpl();
  //   optr->copy(other);
  // };
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  Node::Node(NodePrivate& pimpl, Node* parent) noexcept
  : Object(pimpl)
  {
    this->appendParent(parent);
  };
  
  void Node::replaceChild(Node* current, Node* substitute)
  {
    if (current == substitute)
      return;
    if (current != nullptr)
    {
      this->removeChild(current);
      if (!current->hasParents())
        delete current;
    }
    this->appendChild(substitute);
    // removeChild() and appendChild() internally call modified(). No need to call it explicitely
  };
  
  /**
   * @fn template <typename T = Node*> T findChild(const std::string& name = {}, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const noexcept
   * Returns the child with the given @a name and which can be casted to the type T. You can refine the search by adding @a properties to match. The search can be done recursively (by default) or only in direct children. The latter is available by setting @a recursiveSearch to false.
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
   * In addition, you can add properties to refine the research. Sometimes this could be usefull to distinguish events with the same name but different properties.
   * As presented in the following examples, it is adviced to give matching properties in an initializer list using curly brackets due to the use of the move semantics in the method. Inside, each matching property is given as a pair {key, value} given also by an initializer list. So, for a single matching property, two pairs of curly brackets are used but this is correct.
   * @code
   * btk::Node events("Events");
   * btk::Event evtA("Foo",0.0,"Right","JDoe",&events);
   * btk::Event evtB("Bar",0.0,"Left","JDoe",&events);
   * btk::Event evtC("Toto",1.1,"Left","JDoe",&events);
   * btk::Event evtD("Toto",1.5,"Right","Babar",&events);
   * events.findChild<btk::Event*>("Toto",{{"time",1.5}}); // pointer to evtD object
   * events.findChild<btk::Event*>({},{{"time",0.0},{"context","Left"}}); // pointer to evtB object
   * @endcode
   *
   * In any case you can set the third argument to false for a search in the direct children only. If no properties are added, the second arguement to give is an empty pair or curly brackets (i.e. {}).
   * @code
   * btk::Event* rhs2 = root.findChild<btk::Event*>("RHS2",{},false);
   * btk::ForcePlatform* fp = root.findChild<btk::ForcePlatform*>({},{},false); // {} and std::string() and "" are the same for the name.
   * btk::Node* foo = root.findChild({},{},false);
   * btk::Node* bar = root.findChild("bar",{},false);
   * @endcode
   */
  
  /**
   * @fn template <typename T = Node*> std::list<T> findChildren(const std::string& name = {}, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const noexcept
   * Returns the children with the given @a name and which can be casted to the type T. You can refine the search by adding @a properties to match. The search can be done recursively (by default) or only in direct children. The latter is available by setting @a recursiveSearch to false.
   * As with the method findChild(), you can explicitely or implicitely give the type and/or the name of the children. For example:
   * @code
   * std::list<btk::Event*> events = root.findChildren<btk::Event*>(); // Find all events
   * std::list<btk::Event*> footstrikes = root.findChildren<btk::Event*>("Foot Strike"); // Find all foot strike events
   * std::list<btk::Node*> nodes = root.findChildren("Foot Strike"); // Find all node with the name "Foot Strike"
   * @endcode
   *
   * It is adviced to give matching properties by using an initializer list due to the use of the move semantics in the method.
   * @code
   * btk::Node events("Events");
   * btk::Event evtA("Foo",0.0,"Right","JDoe",&events);
   * btk::Event evtB("Bar",0.0,"Left","JDoe",&events);
   * btk::Event evtC("Toto",1.1,"Left","JDoe",&events);
   * btk::Event evtD("Toto",1.5,"Right","Babar",&events);
   * events.findChildren<btk::Event*>({},{{"time",0.0}}); // evtA and evtB
   * events.findChildren<btk::Event*>({},{{"context","Right"}}); // evtA and evtD
   * events.findChildren<btk::Event*>({},{{"context","Left"}}); // evtB and evtC
   * events.findChildren<btk::Event*>({},{{"subject","JDoe"}}); // evtA, evtB, and evtC
   * @endcode
   */
  
  /**
   * @fn template <typename T = Node*> std::list<T> findChildren(const std::regex& regexp, std::list<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const noexcept
   * Convenient method to find children using a regular expression.
   */
  
  /**
   * Retrieves the first path existing between the current node and the given @a node.
   * If no path exists between both, then an empty list is returned, 
   * The first node in the retrieved path is the current one, while the last is the node to search.
   */
  std::list<const Node*> Node::retrievePath(const Node* node) const noexcept
  {
    auto optr = this->pimpl();
    std::list<const Node*> path;
    optr->retrievePath(path,this,node);
    return path;
  };
  
  /**
   * Implementation of the findChild method.
   */
  Node* Node::findNode(typeid_t id, const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
    {
      Node* node = *it;
      if (node->pimpl()->castable(id) && (name.empty() || (node->name() == name)))
      {
        bool found = true;
        for (auto it = properties.cbegin() ; it != properties.cend() ; ++it)
        {
          if (node->property(it->first) != it->second)
          {
            found = false;
            break;
          }
        }
        if (found)
          return node;
      }
    }
    // In case no corresponding child was found and the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
      {
        Node* node = (*it)->findNode(id,name,std::move(properties),recursiveSearch);
        if (node != nullptr)
          return node;
      }
    }
    return nullptr;
  };
  
  /**
   * Implementation of the findChildren method.
   */
  void Node::findNodes(std::list<void*>* list, typeid_t id, const std::string& name, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (auto itN = optr->Children.cbegin() ; itN != optr->Children.cend() ; ++itN)
    {
      Node* node = *itN;
      if (node->pimpl()->castable(id) && (name.empty() || (node->name() == name)))
      {
        bool found = true;
        for (auto itP = properties.cbegin() ; itP != properties.cend() ; ++itP)
        {
          if (node->property(itP->first) != itP->second)
          {
            found = false;
            break;
          }
        }
        if (found)
          list->emplace_back(node);
      }
        
    }
    // In case the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
        (*it)->findNodes(list,id,name,std::move(properties),recursiveSearch);
    }
  };
  
  /**
   * Implementation of the findChildren method.
   */
  void Node::findNodes(std::list<void*>* list, typeid_t id, const std::regex& regexp, std::list<std::pair<std::string,Any>>&& properties, bool recursiveSearch) const noexcept
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (auto itN = optr->Children.cbegin() ; itN != optr->Children.cend() ; ++itN)
    {
      Node* node = *itN;
      if (node->pimpl()->castable(id) && std::regex_match(node->name(),regexp))
      {
        bool found = true;
        for (auto itP = properties.cbegin() ; itP != properties.cend() ; ++itP)
        {
          if (node->property(itP->first) != itP->second)
          {
            found = false;
            break;
          }
        }
        if (found)
          list->emplace_back(node);
      }
    }
    // In case the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
        (*it)->findNodes(list,id,regexp,std::move(properties),recursiveSearch);
    }
  };
  
  /**
   * Returns true if the current object is castable to another with the given @a typeid_t value, false otherwise.
   */
  bool Node::castable(typeid_t id) const noexcept
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