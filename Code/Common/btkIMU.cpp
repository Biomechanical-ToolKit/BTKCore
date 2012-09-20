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
   * @class IMU btkIMU.h
   * @brief Container of analog channels representing an inertial measurement unit (IMU).
   *
   * This class proposes to store any number of analog channels but the 6 first channels could be 
   * used for 3 accelerometers and 3 gyroscopes as some convenient methods are proposed in this way
   * (see SetChannels(), GetAccelerometerX(), GetAccelerometerY(), GetAccelerometerZ(), GetGyroscopeX(), GetGyroscopeY(), GetGyroscopeZ()).
   * 
   * The storage of the channel is done using an ID and not an array index. You can then use the same ID for some kind of sensor even if other are missing (i.e. IMU with accelerometers disabled or configured with 1 acc, and 2 gyros., etc.).
   *
   * Note: This class is still experimental and could be modified in the next release.
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef IMU::Pointer
   * Smart pointer associated with a IMU object.
   */
  
  /**
   * @typedef IMU::ConstPointer
   * Smart pointer associated with a const IMU object.
   */
    
  /**
   * @fn static Pointer IMU::New(const std::string& label = "IMU", const std::string& desc = "");
   * Creates a smart pointer associated with a IMU object.
   * @warning If you want to use the default created channels, you need to use the command SetFrameNumber as no memory is allocated for the data.
   */
  
  /**
   * Sets the 6 first channels of the IMUs with the analog channels @a accX, @a accY, @a accZ, @a gyroX, @a gyroY, @a gyroZ.
   *
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
  
  /**
   * Returns the analog channels of the IMU in a collection. The analog channels are pushed in the collection by using their ID.
   */
  AnalogCollection::Pointer IMU::GetChannels()
  {
    AnalogCollection::Pointer analogs = AnalogCollection::New();
    for (MapIterator it = this->m_Channels.begin() ; it != this->m_Channels.end() ; ++it)
      analogs->InsertItem(it->second);
    return analogs;
  }

  /**
   * Sets an analog channel to the given ID. If an analog channel is already set to this ID, then it is replaced.
   *
   * @warning: This function assumes that the number of frames corresponds to the number of frames set in the IMU. You can set afterward the number of frames by using the method SetFrameNumber().
   */
  void IMU::SetChannel(int id, Analog::Pointer channel)
  {
    if (channel == this->GetChannelMap(id))
      return;
    this->m_Channels[id] = channel;
    // Set as modified
    this->Modified();
  };
  
  /**
   * Returns the analog channel with the given ID. If there is no analog channel with the given ID, then an exception is thrown.
   */
  Analog::Pointer IMU::GetChannel(int id)
  {
    MapIterator it = this->m_Channels.find(id);
    if ( it == this->m_Channels.end())
      throw(OutOfRangeException("IMU::GetChannel(int)"));
    return it->second;
  };
  
  /**
   * Returns the analog channel with the given ID. If there is no analog channel with the given ID, then an exception is thrown.
   */
  Analog::ConstPointer IMU::GetChannel(int id) const
  {
    MapConstIterator it = this->m_Channels.find(id);
    if ( it == this->m_Channels.end())
      throw(OutOfRangeException("IMU::GetChannel(int) const"));
    return it->second;
  };
  
  /**
   * Returns the analog channel with the given @a label. If there is no analog channel with the given @a label, then an exception is thrown.
   */
  Analog::Pointer IMU::GetChannel(const std::string& label)
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
  
  /**
   * Returns the analog channel with the given @a label. If there is no analog channel with the given @a label, then an exception is thrown.
   */
  Analog::ConstPointer IMU::GetChannel(const std::string& label) const
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
  
  /**
   * @fn int IMU::GetFrameNumber() const
   * Returns the number of frames set in this IMU.
   */
   
  /**
   * Set the number of frames for the IMU and modify also the number of frames for the channels.
   */
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
  
  /**
   * @fn double IMU::GetFrequency() const
   * Returns the frequency set to the IMU. (by default it is 0 Hz).
   */
  
  /**
   * Sets the acquisition sample rate associated to the IMU.
   */
  void IMU::SetFrequency(double f)
  {
    if (this->m_Frequency == f)
      return;
    this->m_Frequency = f;
    this->Modified();
  };
  
  /**
   * @fn Analog::Pointer IMU::GetAccelerometerX()
   * Convenient method to return the analog channel with the ID 0 (which should correspond to an accelerometer measuring data on the X axis of the IMU).
   */
   
  /**
   * @fn Analog::Pointer IMU::GetAccelerometerY()
   * Convenient method to return the analog channel with the ID 1 (which should correspond to an accelerometer measuring data on the Y axis of the IMU).
   */
   
  /**
   * @fn Analog::Pointer IMU::GetAccelerometerZ()
   * Convenient method to return the analog channel with the ID 2 (which should correspond to an accelerometer measuring data on the Z axis of the IMU).
   */
   
  /**
   * @fn Analog::Pointer IMU::GetGyroscopeX()
   * Convenient method to return the analog channel with the ID 3 (which should correspond to a gyroscope measuring data on the X axis of the IMU).
   */
   
  /**
   * @fn Analog::Pointer IMU::GetGyroscopeY()
   * Convenient method to return the analog channel with the ID 4 (which should correspond to a gyroscope measuring data on the Y axis of the IMU).
   */
   
  /**
   * @fn Analog::Pointer IMU::GetGyroscopeZ()
   * Convenient method to return the analog channel with the ID 5 (which should correspond to a gyroscope measuring data on the Z axis of the IMU).
   */

  /**
   * @typedef IMU::Rotation
   * Type definition for Eigen 3x3 rotation matrix (of double).
   */

  /**
   * Convenient method to rotate the analog channels with the IDs 0-5. This method handles the case where some of these analog channels are missing. 
   */
  void IMU::Rotate(const Rotation& R)
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
  
  /**
   * @fn Pointer IMU::Clone() const
   * Deep copy of the current object.
   */
  
  /**
   * Constructor
   */
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
  
  /**
   * Constructor of copy
   */
  IMU::IMU(const IMU& toCopy)
  : DataObjectLabeled(toCopy), m_Channels()
  {
    this->m_Type = toCopy.m_Type;
    this->m_FrameNumber = toCopy.m_FrameNumber;
    for (MapConstIterator it = toCopy.m_Channels.begin() ; it != toCopy.m_Channels.end() ; ++it)
      this->m_Channels.insert(std::make_pair(it->first, it->second ? it->second->Clone() : Analog::Pointer()));
  };
}
