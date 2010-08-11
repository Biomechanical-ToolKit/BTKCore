/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "btkTRBFileIO.h"
#include "btkMetaDataUtils.h"
#include "btkConvert.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <vector>

namespace btk
{
  /**
   * @class TRBFileIOException
   * @brief Exception class for the TRBFileIO class.
   */
  
  /**
   * @fn TRBFileIOException::TRBFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual TRBFileIOException::~TRBFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class TRBFileIO
   * @brief Interface to read/write TRB files.
   *
   * A TRB file contains markers trajectories stored in the IEEE Little endian binary format.
   *
   * Based on reverse engineering, the file format is composed of a header part and a data part.
   * The header is composed of keys and values as the data part. 
   * 
   * The known keys are:
   *  - <tt>0x0101</tt>: ID?
   *  - <tt>0x0102</tt>: ?
   *  - <tt>0x0103</tt>: Data Rate
   *  - <tt>0x0104</tt>: Camera Rate
   *  - <tt>0x0105</tt>: First time (or first frame)?
   *  - <tt>0x0115</tt>: ?
   *  - <tt>0x010C</tt>: Markers' informations (#, color, label)
   *  - <tt>0x010D</tt>: Link (segment) informations (#, extremity1, extremity2, color, label)
   *
   * A key is always followed by the size of its value and is write on 2 bytes. The size correspond to
   * number of word associated with the key. A word corresponds to four bytes.
   * For example, the following string:
   * @verbatim 0800 0001 0000 024C @endverbatim
   * corresponds to the key @c 0x0800 with a size of 1 word (<tt>0x0001</tt>) and a value equal to 588 (<tt>0x024C</tt>).
   *
   * The structure of the file can be summarized as the following:
   * @code
   * // Header
   * 0x0000 0000 FFFF FFFF // Begin Header
   * 0xXXXX XXXX           // Address for the data part
   * 0x0101 ... 
   * 0x0102 ... 
   * 0x0103 ... 
   * 0x0104 ...
   * 0x0105 ...
   * 0x0115 ...
   * 0x010C ... 
   *   |- // repeated by the number of markers
   * 0x010C ...
   * 0x010D ... 
   *   |- // repeated by the number of links
   * 0x010D ...
   * // Data
   * 0x0000 0600 0000 0000
   *   |- // repeated by the number of frames
   * 0x0000 0600 ... 0000 0000
   * @endcode
   *
   * The TRB file format is created by Motion Analysis Corp.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef TRBFileIO::Pointer
   * Smart pointer associated with a TRBFileIO object.
   */
  
  /**
   * @typedef TRBFileIO::ConstPointer
   * Smart pointer associated with a const TRBFileIO object.
   */
  
  /**
   * @fn static TRBFileIO::Pointer TRBFileIO::New()
   * Create a TRBFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first word in the file corresponds to "PathFileType".
   */
  bool TRBFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    // Three first words: 0x0000 0000 FFFF FFFF
    IEEELittleEndianBinaryFileStream bifs(filename, BinaryFileStream::In);
    if ((bifs.ReadI16() != 0x0000) || (bifs.ReadI16() != 0x0000) || (bifs.ReadU16() != 0xFFFF) || (bifs.ReadU16() != 0xFFFF))
      isReadable = false;
    bifs.Close();
    return isReadable;
  };
  
  /**
   * Checks if the suffix of @a filename is TRB.
   */
  bool TRBFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type TRBPos = lowercase.rfind(".trb");
    if ((TRBPos != std::string::npos) && (TRBPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void TRBFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      
      if ((bifs.ReadI16() != 0x0000) || (bifs.ReadI16() != 0x0000) || (bifs.ReadU16() != 0xFFFF) ||  (bifs.ReadU16() != 0xFFFF))
        throw TRBFileIOException("Invalid header key.");
      // Header
      size_t dataAddress;
      uint32_t id, tempU;
      float freq, tempF, firstTime;
      dataAddress = bifs.ReadU16()  << 16 | bifs.ReadU16();
      dataAddress *= 4;
      this->ReadKeyValueU32(&id, &bifs, 0x0101);
      this->ReadKeyValueU32(&tempU, &bifs, 0x0102); // ?
      this->ReadKeyValueFloat(&freq, &bifs, 0x0103);
      this->ReadKeyValueFloat(&tempF, &bifs, 0x0104); // Should be the same than freq
      this->ReadKeyValueFloat(&firstTime, &bifs, 0x0105);
      this->ReadKeyValueU32(&tempU, &bifs, 0x0115); // ?
      // Markers
      std::vector<std::string> labels;
      std::vector<int> indices;
      int numMarkers = -1;
      uint16_t index, color;
      int offset;
      while (1)
      {
        if (bifs.ReadU16() == 0x010C)
        {
          offset = bifs.ReadU16();
          index = bifs.ReadU16();
          color = bifs.ReadU16();
          std::string label = bifs.ReadString((offset - 1) * 4);
          label = label.erase(label.find_last_not_of((char)0x20)+1);
          label = label.erase(label.find_last_not_of((char)0x00)+1);
          if (index > numMarkers)
            numMarkers = index;
          labels.push_back(label);
          indices.push_back(index-1);
        }
        else
          break;
      }
      
      // Link
      // (skipped)
      
      // Data
      // Go to the adress
      bifs.SeekRead(dataAddress, BinaryFileStream::Begin);
      // Get the size of the data
      bifs.SeekRead(0, BinaryFileStream::End);
      size_t dataSize = bifs.TellRead();
      dataSize -= dataAddress;
      size_t dataSizeBis = dataSize;
      // Count the number of frames
      bifs.SeekRead(dataAddress, BinaryFileStream::Begin);
      int numFrames = -1;
      while (1)
      {
        if ((bifs.ReadI16() != 0x0000) || (bifs.ReadI16() != 0x0000) || (bifs.ReadI16() != 0x0600))
          throw TRBFileIOException("Corrupted TRB file. Invalid frame key.");
        index = bifs.ReadU16();
        offset = bifs.ReadU16();
        if (index > numFrames)
          numFrames = index;
        dataSize -= offset * 4;
        if (dataSize <= 0)
          break;
        bifs.SeekRead((offset - 2) * 4 - 2, BinaryFileStream::Current);
      }
      
      // Initialize the acquisition
      output->Init(static_cast<int>(indices.size()), numFrames);
      output->SetPointFrequency(static_cast<double>(freq));
      output->SetFirstFrame(static_cast<int>(firstTime * freq) + 1);
      bifs.SeekRead(dataAddress, BinaryFileStream::Begin);
      // Construct a vector of indices to facilitate the coordinates' extraction
      // And set the markers' label
      std::vector<int> markerIndex = std::vector<int>(numMarkers, -1);
      for (int i = 0 ; i < static_cast<int>(indices.size()) ; ++i)
      {
        output->GetPoint(i)->SetLabel(labels[i]);
        markerIndex[indices[i]] = i;
      }
      //  Extract coordinates
      while (1)
      {
        bifs.SeekRead(6, BinaryFileStream::Current); // 0x0000 00000 06000
        index = bifs.ReadU16() - 1;
        offset = bifs.ReadU16() * 4;
        dataSizeBis -= offset;
        bifs.SeekRead(2, BinaryFileStream::Current); // 0x0000
        offset -= 12;  // Already 12 bytes read
        if (offset > 0)
        {
          while (1)
          {
            Point::Pointer point = output->GetPoint(markerIndex[bifs.ReadU16()-1]);
            bifs.SeekRead(2, BinaryFileStream::Current); // 0x0000
            point->GetValues().coeffRef(index,0) = bifs.ReadFloat(); // X
            point->GetValues().coeffRef(index,1) = bifs.ReadFloat(); // Y
            point->GetValues().coeffRef(index,2) = bifs.ReadFloat(); // Z
            point->GetResiduals().coeffRef(index) = bifs.ReadFloat(); // Residual
            point->GetMasks().coeffRef(index) = static_cast<double>(bifs.ReadU16() | bifs.ReadU16() << 16); // Mask
            offset -= 24;
            if (offset <= 0)
              break;
          }
        }
        if (dataSizeBis <= 0)
          break;
      }
    }
    catch (BinaryFileStreamException& )
    {
      std::string excmsg; 
      if (bifs.EndFile())
        excmsg = "Unexpected end of file.";
      else if (!bifs.IsOpen())
        excmsg = "Invalid file path.";
      else if(bifs.Bad())
        excmsg = "Loss of integrity of the filestream.";
      else if(bifs.Fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the filestream.";
      
      if (bifs.IsOpen()) bifs.Close(); 
      throw(TRBFileIOException(excmsg));
    }
    catch (TRBFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (ANxFileIOException& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(TRBFileIOException(e.what()));
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(TRBFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(TRBFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void TRBFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    btkNotUsed(filename);
    btkNotUsed(input);
    /*
    if (input.get() == 0)
    {
      btkIOErrorMacro(filename, "Empty input. Impossible to write an empty file.");
      return;
    }
    */
    btkErrorMacro("Method not yet implemented.");
    return;
  };
  
  /**
   * Constructor.
   */
  TRBFileIO::TRBFileIO()
  : MotionAnalysisBinaryFileIO()
  {};
};
