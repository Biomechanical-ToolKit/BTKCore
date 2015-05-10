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

namespace btk
{
  class LandmarksTranslator;
  
  class PluginGaitPrivate;
  
  class BTK_MODEL_EXPORT PluginGait : public Rig
  {
    BTK_DECLARE_PIMPL_ACCESSOR(PluginGait)
    
  public:
    PluginGait(int region, int side, Node* parent = nullptr);
  
    virtual bool setup(btk::Model* model) const noexcept;
    virtual bool calibrate(btk::Model* model, btk::Node* trials) noexcept;
    virtual bool generate(btk::Model* model, btk::Node* trials) const noexcept;
    virtual void finalize(btk::Node* trials) const noexcept;
    
    double markerDiameter() const noexcept;
    void setMarkerDiameter(double value) noexcept;
    double headOffset() const noexcept;
    void setHeadOffset(double value) noexcept;
    double rightShoulderOffset() const noexcept;
    void setRightShoulderOffset(double value) noexcept;
    double leftShoulderOffset() const noexcept;
    void setLeftShoulderOffset(double value) noexcept;
    double rightElbowWidth() const noexcept;
    void setRightElbowWidth(double value) noexcept;
    double leftElbowWidth() const noexcept;
    void setLeftElbowWidth(double value) noexcept;
    double rightWristWidth() const noexcept;
    void setRightWristWidth(double value) noexcept;
    double leftWristWidth() const noexcept;
    void setLeftWristWidth(double value) noexcept;
    double rightHandThickness() const noexcept;
    void setRightHandThickness(double value) noexcept;
    double leftHandThickness() const noexcept;
    void setLeftHandThickness(double value) noexcept;
    double interAsisDistance() const noexcept;
    void setInterAsisDistance(double value) noexcept;
    double rightLegLength() const noexcept;
    void setRightLegLength(double value) noexcept;
    double leftLegLength() const noexcept;
    void setLeftLegLength(double value) noexcept;
    double rightAsisTrochanterAPDistance() const noexcept;
    void setRightAsisTrochanterAPDistance(double value) noexcept;
    double leftAsisTrochanterAPDistance() const noexcept;
    void setLeftAsisTrochanterAPDistance(double value) noexcept;
    double rightKneeWidth() const noexcept;
    void setRightKneeWidth(double value) noexcept;
    double leftKneeWidth() const noexcept;
    void setLeftKneeWidth(double value) noexcept;
    double rightAnkleWidth() const noexcept;
    void setRightAnkleWidth(double value) noexcept;
    double leftAnkleWidth() const noexcept;
    void setLeftAnkleWidth(double value) noexcept;
    bool rightFootFlat() const noexcept;
    void setRightFootFlat(bool value) noexcept;
    bool leftFootFlat() const noexcept;
    void setLeftFootFlat(bool value) noexcept;
    double rightStaticPlantarFlexionOffset() const noexcept;
    double rightStaticRotationOffset() const noexcept;
    double leftStaticPlantarFlexionOffset() const noexcept;
    double leftStaticRotationOffset() const noexcept;
    
    virtual LandmarksTranslator* defaultLandmarksTranslator();
  };
};

#endif // __btkPluginGait_h