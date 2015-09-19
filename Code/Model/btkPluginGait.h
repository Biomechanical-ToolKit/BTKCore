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

#ifndef __btkPluginGait_h
#define __btkPluginGait_h

#include "btkModelExport.h"
#include "btkRig.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

namespace btk
{
  class LandmarksTranslator;
  
  class PluginGaitPrivate;
  
  class BTK_NODE(BTK_MODEL_EXPORT, PluginGait) : public Rig
  {
    BTK_DECLARE_PIMPL_ACCESSOR(PluginGait)
    BTK_DECLARE_NODEID(PluginGait, Rig)
    
  public:
    PluginGait(int region, int side, Node* parent = nullptr);
  
    virtual bool setup(btk::Model* model) const _BTK_NOEXCEPT;
    virtual bool calibrate(btk::Model* model, btk::Node* trials) _BTK_NOEXCEPT;
    virtual bool generate(btk::Model* model, btk::Node* trials) const _BTK_NOEXCEPT;
    virtual void finalize(btk::Node* trials) const _BTK_NOEXCEPT;
    
    double markerDiameter() const _BTK_NOEXCEPT;
    void setMarkerDiameter(double value) _BTK_NOEXCEPT;
    double headOffset() const _BTK_NOEXCEPT;
    void setHeadOffset(double value) _BTK_NOEXCEPT;
    double rightShoulderOffset() const _BTK_NOEXCEPT;
    void setRightShoulderOffset(double value) _BTK_NOEXCEPT;
    double leftShoulderOffset() const _BTK_NOEXCEPT;
    void setLeftShoulderOffset(double value) _BTK_NOEXCEPT;
    double rightElbowWidth() const _BTK_NOEXCEPT;
    void setRightElbowWidth(double value) _BTK_NOEXCEPT;
    double leftElbowWidth() const _BTK_NOEXCEPT;
    void setLeftElbowWidth(double value) _BTK_NOEXCEPT;
    double rightWristWidth() const _BTK_NOEXCEPT;
    void setRightWristWidth(double value) _BTK_NOEXCEPT;
    double leftWristWidth() const _BTK_NOEXCEPT;
    void setLeftWristWidth(double value) _BTK_NOEXCEPT;
    double rightHandThickness() const _BTK_NOEXCEPT;
    void setRightHandThickness(double value) _BTK_NOEXCEPT;
    double leftHandThickness() const _BTK_NOEXCEPT;
    void setLeftHandThickness(double value) _BTK_NOEXCEPT;
    double interAsisDistance() const _BTK_NOEXCEPT;
    void setInterAsisDistance(double value) _BTK_NOEXCEPT;
    double rightLegLength() const _BTK_NOEXCEPT;
    void setRightLegLength(double value) _BTK_NOEXCEPT;
    double leftLegLength() const _BTK_NOEXCEPT;
    void setLeftLegLength(double value) _BTK_NOEXCEPT;
    double rightAsisTrochanterAPDistance() const _BTK_NOEXCEPT;
    void setRightAsisTrochanterAPDistance(double value) _BTK_NOEXCEPT;
    double leftAsisTrochanterAPDistance() const _BTK_NOEXCEPT;
    void setLeftAsisTrochanterAPDistance(double value) _BTK_NOEXCEPT;
    double rightKneeWidth() const _BTK_NOEXCEPT;
    void setRightKneeWidth(double value) _BTK_NOEXCEPT;
    double leftKneeWidth() const _BTK_NOEXCEPT;
    void setLeftKneeWidth(double value) _BTK_NOEXCEPT;
    double rightAnkleWidth() const _BTK_NOEXCEPT;
    void setRightAnkleWidth(double value) _BTK_NOEXCEPT;
    double leftAnkleWidth() const _BTK_NOEXCEPT;
    void setLeftAnkleWidth(double value) _BTK_NOEXCEPT;
    bool rightFootFlat() const _BTK_NOEXCEPT;
    void setRightFootFlat(bool value) _BTK_NOEXCEPT;
    bool leftFootFlat() const _BTK_NOEXCEPT;
    void setLeftFootFlat(bool value) _BTK_NOEXCEPT;
    double rightStaticPlantarFlexionOffset() const _BTK_NOEXCEPT;
    double rightStaticRotationOffset() const _BTK_NOEXCEPT;
    double leftStaticPlantarFlexionOffset() const _BTK_NOEXCEPT;
    double leftStaticRotationOffset() const _BTK_NOEXCEPT;
    
    virtual LandmarksTranslator* defaultLandmarksTranslator();
  };
};

#endif // __btkPluginGait_h