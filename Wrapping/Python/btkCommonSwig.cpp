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
 
#include "btkCommonSwig.h"

// ------------------------------------------------------------------------- //
//                                  Event                                    //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(Event);
BTK_SWIG_DEFINE_CLONE(Event);

btkEvent::btkEvent(const std::string& label, double t, const std::string& context, int detectionFlags, const std::string& subject, const std::string& desc, int id)
: btkEvent_shared(btk::Event::New(label, t, context, detectionFlags, subject, desc, id))
{};

btkEvent::btkEvent(const std::string& label, int f, const std::string& context, int detectionFlags, const std::string& subject, const std::string& desc, int id)
: btkEvent_shared(btk::Event::New(label, f, context, detectionFlags, subject, desc, id))
{};

btkEvent::btkEvent(const std::string& label, double t, int f, const std::string& context, int detectionFlags, const std::string& subject, const std::string& desc, int id)
: btkEvent_shared(btk::Event::New(label, t, f, context, detectionFlags, subject, desc, id))
{};

// ------------------------------------------------------------------------- //
//                                  Analog                                   //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(Analog);

btkAnalog::btkAnalog(const std::string& label, const std::string& desc)
: btkAnalog_shared(btk::Analog::New(label, desc))
{};

btkAnalog::btkAnalog(int frameNumber)
: btkAnalog_shared(btk::Analog::New(frameNumber))
{};

btkAnalog::btkAnalog(const std::string& label, int frameNumber)
: btkAnalog_shared(btk::Analog::New(label, frameNumber))
{};

// ------------------------------------------------------------------------- //
//                                 Analog::Data                              //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(AnalogData);

btkAnalogData::btkAnalogData(int frameNumber)
: btkAnalogData_shared(btk::Analog::Data::New(frameNumber))
{};

// ------------------------------------------------------------------------- //
//                                  Point                                    //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(Point);

// shadowed by the last constructor...
// btkPoint::btkPoint(const std::string& label, btkPoint_impl::Type t, const std::string& desc)
// : btkPoint_shared(btk::Point::New(label, t, desc))
// {};

btkPoint::btkPoint(int frameNumber)
: btkPoint_shared(btk::Point::New(frameNumber))
{};

btkPoint::btkPoint(const std::string& label, int frameNumber, btkPoint_impl::Type t, const std::string& desc)
: btkPoint_shared(btk::Point::New(label, frameNumber, t, desc))
{};

// ------------------------------------------------------------------------- //
//                                 Point::Data                               //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(PointData);

btkPointData::btkPointData(int frameNumber)
: btkPointData_shared(btk::Point::Data::New(frameNumber))
{};

// ------------------------------------------------------------------------- //
//                                 ForcePlatform                             //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(ForcePlatform);

btkForcePlatform btkForcePlatformType1()
{
  return btkForcePlatform_shared(btk::ForcePlatformType1::New());
};

btkForcePlatform btkForcePlatformType2()
{
  return btkForcePlatform_shared(btk::ForcePlatformType2::New());
};

btkForcePlatform btkForcePlatformType3()
{
  return btkForcePlatform_shared(btk::ForcePlatformType3::New());
};

btkForcePlatform btkForcePlatformType4()
{
  return btkForcePlatform_shared(btk::ForcePlatformType4::New());
};

btkForcePlatform btkForcePlatformType5()
{
  return btkForcePlatform_shared(btk::ForcePlatformType5::New());
};

btkForcePlatform btkForcePlatformType6()
{
  return btkForcePlatform_shared(btk::ForcePlatformType6::New());
};

// ------------------------------------------------------------------------- //
//                                    Wrench                                 //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(Wrench);

btkWrench::btkWrench(const std::string& label)
: btkWrench_shared(btk::Wrench::New(label))
{};

btkWrench::btkWrench(int frameNumber)
: btkWrench_shared(btk::Wrench::New(frameNumber))
{};

btkWrench::btkWrench(const std::string& label, int frameNumber)
: btkWrench_shared(btk::Wrench::New(label, frameNumber))
{};

// ------------------------------------------------------------------------- //
//                                    IMU                                    //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(IMU);

btkIMU::btkIMU(const std::string& label, const std::string& desc)
: btkIMU_shared(btk::IMU::New(label, desc))
{};


// ------------------------------------------------------------------------- //
//                                Collection                                 //
// ------------------------------------------------------------------------- //
// - EventCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR(EventCollection);
BTK_SWIG_DEFINE_CLONE(EventCollection);
// - AnalogCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR(AnalogCollection);
BTK_SWIG_DEFINE_CLONE(AnalogCollection);
// - PointCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR(PointCollection);
BTK_SWIG_DEFINE_CLONE(PointCollection);
// - ForcePlatformCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR(ForcePlatformCollection);
BTK_SWIG_DEFINE_CLONE(ForcePlatformCollection);
// - WrenchCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR(WrenchCollection);
BTK_SWIG_DEFINE_CLONE(WrenchCollection);
// - IMUCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR(IMUCollection);
BTK_SWIG_DEFINE_CLONE(IMUCollection);

// ------------------------------------------------------------------------- //
//                               MetaDataInfo                                //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(MetaDataInfo);


btkMetaDataInfo::btkMetaDataInfo(int val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(static_cast<int16_t>(val)))
{};

btkMetaDataInfo::btkMetaDataInfo(double val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(static_cast<float>(val)))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::string& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(val))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::vector<int>& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(btkSwigConvert<int16_t>(val)))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::vector<double>& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(btkSwigConvert<float>(val)))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::vector<std::string>& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(val))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::vector<int>& dim, const std::vector<int>& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(btkSwigConvert<uint8_t>(dim), btkSwigConvert<int16_t>(val)))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::vector<int>& dim, const std::vector<double>& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(btkSwigConvert<uint8_t>(dim), btkSwigConvert<float>(val)))
{};

btkMetaDataInfo::btkMetaDataInfo(const std::vector<int>& dim, const std::vector<std::string>& val)
: btkMetaDataInfo_shared(btk::MetaDataInfo::New(btkSwigConvert<uint8_t>(dim), val))
{};

// ------------------------------------------------------------------------- //
//                                 MetaData                                  //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_CLONE(MetaData);

btkMetaData::btkMetaData(const std::string& label) // . const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, "", true))
{};

btkMetaData::btkMetaData(const std::string& label, int val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, static_cast<int16_t>(val), desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, double val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, static_cast<float>(val), desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::string& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, val, desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::vector<int>& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, btkSwigConvert<int16_t>(val), desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::vector<double>& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, btkSwigConvert<float>(val), desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::vector<std::string>& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, val, desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::vector<int>& dim, const std::vector<int>& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, btkSwigConvert<uint8_t>(dim), btkSwigConvert<int16_t>(val), desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::vector<int>& dim, const std::vector<double>& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, btkSwigConvert<uint8_t>(dim), btkSwigConvert<float>(val), desc, isUnlocked))
{};

btkMetaData::btkMetaData(const std::string& label, const std::vector<int>& dim, const std::vector<std::string>& val, const std::string& desc, bool isUnlocked)
: btkMetaData_shared(btk::MetaData::New(label, btkSwigConvert<uint8_t>(dim), val, desc, isUnlocked))
{};

// ------------------------------------------------------------------------- //
//                               Acquisition                                 //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(Acquisition);
BTK_SWIG_DEFINE_CLONE(Acquisition);

// ------------------------------------------------------------------------- //
//                               ProcessObject                               //
// ------------------------------------------------------------------------- //
/* Nothing to do */