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

#ifndef __btkC3DFileIO_h
#define __btkC3DFileIO_h

#include "btkAcquisitionFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkException.h"

namespace btk
{
  class C3DFileIOException : public Exception
  {
  public:
    explicit C3DFileIOException(const std::string& msg)
    : Exception(msg)
    {};
      
    virtual ~C3DFileIOException() throw() {};
  };
  
  class C3DFileIO : public AcquisitionFileIO
  {
  public:
    
    typedef enum {Signed, Unsigned}  AnalogIntegerFormat;
    typedef enum {None = 1, ScalesFromDataUpdate = 2, ScalesFromMetaDataUpdate = 4, MetaDataFromDataUpdate = 8, CompatibleVicon = 16} WritingFlag;

    typedef SharedPtr<C3DFileIO> Pointer;
    typedef SharedPtr<const C3DFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new C3DFileIO());};
        
    // ~C3DFileIO(); // Implicit.
    
    double GetPointScale() const {return this->m_PointScale;};
    void SetPointScale(double s) {this->m_PointScale = s;};
    AnalogIntegerFormat GetAnalogIntegerFormat() const {return this->m_AnalogIntegerFormat;};
    void SetAnalogIntegerFormat(AnalogIntegerFormat f) {this->m_AnalogIntegerFormat = f;};
    std::vector<double>& GetAnalogChannelScale() {return this->m_AnalogChannelScale;};
    const std::vector<double>& GetAnalogChannelScale() const {return this->m_AnalogChannelScale;};
    void SetAnalogChannelScale(const std::vector<double>& s) {this->m_AnalogChannelScale = s;};
    std::vector<int>& GetAnalogZeroOffset() {return this->m_AnalogZeroOffset;};
    const std::vector<int>& GetAnalogZeroOffset() const {return this->m_AnalogZeroOffset;};
    void SetAnalogZeroOffset(const std::vector<int>& s) {this->m_AnalogZeroOffset = s;};
    double GetAnalogUniversalScale() const {return this->m_AnalogUniversalScale;};
    void SetAnalogUniversalScale(double s) {this->m_AnalogUniversalScale = s;};
    int GetWritingFlags() const {return this->m_WritingFlags;};
    void SetWritingFlags(int flags) {this->m_WritingFlags = flags;};
    BTK_IO_EXPORT bool HasWritingFlag(WritingFlag flag);
    
    BTK_IO_EXPORT virtual bool CanReadFile(const std::string& filename);
    BTK_IO_EXPORT virtual bool CanWriteFile(const std::string& filename);
    BTK_IO_EXPORT virtual void Read(const std::string& filename, Acquisition::Pointer output);
    BTK_IO_EXPORT virtual void Write(const std::string& filename, Acquisition::Pointer input);
    
  protected:
    BTK_IO_EXPORT C3DFileIO();
    
  private:
    BTK_IO_EXPORT size_t WriteMetaData(BinaryFileStream* obfs, MetaData::ConstPointer, int id);
    BTK_IO_EXPORT void KeepAcquisitionCompatibleVicon(Acquisition::Pointer input);
    BTK_IO_EXPORT void UpdateScalingFactorsFromData(Acquisition::Pointer input);
    BTK_IO_EXPORT void UpdateScalingFactorsFromMetaData(Acquisition::Pointer input);
    BTK_IO_EXPORT void UpdateMetaDataFromData(Acquisition::Pointer input, int numberOfFrames, int numberAnalogSamplePerFrame);
    BTK_IO_EXPORT void UpdateMetaDataFromSpecializedPoint(Acquisition::Pointer input, MetaData::Pointer point, std::vector<std::string>& typeGroups, Point::Type type, const std::string& label);

    C3DFileIO(const C3DFileIO& ); // Not implemented.
    C3DFileIO& operator=(const C3DFileIO& ); // Not implemented.
    
    // Interface
    class Format
    {
    public:
      Format(BinaryFileStream* bfs) {this->m_Bfs = bfs;};
      virtual ~Format() {};
      virtual void ReadPoint(double* x, double* y, double* z, double* residual, double* cam, double pointScaleFactor) = 0;
      virtual double ReadAnalog() = 0;
      virtual void WritePoint(double x, double y, double z, double residual, double cam, double pointScaleFactor) = 0;
      virtual void WriteAnalog(double v) = 0;
    protected:
      BinaryFileStream* m_Bfs;
    };
    // Integer format + signed analog data
    class IntegerFormatSignedAnalog : public Format
    {
    public:
      IntegerFormatSignedAnalog(BinaryFileStream* bfs) : Format(bfs) {};
      virtual ~IntegerFormatSignedAnalog() {};
      virtual void ReadPoint(double* x, double* y, double* z, double* residual, double* cam, double pointScaleFactor)
      {
        int8_t byteptr[2];
        int16_t residualAndMask;
        *x = this->m_Bfs->ReadI16() * pointScaleFactor;
        *y = this->m_Bfs->ReadI16() * pointScaleFactor;
        *z = this->m_Bfs->ReadI16() * pointScaleFactor;
        residualAndMask = this->m_Bfs->ReadI16();
        memcpy(&byteptr, &residualAndMask, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
        *cam = static_cast<double>(byteptr[0]);
        *residual = (byteptr[0] >= 0 ? static_cast<double>(byteptr[1]) * pointScaleFactor : -1.0);
#else
        *cam = static_cast<double>(byteptr[1]);
        *residual = (byteptr[1] >= 0 ? static_cast<double>(byteptr[0]) * pointScaleFactor : -1.0);
#endif
      };
      virtual double ReadAnalog() {return static_cast<float>(this->m_Bfs->ReadI16());};
      virtual void WritePoint(double x, double y, double z, double residual, double cam, double pointScaleFactor)
      {
        int8_t byteptr[2];
        int16_t residualAndMask; 
#if defined(_MSC_VER)
        this->m_Bfs->Write(static_cast<int16_t>(floor(x / pointScaleFactor + 0.5)));
        this->m_Bfs->Write(static_cast<int16_t>(floor(y / pointScaleFactor + 0.5)));
        this->m_Bfs->Write(static_cast<int16_t>(floor(z / pointScaleFactor + 0.5)));
#else
        this->m_Bfs->Write(static_cast<int16_t>(static_cast<float>(x / pointScaleFactor)));
        this->m_Bfs->Write(static_cast<int16_t>(static_cast<float>(y / pointScaleFactor)));
        this->m_Bfs->Write(static_cast<int16_t>(static_cast<float>(z / pointScaleFactor)));
#endif
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
        byteptr[0] = static_cast<int8_t>(cam);
        byteptr[1] = (byteptr[0] >= 0 ? static_cast<int8_t>(residual / pointScaleFactor) : -1);
#else
        byteptr[1] = static_cast<int8_t>(cam);
        byteptr[0] = (byteptr[1] >= 0 ? static_cast<int8_t>(residual / pointScaleFactor) : -1);
#endif
        memcpy(&residualAndMask, &byteptr, sizeof(residualAndMask));
        this->m_Bfs->Write(residualAndMask);

      };
      virtual void WriteAnalog(double v) {this->m_Bfs->Write(static_cast<int16_t>(v));};
    };
    // Integer format + unsigned analog data
    class IntegerFormatUnsignedAnalog : public IntegerFormatSignedAnalog
    {
    public:
      IntegerFormatUnsignedAnalog(BinaryFileStream* bfs) : IntegerFormatSignedAnalog(bfs) {};
      virtual ~IntegerFormatUnsignedAnalog() {};
      virtual double ReadAnalog() {return static_cast<float>(this->m_Bfs->ReadU16());};
      virtual void WriteAnalog(double v) {this->m_Bfs->Write(static_cast<uint16_t>(v));};
    };
    // Float format + signed analog data
    class FloatFormatSignedAnalog : public Format
    {
    public:
      FloatFormatSignedAnalog(BinaryFileStream* bfs) : Format(bfs) {};
      virtual ~FloatFormatSignedAnalog() {};
      virtual void ReadPoint(double* x, double* y, double* z, double* residual, double* cam, double pointScaleFactor)
      {
        int8_t byteptr[2];
        int16_t residualAndMask;
        *x = this->m_Bfs->ReadFloat();
        *y = this->m_Bfs->ReadFloat();
        *z = this->m_Bfs->ReadFloat();
        residualAndMask = static_cast<int16_t>(this->m_Bfs->ReadFloat());
        memcpy(&byteptr, &residualAndMask, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
        *cam = static_cast<double>(byteptr[0]);
        // FIX: It seems that for UNSGINED 16 bits in float format, the residual is negative.
        //      The residual is now calculated as the fabs(byteptr[1] * scaleFactor3d).
        *residual = (byteptr[0] >= 0 ? fabs(static_cast<double>(byteptr[1]) * pointScaleFactor) : -1.0);
#else
        *cam = static_cast<double>(byteptr[1]);
        *residual = (byteptr[1] >= 0 ? fabs(static_cast<double>(byteptr[0]) * pointScaleFactor) : -1.0);
#endif
      };
      virtual double ReadAnalog() {return this->m_Bfs->ReadFloat();};
      virtual void WritePoint(double x, double y, double z, double residual, double cam, double pointScaleFactor)
      {
        int8_t byteptr[2];
        int16_t residualAndMask;
        this->m_Bfs->Write(static_cast<float>(x));
        this->m_Bfs->Write(static_cast<float>(y));
        this->m_Bfs->Write(static_cast<float>(z));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
        byteptr[0] = static_cast<int8_t>(cam);
        byteptr[1] = (byteptr[0] >= 0 ? static_cast<int8_t>(fabs(residual / pointScaleFactor)) : -1);
#else
        byteptr[1] = static_cast<int8_t>(cam);
        byteptr[0] = (byteptr[1] >= 0 ? static_cast<int8_t>(fabs(residual / pointScaleFactor)) : -1);
#endif
        memcpy(&residualAndMask, &byteptr, sizeof(residualAndMask));
        this->m_Bfs->Write(static_cast<float>(residualAndMask));
      };
      virtual void WriteAnalog(double v) {this->m_Bfs->Write(static_cast<float>(v));};
    };
    // Float format + unsigned analog data
    class FloatFormatUnsignedAnalog : public FloatFormatSignedAnalog
    {
    public:
      FloatFormatUnsignedAnalog(BinaryFileStream* bfs) : FloatFormatSignedAnalog(bfs) {};
      virtual ~FloatFormatUnsignedAnalog() {};
      virtual double ReadAnalog() {return static_cast<uint16_t>(this->m_Bfs->ReadFloat());};
      virtual void WriteAnalog(double v) {this->m_Bfs->Write(static_cast<float>(static_cast<uint16_t>(v)));};
    };

    double m_PointScale;
    std::vector<double> m_AnalogChannelScale;
    std::vector<int> m_AnalogZeroOffset;
    double m_AnalogUniversalScale;
    AnalogIntegerFormat m_AnalogIntegerFormat;
    int m_WritingFlags;
  };
};

#endif // __btkC3DFileIO_h
