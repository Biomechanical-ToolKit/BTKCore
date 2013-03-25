/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#include "btkBSFFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkMetaDataUtils.h"

namespace btk
{
  const int maximumChannelNumber = 32;
  // Internal structure used to store the configuration of each instrument (as proposed in the BSF file format documentation).
  struct BSFFileIO::InstrumentHeader
  {
    int32_t headerSize;
    int32_t serialNumber;
    int32_t layoutPlatformNumber;
    std::string name;
    int32_t numberOfChannels;
    int32_t hardwareStartChannel;
    int32_t hardwareEndChannel;
    int32_t softwareStartChannel;
    int32_t softwareEndChannel;
    float length;
    float width;
    float offset[3];
    float sensitivity[maximumChannelNumber];
    int16_t channels[maximumChannelNumber];
    float transformation[16]; // 4x4 transformation matrix
    float interDistance[3];
    float amplifierGain[maximumChannelNumber];
    float excitationVoltage[maximumChannelNumber];
    float acquisitionCardRange[maximumChannelNumber]; // bits/volt
    float zeroPeriod;
    float latencyPeriod;
    float triggerTime;
    float endTime;
    float postTrigTime;
    int32_t zero[maximumChannelNumber];
    int32_t rate;
    float triggerValue;
    float endValue;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class BSFFileIOException btkBSFFileIO.h
   * @brief Exception class for the BSFFileIO class.
   */
  
  /**
   * @fn BSFFileIOException::BSFFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual BSFFileIOException::~BSFFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class BSFFileIO btkBSFFileIO.h
   * @brief Interface to read/write BSF files.
   *
   * The BSF file format is a proprietary file format developed by the AMTI company (Watertown, USA).
   *
   * The reader of this file format was implemented using the documentation of the software NetForce (appendix B), graciously offered by the AMTI company.
   *
   * @warning The implementation of the writer is restricted and required first the permission of the AMTI company.
   *
   * @ingroup BTKIO
   *
   * @todo Still need more checks when there are extra instruments, what about the instrument offset, coord, interdist and zero?
   */
  
  /**
   * @typedef BSFFileIO::Pointer
   * Smart pointer associated with a BSFFileIO object.
   */
  
  /**
   * @typedef BSFFileIO::ConstPointer
   * Smart pointer associated with a const BSFFileIO object.
   */
  
  /**
   * @fn static BSFFileIO::Pointer BSFFileIO::New()
   * Create a BSFFileIO object an return it as a smart pointer.
   */
  
  /**
   * Check if the content of the file starts with the value 100 which correspond to the latest known version of the BSF file format.
   */
  bool BSFFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    IEEELittleEndianBinaryFileStream bifs(filename, BinaryFileStream::In);
    if (bifs.ReadI32() != 100)
      isReadable = false;
    bifs.Close();
    return isReadable;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void BSFFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      // Main header (SMnHeaderTag)
      if (bifs.ReadI32() != 100)
        throw BSFFileIOException("Invalid BSF file.");
      int32_t headerSize = bifs.ReadI32();
      int32_t numberOfActivePlatforms = bifs.ReadI32();
      int32_t numberOfActiveInstruments = bifs.ReadI32();
      std::string subjectName = btkTrimString(bifs.ReadString(100));
      std::string testDate = btkTrimString(bifs.ReadString(12));
      std::string subjectDateOfBirth = btkTrimString(bifs.ReadString(12));
      double weight = bifs.ReadDouble();
      double height = bifs.ReadDouble();
      std::string sex = btkTrimString(bifs.ReadString(1));
      bifs.SeekRead(3, BinaryFileStream::Current); // Because of "pragma pack(4)": minimum of 4 bytes in the alignment of a member.
      int32_t numberOfTrials = bifs.ReadI32();
      double totaleTimeTrial = bifs.ReadDouble(); // seconds
      int32_t zeroMethod = bifs.ReadI32();
      int32_t weightMethod = bifs.ReadI32();
      int32_t delayAfterKeystroke = bifs.ReadI32();
      int32_t triggerMethod = bifs.ReadI32();
      int32_t triggerPlatform = bifs.ReadI32();
      int32_t preTriggerValue = bifs.ReadI32();
      int32_t postTriggerValue = bifs.ReadI32();
      double triggerValue = bifs.ReadDouble();
      bifs.SeekRead(4, BinaryFileStream::Current); // FIXME: There is 4 extra bytes in the file used to test this reader! What are they?
      int32_t rate = bifs.ReadI32();
      std::string protocol = btkTrimString(bifs.ReadString(150));
      std::string testType = btkTrimString(bifs.ReadString(200));
      std::string commentFile = btkTrimString(bifs.ReadString(150));
      std::string trialDescriptionFile = btkTrimString(bifs.ReadString(150));
      std::string examinerName = btkTrimString(bifs.ReadString(100));
      bifs.SeekRead(2, BinaryFileStream::Current); // WARNING: Two (2) extra bytes in the file tested. Could be for alignment?
      int32_t units = bifs.ReadI32(); // 0: english, 1: metric
      
      if (rate == 0)
        throw BSFFileIOException("Invalid frame rate.");
      
      // Instrument headers (SInstrHeaderTag)
      std::vector<InstrumentHeader> instrumentHeaders(numberOfActivePlatforms + numberOfActiveInstruments);
      for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
        this->extractInstrumentHeader(&bifs, &(instrumentHeaders[i]));
      for (int i = 0 ; i < numberOfActiveInstruments ; ++i)
        this->extractInstrumentHeader(&bifs, &(instrumentHeaders[i+numberOfActivePlatforms]));
      
      // Internal check to verify the compatibility between BTK acquisition's format and data
      for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
      {
        if ((instrumentHeaders[i].rate != 0) && (instrumentHeaders[i].rate != rate))
          throw BSFFileIOException("Unsupported file: An instrument has a different rate. Contact the developers to improve this reader.");
      }
      for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
      {
        if (instrumentHeaders[i].numberOfChannels != 6)
          throw BSFFileIOException("Unsupported file: An force platform has more than 6 channels. Contact the developers to improve this reader.");
      }
      
      // Init
      int totalNumberOfChannels = 0;
      for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
        totalNumberOfChannels += instrumentHeaders[i].numberOfChannels;
      double* scale = new double[totalNumberOfChannels]; // array to transform ADC values to real values
      size_t inc = 0;
      for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
      {
        for (int j = 0 ; j < instrumentHeaders[i].numberOfChannels ; ++j)
          scale[j+inc] = 1000000.0 / (instrumentHeaders[i].sensitivity[j] * instrumentHeaders[i].amplifierGain[j] * instrumentHeaders[i].excitationVoltage[j] * instrumentHeaders[i].acquisitionCardRange[j]);
        inc += instrumentHeaders[i].numberOfChannels;
      }
      output->Init(0, static_cast<int>(totaleTimeTrial * static_cast<double>(rate)), totalNumberOfChannels);
      output->SetPointFrequency(static_cast<double>(rate));
      if (units == 0) // english
      {
        output->SetPointUnit(Point::Marker, "in");
        output->SetPointUnit(Point::Force, "lb");
        output->SetPointUnit(Point::Moment, "in-lb");
      }
      else
      {
        // The FP length/width seems to be set everytime in inch even if the header is set 
        // to metric units (so only used for measured value?).
        for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
        {
           // Convert from inch to meter
          instrumentHeaders[i].length *= 0.0254f;
          instrumentHeaders[i].width *= 0.0254f;
          instrumentHeaders[i].offset[0] *= 0.0254f;
          instrumentHeaders[i].offset[1] *= 0.0254f;
          instrumentHeaders[i].offset[2] *= 0.0254f;
        }
        // Same for the content of the measure. The BSF file format seems to save the data in pound and pound-inch
        for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
        {
          scale[i*6] *= 4.4482216152605;
          scale[i*6+1] *= 4.4482216152605;
          scale[i*6+2] *= 4.4482216152605;
          scale[i*6+3] *= 0.1129848290276167;
          scale[i*6+4] *= 0.1129848290276167;
          scale[i*6+5] *= 0.1129848290276167;
        }
        output->SetPointUnit(Point::Marker, "m");
        output->SetPointUnit(Point::Force, "N");
        output->SetPointUnit(Point::Moment, "Nm");
      }
      Acquisition::AnalogIterator it = output->BeginAnalog();
      std::vector<float> corners(12*numberOfActivePlatforms, 0.0f);
      std::vector<float> origin(3*numberOfActivePlatforms, 0.0f);
      std::vector<int16_t> channel(6*numberOfActivePlatforms);
      std::string suffix = ((numberOfActivePlatforms == 1) ? "" : "1");
      float globalOrigin[3] = {0.0f, 0.0f, 0.0f};
      int numToAdaptChannelIndex = 0;
      for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
      {
        (*it)->SetLabel("Fx" + suffix);
        (*it)->SetUnit(output->GetPointUnit(Point::Force));
        (*it)->SetScale(scale[i*6]);
        ++it;
        (*it)->SetLabel("Fy" + suffix);
        (*it)->SetUnit(output->GetPointUnit(Point::Force));
        (*it)->SetScale(scale[i*6+1]);
        ++it;
        (*it)->SetLabel("Fz" + suffix);
        (*it)->SetUnit(output->GetPointUnit(Point::Force));
        (*it)->SetScale(scale[i*6+2]);
        ++it;
        (*it)->SetLabel("Mx" + suffix);
        (*it)->SetUnit(output->GetPointUnit(Point::Moment));
        (*it)->SetScale(scale[i*6+3]);
        ++it;
        (*it)->SetLabel("My" + suffix);
        (*it)->SetUnit(output->GetPointUnit(Point::Moment));
        (*it)->SetScale(scale[i*6+4]);
        ++it;
        (*it)->SetLabel("Mz" + suffix);
        (*it)->SetUnit(output->GetPointUnit(Point::Moment));
        (*it)->SetScale(scale[i*6+5]);
        ++it;
        if (i > 0)
        {
          if ((instrumentHeaders[i].interDistance[0] == 0.0f) && (instrumentHeaders[i].interDistance[1] == 0.0f) && (instrumentHeaders[i].interDistance[2] == 0.0f))
          {
            btkErrorMacro("The distance with the previous force platform is set to 0. The platform is automatically shifted in the front of the previous.");
            instrumentHeaders[i].interDistance[1] = static_cast<float>(instrumentHeaders[i].length + instrumentHeaders[i-1].length) / 2.0f;
          }
        }
        globalOrigin[0] += instrumentHeaders[i].interDistance[0];
        globalOrigin[1] += instrumentHeaders[i].interDistance[1];
        globalOrigin[2] += instrumentHeaders[i].interDistance[2];
        this->extractConfiguration(&(instrumentHeaders[i]), globalOrigin, &(channel[i*6]), &(corners[i*12]), &(origin[i*3]));
        for (int j = 0 ; j < 6 ; ++j)
          channel[i*6+j] += numToAdaptChannelIndex;
        numToAdaptChannelIndex += instrumentHeaders[i].numberOfChannels;
        suffix = ToString(i+2);
      }
      // Create the metadata FORCE_PLATFORM
      btk::MetaData::Pointer forcePlatform = btk::MetaData::New("FORCE_PLATFORM");
      output->GetMetaData()->AppendChild(forcePlatform);
      // - FORCE_PLATFORM:USED
      forcePlatform->AppendChild(btk::MetaData::New("USED", static_cast<int16_t>(numberOfActivePlatforms)));
      // - FORCE_PLATFORM:TYPE
      forcePlatform->AppendChild(btk::MetaData::New("TYPE", std::vector<int16_t>(numberOfActivePlatforms,2)));
      // - FORCE_PLATFORM:ZERO
      std::vector<int16_t> zero(2,0); zero[0] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("ZERO", zero));
      // - FORCE_PLATFORM:CORNERS
      std::vector<uint8_t> dims(3); dims[0] = 3; dims[1] = 4; dims[2] = numberOfActivePlatforms;
      forcePlatform->AppendChild(btk::MetaData::New("CORNERS", dims, corners));
      // - FORCE_PLATFORM:ORIGIN
      dims.resize(2); dims[0] = 3; dims[1] = numberOfActivePlatforms;
      forcePlatform->AppendChild(btk::MetaData::New("ORIGIN", dims, origin));
      // - FORCE_PLATFORM:CHANNEL
      dims.resize(2); dims[0] = 6; dims[1] = numberOfActivePlatforms;
      forcePlatform->AppendChild(btk::MetaData::New("CHANNEL", dims, channel));
      // - FORCE_PLATFORM:CAL_MATRIX
      dims.resize(3); dims[0] = 6; dims[1] = 6; dims[2] = 0;
      forcePlatform->AppendChild(btk::MetaData::New("CAL_MATRIX", dims, std::vector<float>()));
      // Add a metadata to notify that the first frame was not set.
      MetaData::Pointer btkPointConfig = MetaDataCreateChild(output->GetMetaData(), "BTK_POINT_CONFIG");
      MetaDataCreateChild(btkPointConfig, "NO_FIRST_FRAME", static_cast<int8_t>(1));
      
      // Data
      // Note: We want the reaction of the measure, so all the data are multiplied by -1.
      for (int i = 0 ; i < output->GetAnalogFrameNumber() ; ++i)
      {
        int inc = 0;
        for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          (*it)->GetValues().coeffRef(i) = -1.0f * static_cast<double>(bifs.ReadI16()) * scale[inc++]; 
      }
      
      // Cleaning
      delete[] scale;
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
      throw(BSFFileIOException(excmsg));
    }
    catch (BSFFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close();
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(BSFFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(BSFFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  BSFFileIO::BSFFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Integer)
  {};
  
  void BSFFileIO::extractInstrumentHeader(IEEELittleEndianBinaryFileStream* bifs, BSFFileIO::InstrumentHeader* header) const
  {
    header->headerSize = bifs->ReadI32();
    header->serialNumber = bifs->ReadI32();
    header->layoutPlatformNumber = bifs->ReadI32();
    header->name = btkTrimString(bifs->ReadString(20));
    header->numberOfChannels = bifs->ReadI32();
    header->hardwareStartChannel = bifs->ReadI32();
    header->hardwareEndChannel = bifs->ReadI32();
    header->softwareStartChannel = bifs->ReadI32();
    header->softwareEndChannel = bifs->ReadI32();
    header->length = bifs->ReadFloat();
    header->width = bifs->ReadFloat();
    bifs->ReadFloat(3, header->offset);
    bifs->ReadFloat(maximumChannelNumber, header->sensitivity);
    for (int i = 0 ; i < maximumChannelNumber ; ++i)
    {
      header->channels[i] = bifs->ReadI16();
      bifs->SeekRead(2, BinaryFileStream::Current);
    }
    bifs->ReadFloat(16, header->transformation);
    bifs->ReadFloat(3, header->interDistance);
    bifs->ReadFloat(maximumChannelNumber, header->amplifierGain);
    bifs->ReadFloat(maximumChannelNumber, header->excitationVoltage);
    bifs->ReadFloat(maximumChannelNumber, header->acquisitionCardRange);
    header->zeroPeriod = bifs->ReadFloat();
    header->latencyPeriod = bifs->ReadFloat();
    header->triggerTime = bifs->ReadFloat();
    header->endTime = bifs->ReadFloat();
    header->postTrigTime = bifs->ReadFloat();
    bifs->ReadI32(maximumChannelNumber, header->zero);
    header->rate = bifs->ReadI32();
    header->triggerValue = bifs->ReadFloat();
    header->endValue = bifs->ReadFloat();
    /*
    std::cout << "Length x Width: " << ToString(header->length) << " x " << ToString(header->width) << "\n";
    std::cout << "Offset: ";
    for (size_t i = 0 ; i < 3 ; ++i)
      std::cout << ToString(header->offset[i]) << ", ";
    std::cout << "\n";
    std::cout << "Interdistance: ";
    for (size_t i = 0 ; i < 3 ; ++i)
      std::cout << ToString(header->interDistance[i]) << ", ";
    std::cout << "\n";
    std::cout << "Transformation: ";
    for (size_t i = 0 ; i < 16 ; ++i)
      std::cout << ToString(header->transformation[i]) << ", ";
    std::cout << std::endl;
    */
  };
  
  void BSFFileIO::extractConfiguration(const InstrumentHeader* header, const float* go, int16_t* ci, float* c, float* o) const
  {
    // Channel index
    for (int i = 0 ; i < 6 ; ++i)
      ci[i] = header->channels[i]+1; // This is a 1-based index
    // Corners expressed in the global frame.
    // The global frame is set here as: axis X going forward, axis Y on the left and axis Z going upward.
    // The corners are set to have the corner #1 on the bottom left side, #2 on the top left side, #3 on the top right side and #4 on the bottom right side.
    const float cx = header->width / 2.0f;
    const float cy = header->length / 2.0f;
    c[0] =    cx + go[0];
    c[1] =   -cy + go[1];
    c[2] =  0.0f + go[2];
    c[3] =   -cx + go[0];
    c[4] =   -cy + go[1];
    c[5] =  0.0f + go[2];
    c[6] =   -cx + go[0];
    c[7] =    cy + go[1];
    c[8] =  0.0f + go[2];
    c[9] =    cx + go[0];
    c[10] =   cy + go[1];
    c[11] = 0.0f + go[2];
    // - Origin (expressed in the global frame) and centered above the origin of the global frame
    o[0] = header->offset[0];
    o[1] = header->offset[1];
    o[2] = header->offset[2];
  };
};
