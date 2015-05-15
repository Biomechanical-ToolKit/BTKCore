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

#ifndef __btC3DHandler_p_h
#define __btC3DHandler_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkIOHandler_p.h"
#include "btkC3DHandler.h"
#include "btkMacros.h"

#include <type_traits>
#include <vector>

namespace btk
{
  class Trial;
  
  class C3DHandlerPrivate : public IOHandlerPrivate
  {
    BTK_DECLARE_IOHANDLER_OPTIONS(
      make_option<C3DHandler::EncodingFormat>::single_choice<C3DHandler::Encoding::Binary>(),
      make_option<C3DHandler::ByteOrderFormat>::multiple_choices<C3DHandler::ByteOrder::VAXLittleEndian,C3DHandler::ByteOrder::IEEELittleEndian,C3DHandler::ByteOrder::IEEEBigEndian>::init(C3DHandler::ByteOrder::Native),
      make_option<C3DHandler::DataStorageFormat>::multiple_choices<C3DHandler::DataStorage::Integer,C3DHandler::DataStorage::Float>::init(C3DHandler::DataStorage::Float),
      make_option<C3DHandler::AnalogStorageFormat>::multiple_choices<C3DHandler::AnalogStorage::Signed,C3DHandler::AnalogStorage::Unsigned>::init(C3DHandler::AnalogStorage::Signed)
    )
    
  public:
    C3DHandlerPrivate();
    ~C3DHandlerPrivate() noexcept;

    C3DHandlerPrivate(const C3DHandlerPrivate& ) = delete;
    C3DHandlerPrivate(C3DHandlerPrivate&& ) noexcept = delete;
    C3DHandlerPrivate& operator=(const C3DHandlerPrivate& ) = delete;
    C3DHandlerPrivate& operator=(const C3DHandlerPrivate&& ) noexcept = delete;
    
    double PointScale;
    int PointMaximumInterpolationGap;
    int AnalogResolution;
    std::vector<double> AnalogChannelScale;
    std::vector<double> AnalogZeroOffset;
    double AnalogUniversalScale;
    bool AnalogSignedIntegerFormat;
    
    template <typename T>
    static inline typename std::enable_if<!std::is_same<T,std::string>::value,T>::type generateDefaultValue(T&& blank, int idx)
    {
      _BTK_UNUSED(idx);
      return blank;
    };
  
    template <typename T>
    static inline typename std::enable_if<std::is_same<T,std::string>::value,T>::type generateDefaultValue(T&& blank, int idx)
    {
      return (blank.empty() ? blank : blank + std::to_string(idx));
    };
    
    template <typename T>
    static void mergeProperties(std::vector<T>* target, Trial* trial, const std::string& base, int finalSize = -1, T&& defaultValue = T());
  };
  
  // ----------------------------------------------------------------------- //
  
  class BinaryStream;
  
  class C3DDataStream
  {
  public:
    C3DDataStream(BinaryStream* raw);
    virtual ~C3DDataStream();
    
    virtual void readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor) = 0;
    virtual double readAnalog() = 0;
    virtual void writePoint(double x, double y, double z, double residual, double pointScaleFactor) = 0;
    virtual void writeAnalog(double v) = 0;
    
    C3DDataStream(const C3DDataStream& ) = delete;
    C3DDataStream(C3DDataStream&& ) noexcept = delete;
    C3DDataStream& operator=(const C3DDataStream& ) = delete;
    C3DDataStream& operator=(const C3DDataStream&& ) noexcept = delete;
    
  protected:
    BinaryStream* Raw;
  };
  
  // Integer format + signed analog data
  class C3DDataStream_SignedInteger : public C3DDataStream
  {
  public:
    C3DDataStream_SignedInteger(BinaryStream* raw);
    ~C3DDataStream_SignedInteger();
    
    virtual void readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor) final;
    virtual double readAnalog() override;
    virtual void writePoint(double x, double y, double z, double residual, double pointScaleFactor) final;
    virtual void writeAnalog(double v) override;
  };
  
  // Integer format + unsigned analog data
  class C3DDataStream_UnsignedInteger : public C3DDataStream_SignedInteger
  {
  public:
    C3DDataStream_UnsignedInteger(BinaryStream* raw);
    ~C3DDataStream_UnsignedInteger();
    
    virtual double readAnalog() final;
    virtual void writeAnalog(double v) final;
  };
  
  // Float format + signed/unsigned analog data
  class C3DDataStream_Float : public C3DDataStream
  {
  public:
    C3DDataStream_Float(BinaryStream* raw);
    ~C3DDataStream_Float();
    
    virtual void readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor) final;
    virtual double readAnalog() final;
    virtual void writePoint(double x, double y, double z, double residual, double pointScaleFactor) final;
    virtual void writeAnalog(double v) final;
  };
};

#endif // __btC3DDataStream_p_h
