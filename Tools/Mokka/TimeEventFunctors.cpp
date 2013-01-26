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

#include "TimeEventFunctors.h"
#include "TimeEventControlerWidget.h"

#include <vtkObjectFactory.h>

// ----------------------- CurrentFrameFunctor ----------------------- //

int CurrentFrameFunctor::operator()()
{
  if (!this->mp_Object)
    return 0;
  return this->mp_Object->currentFrame();
};

CurrentFrameFunctor::CurrentFrameFunctor(TimeEventControlerWidget* w)
: btk::VTKCurrentFrameFunctor()
{
  this->mp_Object = w;
};

// ----------------------- RegionOfInterestFunctor ----------------------- //

void RegionOfInterestFunctor::operator()(int& left, int& right)
{
  if (!this->mp_Object)
  {
    left = 0;
    right = 0;
  }
  else
  {
    left = this->mp_Object->leftBound(); 
    right = this->mp_Object->rightBound();
  }
};

RegionOfInterestFunctor::RegionOfInterestFunctor(TimeEventControlerWidget* w)
: btk::VTKRegionOfInterestFunctor()
{
  this->mp_Object = w;
};

// ----------------------- EventsFunctor ----------------------- //

bool EventsFunctor::operator()(int index, int& typeId, float& x, double rgb[3])
{
  typeId = -1;
  x = 0.0f;
  rgb[0] = 0.0; rgb[1] = 0.0; rgb[2] = 0.0;
  if (!this->mp_Object)
    return false;
  int frame = -1;
  int side = -1;
  bool res = this->mp_Object->eventItemData(index, typeId, frame, side, rgb);
  if (this->mp_FrameMapper.get() != NULL)
    x = (*this->mp_FrameMapper)(frame, side, this->mp_Object->acquisition()->firstFrame());
  else
    x = static_cast<float>(frame);
  return res;
};

EventsFunctor::EventsFunctor(TimeEventControlerWidget* w)
{
  this->mp_Object = w;
};
