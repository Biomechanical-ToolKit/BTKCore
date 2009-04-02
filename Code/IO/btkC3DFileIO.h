/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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
    typedef SharedPtr<C3DFileIO> Pointer;
    typedef SharedPtr<const C3DFileIO> ConstPointer;
    
    static Pointer New() {return Pointer(new C3DFileIO());};
    using AcquisitionFileIO::SetByteOrder;
    
    ~C3DFileIO() {};
    
    virtual bool CanReadFile(const std::string& filename);
    virtual bool CanWriteFile(const std::string& filename);
    virtual void Read(const std::string& filename, Acquisition::Pointer output);
    virtual void Write(const std::string& filename, Acquisition::Pointer input);
    
  protected:
    C3DFileIO();
    
  private:
    C3DFileIO(const C3DFileIO& ); // Not implemented.
    C3DFileIO& operator=(const C3DFileIO& ); // Not implemented.
    
    void collapse(MetaDataEntry::ConstPointer group, int numberToCollapse,
                  const std::string& base, std::vector<std::string>& collapsed,
                  bool needToFillBlank);
    
    class Format
    {
    public:
      Format(BinaryFileStream* bfs) {m_Bfs = bfs;};
      //virtual void ReadPoint(Eigen::Matrix<double,Eigen::Dynamic,3>::RowXpr* coord, double* residual, double* cam, float markerScaleFactor) = 0;
      virtual void ReadPoint(double* x, double* y, double* z, double* residual, double* cam, float markerScaleFactor) = 0;
      virtual float ReadAnalog() = 0;
    protected:
      BinaryFileStream* m_Bfs;
    };
    class IntegerFormat : public Format
    {
    public:
      IntegerFormat(BinaryFileStream* bfs) : Format(bfs) {};
      //virtual void ReadPoint(Eigen::Matrix<double,Eigen::Dynamic,3>::RowXpr* coord, double* residual, double* cam, float markerScaleFactor)
      virtual void ReadPoint(double* x, double* y, double* z, double* residual, double* cam, float markerScaleFactor)
      {
        int8_t byteptr[2];
        int16_t residualAndMask;
        *x = this->m_Bfs->ReadI16() * markerScaleFactor;
        *y = this->m_Bfs->ReadI16() * markerScaleFactor;
        *z = this->m_Bfs->ReadI16() * markerScaleFactor;
        residualAndMask = this->m_Bfs->ReadI16();
        memcpy(&byteptr, &residualAndMask, sizeof(byteptr));
        *cam = byteptr[0];
        *residual = (byteptr[1] >= 0 ? byteptr[0] * markerScaleFactor : -1);
      };
      virtual float ReadAnalog() {return static_cast<float>(this->m_Bfs->ReadI16());};
    };
    class FloatFormat : public Format
    {
    public:
      FloatFormat(BinaryFileStream* bfs) : Format(bfs) {};
      //virtual void ReadPoint(Eigen::Matrix<double,Eigen::Dynamic,3>::RowXpr* coord, double* residual, double* cam, float markerScaleFactor)
      virtual void ReadPoint(double* x, double* y, double* z, double* residual, double* cam, float markerScaleFactor)
      {
        int8_t byteptr[2];
        int16_t residualAndMask;
        *x = this->m_Bfs->ReadFloat();
        *y = this->m_Bfs->ReadFloat();
        *z = this->m_Bfs->ReadFloat();
        residualAndMask = static_cast<int16_t>(this->m_Bfs->ReadFloat());
        memcpy(&byteptr, &residualAndMask, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
        *cam = byteptr[0];
        // FIX: It seems that for UNSGINED 16 bits in float format, the residual is negative.
        //      The residual is now calculated as the fabs(byteptr[1] * scaleFactor3d).
        *residual = (byteptr[0] >= 0 ? fabs(byteptr[1] * markerScaleFactor) : -1);
#else
        *cam = byteptr[1];
        *residual = (byteptr[1] >= 0 ? fabs(byteptr[0] * markerScaleFactor) : -1);
#endif
      };
      virtual float ReadAnalog() {return this->m_Bfs->ReadFloat();};
    };
  };
};

#endif // __btkC3DFileIO_h
