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

#ifndef __btkNodeid_h
#define __btkNodeid_h

#define BTK_DECLARE_NODEID(derivedclass,baseclass) \
  static_assert(!std::is_same<baseclass,derivedclass>::value,"The base class cannot be the same than the current class."); \
  virtual bool castable(btk::nodeid_t id) const noexcept override \
  { \
    if (this->baseclass##Private::castable(id)) \
      return true; \
    return (btk::nodeid<derivedclass>() == id); \
  };

namespace btk
{
  class nodeid_t
  {
  public:
    nodeid_t() = delete;
    ~nodeid_t() = default;
    nodeid_t(const nodeid_t& ) = default;
    nodeid_t(nodeid_t&& ) noexcept = default;
    nodeid_t& operator=(const nodeid_t& ) = default;
    nodeid_t& operator=(nodeid_t&& ) noexcept = default;
    
    friend constexpr bool operator==(nodeid_t lhs, nodeid_t rhs) noexcept {return (lhs.id == rhs.id);};
    friend constexpr bool operator!=(nodeid_t lhs, nodeid_t rhs) noexcept {return (lhs.id != rhs.id);};
    
  private:
    template<typename T> friend constexpr nodeid_t nodeid() noexcept;
    
    using sig = nodeid_t();
    sig* id;
    
    constexpr nodeid_t(sig* id) : id{id} {}
  };

  template<typename T>
  constexpr nodeid_t nodeid() noexcept
  {
    return &nodeid<T>;
  };
  
  /**
   * @class nodeid_t btkNodeid.h
   * @brief Unique identifier for each inherting Node class
   *
   * This class is used only to determine if node's children can be cast to a specfic derived class (see Node:findChild(), Node::findChildren().
   * However the code is universal and could be used with any type. Indeed, the identifier is unique for any type (i.e. nodeid<int>() != nodeid<float>() != nodeid<Node>() != nodeid<Node*>() != nodeid<const Node>(), etc.)
   *
   * Largely inspired by http://codereview.stackexchange.com/questions/48594/unique-type-id-no-rtti
   */
  
  /**
   * @fn template<typename T> constexpr nodeid_t nodeid() noexcept
   * Returns the identifier associated with the given template type
   */
}

#endif // __btkNodeid_h