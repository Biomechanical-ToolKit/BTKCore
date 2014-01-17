/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

BTK_SWIG_DECLARE_CLASS(AcquisitionFileIO)
{
public:
  enum {TypeNotApplicable = btk::AcquisitionFileIO::TypeNotApplicable, ASCII = btk::AcquisitionFileIO::ASCII, Binary = btk::AcquisitionFileIO::Binary};
  enum {OrderNotApplicable = btk::AcquisitionFileIO::OrderNotApplicable, IEEE_LittleEndian = btk::AcquisitionFileIO::IEEE_LittleEndian, VAX_LittleEndian = btk::AcquisitionFileIO::VAX_LittleEndian, IEEE_BigEndian = btk::AcquisitionFileIO::IEEE_BigEndian};
  enum {StorageNotApplicable = btk::AcquisitionFileIO::StorageNotApplicable, Float = btk::AcquisitionFileIO::Float, Integer = btk::AcquisitionFileIO::Integer};
  enum {UpdateNotApplicable = btk::AcquisitionFileIO::UpdateNotApplicable, NoUpdate = btk::AcquisitionFileIO::NoUpdate, DataBasedUpdate = btk::AcquisitionFileIO::DataBasedUpdate, MetaDataBasedUpdate = btk::AcquisitionFileIO::MetaDataBasedUpdate, FileFormatOption = btk::AcquisitionFileIO::FileFormatOption};

  btkAcquisitionFileIO(const btkAcquisitionFileIO_shared& toCopy) : btkAcquisitionFileIO_shared(toCopy) {};
  BTK_SWIG_DECLARE_POINTER_OPERATOR(AcquisitionFileIO);
private:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(AcquisitionFileIO);
};

BTK_SWIG_DECLARE_CLASS(AcquisitionFileReader)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(AcquisitionFileReader);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(AcquisitionFileReader);
};

BTK_SWIG_DECLARE_CLASS(AcquisitionFileWriter)
{
public:
  BTK_SWIG_DECLARE_DEFAULT_CTOR(AcquisitionFileWriter);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(AcquisitionFileWriter);
};

BTK_SWIG_DECLARE_CLASS_INHERIT(C3DFileIO, AcquisitionFileIO)
{
public:
  enum {Signed, Unsigned};
  enum {CompatibleVicon = btk::C3DFileIO::CompatibleVicon};
  BTK_SWIG_DECLARE_DEFAULT_IO_CTOR(C3DFileIO);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(C3DFileIO);
};

BTK_SWIG_DECLARE_CLASS_IO(ANBFileIO);

BTK_SWIG_DECLARE_CLASS_IO(ANCFileIO);

BTK_SWIG_DECLARE_CLASS_IO(ANGFileIO);

BTK_SWIG_DECLARE_CLASS_IO(CALForcePlateFileIO);

BTK_SWIG_DECLARE_CLASS_IO(CLBFileIO);

BTK_SWIG_DECLARE_CLASS_IO(DelsysEMGFileIO);

BTK_SWIG_DECLARE_CLASS_IO(MDFFileIO);

BTK_SWIG_DECLARE_CLASS_IO(EMFFileIO);

BTK_SWIG_DECLARE_CLASS_IO(EMxFileIO);

BTK_SWIG_DECLARE_CLASS_IO(GRxFileIO);

BTK_SWIG_DECLARE_CLASS_IO(MOMFileIO);

BTK_SWIG_DECLARE_CLASS_IO(PWRFileIO);

BTK_SWIG_DECLARE_CLASS_IO(RAxFileIO);

BTK_SWIG_DECLARE_CLASS_IO(RICFileIO);

BTK_SWIG_DECLARE_CLASS_IO(TDFFileIO);

BTK_SWIG_DECLARE_CLASS_IO(TRBFileIO);

BTK_SWIG_DECLARE_CLASS_IO(TRCFileIO);

BTK_SWIG_DECLARE_CLASS_IO(XLSOrthoTrakFileIO);

BTK_SWIG_DECLARE_CLASS_IO(XMOVEFileIO);
