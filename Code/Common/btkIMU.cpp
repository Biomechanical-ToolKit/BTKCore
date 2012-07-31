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
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO IMU SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "btkIMU.h"

namespace btk
{
  /**
   * @warning: This function assumes that the number of frames corresponds to the number of frames set in the IMU. You can set afterward the number of frames by using the method SetFrameNumber().
   */
  void IMU::SetChannels(Analog::Pointer accX, Analog::Pointer accY, Analog::Pointer accZ, 
                        Analog::Pointer gyroX, Analog::Pointer gyroY, Analog::Pointer gyroZ)
  {
    if ((this->GetChannelMap(0) == accX) && (this->GetChannelMap(1) == accY) && (this->GetChannelMap(2) == accZ)
       && (this->GetChannelMap(3) == gyroX) && (this->GetChannelMap(4) == gyroY) && (this->GetChannelMap(5) == gyroZ))
      return;
    if (accX) this->m_Channels[0] = accX;
    if (accY) this->m_Channels[1] = accY;
    if (accZ) this->m_Channels[2] = accZ;
    if (gyroX) this->m_Channels[3] = gyroX;
    if (gyroY) this->m_Channels[4] = gyroY;
    if (gyroZ) this->m_Channels[5] = gyroZ;
    // Set as modified
    this->Modified();
  };
  
  AnalogCollection::Pointer IMU::GetChannels()
  {
    AnalogCollection::Pointer analogs = AnalogCollection::New();
    for (MapIterator it = this->m_Channels.begin() ; it != this->m_Channels.end() ; ++it)
      analogs->InsertItem(it->second);
    return analogs;
  }

  /**
   * @warning: This function assumes that the number of frames corresponds to the number of frames set in the IMU. You can set afterward the number of frames by using the method SetFrameNumber().
   */
  void IMU::SetChannel(int idx, Analog::Pointer channel)
  {
    if (channel == this->GetChannelMap(idx))
      return;
    this->m_Channels[idx] = channel;
    // Set as modified
    this->Modified();
  };
  
  Analog::Pointer IMU::GetChannel(int idx)
  {
    MapIterator it = this->m_Channels.find(idx);
    if ( it == this->m_Channels.end())
      throw(OutOfRangeException("IMU::GetChannel(int)"));
    return it->second;
  };
  
  Analog::ConstPointer IMU::GetChannel(int idx) const
  {
    MapConstIterator it = this->m_Channels.find(idx);
    if ( it == this->m_Channels.end())
      throw(OutOfRangeException("IMU::GetChannel(int) const"));
    return it->second;
  };
  
  Analog::Pointer IMU::GetChannel(const std::string label)
  {
    MapIterator it = this->m_Channels.begin();
    while (it != this->m_Channels.end())
    {
      if (it->second->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    if (it == this->m_Channels.end())
      throw(OutOfRangeException("IMU::GetChannel(string)"));
    return it->second;
  };
  
  Analog::ConstPointer IMU::GetChannel(const std::string label) const
  {
    MapConstIterator it = this->m_Channels.begin();
    while (it != this->m_Channels.end())
    {
      if (it->second->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    if (it == this->m_Channels.end())
      throw(OutOfRangeException("IMU::GetChannel(string) const"));
    return it->second;
  };
  
  void IMU::SetFrameNumber(int fn)
  {
    if (this->m_FrameNumber == fn)
      return;
    for (MapIterator it = this->m_Channels.begin() ; it != this->m_Channels.end() ; ++it)
    {
      if (it->second)
        it->second->SetFrameNumber(fn);
    }
    this->m_FrameNumber = fn;
    this->Modified();
  };
  
  void IMU::SetFrequency(double f)
  {
    if (this->m_Frequency == f)
      return;
    this->m_Frequency = f;
    this->Modified();
  };

  void IMU::Rotate(const Eigen::Matrix<double,3,3>& R)
  {
    // Better way to do this?
    // Because the size of the data can be huge, it is preferred
    // to priorize the memory instead of the speed.
    Analog::Pointer accX, accY, accZ, gyroX, gyroY, gyroZ;
    typedef SharedPtr<IMU::Data> DataPtr;
    DataPtr aX = (accX = this->GetChannelMap(0)) ? DataPtr(new IMU::ChannelData(accX)) : DataPtr(new IMU::EmptyData);
    DataPtr aY = (accY = this->GetChannelMap(1)) ? DataPtr(new IMU::ChannelData(accY)) : DataPtr(new IMU::EmptyData);
    DataPtr aZ = (accZ = this->GetChannelMap(2)) ? DataPtr(new IMU::ChannelData(accZ)) : DataPtr(new IMU::EmptyData);
    DataPtr gX = (gyroX = this->GetChannelMap(3)) ? DataPtr(new IMU::ChannelData(gyroX)) : DataPtr(new IMU::EmptyData);
    DataPtr gY = (gyroY = this->GetChannelMap(4)) ? DataPtr(new IMU::ChannelData(gyroY)) : DataPtr(new IMU::EmptyData);
    DataPtr gZ = (gyroZ = this->GetChannelMap(5)) ? DataPtr(new IMU::ChannelData(gyroZ)): DataPtr(new IMU::EmptyData);
    for (int i = 0 ; i < this->m_FrameNumber ; ++i)
    {
      Eigen::Matrix<double,3,1> v;
      // Accelerometer
      v.coeffRef(0) = aX->GetValue(i);
      v.coeffRef(1) = aY->GetValue(i);
      v.coeffRef(2) = aZ->GetValue(i);
      v = R * v;
      aX->SetValue(i, v.coeff(0));
      aY->SetValue(i, v.coeff(1));
      aZ->SetValue(i, v.coeff(2));
      // Gyroscope
      v.coeffRef(0) = gX->GetValue(i);
      v.coeffRef(1) = gY->GetValue(i);
      v.coeffRef(2) = gZ->GetValue(i);
      v = R * v;
      gX->SetValue(i, v.coeff(0));
      gY->SetValue(i, v.coeff(1));
      gZ->SetValue(i, v.coeff(2));
    }
    this->Modified();
  };
  
  IMU::IMU(int type, const std::string& label, const std::string& desc)
  : DataObjectLabeled(label, desc), m_Channels()
  {
    this->m_Type = type;
    this->m_FrameNumber = 0;
    this->m_Frequency = 0.0;
    this->m_Channels.insert(std::make_pair(0,Analog::New("Acc X")));
    this->m_Channels.insert(std::make_pair(1,Analog::New("Acc Y")));
    this->m_Channels.insert(std::make_pair(2,Analog::New("Acc Z")));
    this->m_Channels.insert(std::make_pair(3,Analog::New("Gyro X")));
    this->m_Channels.insert(std::make_pair(4,Analog::New("Gyro Y")));
    this->m_Channels.insert(std::make_pair(5,Analog::New("Gyro Z")));
  };
  
  IMU::IMU(const IMU& toCopy)
  : DataObjectLabeled(toCopy), m_Channels()
  {
    this->m_Type = toCopy.m_Type;
    this->m_FrameNumber = toCopy.m_FrameNumber;
    for (MapConstIterator it = toCopy.m_Channels.begin() ; it != toCopy.m_Channels.end() ; ++it)
      this->m_Channels.insert(std::make_pair(it->first, it->second ? it->second->Clone() : Analog::Pointer()));
  };
}
