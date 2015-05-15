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

#include "btkTrial.h"
#include "btkTrial_p.h"
#include "btkTimeSequence.h"
#include "btkEvent.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  TrialPrivate::TrialPrivate(Trial* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  TrialPrivate::~TrialPrivate() _BTK_NOEXCEPT = default;
  
  Node* TrialPrivate::makePint() const
  {
    return new Trial(this->Name);
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Trial::Trial(const std::string& name, Node* parent)
  : Node(*new TrialPrivate(this,name),parent)
  {};
  
  Trial::~Trial() _BTK_NOEXCEPT = default;
  
  Node* Trial::timeSequences()
  {
    auto pt = this->findChild("TimeSequences",{},false);
    if (pt == nullptr)
      pt = new Node("TimeSequences",this);
    return pt;
  };
  
  TimeSequence* Trial::timeSequence(unsigned idx)
  {
    auto node = this->timeSequences();
    if (node->children().size() <= idx)
      return nullptr;
    auto it = node->children().begin();
    std::advance(it,idx);
    return node_cast<TimeSequence*>(*it);
  };
  
  Node* Trial::events()
  {
    auto pt = this->findChild("Events",{},false);
    if (pt == nullptr)
      pt = new Node("Events",this);
    return pt;
  };
  
  Event* Trial::event(unsigned idx)
  {
    auto node = this->events();
    if (node->children().size() <= idx)
      return nullptr;
    auto it = node->children().begin();
    std::advance(it,idx);
    return node_cast<Event*>(*it);
  };
};