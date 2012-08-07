/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#ifndef __btkCommonSwig_h
#define __btkCommonSwig_h

#define BTK_SWIG_HEADER_DECLARATION

#include "btkSwigUtils.h"
#include "btkSwigMacros.h"

#include <btkEvent.h>
#include <btkAnalog.h>
#include <btkPoint.h>
#include <btkForcePlatform.h>
#include <btkForcePlatformTypes.h>
#include <btkWrench.h>
#include <btkIMU.h>
#include <btkEventCollection.h>
#include <btkAnalogCollection.h>
#include <btkPointCollection.h>
#include <btkForcePlatformCollection.h>
#include <btkWrenchCollection.h>
#include <btkIMUCollection.h>
#include <btkMetaDataInfo.h>
#include <btkMetaData.h>
#include <btkMetaDataUtils.h>
#include <btkAcquisition.h>
#include <btkProcessObject.h>

typedef btk::DataObject btkDataObject_impl;
typedef btk::ProcessObject btkProcessObject_impl;
#include "Common/btkCommonSwig_Event.h"
#include "Common/btkCommonSwig_Analog.h"
#include "Common/btkCommonSwig_Point.h"
#include "Common/btkCommonSwig_ForcePlatform.h"
#include "Common/btkCommonSwig_Wrench.h"
#include "Common/btkCommonSwig_IMU.h"
#include "Common/btkCommonSwig_Collection.h"
#include "Common/btkCommonSwig_MetaData.h"
#include "Common/btkCommonSwig_Acquisition.h"

#endif // __btkCommonSwig_h