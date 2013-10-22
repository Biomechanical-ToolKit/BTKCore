/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#include "btkGroundReactionWrenchFilter.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class GroundReactionWrenchFilter btkGroundReactionWrenchFilter.h
   * @brief Transform force platform data into ground reaction wrenches.
   *
   * Theses wrenches are expressed in the global frame. The point of application (PWA)
   * of each wrench is calculated from Shimba (1984).
   * Compared to the center of pressure (COP), the PWA take into account all the forces
   * and moments (and does not assume any null forces).
   *
   * Note: For gait analysis, the COP and PWA are very similar.
   *
   * Because the computation of the PWA (as the COP) is sensitive to small (vertical) forces, the methods 
   * SetThresholdValue and SetThresholdState() are provided to not compute the PWA if the vertical forces is
   * the threshold. You first need to set the value and then activate the threshold method.
   * By default, this threshold is not activated and the values is set to 0.
   * As for example:
   * @code
   * btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New()
   * grwf->SetThresholdValue(5.0); // 5 newtons
   * grwf->SetThresholdState(true);
   * @endcode
   *
   * Finally, you can also use the method SetTransformToGlobalFrame() to have the wrench expressed in the frame of the force platform.
   *
   * @par Reference
   * Shimba T.@n
   * <em>An estimation of center of gravity from force platform data</em>.@n
   * Journal of Biomechanics, <b>1984</b>, 17(1), 53–60.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef GroundReactionWrenchFilter::Pointer
   * Smart pointer associated with a GroundReactionWrenchFilter object.
   */
  
  /**
   * @typedef GroundReactionWrenchFilter::ConstPointer
   * Smart pointer associated with a const GroundReactionWrenchFilter object.
   */
    
  /**
   * @fn static Pointer GroundReactionWrenchFilter::New();
   * Creates a smart pointer associated with a GroundReactionWrenchFilter object.
   */

  /**
   * @fn bool GroundReactionWrenchFilter::GetThresholdState() const
   * Returns the state of the threshold used to suppress false PWA.
   */

  /**
   * Sets the threshold state.
   */
  void GroundReactionWrenchFilter::SetThresholdState(bool activated)
  {
    if (this->m_ThresholdActivated == activated)
      return;
    this->m_ThresholdActivated = activated;
    this->Modified();
  };

  /**
   * @fn double GroundReactionWrenchFilter::GetThresholdValue() const
   * Returns the value used to suppress PWA computed with a Fz value lower or equal than it.
   * 
   * The threshold must be activated (see GroundReactionWrenchFilter::SetThresholdState) to be used during the computation of the PWA.
   */
  
  /**
   * Sets the threshold value.
   *
   * The threshold must be activated (see GroundReactionWrenchFilter::SetThresholdState) to be used during the computation of the PWA.
   */
  void GroundReactionWrenchFilter::SetThresholdValue(double v)
  {
    if (fabs(this->m_ThresholdValue - v) <= std::numeric_limits<double>::epsilon())
      return;
    if (v < 0.0)
      btkWarningMacro("Negative threshold has no effect on the algorithm because it compares the threshold value with the absolute value of Fz.");
    this->m_ThresholdValue = v;
    this->Modified();
  };

  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  GroundReactionWrenchFilter::GroundReactionWrenchFilter()
  : ForcePlatformWrenchFilter()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    this->m_ThresholdActivated = false;
    this->m_ThresholdValue = 0.0;
  };
  
  /**
   * Finish the computation of the ground reaction wrench for the force platform type I (nothing to do).
   */
  void GroundReactionWrenchFilter::FinishTypeI(Wrench::Pointer /* wrh */, ForcePlatform::Pointer /* fp */, int /* index */)
  {
    /*
    ForcePlatform::Origin origin;
    origin << 0, 0, fp->GetOrigin().z();
    if (origin.z()  > 0)
    {
      btkWarningMacro("Vertical offset between the origin of the force platform #" + ToString(index + " and the center of the working surface seems to be misconfigured (positive value). The opposite of this offset is used.");
      origin.z() *= -1;
    }
    */
    //this->FinishGRWComputation(grw, origin);
  };
  
  /**
   * Finish the computation of the ground reaction wrench for the AMTI force platforms.
   */
  void GroundReactionWrenchFilter::FinishAMTI(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index)
  {
    ForcePlatform::Origin origin = fp->GetOrigin();
    if (origin.z() > 0)
    {
      btkWarningMacro("Origin for the force platform #" + ToString(index) + " seems to be located from the center of the working surface instead of the inverse. Data are inverted to locate the center of the working surface from the platform's origin.");
      origin *= -1;
    }
    this->FinishGRWComputation(wrh, origin);

  };
   
  /**
   * Finish the computation of the ground reaction wrench for the Kislter force platform.
   */
  void GroundReactionWrenchFilter::FinishKistler(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index)
  {
    ForcePlatform::Origin origin;
    origin << 0, 0, fp->GetOrigin().z();
    if (origin.z()  > 0)
    {
      btkWarningMacro("Vertical offset between the origin of the force platform #" + ToString(index) + " and the center of the working surface seems to be misconfigured (positive value). The opposite of this offset is used.");
      origin.z() *= -1;
    }
    this->FinishGRWComputation(wrh, origin);
  };
};

