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

#ifndef __btkNode_p_h
#define __btkNode_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkObject_p.h"
#include "btkTypeid.h"
#include "btkNodeid.h" // Macro BTK_DECLARE_NODEID used by inheriting classes.

#include <string>
#include <unordered_map>
#include <list>

namespace btk
{
  class Node;
  
  class NodePrivate : public ObjectPrivate
  {
    BTK_DECLARE_PINT_ACCESSOR(Node)

  public:
    static bool retrievePath(std::list<const Node*>& path, const Node* current, const Node* stop);
    
    NodePrivate() = delete;
    NodePrivate(Node* pint, const std::string& name);
    ~NodePrivate() noexcept;
    NodePrivate(const NodePrivate& ) = delete;
    NodePrivate(NodePrivate&& ) noexcept = delete;
    NodePrivate& operator=(const NodePrivate& ) = delete;
    NodePrivate& operator=(const NodePrivate&& ) noexcept = delete;
    
    virtual bool castable(typeid_t id) const noexcept;
    
    virtual bool staticProperty(const char* key, btk::Any* value) const noexcept;
    virtual bool setStaticProperty(const char* key, const btk::Any* value) noexcept;
    
    bool attachParent(Node* node) noexcept;
    bool detachParent(Node* node) noexcept;
    
    bool attachChild(Node* node) noexcept;
    bool detachChild(Node* node) noexcept;
    
    std::string Name;
    std::string Description;
    std::unordered_map<std::string,Any> Properties;
    std::list<Node*> Parents;
    std::list<Node*> Children;
    
  protected:
    Node* mp_Pint;
  };
};

#endif // __btkObject_p_h
