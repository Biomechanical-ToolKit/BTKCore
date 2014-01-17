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

#include "btkKistlerDATFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkMetaDataUtils.h"

namespace btk
{
  /**
   * @class KistlerDATFileIOException btkKistlerDATFileIO.h
   * @brief Exception class for the KistlerDATFileIO class.
   */
  
  /**
   * @fn KistlerDATFileIOException::KistlerDATFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual KistlerDATFileIOException::~KistlerDATFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class KistlerDATFileIO btkKistlerDATFileIO.h
   * @brief Interface to read Kistler DAT files.
   *
   * The DAT file format is used in Kistler systems and saved by the software BioWare.
   *
   * WARNING: This file IO is totally experimental and based only on DAT files saved by BioWare 3. More data are needed to check with the new version of the software BioWare.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef KistlerDATFileIO::Pointer
   * Smart pointer associated with a KistlerDATFileIO object.
   */
  
  /**
   * @typedef KistlerDATFileIO::ConstPointer
   * Smart pointer associated with a const KistlerDATFileIO object.
   */
  
  /**
   * @fn static KistlerDATFileIO::Pointer KistlerDATFileIO::New()
   * Create a KistlerDATFileIO object an return it as a smart pointer.
   */
  
  /**
   * Check if the beginning of the file start by 0x 20 00 00 00.
   * NOTE: Need to double check if this is a valid header key for the Kistler data. This check result from only a couple of files from the same source.
   */
  bool KistlerDATFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    NativeBinaryFileStream bifs(filename, BinaryFileStream::In);
    if (bifs.ReadI32() != 2)
      isReadable = false;
    bifs.Close();
    return isReadable;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void KistlerDATFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      // WARNING: The header key is maybe not a key but the number of force platforms in the file!
      if (bifs.ReadI32() != 2)
        throw(KistlerDATFileIOException("Wrong header key. It is not a proper Kistler DAT file."));
      
      // NOTE: It seems there are several headers in the file. A general header at the beginning
      // which resume acquisition's information (number of channels, number of frames, sample 
      // rate, ...) and another one for each force plate. It is not yet clear
      
      // General header?
      // ---------------
      // FIXME: What is the meaning of the next 66 bytes?
      bifs.SeekRead(66, BinaryFileStream::Current);
      // 0x70: (int32) Number of channels
      int numberOfChannels = bifs.ReadI32();
      // 0x74: (int32) Number of frames
      int numberOfFrames = bifs.ReadI32();
      // 0x78: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x82: (int32) ADC resolution? Number of ADC channels on the board?
      int digitalResolution = bifs.ReadI32();
      // 0x86: (double) Sample frequency
      double sampleFrequency = bifs.ReadDouble();
      // 0x94: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x98: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x102: Normalized force (N) - SKIPPED
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x106: Normalized length (mm) - SKIPPED
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x110: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x114: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x118: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x122: ?? (Could be an index) 
      bifs.SeekRead(2, BinaryFileStream::Current);
      // 0x124: (int16_t) Number of characters (n) in the next string
      // 0x126: (string) What is this string? - SKIPPED
      bifs.SeekRead(bifs.ReadU16(), BinaryFileStream::Current);
      
      // Force plate header?
      // -------------------
      // For simplification the adress in the description is reset to 0, but this is right after
      // the general header, so add 126+n to find the good position in a file.
      // 0x00: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x04: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x08: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x12: (int32) Number of channels
      if (bifs.ReadI32() != numberOfChannels)
        throw(KistlerDATFileIOException("Wrong number of channels declared."));
      // 0x16: (int32) Number of frames
      if (bifs.ReadI32() != numberOfFrames)
        throw(KistlerDATFileIOException("Wrong number of frames declared."));
      // 0x20: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x24: (int32) ADC resolution
      if (bifs.ReadI32() != digitalResolution)
        throw(KistlerDATFileIOException("Wrong ADC resolution declared."));
      // 0x28: (double) sample frequency
      if (bifs.ReadDouble() != sampleFrequency)
        throw(KistlerDATFileIOException("Wrong sample frequency declared."));
      // 0x36: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x40: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x44: (int32) Normalized force (N) - SKIPPED
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x48: (int32) Normalized length (mm) - SKIPPED
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x52: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x56: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x60: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x64: (uint8) Number of characters (i) for the manufacturer of the force plateform
      // 0x65: (string) Manufacter of the force plateform - SKIPPED
      bifs.SeekRead(bifs.ReadU8(), BinaryFileStream::Current);
      // 0x65+i: (int16) Number of characters (j) for the reference of the force plateform
      // 0x66+i: (string) Reference of the force plateform - SKIPPED
      bifs.SeekRead(bifs.ReadU8(), BinaryFileStream::Current);
      // 0x66+i+j: (uint8) Number of characters (k) for the ? (serial number?)
      // 0x67+i+j: (string) (Serial number?) of the force plateform - SKIPPED
      bifs.SeekRead(bifs.ReadU8(), BinaryFileStream::Current);
      // 0x67+i+j+k: (int32) Contact period start (sample #) - SKIPPED
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x71+i+j+k: (int32) Contact period end (sample #) - SKIPPED
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x75+i+j+k: ?? - Seems to be a double
      bifs.SeekRead(8, BinaryFileStream::Current);
      // 0x83+i+j+k: ??
      bifs.SeekRead(4, BinaryFileStream::Current);
      // 0x87+i+j+k: (double) FP width (mm)
      double width = bifs.ReadDouble();
      // 0x95+i+j+k: (double) FP length (mm)
      double length = bifs.ReadDouble();
      // 0x103+i+j+k: (double) FP a (mm)
      double a = bifs.ReadDouble();
      // 0x111+i+j+k: (double) FP b (mm)
      double b = bifs.ReadDouble();
      // What are the next 24 bytes? They are set to 0.
      bifs.SeekRead(24, BinaryFileStream::Current);
      // Why the parameter az0 is repeated two times?
      double az0 = bifs.ReadDouble();
      bifs.SeekRead(8, BinaryFileStream::Current); // az0 is written a second time
      // The next values are 3 strings which contain the word "INTEGRATED"
      // Maybe this is related to know if the component of each sensor is linked to built-in charge amplifier
      bifs.SeekRead(bifs.ReadU8(), BinaryFileStream::Current); // Integrated
      bifs.SeekRead(bifs.ReadU8(), BinaryFileStream::Current); // Integrated
      bifs.SeekRead(bifs.ReadU8(), BinaryFileStream::Current); // Integrated
      // Next double value could be the output voltage
      bifs.SeekRead(8, BinaryFileStream::Current);
      // Next 8 double values could be a scale factor for the amplifier (pC->V)
      // Blank of 64 bytes (could be for 8 other channels?)
      bifs.SeekRead(128, BinaryFileStream::Current);
      // Next 8 double values could be the calibrated partial range
      // Blank of 64 bytes (could be for 8 other channels?)
      bifs.SeekRead(128, BinaryFileStream::Current);
      // There is then 104 blank bytes
      bifs.SeekRead(104, BinaryFileStream::Current);
      
      // Initialize the output
      output->Init(0,numberOfFrames,numberOfChannels);
      output->SetPointFrequency(sampleFrequency);
      
      // Then the real (float) measured values (Fx12, Fx34, Fy14, Fy23, Fz1, Fz2, Fz4) directly in newtons
      // WARNING: These values are stored by channels and not by samples
      int inc = 0;
      const char* labels[8] = {"Fx12", "Fx34", "Fy14", "Fy23", "Fz1", "Fz2", "Fz3", "Fz4"};
      for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
      {
        (*it)->SetLabel(labels[inc++]);
        (*it)->SetUnit("N");
        for (int i = 0 ; i < numberOfFrames ; ++i)
          (*it)->GetValues().coeffRef(i) = -1.0 * bifs.ReadFloat(); // -1.0: BTK stores the reaction.
      }
      
      // Compute the origin and the coordinates of the corners
      std::vector<float> corners(12);
      std::vector<float> origin(3);
      // TODO: The next lines could be factorized with the code of the method AMTIForcePlatformFileIO::computeGeometryFromDimensions
      float cx = static_cast<float>(width) / 2.0f;
      float cy = static_cast<float>(length) / 2.0f;
      // Corners expressed in the global frame.
      // The global frame is set here as: axis X going forward, axis Y on the left and axis Z going upward.
      // The corners are set to have the corner #1 on the bottom left side, #2 on the top left side, #3 on the top right side and #4 on the bottom right side.
      corners[0] = cx;
      corners[1] = -cy;
      corners[2] = 0.0f;
      corners[3] = -cx;
      corners[4] = -cy;
      corners[5] = 0.0f;
      corners[6] = -cx;
      corners[7] = cy;
      corners[8] = 0.0f;
      corners[9] = cx;
      corners[10] = cy;
      corners[11] = 0.0f;
      // - Origin (expressed in the global frame) and centered above the origin of the global frame
      origin[0] = (float)a;
      origin[1] = (float)b;
      origin[2] = (float)az0;
      
      // Create the metadata FORCE_PLATFORM
      btk::MetaData::Pointer forcePlatform = btk::MetaData::New("FORCE_PLATFORM");
      output->GetMetaData()->AppendChild(forcePlatform);
      // - FORCE_PLATFORM:USED
      forcePlatform->AppendChild(btk::MetaData::New("USED", static_cast<int16_t>(1)));
      // - FORCE_PLATFORM:TYPE
      forcePlatform->AppendChild(btk::MetaData::New("TYPE", std::vector<int16_t>(1,3)));
      // - FORCE_PLATFORM:ZERO
      std::vector<int16_t> zero(2,0); zero[0] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("ZERO", zero));
      // - FORCE_PLATFORM:CORNERS
      std::vector<uint8_t> dims(3); dims[0] = 3; dims[1] = 4; dims[2] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("CORNERS", dims, corners));
      // - FORCE_PLATFORM:ORIGIN
      dims.resize(2); dims[0] = 3; dims[1] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("ORIGIN", dims, origin));
      // - FORCE_PLATFORM:CHANNEL
      dims.resize(2); dims[0] = 8; dims[1] = 1;
      std::vector<int16_t> channel(8);
      for (int i = 0 ; i < 8 ; ++i)
        channel[i] = i+1;
      forcePlatform->AppendChild(btk::MetaData::New("CHANNEL", dims, channel));
      // Add a metadata to notify that the first frame was not set.
      MetaData::Pointer btkPointConfig = MetaDataCreateChild(output->GetMetaData(), "BTK_POINT_CONFIG");
      MetaDataCreateChild(btkPointConfig, "NO_FIRST_FRAME", static_cast<int8_t>(1));
    }
    catch (BinaryFileStreamFailure& )
    {
      std::string excmsg; 
      if (bifs.EndFile())
        excmsg = "Unexpected end of file.";
      else if (!bifs.IsOpen())
        excmsg = "Invalid file path.";
      else if(bifs.Bad())
        excmsg = "Loss of integrity of the file stream.";
      else if(bifs.Fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the file stream.";
      
      if (bifs.IsOpen()) bifs.Close();    
        throw(KistlerDATFileIOException(excmsg));
    }
    catch (KistlerDATFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(KistlerDATFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(KistlerDATFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  KistlerDATFileIO::KistlerDATFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};
