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

#include "btkModel.h"
#include "btkModel_p.h"
#include "btkRig.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  ModelPrivate::ModelPrivate(Model* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  ModelPrivate::~ModelPrivate() = default;
  
  Node* ModelPrivate::makePint() const
  {
    return new Model(this->Name, nullptr); // The rig will be cloned and parent afterward in the Node::clone() method
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Model::Model(const std::string& name, Rig* rig, Node* parent)
  : Node(*new ModelPrivate(this,name), parent)
  {
    if (rig != nullptr)
    {
      this->appendChild(rig);
      rig->setup(this);
    }
  };
  
  Model::~Model() = default;
  
  Node* Model::segments()
  {
    auto pt = this->findChild("Segments",{},false);
    if (pt == nullptr)
      pt = new Node("Segments",this);
    return pt;
  };

  Node* Model::joints()
  {
    auto pt = this->findChild("Joints",{},false);
    if (pt == nullptr)
      pt = new Node("Joints",this);
    return pt;
  };
  
  Rig* Model::rig() const noexcept
  {
    return this->findChild<Rig*>({},{},false);
  };
};