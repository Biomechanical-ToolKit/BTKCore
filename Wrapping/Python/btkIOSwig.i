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

%module btkIO
%{
#include "btkCommonSwig.h"
#include "btkIOSwig.h"
%}
%include <std_string.i>

#undef BTK_SWIG_HEADER_DECLARATION

#include "btkSwigMacros.h"

%include "btkIOSwig_docstring.i"

%include "IO/btkIOSwig_AcquisitionFile.h"

// ------------------------------------------------------------------------- //
//                             AcquisitionFileIO                             //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS(AcquisitionFileIO)
{
public:
  typedef enum {TypeNotApplicable, ASCII, Binary} FileType;
  typedef enum {OrderNotApplicable = 0, IEEE_LittleEndian, VAX_LittleEndian, IEEE_BigEndian} ByteOrder;
  typedef enum {StorageNotApplicable = 0, Float = -1, Integer = 1} StorageFormat;

  FileType GetFileType() const;
  ByteOrder GetByteOrder() const;
  std::string GetByteOrderAsString() const;
  void SetByteOrder(ByteOrder b);
  StorageFormat GetStorageFormat() const;
  std::string GetStorageFormatAsString() const;
  void SetStorageFormat(StorageFormat s);

  virtual bool CanReadFile(const std::string& filename) = 0;
  virtual bool CanWriteFile(const std::string& filename) = 0;
  virtual void Read(const std::string& filename, btkAcquisition output) = 0;
  virtual void Write(const std::string& filename, btkAcquisition input) = 0;
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(AcquisitionFileIO);
};

// ------------------------------------------------------------------------- //
//                           AcquisitionFileReader                           //
// ------------------------------------------------------------------------- //

%extend btkAcquisitionFileReader
{
  btkAcquisitionFileIO GetAcquisitionIO() {return btkAcquisitionFileIO((*$self)->btkAcquisitionFileReader_impl::GetAcquisitionIO());};
};

BTK_SWIG_DECLARE_IMPL_CLASS_FILTER(AcquisitionFileReader)
{
public:
  btkAcquisition GetOutput();
  bool GetDisableFilenameExceptionState() const;
  void SetDisableFilenameExceptionState(bool );
  const std::string& GetFilename() const;
  void SetFilename(const std::string& );
  void SetAcquisitionIO(btkAcquisitionFileIO io = btkAcquisitionFileIO());
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(AcquisitionFileReader);
};

// ------------------------------------------------------------------------- //
//                           AcquisitionFileWriter                           //
// ------------------------------------------------------------------------- //

%extend btkAcquisitionFileWriter
{
  btkAcquisitionFileIO GetAcquisitionIO() {return btkAcquisitionFileIO((*$self)->btkAcquisitionFileWriter_impl::GetAcquisitionIO());};
};

BTK_SWIG_DECLARE_IMPL_CLASS_FILTER(AcquisitionFileWriter)
{
public:
  btkAcquisition GetInput();
  void SetInput(btkAcquisition );
  const std::string& GetFilename() const;
  void SetFilename(const std::string& );
  void SetAcquisitionIO(btkAcquisitionFileIO io = btkAcquisitionFileIO());
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(AcquisitionFileWriter);
};

// ------------------------------------------------------------------------- //
//                                  C3DFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS(C3DFileIO) : public btkAcquisitionFileIO_impl
{
public:
  typedef enum {Signed, Unsigned} AnalogIntegerFormat;
  typedef enum {None_ = 1, ScalesFromDataUpdate = 2, ScalesFromMetaDataUpdate = 4, MetaDataFromDataUpdate = 8, CompatibleVicon = 16} WritingFlag; // None replaced by None_ because it is a keyword under python
  double GetPointScale() const;
  void SetPointScale(double s);
  AnalogIntegerFormat GetAnalogIntegerFormat();
  void SetAnalogIntegerFormat(AnalogIntegerFormat f);
  std::vector<double>& GetAnalogChannelScale();
  void SetAnalogChannelScale(const std::vector<double>& s);
  std::vector<int>& GetAnalogZeroOffset();
  void SetAnalogZeroOffset(const std::vector<int>& s);
  double GetAnalogUniversalScale() const;
  void SetAnalogUniversalScale(double s);
  int GetWritingFlags() const;
  void SetWritingFlags(int flags);
  bool HasWritingFlag(WritingFlag flag);
};

// ------------------------------------------------------------------------- //
//                                  ANBFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(ANBFileIO);

// ------------------------------------------------------------------------- //
//                                  ANCFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS(ANCFileIO) : public btkAcquisitionFileIO_impl
{
public:
  int GetFileGeneration() const;
  void SetFileGeneration(int gen);
};

// ------------------------------------------------------------------------- //
//                                  ANGFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(ANGFileIO);

// ------------------------------------------------------------------------- //
//                             CALForcePlateFileIO                           //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(CALForcePlateFileIO);

// ------------------------------------------------------------------------- //
//                                  EMFFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(EMFFileIO);

// ------------------------------------------------------------------------- //
//                                  EMxFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(EMxFileIO);

// ------------------------------------------------------------------------- //
//                                  GRxFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(GRxFileIO);

// ------------------------------------------------------------------------- //
//                                  MOMFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(MOMFileIO);

// ------------------------------------------------------------------------- //
//                                  PWRFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(PWRFileIO);

// ------------------------------------------------------------------------- //
//                                  RAxFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(RAxFileIO);

// ------------------------------------------------------------------------- //
//                                  RICFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(RICFileIO);

// ------------------------------------------------------------------------- //
//                                  TRBFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(TRBFileIO);

// ------------------------------------------------------------------------- //
//                                  TRCFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(TRCFileIO);

// ------------------------------------------------------------------------- //
//                                  TDFFileIO                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(TDFFileIO);

// ------------------------------------------------------------------------- //
//                             XLSOrthoTrakFileIO                            //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS_IO(XLSOrthoTrakFileIO);
