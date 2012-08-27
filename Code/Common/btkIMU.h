/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barr�
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

#ifndef __btkIMU_h
#define __btkIMU_h

#include "btkConfigure.h"

#include <btkDataObject.h>
#include <btkAnalogCollection.h>

#include <map>

namespace btk
{
  class IMU : public DataObjectLabeled
  {
  public:
    typedef SharedPtr<IMU> Pointer;
    typedef SharedPtr<const IMU> ConstPointer;

    static Pointer New(const std::string& label = "IMU", const std::string& desc = "") {return Pointer(new IMU(1, label, desc));};
    
    BTK_COMMON_EXPORT void SetChannels(Analog::Pointer accX, Analog::Pointer accY, Analog::Pointer accZ, 
                                       Analog::Pointer gyroX, Analog::Pointer gyroY, Analog::Pointer gyroZ);
    BTK_COMMON_EXPORT AnalogCollection::Pointer GetChannels();
    BTK_COMMON_EXPORT void SetChannel(int idx, Analog::Pointer channel);
    BTK_COMMON_EXPORT Analog::Pointer GetChannel(int idx);
    BTK_COMMON_EXPORT Analog::ConstPointer GetChannel(int idx) const;
    BTK_COMMON_EXPORT Analog::Pointer GetChannel(const std::string label);
    BTK_COMMON_EXPORT Analog::ConstPointer GetChannel(const std::string label) const;
    
    int GetFrameNumber() const {return this->m_FrameNumber;};
    BTK_COMMON_EXPORT void SetFrameNumber(int fn);
    
    double GetFrequency() const {return this->m_Frequency;};
    BTK_COMMON_EXPORT void SetFrequency(double f);
    
    Analog::Pointer GetAccelerometerX() {return this->GetChannelMap(0);};
    Analog::Pointer GetAccelerometerY() {return this->GetChannelMap(1);};
    Analog::Pointer GetAccelerometerZ() {return this->GetChannelMap(2);};
    Analog::Pointer GetGyroscopeX() {return this->GetChannelMap(3);};
    Analog::Pointer GetGyroscopeY() {return this->GetChannelMap(4);};
    Analog::Pointer GetGyroscopeZ() {return this->GetChannelMap(5);};
    
    BTK_COMMON_EXPORT void Rotate(const Eigen::Matrix<double,3,3>& R);
    
    Pointer Clone() const {return Pointer(new IMU(*this));};

  protected:
    BTK_COMMON_EXPORT IMU(int type, const std::string& label, const std::string& desc);
    BTK_COMMON_EXPORT IMU(const IMU& toCopy);

  private:
    typedef std::map<int,Analog::Pointer>::iterator MapIterator;
    typedef std::map<int,Analog::Pointer>::const_iterator MapConstIterator;
    
    Analog::Pointer GetChannelMap(int idx)
    {
      MapIterator it = this->m_Channels.find(idx);
      if (it != this->m_Channels.end())
        return it->second;
      return Analog::Pointer();
    };
    
    class Data
    {
    public:
      virtual ~Data() {};
      virtual double GetValue(int i) = 0;
      virtual void SetValue(int i, double v) = 0;
    };
    class EmptyData : public Data
    {
    public:
      virtual double GetValue(int ) {return 0.0;};
      virtual void SetValue(int , double ) {};
    };
    class ChannelData : public Data
    {
    public:
      ChannelData(Analog::Pointer ptr) : mp_Channel(ptr) {};
      virtual double GetValue(int i) {return this->mp_Channel->GetValues().coeff(i);};
      virtual void SetValue(int i, double v) {this->mp_Channel->GetValues().coeffRef(i) = v;};
    private:
      Analog::Pointer mp_Channel;
    };
  
    IMU& operator=(const IMU& ); // Not implemented.
  
    int m_Type;
    int m_FrameNumber;
    double m_Frequency;
    std::map<int,Analog::Pointer> m_Channels;
  };
  
  // TODO: Create a class IMMU inheriting from IMU
  // Type I: IMU
  // Type II: IMMU
  // Analog::Pointer GetMagnetometerX() {return this->GetChannel(6);};
  // Analog::Pointer GetMagnetometerY() {return this->GetChannel(7);};
  // Analog::Pointer GetMagnetometerZ() {return this->GetChannel(8);};
};
#endif