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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DINode* trialsRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __btkRig_h
#define __btkRig_h

#include "btkModelExport.h"
#include "btkNode.h"

#include <string>

namespace btk
{
  class Model;
  class Trial;
  class LandmarksTranslator;
  
  class RigPrivate;
  
  class BTK_MODEL_EXPORT Rig : public Node
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Rig)
    
  public:
    // static const std::vector<std::string>& requiredBonyLandmarks() const noexcept;
    // static const RotationSequence predefinedJointRotationSequence(const std::string& /* jointLabel */) const noexcept;
    
    ~Rig() noexcept;
    
    Rig(const Rig& ) = delete;
    Rig(Rig&& ) noexcept = delete;
    Rig& operator=(const Rig& ) = delete;
    Rig& operator=(Rig&& ) noexcept = delete;
    
    // LandmarksTranslator* predefinedBonyLandmarksTranslator() const noexcept;
    
    virtual bool setup(Model* model) const noexcept = 0;
    virtual bool calibrate(Model* model, Node* trials) noexcept = 0;
    virtual bool generate(Model* model, Node* trials) const noexcept = 0;
    virtual void finalize(Node* trials) const noexcept = 0;
    
    virtual LandmarksTranslator* defaultLandmarksTranslator() = 0;
    
  protected:
    Rig(const std::string& name, int region, int side, Node* parent);
    Rig(RigPrivate& pimpl, Node* parent) noexcept;
        
    // virtual std::vector<std::string> requiredLandmarks() const noexcept = 0;
    
    // std::vector<std::string> mapLandmarks(const Trial* trial) const noexcept;
  };
};

#endif // __btkRig_h