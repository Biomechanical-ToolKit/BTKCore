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

#ifndef __btkProperty_h
#define __btkProperty_h

#include "btkAny.h"
#include "btkLogger.h"
#include "btkMacros.h" // _BTK_CONSTEXPR, _BTK_NOEXCEPT

#include <type_traits>

#define BTK_DECLARE_STATIC_PROPERTIES(derivedclass,baseclass, ...) \
  static_assert(!std::is_same<baseclass,derivedclass>::value,"The base class cannot be the same than the current class."); \
  private: \
  _BTK_STATIC_PROPERTIES(__VA_ARGS__); \
  public: \
  virtual bool staticProperty(const char* key, btk::Any* value) const _BTK_NOEXCEPT override \
  { \
    return (this->baseclass##Private::staticProperty(key,value) ? true : derivedclass##Private::StaticProperties::visit(this->pint(),key,value)); \
  }; \
  virtual bool setStaticProperty(const char* key, const btk::Any* value) _BTK_NOEXCEPT override \
  { \
    return (this->baseclass##Private::setStaticProperty(key,value) ? true : derivedclass##Private::StaticProperties::visit(this->pint(),key,value)); \
  }; \
  private:

#define _BTK_STATIC_PROPERTIES(...) \
  struct StaticProperties : btk::__details::_Properties<StaticProperties> \
  { \
    static inline auto make_properties() -> std::add_lvalue_reference<decltype(std::make_tuple(__VA_ARGS__))>::type \
    { \
      static auto props = std::make_tuple(__VA_ARGS__); \
      return props; \
    }; \
  };

namespace btk
{ 
  template <typename T, typename U, U (T::*A)() const, void (T::*M)(U) = nullptr>
  struct Property
  {
    using ValueType = typename std::decay<U>::type;
    
    const char* Label;
    size_t Size;

    template <size_t N>
    _BTK_CONSTEXPR Property(const char(&l)[N])
    : Label(l), Size(N-1)
    {};
    
    U get(const T* obj) const _BTK_NOEXCEPT
    {
      return (obj->*A)();
    }
    
    void set(T* obj, U value) const _BTK_NOEXCEPT
    {
      if (M == nullptr)
        Logger::error("No mutator method defined for the static property '%s'.",this->Label);
      else
        (obj->*M)(value);
    }
  };
  
  namespace __details
  {   
    template <typename Derived>
    struct _Properties
    {
      template <typename Object, typename Value>
      static inline bool visit(Object* obj, const char* key, Value* val)
      {
        const auto& properties = Derived::make_properties();
        return search<0,std::tuple_size<typename std::decay<decltype(properties)>::type>::value-1>(properties,obj,key,val);
      };
  
    private:
      
      template <typename Prop, typename Object, typename Value>
      static inline bool accept(Prop&& prop, Object* obj, const char* key, Value* val)
      {
        if (strncmp(key,prop.Label,prop.Size) == 0)
        {
          val->assign(prop.get(obj));
          return true;
        }
        return false;
      }
      
      template <typename Prop, typename Object, typename Value>
      static inline bool accept(Prop&& prop, Object* obj, const char* key, const Value* val)
      {
        if (strncmp(key,prop.Label,prop.Size) == 0)
        {
          using value_t = typename std::remove_const<typename std::remove_reference<Prop>::type>::type::ValueType;
          prop.set(obj,val->template cast<value_t>());
          return true;
        }
        return false;
      }
      
      template <size_t Index, size_t Size, typename Props, typename Object, typename Value>
      static inline typename std::enable_if<Index != Size, bool>::type search(Props&& properties, Object* obj, const char* key, Value* val)
      {
        if (accept(std::get<Index>(properties),obj,key,val))
          return true;
        return search<Index+1,Size>(std::forward<Props>(properties),obj,key,val);
      };
      
      template <size_t Index, size_t Size, typename Props, typename Object, typename Value>
      static inline typename std::enable_if<Index == Size, bool>::type search(Props&& properties, Object* obj, const char* key, Value* val)
      {
        return accept(std::get<Size>(properties),obj,key,val);
      };
    };
  };
  
  /**
   * @struct Property btkProperty.h
   * @brief Define a static propery used in btk::Node class or inheriting classes.
   * @tparam T Class receiving the static property
   * @tparam U Exact type returned/received by the accessor/mutator given to the constructor.
   *
   * A Property object define a static property. Compared to a dynamic one, a static property
   * is linked with a accessor and optionaly a mutator declared in the same class than the property.
   *
   * Because a Node object can store properties with any string as key, a user
   * could decide to call a property with a key corresponding to an existing member (for example, Node's name).
   * What should be the behaviour of this property? Instead of having a possible ambiguity between
   * properties and other members known at compile time (or even data duplication), it was decided to implement a mechanism 
   * of static and dynamic properties transparent to the user. The static properties would
   * reflect some members known at compile time, while dynamic properties are created during run time.
   *
   * @note the type @a U must be exactly the same than the type returned/required by the accessor/mutator with all the qualifiers. For example, if the accessor is const std::string& name() const, and the mutator is void setName(const std::string& name), then @a U as to be set to const std::string& (i.e. const reference to a std::string).
   *
   * A static property is defined only by is constructor. All the other methods are used only internaly.
   * Moreover, this constructor has to be used in the macro BTK_DECLARE_STATIC_PROPERTIES which itself must be included in the declaration of the private implementation of a class inheriting of btk::Node.
   * For example:
   * 
   * @code
   * class TestNodePrivate;
   * 
   * class TestNode : public btk::Node
   * {
   *   BTK_DECLARE_PIMPL_ACCESSOR(TestNode) // For the private implementation (pimpl)
   *   
   * public:
   *   TestNode(const std::string& name);
   *   
   *   int version() const;
   *   void setVersion(int value);
   * };
   * 
   * class TestNodePrivate : public btk::NodePrivate
   * {
   *   BTK_DECLARE_PINT_ACCESSOR(TestNode) // To have access to the public interface (pint) from the private implementation
   *   
   *   BTK_DECLARE_STATIC_PROPERTIES(TestNodePrivate, btk::NodePrivate,
   *     btk::Property<TestNode,int,&TestNode::version,&TestNode::setVersion>("version")
   *   )
   *   
   * public:
   *   TestNodePrivate(TestNode* pint, const std::string& name) :  btk::NodePrivate(pint,name), Version(1) {};
   *   int Version;
   * };
   * @endcode
   *
   * In this example, The class TestNode will have a new property name "version" which is associated with the accessor TestNode::version and the mutator TestNode::setVersion. Internally, these methods rely on the member TestNodePrivate::Version.
   * If the member is read-only the pointer to the mutation must be set to nullptr. For example:
   *
   * @code
   * btk::Property<TestNode,int,&TestNode::version>("version") // by default the mutator is set to nullptr
   * @endcode
   */
  
  /**
   * @var Property::Label
   * Array of characters corresponding to the name of the property.
   */
  /**
   * @var Property::Size
   * Size of the array.
   */
  /**
   * @var Property::Accessor
   * Pointer to member function with the prototype: U accessor() const;
   */
  /**
   * @var Property::Mutator
   * Pointer to member function with the prototype: void mutator(U );
   */
  
  /**
   * @fn template <typename T, typename U> template <std::size_t N, typename A = std::nullptr_t, typename M = std::nullptr_t> constexpr Property<T,U>::Property(const char(&l)[N], A&& a = nullptr, M&& m = nullptr)
   * Constructor.
   */ 

  /**
   * @fn template <typename T, typename U> template <typename Object> inline bool Property<T,U>::accept(Object* obj, const char* label, const Any* value) const
   * Check if the given @a label correspond to the name of the property. 
   * If it is the case, then the content of the property is modified by using the 
   * content of the given @a value. Internally, the associated mutator is used with the
   * callable object @a obj. The content of the @a value object is casted to the type 
   * requested by the mutator.
   */

  /**
   * @fn template <typename T, typename U> template <typename Object> inline bool  Property<T,U>::accept(const Object* obj, const char* label, Any* value) const
   * Check if the given @a label correspond to the name of the property. 
   * If it is the case, then the content of the property is copied to the given @a value.
   * Internally, the associated accessor is used with the callable object @a obj.
   */

  /**
   * @struct Properties btkProperty.h
   * @brief Template interface used to manage static properties defined for a class. 
   * @tparam Derived Name of the derived class to create a CRTP (Curiously recurring template pattern).
   *
   * The goal of this interface is to visit each static properties defined for an object and 
   * see if the given key exist. If it is the case, then the associated given value is accessed 
   * or modified depending the context. All this part is transparent for the user which will use
   * the method Node::property() and Node::setProperty().
   *
   * @note This class should not be used directly but instead the macro BTK_DECLARE_STATIC_PROPERTIES.
   */
  
  /**
   * @fn template <typename Derived> template <typename Object, typename Value> static inline bool Properties<Derived>::visit(Object* obj, const char* key, Value* val)
   * Visit known static properties defined in @a Derived and if one of them corresponds to the @a key, then the assocated accessor and/or mutator is applied on the callable object @a obj combined with the value @a val.
   */
};

#endif // __btkProperty_h