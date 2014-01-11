/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/MDF/FileFormatMDF.h"
#include "Open3DMotion/MotionFile/Formats/MDF/MDFDescriptor.h"
#include "Open3DMotion/MotionFile/Formats/MDF/MDFVarTypes.h"
#include "Open3DMotion/MotionFile/Formats/MDF/FileFormatOptionsMDF.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/Biomechanics/Trial/Gait/GaitEvents.h"
#include "Open3DMotion/MotionFile/Formats/MDF/ForcePlateMDF.h"

#include <sstream>
#include <memory>
#include <string.h>

namespace Open3DMotion
{


	class HeaderEntry
  {
  public:
    // empty (invalid) entry
    HeaderEntry() :
      DataType(0xFFFF),
      ElementSize(0),
      Channels(0)
    {
    }

    // construct from file
    HeaderEntry(std::istream& is);

    // construct as specified
    HeaderEntry(enum MDF_VAR_TYPE t, 
                size_t dim, 
                size_t elsize,
                size_t num_channels) :
      DataType(t),
      Dimensions(dim),
      ElementSize(elsize),
      Channels(num_channels)
    {
    }

		// write to file
    void Write(std::ostream& os);

  public:
    size_t DataType;
    size_t Dimensions;
    size_t ElementSize;
    size_t Channels;
  };

  //-- Old scaling constant of Force
  //   (assumed when reading old files)
  const static double MDF_OldForceScalingConst(0.05);

  //-- Old scaling constant of EMG data (datafile-to-uVolts):
  //   (assumed when reading old files)
  const static double MDF_OldEMGScalingConst(1.84);

	//-- Standard offset in time from one marker to the next in MDF files
	//   (Comes from the hardware spec for mpx30 and CX1 systems)
	const static double MDF_PerMarkerTimeOffset = 0.000172;

  class MDF_CalculatedGroup_class
  {
  public:
    const char* group;
    const char* units;
    const char* numberunitsformat;
    const char* numberformat;
    float fileresolution;
  } 
  MDF_CalculatedGroup [NumDataTypes-DataMarker] =
  {
    { "Marker Position",        "mm",    "%.1fmm",          "%.2f", (float)0.10  },
    { "Force",                  "N",     "%.1fN",           "%.2f", (float)0.10  },
    { "Force Vector Position",  "mm",    "%.1fmm",          "%.1f", (float)0.10  },
//  { "EMG",                    "uV",    "%.1fuV",          "%.1f", (float)0.10  },
    { "ADC/EMG",                "mV",    "%.1fmV",          "%.1f", (float)0.10  },
    { "Marker Velocity",        "m/s",   "%.3fm/s",         "%.4f", (float)0.005 },
    { "Marker Acceleration",    "m/s�",  "%.3fm/s�",        "%.3f", (float)0.05  },
    { "Joint Angle",            "deg",   "%.1f degrees",    "%.1f", (float)0.10  },
    { "Joint Angular Vel.",     "rad/s", "%.3f radians/s",  "%.3f", (float)0.001 },
    { "Joint Moment",           "Nm",    "%.2fNm",          "%.2f", (float)0.01  },
    { "Joint Power",            "W",     "%.1fW",           "%.1f", (float)0.10  },
    { "Marker Separation",      "mm",    "%.1fmm",          "%.1f", (float)0.10  },
    { "Vector Angle",           "deg",   "%.2f degrees",    "%.2f", (float)0.10  },
    { "VectAnglular Vel.",      "rad/s", "%.3f radians/s",  "%.3f", (float)0.001 },
    { "VectAnglular Acc.",      "rad/s�","%.3f radians/s�", "%.3f", (float)0.001 },
    { "2D Joint Moment",        "Nm",    "%.2fNm",          "%.2f", (float)0.01  },
    { "2D Joint Power",         "W",     "%.1fW",           "%.1f", (float)0.10  },
    { "Segment Ref.Point",      "mm",    "%.1fmm",          "%.1f", (float)0.10  },
    { "Segment Rotation",       "deg",   "%.1f degrees",    "%.3f", (float)0.01  },
    { "Joint Centre Vel.",      "m/s",   "%.3fm/s",         "%.4f", (float)0.005 },
    { "Joint Centre Acc.",      "m/s�",  "%.3fm/s�",        "%.3f", (float)0.05  },
    { "Segment EVB.x ",         " ",     "%.4f",            "%.6f", (float)0.00005 },
    { "Segment EVB.y ",         " ",     "%.4f",            "%.6f", (float)0.00005 },
    { "Segment EVB.z ",         " ",     "%.4f",            "%.6f", (float)0.00005 },
    { "Segment Angular Vel.",   "rad/s", "%.3f radians/s",  "%.3f", (float)0.001 },
    { "Segment Angular Acc.",   "rad/s�","%.3f radians/s�", "%.3f", (float)0.001 },
    { "3D Joint Rot.Vel.",      "rad/s", "%.3f radians/s",  "%.3f", (float)0.001 },
    { "3D Joint Moment",        "Nm/kg", "%.3fNm/kg",       "%.3f", (float)0.001 },
    { "3D Joint Power Sum",     "W/kg",  "%.3fW/kg",        "%.3f", (float)0.001 },
    { "3D Joint Power",         "W/kg",  "%.3fW/kg",        "%.3f", (float)0.001 },
    { "Analogue Channel",       " ",     "%.2f",            "%.2f", (float)1.0   },
    { "Digital Channel",        " ",     "%.0f",            "%.0f", (float)1.0   },
    { "Virtual Marker Pos.",    "mm",    "%.1fmm",          "%.2f", (float)0.10  },
    { "Virtual Marker Vel.",    "m/s",   "%.3fm/s",         "%.4f", (float)0.005 },
    { "Virtual Marker Acc.",    "m/s�",  "%.3fm/s�",        "%.3f", (float)0.05  },
    { "Rigid Body Position",    "mm",    "%.1fmm",          "%.1f", (float)0.10  },
    { "Rigid Body Velocity",    "m/s",   "%.3fm/s",         "%.4f", (float)0.005 },
    { "Rigid Body Rotation",    "deg",   "%.2f degrees",    "%.3f", (float)0.001 },
    { "Rigid Body EVB.x",       " ",     "%.4f",            "%.6f", (float)0.00005 },
    { "Rigid Body EVB.y",       " ",     "%.4f",            "%.6f", (float)0.00005 },
    { "Rigid Body EVB.z",       " ",     "%.4f",            "%.6f", (float)0.00005 },
    { "Marker |Vel|",           "m/s",   "%.3fm/s",         "%.3f", (float)0.005 },
    { "Marker |Acc|",           "m/s�",  "%.3fm/s�",        "%.3f", (float)0.005 },
    { "Marker InView",          " ",     "%.0f",            "%.0f", (float)1.0   }
  };

	MDFDescriptor* FileFormatOptionsMDF::NewDescriptor() const
	{
		if (FormatVersion == FileFormatOptionsMDF::VERSION2)
		{
			return new MDFDescriptorVersion2;
		}
		else
		{
			return new MDFDescriptorVersion3;
		}
	}

  HeaderEntry::HeaderEntry(std::istream& is)
  {
    UInt16 wType(0xFFFF), wChannels(0);

    // read values
    is.read((char*)&wType,2);
    is.read((char*)&wChannels,2);

    // extract type
    DataType = wType & 0xFF;

    // extract channel count
    Channels = wChannels;

    // extract dimensionality
    Dimensions = (wType & 0xF000) >> 12;

    // extract type size
    if (DataType == VAR_VIDEO_AVI)
    {
      // video is a special case
      // - needs a big element size to cope with 
      //   larger amounts of data
      ElementSize = 256;
    }
    else if (DataType == VAR_FORCE_PLATE_TYPE)
    {
      // these strings use a fixed number of characters
      // as the element size
      ElementSize = 32;
    }
    else
    {
      // extract info
      ElementSize = (wType & 0x0F00) >> 8;
    }
  }

  void HeaderEntry::Write(std::ostream& os)
  {
    UInt16 wType = (UInt16)
      (((UInt16)Dimensions & 0xF) << 12) | 
      (((UInt16)ElementSize & 0xF) << 8) |
       ((UInt16)DataType & 0xFF);
    UInt16 wChannels = (UInt16)Channels;
    os.write((const char*)&wType, 2);
    os.write((const char*)&wChannels,2);
  }


	void DecodeMDFString(std::string& str, const std::vector<UInt8>& data_element)
	{
		// total bytes
		size_t size = data_element.size();
	
		// leave str empty if no data present
		if (size > 0)
		{
			// discard last byte if it's a null terminator
			if (data_element[size-1] == '\0')
			{
				--size;
			}

			// store if still any elements available
			if (size > 0)
			{
				str = std::string((const char*) &data_element[0], size);
			}
		}		
	}

	void EncodeMDFString(std::ostream& os, const std::string& str, bool writeCount)
	{
    UInt16 wElements = (UInt16)str.size()+1;
		if (writeCount)
      os.write((const char*)&wElements,2);
		os.write(str.c_str(), (std::streamsize)wElements);
	}

	// check id and number type
  bool FileFormatMDF::Probe(const MotionFileHandler& /*context*/, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException)
	{
		FileFormatOptionsMDF mdf_options;

    const char id_text[] = "CODAmotion Analysis Text";
    
    // read potential file id
    char id[sizeof(id_text)];
    is.read(id, sizeof(id_text));

    // don't support text files here
    // - that's done in FileFormatMDFText
    if (strncmp(id, id_text,sizeof(id_text)) == 0)
      return false;
    
    // but if it says 'CODA' then that's enough
    if (strncmp(id, id_text, 4) != 0)
      return false;

    // read version number
    UInt16 version(0);
    is.seekg(4, std::ios::beg);
    is.read((char*)&version,2);

		// store it
		mdf_options.FormatVersion = (version & 0x00FFUL);
    
		// convert to tree object
		readoptions = mdf_options.ToTree();
		
		// done ok
		return true;
	}

  // Read MDF
  TreeValue* FileFormatMDF::Read(const MotionFileHandler& /*context*/, std::istream& is, const BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) 
	{
		// get options
		FileFormatOptionsMDF mdf_options;
		mdf_options.FromTree(readoptions);

		// MDF descriptor item
		std::auto_ptr<MDFDescriptor> mdf_descriptor( mdf_options.NewDescriptor() );

		// new trial object
		std::auto_ptr<TrialMDF> trial( new TrialMDF );

    // Don't put manufacturer info in
    // - if external c3d is converted to mdf and back
    //   then could end up with false information

    // MDF acquisition info
		trial->Acq.MeasurementSystem.Name = "File Import";
		trial->Acq.MeasurementSystem.SoftwareName = "MDF";
		trial->Acq.MeasurementSystem.SoftwareVersion = mdf_descriptor->CreationSoftwareVersion();

		// Do not set company name: 
		// There is nothing to prevent an MDF file being an imported file from another system

    // skip file id
    is.seekg(6, std::ios::beg);

    // read number of entries
    UInt16 w(0);
    is.read((char*)&w,2);
    size_t num_entries(w);

    // read header entries
    std::vector<HeaderEntry> header(num_entries);
    size_t i;
    for (i = 0; i < num_entries; i++)
      header[i] = HeaderEntry(is);

    // read all entries into map
    // - map is indexed by data type id
    std::map<size_t, std::vector< std::vector<UInt8> >, std::less<size_t> > data;
    std::map<size_t, size_t, std::less<size_t> > elementsize;
    for (i = 0; i < num_entries; i++)
    {
      elementsize[header[i].DataType] = header[i].ElementSize;
      data[header[i].DataType].resize(header[i].Channels);
      for (size_t j = 0; j < header[i].Channels; j++)
      {
        // find num elements in array
        UInt16 wElements(0);
        is.read((char*)&wElements,2);

        // allocate array
        size_t arraysize = (size_t)wElements * header[i].ElementSize;
        data[header[i].DataType][j].resize(arraysize);

        // read data
        is.read((char*)&(data[header[i].DataType][j][0]), arraysize);
      }
    }

#if 0
    const char* varName[] = {
      "VAR_TEXT_COMMENT",           // User comments.  Only one comment is allowed per file
      "VAR_DATE_CREATE",            // Date of creation.  Only one per file.
      "VAR_POSITION_MARKER",        // Marker position
      "VAR_ANALOGUE_FORCE",         // Force data
      "VAR_ANALOGUE_EMG",           // EMG data
      "VAR_FLAG_IN_VIEW",           // Each bit corresponds to a marker epoch and is TRUE if marker is in view
      "VAR_FLAG_EVENT",             // Logic data
      "VAR_TIME_RESOLUTION_MARKER", // Time resolution of one marker epoch (in 1 / 100000 seconds)
      "VAR_TIME_RESOLUTION_FORCE",  
      "VAR_TIME_RESOLUTION_EMG",
      "VAR_TIME_RESOLUTION_EVENT",
      "VAR_TIME_RESOLUTION_CURSOR",
      "VAR_TIME_CURSOR",
      "VAR_POSITION_RESOLUTION_MARKER", // Position resolution of one interger of marker position (in 1/1000 mm)
      "VAR_MARKER_NUMBER_HARDWARE",     // The hardware marker identifiers
      "VAR_TIME_ZOOM",
      "VAR_TIME_RESOLUTION_ZOOM",
      "VAR_GAIT_CYCLES",
      "VAR_FORCE_RESOLUTION",
      "VAR_EMG_RESOLUTION", 
      "VAR_FORCE_PLATE_CONSTANTS",
      "VAR_FORCE_PLATE_POSITION",
      "VAR_MARKER_NAMES",
      "VAR_ANALOGUE_FORCE_NAMES",
      "VAR_ANALOGUE_EMG_NAMES",
      "VAR_PATIENT_ID",
      "VAR_PATIENT_CLASSIFICATION",
      "VAR_PATIENT_DATA",
      "VAR_PATIENT_SEGMENT_DATA",
      "VAR_PATIENT_DATA_NAMES",
      "VAR_VIDEO_AVI",		           // VIDEO
      "VAR_FORCE_PLATE_TYPE",
      "VAR_FORCE_PLATE_FLAGS",
      "VAR_FORCE_PLATE_COP_COEFFS",
      "VAR_ADCEMG_CHANNELNUM",
      "VAR_MARKER_INTENSITY",
      "VAR_FORCE_SEGMENT"
    };
    ofstream os_file("dbg_mdf_data.txt");
    for (i = 0; i < num_entries; i++)
    {
      DWORD idt = header[i].DataType;
      os_file << "Datatype " << idt;
      if (idt < 37)
        os_file << ": " << varName[idt];
      os_file << endl;
      os_file << "  channels: " << (DWORD)data[idt].size() << endl;
      for (DWORD j = 0; j < (DWORD)data[idt].size(); j++)
      {
        os_file << "    channel[" << j << "] bytes: " << (DWORD)data[idt][j].size() << endl;
      }
      os_file << endl;
    }
#endif

    //-- transfer from data arrays ---------------------------

    //-- marker data
    size_t num_markers = data[VAR_POSITION_MARKER].size();
    if (num_markers && (elementsize[VAR_POSITION_MARKER] != mdf_descriptor->MarkerElementSize()))
      throw(MotionFileException(MotionFileException::formaterror, "Unexpected element size for markers"));
    TimeRange timerange_marker0;
    for (i = 0; i < num_markers; i++)
    {
      // need time range
      if (i >= data[VAR_TIME_RESOLUTION_MARKER].size() ||
          data[VAR_TIME_RESOLUTION_MARKER][i].size() != 2)
        throw(MotionFileException(MotionFileException::formaterror, "Unknown frame rate for marker"));

      // time resolution
      UInt16 timeres = *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_MARKER][i][0]);

      // scale factor
      float scale(1.0);
      if (i < data[VAR_POSITION_RESOLUTION_MARKER].size())
        scale = (float)1E-3 * *reinterpret_cast<Int16*>(&data[VAR_POSITION_RESOLUTION_MARKER][i][0]);

      // find name
      std::string name;
      if (i < data[VAR_MARKER_NAMES].size())
      {
				// get name from file
        DecodeMDFString(name, data[VAR_MARKER_NAMES][i]);

				// HACK: enforce unique names by renaming if an existing sequence with this name exists
				const TimeSequence* tsExisting = trial->Acq.GetTS(TrialSectionAcq::TSGroupMarker, name.c_str());
				if (tsExisting)
					name.clear();	// clear name to force auto-generation of new name
			}
      
			// generate name from marker number if none exists
			if (name.empty())
      {
        std::ostringstream sname;
        sname << "Marker" << (i+1) << std::ends;
        name = sname.str();
			}

      // find hardware number (note it is zero-based in MDF files)
      Int32 hardware(i);
      if (i < data[VAR_MARKER_NUMBER_HARDWARE].size())
        hardware = *reinterpret_cast<UInt16*>(&data[VAR_MARKER_NUMBER_HARDWARE][i][0]);

			// HACK: only support 99 unique marker ID's - after that assume multi-coda mode
			hardware = hardware % 100;

			// sequence and attributes (note hardware number is 1-based in attributes)
      size_t num_frames = data[VAR_POSITION_MARKER][i].size() / mdf_descriptor->MarkerElementSize();
      // size_t inview_size = elementsize[VAR_FLAG_IN_VIEW];
      // size_t num_inview = data[VAR_FLAG_IN_VIEW][i].size();

			// build time sequence
      TimeRange tr;
			tr.Rate = (double)timeres;
			tr.Start = MDF_PerMarkerTimeOffset*hardware;
			tr.Frames = num_frames;
			TimeSequence* ts = TSFactoryOccValue(3).New(tr, memfactory);

			// set attributes
			ts->Units = "mm";
			ts->Group = TrialSectionAcq::TSGroupMarker;
			ts->Channel = name;
			ts->HardwareID = hardware + 1;

      mdf_descriptor->TransferMarker(*ts, 
        &data[VAR_POSITION_MARKER][i][0],
        &data[VAR_FLAG_IN_VIEW][i][0],
        scale);

			// store
			trial->Acq.TimeSequences.Add(ts);

      // store timerange - needed for calc data later
			if (i == 0)
				timerange_marker0 = tr;
    }

		//-- EMG
    size_t num_emg = data[VAR_ANALOGUE_EMG].size();
    if (num_emg && (elementsize[VAR_ANALOGUE_EMG] != 2))
      throw(MotionFileException(MotionFileException::formaterror,"Unexpected element size for EMG data"));
    for (i = 0; i < num_emg; i++)
    {
      // time resolution
      UInt16 timeres(0);
      if (i < data[VAR_TIME_RESOLUTION_EMG].size())
        timeres= *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_EMG][i][0]);
      else if (data[VAR_TIME_RESOLUTION_MARKER].size())
        timeres = *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_MARKER][0][0]);
      else
        throw(MotionFileException(MotionFileException::formaterror, "Unknown frame rate for force"));

      // position resolution
      double scale(0.0);
      if (i < data[VAR_EMG_RESOLUTION].size())
        scale = *reinterpret_cast<float*>(&data[VAR_EMG_RESOLUTION][i][0]);
      else
        scale = MDF_OldEMGScalingConst;

      // find name
      std::string name;
      if (i < data[VAR_ANALOGUE_EMG_NAMES].size())
      {
				DecodeMDFString(name, data[VAR_ANALOGUE_EMG_NAMES][i]);
      }
      else
      {
        std::ostringstream sname;
        sname << "EMG" << (i+1) << std::ends;
        name = sname.str();
      }

			// 1-based hardware ID - note that due to force and EMG being split up this may be wrong
			Int32 hardwareID = i + 1;

			// comupute frame count from size of data
      size_t num_frames = data[VAR_ANALOGUE_EMG][i].size() / 2;

			// make sequence
      TimeRange tr;
			tr.Rate = (double)timeres;
			tr.Start = 0.0;
			tr.Frames = num_frames;
			TimeSequence* ts = TSFactoryValue(1).New(tr, memfactory);
			ts->Group = TrialSectionAcq::TSGroupAnalog;
			ts->Channel = name;
			ts->Units = "mV";
			ts->Scale = scale;
			ts->Offset = 0.0;
      ts->HardwareID = hardwareID;
			
			// copy data (unscaled)
      Int16* a = reinterpret_cast<Int16*>(&data[VAR_ANALOGUE_EMG][i][0]);
			for (TSScalarIter iter_ts(*ts); iter_ts.HasFrame(); iter_ts.Next(), a++)
      {
				iter_ts.Value() = *a;
      }

			// attach
			trial->Acq.TimeSequences.Add(ts);
    }
		
		// get force plate count
		size_t num_plates = ForcePlateMDF::NumPlates(data, elementsize);			

		//-- force
    if (num_plates)
    {
			// get data from each plate
      for (size_t iplate = 0; iplate < num_plates; iplate++)
      {
				// This is derived from ForcePlateModel with necessary stuff to parse MDF info
				ForcePlateMDF fp_mdf;
				
				// read plate descriptor
				fp_mdf.ParseMDF(data, elementsize, iplate);

				// add force info for this plate onto tree
				trial->Acq.ForcePlates.Add(fp_mdf);

				// get analog data and calibration for this plate
        for (size_t ichannel = 0; ichannel < 8; ichannel++)
        {
          // MDF uses 8 channels per plate, even for 6-channel AMTI
					// For AMTI plates, the last two channels are computed CoP

					// file index for this channel
					size_t mdf_plate_channel = fp_mdf.RuntimeChannelToMDFChannel(ichannel);
          size_t index_file_force = 8*iplate + mdf_plate_channel;
          
					// channel time resolution
          UInt16 timeres(0);
          if (data[VAR_TIME_RESOLUTION_FORCE].size())
            timeres= *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_FORCE][0][0]);
          else if (data[VAR_TIME_RESOLUTION_MARKER].size())
            timeres = *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_MARKER][0][0]);
          else
            throw(MotionFileException(MotionFileException::formaterror, "Unknown frame rate for force"));

          // channel position resolution
          double scale(0.0);
          if (index_file_force < data[VAR_FORCE_RESOLUTION].size())
            scale = *reinterpret_cast<float*>(&data[VAR_FORCE_RESOLUTION][index_file_force][0]);
          else
            scale = MDF_OldForceScalingConst;

          // for moments in AMTI plates, scale Nm -> Nmm and negate Fx, Fy
          scale *= fp_mdf.MDFChannelScale(mdf_plate_channel);

					// find name
          std::string name;
          if (index_file_force < data[VAR_ANALOGUE_FORCE_NAMES].size())
          {
            DecodeMDFString(name, data[VAR_ANALOGUE_FORCE_NAMES][index_file_force]);
          }
          else
          {
            // TODO: construct name according to force plate type
            //       (Fx, Mx, etc)
            std::ostringstream sname;
            sname << "Force" << (index_file_force+1) << std::ends;
						name = sname.str();
          }

					// hardware id
					size_t hardwareID = num_emg + 8*iplate + ichannel + 1;

					// total frames
          size_t num_frames = data[VAR_ANALOGUE_FORCE][index_file_force].size() / 2;

					// make time range object
					TimeRange t;
					t.Rate = (double)timeres;
					t.Start = 0;
					t.Frames = num_frames;

					// make sequence and set attributes
					TimeSequence* ts = TSFactoryValue(1).New(t, memfactory);
					ts->Group = TrialSectionAcq::TSGroupAnalog;
					ts->Channel = name;
					ts->HardwareID = hardwareID;
					ts->Units = "ADC";
					ts->Scale = scale;
					ts->Offset = 0.0;
					ts->ScaleUnits = fp_mdf.MDFChannelUnits(mdf_plate_channel);
					
					// copy data
					Int16* a = reinterpret_cast<Int16*>(&data[VAR_ANALOGUE_FORCE][index_file_force][0]);
					for (TSScalarIter iter_ts(*ts); iter_ts.HasFrame(); iter_ts.Next(), a++)
          {
            iter_ts.Value() = *a;
          }

					// add to acq section
					trial->Acq.TimeSequences.Add(ts);
        }
      }
    }

#if 0
		// get segments
		DWORD fp_segments = ForceSegmentMDF::NumSegments(data, elementsize);

		// set segment info
		for (DWORD iseg = 0; iseg < fp_segments; iseg++)
    {
				// read segment descriptor
				ForceSegmentMDF fp_segment_mdf(data, elementsize, iseg);

				// write new segment in tree
				fp_segment_mdf.write_branch(contents, iseg);
		}
#endif

		// date of creation
		if (data[VAR_DATE_CREATE].size())
		{			
			if (data[VAR_DATE_CREATE][0].size() == 6)
			{
				const Int16* date = reinterpret_cast<Int16*>(&data[VAR_DATE_CREATE][0][0]);
				trial->Acq.Date.Day = (Int32)date[0];
				trial->Acq.Date.Month = (Int32)date[1];
				trial->Acq.Date.Year = (Int32)date[2];
			}
		}

    // comments
    if (data[VAR_TEXT_COMMENT].size())
    {
			DecodeMDFString(trial->UserInput.Comment.Value(), data[VAR_TEXT_COMMENT][0]);
    }

    // patient id
    if (data[VAR_PATIENT_ID].size())
    {
			DecodeMDFString(trial->UserInput.Subject.ID.Value(), data[VAR_PATIENT_ID][0]);
    }

    // classification
    if (data[VAR_PATIENT_CLASSIFICATION].size())
    {
			DecodeMDFString(trial->UserInput.Subject.Classification.Value(), data[VAR_PATIENT_CLASSIFICATION][0]);
    }

    // other variables
    size_t num_subject_data = data[VAR_PATIENT_DATA].size();
		double subjectWeight = -1.0;
    if (num_subject_data >= 5)
    {
			// decode gender
      trial->UserInput.Subject.Gender = *reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][0][0]) ? "male" : "female";
      
			// decode date
      // TODO: compute full year from age and acquisition date
			Int32 dob = *reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][1][0]);
      Int32 dob_month = dob / 100;
      Int32 dob_year = 1900 + dob - (100*dob_month);
			trial->UserInput.Subject.DateOfBirth.Month = dob_month;
			trial->UserInput.Subject.DateOfBirth.Year = dob_year;
      
			// other params
			trial->UserInput.Subject.Age = (Int32)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][2][0]);
			trial->UserInput.Subject.Weight = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][3][0]);
      trial->UserInput.Subject.Height = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][4][0]);
    }

    // segment parameters
    if(num_subject_data >= 11 )
    {
			trial->UserInput.Subject.PelvicWidth = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][5][0]);
      trial->UserInput.Subject.PelvicDepth = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][6][0]);
      trial->UserInput.Subject.LKneeWidth = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][7][0]);
      trial->UserInput.Subject.RKneeWidth = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][8][0]);
      trial->UserInput.Subject.LAnkleWidth = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][9][0]);
      trial->UserInput.Subject.RAnkleWidth = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][10][0]);
    }

    // Hip model - here for historical purposes - 
    // should not be used for calculation
    if (num_subject_data >= 14)
    {
			trial->MDF.HipOffsetRatioX = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][11][0]);
			trial->MDF.HipOffsetRatioY = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][12][0]);
      trial->MDF.HipOffsetRatioZ = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][13][0]);
    }

    // Sacral offset
    if(num_subject_data >= 17)
    {
      trial->UserInput.Subject.SacralOffset = (double)*reinterpret_cast<Int16*>(&data[VAR_PATIENT_DATA][14][0]);
    }

		// Segment data
    size_t num_segment_data = data[VAR_PATIENT_SEGMENT_DATA].size();
		if (num_segment_data >= 15)
		{
			// segment lengths: for some reason these are in m not mm
			const float* thighLength = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][0][0]);
			const float* shankLength = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][1][0]);
			const float* footLength = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][2][0]);
			trial->UserInput.Subject.LThighLength = 1000.0 * (double)thighLength[0];
			trial->UserInput.Subject.LShankLength = 1000.0 * (double)shankLength[0];
			trial->UserInput.Subject.LFootLength = 1000.0 * (double)footLength[0];
			trial->UserInput.Subject.RThighLength = 1000.0 * (double)thighLength[1];
			trial->UserInput.Subject.RShankLength = 1000.0 * (double)shankLength[1];
			trial->UserInput.Subject.RFootLength = 1000.0 * (double)footLength[1];

			// need body mass for this next bit
			if (subjectWeight > 0.0)
			{
				// segment mass ratios (multiple of body mass)
				const float* thighMassRatio = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][3][0]);
				const float* shankMassRatio = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][4][0]);
				const float* footMassRatio = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][5][0]);
				trial->UserInput.Subject.LThighMass = thighMassRatio[0] * subjectWeight;
				trial->UserInput.Subject.LShankMass = shankMassRatio[0] * subjectWeight;
				trial->UserInput.Subject.LFootMass = footMassRatio[0] * subjectWeight;
				trial->UserInput.Subject.RThighMass = thighMassRatio[1] * subjectWeight;
				trial->UserInput.Subject.RShankMass = shankMassRatio[1] * subjectWeight;
				trial->UserInput.Subject.RFootMass = footMassRatio[1] * subjectWeight;

				// 6, 7, and 8 are offset to centre of mass
				// TODO: check this is not actually needed for inertial calcs
				//       i.e. that it cancels out
				const float* thighCR = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][6][0]);
				const float* shankCR = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][7][0]);
				const float* footCR = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][8][0]);
				trial->MDF.COM_LThigh = (double)thighCR[0];
		    trial->MDF.COM_LShank = (double)shankCR[0];
				trial->MDF.COM_LFoot = (double)footCR[0];
	      trial->MDF.COM_RThigh = (double)thighCR[1];
		    trial->MDF.COM_RShank = (double)shankCR[1];
			  trial->MDF.COM_RFoot = (double)footCR[1];

				// segment radii of gyration (X = Y), loaded in metres not mm
				const float* thighRGXY = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][9][0]);
				const float* shankRGXY = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][10][0]);
				const float* footRGXY = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][11][0]);
				trial->UserInput.Subject.RadGyr_LThigh_X = 1000.0 * (double)(thighRGXY[0] * thighLength[0]);
				trial->UserInput.Subject.RadGyr_LThigh_Y = 1000.0 * (double)(thighRGXY[0] * thighLength[0]);
				trial->UserInput.Subject.RadGyr_LShank_X = 1000.0 * (double)(shankRGXY[0] * shankLength[0]);
				trial->UserInput.Subject.RadGyr_LShank_Y = 1000.0 * (double)(shankRGXY[0] * shankLength[0]);
				trial->UserInput.Subject.RadGyr_LFoot_X = 1000.0 * (double)(footRGXY[0] * footLength[0]);
				trial->UserInput.Subject.RadGyr_LFoot_Y = 1000.0 * (double)(footRGXY[0] * footLength[0]);
				trial->UserInput.Subject.RadGyr_RThigh_X = 1000.0 * (double)(thighRGXY[1] * thighLength[1]);
				trial->UserInput.Subject.RadGyr_RThigh_Y = 1000.0 * (double)(thighRGXY[1] * thighLength[1]);
				trial->UserInput.Subject.RadGyr_RShank_X = 1000.0 * (double)(shankRGXY[1] * shankLength[1]);
				trial->UserInput.Subject.RadGyr_RShank_Y = 1000.0 * (double)(shankRGXY[1] * shankLength[1]);
				trial->UserInput.Subject.RadGyr_RFoot_X = 1000.0 * (double)(footRGXY[1] * footLength[1]);
				trial->UserInput.Subject.RadGyr_RFoot_Y = 1000.0 * (double)(footRGXY[1] * footLength[1]);

				// segment radii of gyration (Z), loaded in metres not mm
				const float* thighRGZ = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][12][0]);
				const float* shankRGZ = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][13][0]);
				const float* footRGZ = reinterpret_cast<const float*>(&data[VAR_PATIENT_SEGMENT_DATA][14][0]);
				trial->UserInput.Subject.RadGyr_LThigh_Z = 1000.0 * (double)(thighRGZ[0] * thighRGXY[0] * thighLength[0]);
				trial->UserInput.Subject.RadGyr_LShank_Z = 1000.0 * (double)(shankRGZ[0] * shankRGXY[0] * shankLength[0]);
				trial->UserInput.Subject.RadGyr_LFoot_Z = 1000.0 * (double)(footRGZ[0] * footRGXY[0] * footLength[0]);
				trial->UserInput.Subject.RadGyr_RThigh_Z = 1000.0 * (double)(thighRGZ[1] * thighRGXY[1] * thighLength[1]);
				trial->UserInput.Subject.RadGyr_RShank_Z = 1000.0 * (double)(shankRGZ[1] * shankRGXY[1] * shankLength[1]);
				trial->UserInput.Subject.RadGyr_RFoot_Z = 1000.0 * (double)(footRGZ[1] * footRGXY[1] * footLength[1]);
			}
		}

		// bars includes cursors and marks
		size_t num_bars = data[VAR_TIME_CURSOR].size();

    // cursors only
		if (num_bars && 
				data[VAR_TIME_RESOLUTION_CURSOR].size() && 
				data[VAR_TIME_CURSOR][0].size() == 4)
		{
			// curors - get values
			UInt16 timeres = *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_CURSOR][0][0]);
			Int16* ptr_time = reinterpret_cast<Int16*>(&data[VAR_TIME_CURSOR][0][0]);
			double t0 = (double)ptr_time[0] / (double)timeres;
			double t1 = (double)ptr_time[1] / (double)timeres;

			// new event group for MDF cursors
			EventArray cursor_times;
			cursor_times.AddEvent(t0, "Start");
			cursor_times.AddEvent(t1, "End");
      EventGroup* cursor_group = new EventGroup;
			cursor_group->Name = "MDFCursor";
			cursor_group->SetEvents(cursor_times, memfactory);
			trial->UserInput.EventGroups.Add(cursor_group);
		}

		// marks
		if (num_bars > 1)
		{
			// new array for mark times
			EventArray mark_times;
			mark_times.SetIDName(1, "Start");
			mark_times.SetIDName(2, "End");
			
			// loop through all
			for (size_t ibar = 1; ibar < num_bars; ibar++)
			{
				// check ranges
				if (i >= data[VAR_TIME_RESOLUTION_CURSOR].size() ||
						data[VAR_TIME_CURSOR][ibar].size() != 4UL)
						continue;

				// get values
				UInt16 timeres = *reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_CURSOR][ibar][0]);
				Int16* ptr_time = reinterpret_cast<Int16*>(&data[VAR_TIME_CURSOR][ibar][0]);
				double t0 = (double)ptr_time[0] / (double)timeres;
				double t1 = (double)ptr_time[1] / (double)timeres;

				// add events to list
				mark_times.AddEvent(t0, 1);
				mark_times.AddEvent(t1, 2);
			}

			// add
			EventGroup* mark_group = new EventGroup;
			mark_group->Name = "MDFMark";
			mark_group->SetEvents(mark_times, memfactory);
			trial->UserInput.EventGroups.Add(mark_group);
		}

    // Gait cycles
    size_t num_gait_cycles = data[VAR_GAIT_CYCLES].size();
    if (num_gait_cycles && ((num_gait_cycles % 2) == 0) &&
        data[VAR_TIME_RESOLUTION_CURSOR].size())
    {
			// arrays of events
			EventArray all_events;
			EventArray representative_events;

      // time scale factor
      double tscale = (double)*reinterpret_cast<UInt16*>(&data[VAR_TIME_RESOLUTION_CURSOR][0][0]);

      for (i = 0; i < num_gait_cycles; i++)
      {
        if (data[VAR_GAIT_CYCLES][i].size() < 6)
          continue;
        
				// time-order the cycle events
				double hd0 = *reinterpret_cast<Int16*>(&data[VAR_GAIT_CYCLES][i][0]) / tscale;
        double to  = *reinterpret_cast<Int16*>(&data[VAR_GAIT_CYCLES][i][2]) / tscale;
        double hd1 = *reinterpret_cast<Int16*>(&data[VAR_GAIT_CYCLES][i][4]) / tscale;
        if (to >= 0.0 && hd0 > to)
          std::swap(hd0,to);
        if (hd1 >= 0.0 && to > hd1)
          std::swap(hd1,to);
        if (to >= 0.0 && hd0 > to)
          std::swap(hd0,to);

				// set in event group structures
				if (i % 2 == 0)
        {
					// add all defined events to ef_all
					if (hd0 >= 0.0)
						all_events.AddEvent(hd0, GaitEvents::LStart);
					if (to >= 0.0)
						all_events.AddEvent(to, GaitEvents::LToeOff);
					if (hd1 >= 0.0)
						all_events.AddEvent(hd1, GaitEvents::LEnd);

					// add rep cycle on first group only
					if (i == 0)
					{
						if (hd0 >= 0.0)
							representative_events.AddEvent(hd0, GaitEvents::LStart);
						if (to >= 0.0)
							representative_events.AddEvent(to, GaitEvents::LToeOff);
						if (hd1 >= 0.0)
							representative_events.AddEvent(hd1, GaitEvents::LEnd);
					}
        }
        else
        {
					// add all defined events to ef_all (right)
					if (hd0 >= 0.0)
	          all_events.AddEvent(hd0, GaitEvents::RStart);
					if (to >= 0.0)
	          all_events.AddEvent(to, GaitEvents::RToeOff);
					if (hd1 >= 0.0)
	          all_events.AddEvent(hd1, GaitEvents::REnd);

					// add rep cycle on first group only (right)
					if (i == 1)
					{
						if (hd0 >= 0.0)
							representative_events.AddEvent(hd0, GaitEvents::RStart);
						if (to >= 0.0)
							representative_events.AddEvent(to, GaitEvents::RToeOff);
						if (hd1 >= 0.0)
							representative_events.AddEvent(hd1, GaitEvents::REnd);
					}
				}
			}

			// add all events to trial
			EventGroup* gait_cycle_group = new EventGroup;
			gait_cycle_group->Name = GaitEvents::GroupName;
			gait_cycle_group->SetEvents(all_events, memfactory);
			trial->UserInput.EventGroups.Add(gait_cycle_group);

			// add representative group to trial
			EventGroup* representative_gait_cycle_group = new EventGroup;
			representative_gait_cycle_group->Name = GaitEvents::RepresentativeGroupName;
			representative_gait_cycle_group->SetEvents(representative_events, memfactory);
			trial->UserInput.EventGroups.Add(representative_gait_cycle_group);
    }

    //-- calculated data (MDR)

    size_t iCalcDataGroup(0);
    for (i = DataMarker; i < NumDataTypes; i++)
    {
      if (data[i].empty())
        continue;

      // deduce timerange based on all data sequences being 
      // the same length
      if (!num_markers)
        throw(MotionFileException(
        MotionFileException::formaterror,
        "No raw marker data present (required to deduce frame rate for calculated data)"));

			// group names are accessible directly
			std::string mdf_group_name;
			DecodeMDFString(mdf_group_name, data[DataGroupName][iCalcDataGroup]);

      // channel names are concatenated
      const std::vector<UInt8>& channelnames = data[DataChannelNames][iCalcDataGroup];

      // get scaling
      float scaling = *reinterpret_cast<float*>(&data[DataScaling][iCalcDataGroup][0]);

      // retrieve each channel
			std::string str_group = std::string("Calculated ") + mdf_group_name;
      const char* ch = (const char*)&channelnames[0];
      size_t ich(0);
      for (size_t j = 0; j < data[i].size(); j++)
      {
				// must have 2 bytes per element
				if (elementsize[i] > 0 && elementsize[i] % 2 != 0)
				{
					continue;
				}

				// extract name
        std::string str_name;
        while (*ch != '\t' && ich < channelnames.size())
        {
          str_name += *ch;
          ch++;
          ich++;
        }

        // point to next (for next iteration)
        ch++;
        ich++;

        // number of frames
        size_t num_frames = data[i][j].size() / elementsize[i];

        // find sample rate
        double rateHz = 
					timerange_marker0.Rate *
            (double) num_frames /
						(double)timerange_marker0.Frames;

        // time range for data
        TimeRange timerange;
				timerange.Rate = rateHz;
				timerange.Start = 0.0;
				timerange.Frames = num_frames;

				// units for data
				std::string str_units(MDF_CalculatedGroup[i-DataMarker].units);

				// find value dimension (number of dims per frame)
				size_t valuedim = elementsize[i] / 2;

				// copy
				TimeSequence* ts = TSFactoryValue(valuedim).New(timerange, memfactory);
				ts->Channel = str_name;
				ts->Group = str_group;
				ts->HardwareID = j + 1;
				ts->Units = str_units;

        const Int16* value = reinterpret_cast<const Int16*>(&data[i][j][0]);
				if (valuedim == 1)
				{
					// scalar data
					for (TSScalarIter iter_ts(*ts); iter_ts.HasFrame(); iter_ts.Next(), value++)
					{
						iter_ts.Value() = *value * scaling;
					}
				}
				else if (valuedim == 3)
				{
					// 3D data
					for (TSVector3Iter iter_ts(*ts); iter_ts.HasFrame(); iter_ts.Next(), value+=3)
					{
						iter_ts.Value()[0] = value[0] * scaling;
						iter_ts.Value()[1] = value[1] * scaling;
						iter_ts.Value()[2] = value[2] * scaling;
					}
				}
				else
				{
					delete ts;
					continue;
				}

				// got sequence ok

			}

      // count calculated data groups
      iCalcDataGroup++;
    }

		return trial->ToTree();
	}

  // Write MDF
  void FileFormatMDF::Write(const MotionFileHandler& /*context*/, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
	{
		// get write options
		FileFormatOptionsMDF mdf_options;
		mdf_options.FromTree( writeoptions );

		// create machine-specific writer
		std::auto_ptr<MDFDescriptor> descriptor( mdf_options.NewDescriptor() );

		// parse to trial structure
		TrialMDF trial;
		trial.FromTree(contents);

		// Write CODA id
    os.write("CODA", 4);
		UInt32 wVersion = mdf_options.FormatVersion;
    os.write((char*)&wVersion,2);

    // get details for all markers 
    std::vector<const TimeSequence*> marker;
		trial.Acq.GetTSGroup(marker, TrialSectionAcq::TSGroupMarker);

		// get all analog data
    std::vector<const TimeSequence*> analog;
		trial.Acq.GetTSGroup(analog, TrialSectionAcq::TSGroupAnalog);

		// find number of force plates
		size_t force_plates = trial.Acq.ForcePlates.NumElements();

		size_t i;

		// get detailed plate info
		const MapArrayCompound<ForcePlate>& allfp =  trial.Acq.ForcePlates;

		// TODO: remap as required for MDF save
		// RemapForceplate().Remap(calanalog, allfp, L"MDF");

		// use MDF utility class
		std::vector<ForcePlateMDF> fp_mdf(force_plates);
		for (i = 0; i < force_plates; i++)
		{
			fp_mdf[i] = ForcePlateMDF(allfp[i]);
		}

		// check that we know how to save the specified plates
		for (i = 0; i < force_plates; i++)
		{
			// this will throw ErrorMotionFile if not recognised plate type
			fp_mdf[i].MDFPlateID();
			
			// get orientation MDF-style
			ForcePlateMDF::Orientation orient = fp_mdf[i].MDFOrientation();

			// must be 0, 90, 180, 270 angles w.r.t. room coords
			if (orient == ForcePlateMDF::orient_invalid)
				throw MotionFileException(MotionFileException::notsupported, "arbitary forceplate orientation is unsupported");
			// TEMP: only actually allow 0 degree angle
			// else if (orient != ForcePlateMDF::orient000)
			// throw ErrorMotionFile(ErrorMotionFile::notsupported, "only zero-angle forceplate supported in this version");
		}

		// make array of all force channels in MDF order - a value of NODATA means missing data
		const size_t NODATA = 0xFFFFFFFFUL;
		std::vector<size_t> forceindex;
		for (i = 0; i < force_plates; i++)
		{
			// cannot have more than 8 channels
			if ((fp_mdf[i].Channels.NumElements() + fp_mdf[i].PreCompCoPChannels.NumElements()) > 8)
				throw MotionFileException(MotionFileException::notsupported, "force plates of more than 8 channels not supported in MDF");

			// get all channel ID's
			const MapArrayInt32& analogid = fp_mdf[i].Channels;
			const MapArrayInt32& copid = fp_mdf[i].PreCompCoPChannels;
			for (size_t j = 0; j < 8; j++)
			{
				// convert loop count (MDF index) to runtime channel index
				size_t runtime_channel = fp_mdf[i].MDFChannelToRuntimeChannel(j);

				// get hardware ID of required channel
				size_t hardwareID = NODATA;
				if (runtime_channel < analogid.NumElements())
					hardwareID = analogid[runtime_channel];
				else if ((runtime_channel-analogid.NumElements()) < copid.NumElements())
					hardwareID = copid[runtime_channel-analogid.NumElements()];

				// convert to analog array index if found (otherwise leave as NODATA)
				size_t index = NODATA;
				if (hardwareID != NODATA)
				{
					for (size_t itest = 0; itest < analog.size(); itest++)
					{
						if (analog[itest]->HardwareID.Value() == (Int32)hardwareID)
						{
							index = itest;
							break;
						}
					}

				}

				// add to list
				forceindex.push_back(index);
			}
		}

		// determine which analog channels are non-force ones
		std::vector<size_t> emgindex;
		for (i = 0; i < analog.size(); i++)
		{
			// assume EMG
			bool isemg = true;

			// search all force channels
			for (size_t j = 0; j < forceindex.size(); j++)
			{
				if (forceindex[j] == i)
				{
					isemg = false;
					break;
				}
			}

			// add non-force channels to array
			if (isemg)
				emgindex.push_back(i);
		}

    //-- header
    std::list<HeaderEntry> header;

		// creation date
		bool have_date_create = trial.Acq.Date.IsSet();

		// reference to subject info
		const TrialSubject& subject = trial.UserInput.Subject;

		// comments, subject data, etc.
		const std::string& comment = trial.UserInput.Comment;
		const std::string& subject_id = subject.ID;
		const std::string& subject_class = subject.Classification;

		// header fields - data must also be written in this order later
		if (comment.size())
      header.push_back(HeaderEntry(VAR_TEXT_COMMENT,1,1,1));
    if (have_date_create)
			header.push_back(HeaderEntry(VAR_DATE_CREATE, 3, 6, 1));
    if (subject_id.size())
      header.push_back(HeaderEntry(VAR_PATIENT_ID,1,1,1));
    if (subject_class.size())
      header.push_back(HeaderEntry(VAR_PATIENT_CLASSIFICATION,1,1,1));


		// subject data count
		bool subject_data_basic = subject.Gender.IsSet() || subject.DateOfBirth.IsSet() || subject.Age.IsSet() || subject.Height.IsSet() || subject.Weight.IsSet();
		bool subject_data_widths = subject.PelvicWidth.IsSet() || subject.PelvicDepth.IsSet() || subject.LKneeWidth.IsSet() || subject.RKneeWidth.IsSet() || subject.LAnkleWidth.IsSet() || subject.RAnkleWidth.IsSet();

		// TODO: get limb inertial params too

		// data count
		size_t subject_data_count(0);
		if (subject_data_widths)
			subject_data_count = 11;
		else if (subject_data_basic)
			subject_data_count = 5;

		// add headers for data
		if (subject_data_count)
      header.push_back(HeaderEntry(VAR_PATIENT_DATA,1,2,subject_data_count));

    // marker header
    size_t num_markers = marker.size();
    if (num_markers)
    {
      header.push_back(HeaderEntry(VAR_TIME_RESOLUTION_MARKER,1,2,num_markers));
      header.push_back(HeaderEntry(VAR_POSITION_RESOLUTION_MARKER,1,2,num_markers));
      header.push_back(HeaderEntry(VAR_MARKER_NAMES,1,1,num_markers));
      header.push_back(HeaderEntry(VAR_MARKER_NUMBER_HARDWARE,1,2,num_markers));
			header.push_back(HeaderEntry(VAR_POSITION_MARKER,3,descriptor->MarkerElementSize(),num_markers));
      header.push_back(HeaderEntry(VAR_FLAG_IN_VIEW,1,2,num_markers));
    }

    // all analog except force is classified as emg
		size_t num_emg = emgindex.size();

		// may need to insert this many extra channels as codaMA requires all data in sets of 8
		size_t emg_extra = 0;
		if (num_emg % 8 != 0)
		{
			emg_extra = 8 - (num_emg % 8);
			num_emg += emg_extra;
		}

		if (num_emg)
    {
      header.push_back(HeaderEntry(VAR_TIME_RESOLUTION_EMG,1,2,num_emg));
      header.push_back(HeaderEntry(VAR_EMG_RESOLUTION,1,4,num_emg));
      header.push_back(HeaderEntry(VAR_ANALOGUE_EMG_NAMES,1,1,num_emg));
      header.push_back(HeaderEntry(VAR_ANALOGUE_EMG,1,2,num_emg));
    }

    if (force_plates)
    {
      header.push_back(HeaderEntry(VAR_FORCE_PLATE_FLAGS,1,2,force_plates));
      header.push_back(HeaderEntry(VAR_FORCE_PLATE_TYPE,1,32,force_plates));
      header.push_back(HeaderEntry(VAR_FORCE_PLATE_CONSTANTS,3,6,force_plates));
      header.push_back(HeaderEntry(VAR_FORCE_PLATE_POSITION,3,6,4*force_plates));
      header.push_back(HeaderEntry(VAR_TIME_RESOLUTION_FORCE,1,2,force_plates));
      header.push_back(HeaderEntry(VAR_FORCE_RESOLUTION,1,4,8*force_plates));
      header.push_back(HeaderEntry(VAR_ANALOGUE_FORCE_NAMES,1,1,8*force_plates));
      header.push_back(HeaderEntry(VAR_ANALOGUE_FORCE,1,2,8*force_plates));
    }

		const EventGroup* gait_event_group = trial.UserInput.GetEventGroup(GaitEvents::GroupName);
    if (gait_event_group)
    {
      header.push_back(HeaderEntry(VAR_TIME_RESOLUTION_CURSOR,1,2,1));
      header.push_back(HeaderEntry(VAR_GAIT_CYCLES,1,2,2));
    }

    // write header
    UInt16 wEntries((UInt16)header.size());
    os.write((const char*)&wEntries,2);
    for (std::list<HeaderEntry>::iterator ientry(header.begin()); ientry != header.end(); ientry++)
      ientry->Write(os);

    //-- data arrays
    UInt16 wElements;

    // comments
    if (comment.size())
    {
			// TODO: sort this out
			EncodeMDFString(os, comment, true);
    }

    // creation date
		if (have_date_create)
		{
      wElements = 1;
			short dd = (short)trial.Acq.Date.Day;
			short mm = (short)trial.Acq.Date.Month;
			short yy = (short)trial.Acq.Date.Year;
      os.write((const char*)&wElements,2);
			os.write((const char*)&dd, 2);
			os.write((const char*)&mm, 2);
			os.write((const char*)&yy, 2);
		}

    // subject id
    if (subject_id.size())
    {
			EncodeMDFString(os, subject_id, true);
    }

    // subject classification
    if (subject_class.size())
    {
			EncodeMDFString(os, subject_class, true);
    }

    // basic subject data:
    //   gender, dob, age, height, weight
    if (subject_data_count >= 5)
    {
      short s;
      wElements = 1;

      s = (_stricmp(subject.Gender, "male") == 0) ? 1 : 0;
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);

			s = (short)(100*subject.DateOfBirth.Month + ((subject.DateOfBirth.Year-1900)%100));
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);

			s = (short)subject.Age;
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);

			s = (short)(subject.Weight+0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);

			s = (short)(subject.Height+0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);
    }

		// joint width subject data
		if (subject_data_count >= 11)
		{
      short s;
      wElements = 1;

			s = (short)(subject.PelvicWidth+ 0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);			

			s = (short)(subject.PelvicDepth+ 0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);			

			s = (short)(subject.LKneeWidth+ 0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);			

			s = (short)(subject.RKneeWidth+ 0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);			

			s = (short)(subject.LAnkleWidth+ 0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);			

			s = (short)(subject.RAnkleWidth+ 0.5);
      os.write((const char*)&wElements,2);
      os.write((const char*)&s,2);			
		}

    if (num_markers)
    {
      // rate
      wElements = 1;
      UInt16 rateMarker((UInt16)marker[0]->Rate);
      for (i = 0; i < num_markers; i++)
      {
        os.write((const char*)&wElements,2);
        os.write((const char*)&rateMarker,2);
      }

      // position resolution (microns per integer unit)
      wElements = 1;
      UInt16 resMarker(descriptor->MarkerWriteScaleMicrons());
      for (i = 0; i < num_markers; i++)
      {
        os.write((const char*)&wElements,2);
        os.write((const char*)&resMarker,2);
      }

      // names
      for (i = 0; i < num_markers; i++)
      {
				EncodeMDFString(os, marker[i]->Channel, true);
      }

      // hardware numbers
      for (i = 0; i < num_markers; i++)
      {
        wElements = 1;
        os.write((const char*)&wElements,2);
        UInt16 wNumber = (UInt16)marker[i]->HardwareID;
				if (wNumber > 0)
					wNumber--;	// make numbers zero-based
        os.write((const char*)&wNumber,2);
      }

      // positions
      for (i = 0; i < num_markers; i++)
        descriptor->WriteMarker(os, *marker[i]);

      // in-view
      for (i = 0; i < num_markers; i++)
      {
        // number of frames
				Int32 num_frames = marker[i]->NumFrames();
        wElements = (UInt16)((num_frames / 16) + 1);
        os.write((const char*)&wElements,2);

				try
				{
					// iterator - will throw exception of no occlusion found
					TSOccVector3ConstIter iter_ts(*marker[i]);

					// inview flags
					for (size_t j = 0; j < wElements; j++)
					{
						UInt16 w(0x0000);
						for (size_t k = 0; (k < 16) && iter_ts.HasFrame(); k++, iter_ts.Next())
						{
							if (!iter_ts.Occluded())
								w |= (0x8000 >> k);
						}
						os.write(((const char*)&w)+1,1);
						os.write(((const char*)&w),1);
					}
				}
				catch (const NoSuchFieldException&)
				{
					// write as visible in all frames
					UInt16 w(0xFFFF);
					for (size_t j = 0; j < wElements; j++)
						os.write((const char*)&w, 2);					
				}
 
     }
    }

    //-- emg
    // exclude channels which are force and do separately
    if (num_emg)
    {
      // rate
      wElements = 1;
			std::vector<size_t>::const_iterator iter_emgindex;
      for (iter_emgindex = emgindex.begin(); iter_emgindex != emgindex.end(); iter_emgindex++)
      {
        UInt16 rateEMG((UInt16)analog[*iter_emgindex]->Rate);
        os.write((const char*)&wElements,2);
        os.write((const char*)&rateEMG,2);
      }

			// repeat last channel rate for extra blank channels
			for (i = emgindex.size(); i < num_emg; i++)
			{
        UInt16 rateEMG((UInt16)analog[emgindex[0]]->Rate);
        os.write((const char*)&wElements,2);
        os.write((const char*)&rateEMG,2);
			}

			// default position resolution (millivolts per integer unit)
			// TODO: check if this is always ok to use
			std::vector<float> resEMG(num_emg, 0.1f);
			std::vector<float>::const_iterator iter_resEMG;

			// write (this does actual channels and any extras)
      wElements = 1;
			for (iter_resEMG = resEMG.begin(); iter_resEMG != resEMG.end(); iter_resEMG++)
      {
        os.write((const char*)&wElements,2);
        os.write((const char*)&(*iter_resEMG),4);
      }

      // names
      for (iter_emgindex = emgindex.begin(); iter_emgindex != emgindex.end(); iter_emgindex++)
      {
				EncodeMDFString(os, analog[*iter_emgindex]->Channel, true);
      }

			// "NotUsedX" names for dummy channels
			for (i = emgindex.size(); i < num_emg; i++)
			{
				std::ostringstream unusedname;
				unusedname << "NotUsed" << (i+1) << std::ends;
				EncodeMDFString(os, unusedname.str(), true);
			}

			// value arrays
      for (iter_emgindex = emgindex.begin(), iter_resEMG = resEMG.begin(); iter_emgindex != emgindex.end(); iter_emgindex++, iter_resEMG++)
      {
				// time sequence
				const TimeSequence* ts = analog[*iter_emgindex];

				// scale factors
				double scale = ts->Scale;
				double offset = ts->Offset;

				// write array size
				wElements = (UInt16)ts->NumFrames();
        os.write((const char*)&wElements,2);

				// write data
				for (TSScalarConstIter iter_ts(*ts); iter_ts.HasFrame(); iter_ts.Next())
        {
					// apply calibration
					double calvalue = scale * (iter_ts.Value() - offset);

					// scale by file scale factor
					double d = calvalue / *iter_resEMG;

					// write
					short s = (short)floor(d+0.5);
          os.write((const char*)&s, 2);
        }
      }

			// padding for extra channels
			for ( ; iter_resEMG != resEMG.end(); iter_resEMG++)
			{
				// use same num elements as other channels
	      os.write((const char*)&wElements,2);

				// write zero data
				short s = 0;
				for (size_t j = 0; j < wElements; j++)
			  {
		      os.write((const char*)&s, 2);
			  }
			}
		}

    //-- force
    if (force_plates)
    {
      // force plate type flags
      wElements = 1;
      for (i = 0; i < force_plates; i++)
      {
				UInt8 mdfplateid = fp_mdf[i].MDFPlateID();
        UInt16 wFlags = (UInt16)mdfplateid << 8;
        os.write((const char*)&wElements,2);
        os.write((const char*)&wFlags,2);
      }

      // force plate type strings
      wElements = 1;
      for (i = 0; i < force_plates; i++)
      {
				std::string strFP = fp_mdf[i].Type.Value();
				strFP += " ";
				strFP += fp_mdf[i].Model.Value();
				wElements = 1;
        os.write((const char*)&wElements,2);
				if (strFP.size() < 32)
				{
					os.write(strFP.c_str(),(std::streamsize)strFP.size());
					for (size_t i = strFP.size(); i < 32; i++)
						os.put('\0');
				}
				else
				{
					os.write(strFP.c_str(), 32);
				}
      }

      // force plate constants
      wElements = 1;
      for (i = 0; i < force_plates; i++)
      {
        os.write((const char*)&wElements,2);
        short wSensor[3];
        fp_mdf[i].MDFSensorConstants(wSensor);
        os.write((const char*)wSensor,6);
      }

      // force plate outline
      // - a 1-D array of corners (4 corners per plate)
      wElements = 1;
      for (i = 0; i < (4*force_plates); i++)
      {
				// get outline - transformed if necessary
				std::vector<Vector3> outline;
				fp_mdf[i/4].MDFOutline(outline);

				// write
        os.write((const char*)&wElements,2);
        for (size_t idim = 0; idim < 3; idim++)
        {
          short wCoord = (short)(10.0*outline[i%4][idim]);
          os.write((const char*)&wCoord,2);
        }
      }

      // rate - use first channel from each plate
      wElements = 1;
      for (i = 0; i < force_plates; i++)
      {
				size_t index = forceindex[i*8];
				if (index == NODATA)
					throw MotionFileException(MotionFileException::formaterror, "missing first channel of forceplate data");
        UInt16 rateForce = (UInt16)analog[index]->Rate;
				os.write((const char*)&wElements,2);
        os.write((const char*)&rateForce,2);
      }

      // force channel resolution - 8 channels per plate
      wElements = 1;
      for (i = 0; i < force_plates; i++)
      {
        for (size_t j = 0; j < 8; j++)
        {
          float resForce = fp_mdf[i].MDFDefaultSaveRes(j);
          os.write((const char*)&wElements,2);
          os.write((const char*)&resForce,4);
        }
      }

      // names
			for (size_t ichannel = 0; ichannel < forceindex.size(); ichannel++)
      {
				// get index of this plate
				size_t index = forceindex[ichannel];

				// cater for nodata and data case
        if (index == NODATA)
        {
	        // dummy entries for nodata channels
          std::ostringstream strName;
          strName << "NotUsed" << (ichannel+1) << '\0';
          wElements = (UInt16)strName.str().length() + 1;
          os.write((const char*)&wElements,2);
          os.write(strName.str().c_str(),wElements);
        }
				else
				{
          // write name
					EncodeMDFString(os, analog[index]->Channel, true);
				}
      }

      // value arrays
      for (i = 0; i < force_plates; i++)
      {
        for (size_t j = 0; j < 8; j++)
        {
					// scale factor for saving this plate
          double writescale = fp_mdf[i].MDFDefaultSaveRes(j) * fp_mdf[i].MDFChannelScale(j);

					// get analog index for this channel
					size_t index = forceindex[i*8 + j];

					// write zeros if no data, or data otherwise
					if (index == NODATA)
					{
						// use num frames from first element of this plate
						size_t firstindex = forceindex[i*8];
						if (firstindex == NODATA)
							throw MotionFileException(MotionFileException::formaterror, "missing first channel of forceplate data");
						wElements = (UInt16)analog[firstindex]->NumFrames();

						// write zeros
						os.write((const char*)&wElements,2);
						short wZero(0);
						for (size_t j = 0; j < wElements; j++)
							os.write((const char*)&wZero, 2);
					}
					else
					{
						// get sequence and calibration
						const TimeSequence* ts = analog[index];
						double scale = ts->Scale;
						double offset = ts->Offset;

						// write data
						wElements = (UInt16)ts->NumFrames();
						os.write((const char*)&wElements,2);
						for (TSScalarConstIter iter_ts(*ts); iter_ts.HasFrame(); iter_ts.Next())
						{
							// apply calibration
							double calvalue = scale * (iter_ts.Value() - offset);

							// scale by file scale factor
							double d = calvalue / writescale;

							// write
							short s = (short)floor(d+0.5);
							os.write((const char*)&s, 2);
						}
					}
				}
      }
    }

    //-- event data

    if (gait_event_group)
    {
      // cursor resolution
      wElements = 1;
      os.write((const char*)&wElements,2);
      UInt16 wRes = 400;
      os.write((const char*)&wRes,2);
      
			EventArray events;
			gait_event_group->GetEvents(events);
			size_t num_events = events.NumEvents();

      // make lists of l gait cycle and r gait cycle
      std::list<double> flagL, flagR;
      for (size_t event_index = 0; event_index < num_events; event_index++)
      {
				const std::string& name = events.EventName(event_index);
				double t = events.EventTime(event_index);

        // left cycle
        if ((flagL.size() == 0 && _stricmp(name.c_str(),GaitEvents::LStart) == 0) ||
            (flagL.size() == 1 && _stricmp(name.c_str(),GaitEvents::LToeOff) == 0) ||
            (flagL.size() == 2 && _stricmp(name.c_str(),GaitEvents::LEnd) == 0))
        {
          flagL.push_back(t);
        }

        // right cycle
        if ((flagR.size() == 0 && _stricmp(name.c_str(),GaitEvents::RStart) == 0) ||
            (flagR.size() == 1 && _stricmp(name.c_str(),GaitEvents::RToeOff) == 0) ||
            (flagR.size() == 2 && _stricmp(name.c_str(),GaitEvents::REnd) == 0))
        {
          flagR.push_back(t);
        }
      }

      // left gait cycles
      if (flagL.size() >= 3)
      {
        wElements = (UInt16)(3 * (flagL.size() / 3));
        os.write((const char*)&wElements,2);
        std::list<double>::const_iterator iL(flagL.begin());
        for (i = 0; i < wElements; i++,iL++)
        {
          UInt16 w;
          w = (UInt16)(400*(*iL) + 0.5);
          os.write((const char*)&w,2);
        }
      }
      else
      {
        // must have at least one for CODAmotion Analysis
        wElements = 3;
        os.write((const char*)&wElements,2);
        UInt16 w0[3] = { 0, 0, 0 };
        os.write((const char*)w0,6);
      }

      // right gait cycles
      if (flagR.size() >= 3)
      {
        wElements = (UInt16)(3 * (flagR.size() / 3));
        os.write((const char*)&wElements,2);
        std::list<double>::const_iterator iR(flagR.begin());
        for (i = 0; i < wElements; i++, iR++)
        {
          UInt16 w;
          w = (UInt16)(400*(*iR) + 0.5);
          os.write((const char*)&w,2);
        }
      }
      else
      {
        // must have at least one for CODAmotion Analysis
        wElements = 3;
        os.write((const char*)&wElements,2);
        UInt16 w0[3] = { 0, 0, 0 };
        os.write((const char*)w0,6);
      }
    }
	}
}
