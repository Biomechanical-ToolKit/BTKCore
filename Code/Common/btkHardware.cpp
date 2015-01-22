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

#include "btkHardware.h"
#include "btkHardware_p.h"
#include "btkTimeSequence.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  HardwarePrivate::HardwarePrivate(Hardware* pint, const std::string& name)
  : NodePrivate(pint,name), MeasurandsTimestamp(0ul)
  {};
  
  HardwarePrivate::~HardwarePrivate() noexcept = default;
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class Hardware btkHardware.h
   * @brief
   *
   *
   *
   * @ingroup BTKCommon
   */
  
  /**
   * Constructor.
   */
  Hardware::Hardware(const std::string& name, Node* parent)
  : Node(*new HardwarePrivate(this,name),parent)
  {};
  
  /**
   * Destructor (default)
   */
  Hardware::~Hardware() noexcept = default;
  
  /**
   * Convenient method to extract time sequences associated with the signals recorded by the hardware
   *
   * Internaly, this method find the TimeSequence objects which are in the child "Channels".
   */
  std::list<TimeSequence*> Hardware::channels() const noexcept
  {
    Node* channelsNode = this->findChild("Channels",{},false);
    if (channelsNode == nullptr)
      return std::list<TimeSequence*>();
    return channelsNode->findChildren<TimeSequence*>({},{},false);
  };
  
  /**
   * Convenient method to extract signals corresponding to the hardware measurements.
   * Compared to the channels() method, this one will process channels (e.g. calibration) and add the results into the child "Measurands". Thus the results is returned.
   * The processing is provided by each inheriting class. The processing associated with force platforms will be different than the one for inertial units or pressure insoles.
   * Internaly the measurands are only computed when the hardware configuration is modified. So calling this method several times without modification of the hardware will correspond only to extract the time sequences from the child "Measurands".
   */
  std::list<TimeSequence*> Hardware::measurands() noexcept
  {
    auto optr = this->pimpl();
    const std::string mnn("Measurands");
    Node* measurandsNode = this->findChild(mnn,{},false);
    // Cache
    if ((measurandsNode != nullptr) && (this->timestamp() <= optr->MeasurandsTimestamp))
      return measurandsNode->findChildren<TimeSequence*>({},{},false);
    // No channels?
    std::list<TimeSequence*> measurands;
    Node* channelsNode = this->findChild("Channels",{},false);
    if ((channelsNode == nullptr) || (channelsNode->findChildren<TimeSequence*>({},{},false).empty()))
      return measurands;
    // Generate data
    if (measurandsNode == nullptr)
      measurandsNode = new Node(mnn,this);
    measurands = measurandsNode->findChildren<TimeSequence*>({},{},false);
    this->generateMeasurands(measurands);
    this->modified();
    optr->MeasurandsTimestamp = this->timestamp();
    return measurands;
  };
};
