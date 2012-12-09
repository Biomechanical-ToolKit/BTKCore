/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "FileFormatC3D.h"
#include "MapC3DParameters.h"
#include "C3DTSFactory.h"
#include "FileFormatOptionsC3D.h"
#include "C3DMachine.h"

#include <map>
#include <limits>
#include <strstream>
#include <stdio.h>
#include <math.h>

using namespace std;

namespace Open3DMotion
{
	class C3DSection : public MapCompound
	{
	public:
		C3DSection()
		{
			REGISTER_MEMBER(FirstFrame);
			REGISTER_MEMBER(InterpolationGap);
			REGISTER_MEMBER(ParameterHeader);
			REGISTER_MEMBER(Parameters);
		}

	public:
		MapInt32 FirstFrame;
		MapInt32 InterpolationGap;
		MapInt32 ParameterHeader;
		MapC3DParameters Parameters;
	};

	class TrialC3D : public Trial
	{
	public:
		TrialC3D()
		{
			REGISTER_MEMBER(C3D);
		}

	public:
		C3DSection C3D;
	};
	
	// check file id, processor type and number format
  bool FileFormatC3D::Probe(const MotionFileHandler& context, TreeValue*& readoptions, istream& is) const throw(MotionFileException)
  {
    FileFormatOptionsC3D options;

    // location of param section
		UInt8 param_offset(0);
    C3DMachine::ReadByte(param_offset, is);

    // C3D id
		UInt8 id_c3d(0);
    C3DMachine::ReadByte(id_c3d, is);

    // must have c3d id
    if (id_c3d != 0x50)
      return false;
    
    // read processor type
		UInt8 id_proc(0);
		is.seekg(512*((int)param_offset-1)+3, ios::beg);
    C3DMachine::ReadByte(id_proc, is);

		// determine processor type or fail if not recognised
		if (id_proc == C3DMachine_PC().ProcessorID())
		{
			options.Processor = FileFormatOptionsC3D::ProcessorPC;
		}
		else if (id_proc == C3DMachine_DEC().ProcessorID())
		{
			options.Processor = FileFormatOptionsC3D::ProcessorDEC;
		}
		else if (id_proc == C3DMachine_SGI().ProcessorID())
		{
			options.Processor = FileFormatOptionsC3D::ProcessorSGI;
		}
		else
		{
			return false;
		};

		// use format to read scale
    float scale(0.0);
		auto_ptr<C3DMachine> machine ( options.NewMachine() );
    is.seekg(12, ios::beg);
		machine->ReadFloat(scale, is);

		// check number format
    // - negative scale factor means floating point data
		options.FloatingPoint = (scale <= 0.0f) ? true : false;

		// retain options info
		readoptions = options.ToTree();

    // ok
    return true;
  }

  //----------------------------------------------------------
  // Read C3D

  TreeValue* FileFormatC3D::Read(const MotionFileHandler& context, istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) 
  {
		// parse options
		FileFormatOptionsC3D c3doptions;
		c3doptions.FromTree( readoptions );

		// create machine-specific reader
		std::auto_ptr<C3DMachine> machine( c3doptions.NewMachine() );

		// trial object to read
		std::auto_ptr<TrialC3D> trial( new TrialC3D );

		// go to param value
    is.clear();
    is.seekg(0, ios::beg);

    // exception on eof or read failure
    is.exceptions(ios::badbit|ios::eofbit|ios::failbit);

    // ptr to param block
    UInt8 block_param(0);
    C3DMachine::ReadByte(block_param, is);
    int index_param = 512*((int)block_param-1);

    // number of markers (should be specified in params)
    UInt16 header_point_used(0);
    is.seekg(2, ios::beg);
    machine->ReadWord(header_point_used, is);

    // total analog samples for each marker frame
    // ( = num analog channnels x analog rate / marker rate )
    // should not be needed if ANALOG/USED is specified in params
    UInt16 header_total_analog_per_frame(0);
    machine->ReadWord(header_total_analog_per_frame, is);

    // index of start-frame (one-based)
    UInt16 header_frame_first(0);
    machine->ReadWord(header_frame_first, is);
		trial->C3D.FirstFrame.Value() = (Int32)header_frame_first;

    // index of last-frame (one-based)
    // - shouldn't be needed if POINT/FRAMES is specified in params
    UInt16 header_frame_last(0);
    machine->ReadWord(header_frame_last, is);

    // interpolation gap value (used on passive systems)
    UInt16  interp_gap(0);
    is.seekg(10, ios::beg);
    machine->ReadWord(interp_gap, is);
		trial->C3D.InterpolationGap.Value() = (Int32)interp_gap;

    //-------------------------------------------------
    // The following should also be specified in params
    // but some non-conformant files put them only here
    // in the header

    // 3D point scale (should also be specified in params)
    float header_point_scale(0.0f);
    machine->ReadFloat(header_point_scale, is);

    // data start 
    UInt16 header_data_start(0);
    machine->ReadWord(header_data_start, is);

    // analog frames per marker frame
    UInt16 header_analog_per_marker(0);
    machine->ReadWord(header_analog_per_marker, is);

    // marker rate
    float header_point_rate(0.0f);
    machine->ReadFloat(header_point_rate, is);

    //-------------------------------------------------

    // read whether has 4-BYTE labels for events
    // (not supported otherwise)
    is.seekg(298, ios::beg);
    UInt16 event_labels(0);
    machine->ReadWord(event_labels,is);

    // read events
    if (event_labels == 12345)
    {
      UInt16 num_events(0);
      machine->ReadWord(num_events,is);
      if (num_events)
      {
        // Eddi's document only describes support for 18 events
        if (num_events > 18)
          throw(MotionFileException(MotionFileException::notsupported,"More than 18 discrete events not supported"));
        
        // skip unused
        is.seekg(2,ios::cur);

        // event descriptors
        vector<float> event_time(num_events);
        vector<UInt8> event_off(num_events);

        // times
        UInt16 ievent;
        for (ievent = 0; ievent < num_events; ievent++)
          machine->ReadFloat(event_time[ievent],is);
        is.seekg((18-num_events)*4,ios::cur);

        // display on/off info
        for (ievent = 0; ievent < num_events; ievent++)
          C3DMachine::ReadByte(event_off[ievent],is);
        is.seekg((18-num_events)+2,ios::cur);

        // make C3D event group
        EventArray c3deventarray;

        // make labels and build event array
        for (ievent = 0; ievent < num_events; ievent++)
        {
          char label[32];
          is.read(&label[0],4);
          if (event_off[ievent])
            strcpy(label+4, " (DISPLAY OFF)");
          else
            label[4] = '\0';

					c3deventarray.AddEvent(event_time[ievent], label);
        }

				// store
				EventGroup* c3devents = new EventGroup();
				c3devents->Name = "C3D Events";
				c3devents->SetEvents(c3deventarray, memfactory);
				trial->Acq.EventGroups.Add(c3devents);
      }
    }

    // seek to parameter header
    is.seekg(index_param, ios::beg);

    // parameter header 
    // - not used but good for backwards compatability
    UInt16 param_header(0);
    machine->ReadWord(param_header, is);
		trial->C3D.ParameterHeader = param_header;

    // skip processor id and block count
    is.seekg(2, ios::cur);

		// read parameters
		trial->C3D.Parameters.FromFile(*machine, is);

    // find next block after params
    Int32 index_param_end = (Int32)is.tellg();
    Int32 block_after_param = index_param_end / 512L;
    if ((index_param_end % 512L) != 0)
      block_after_param++;

    // get data block parameter
    // should be an error here if not found - but some files rely on DATA_START in header
		Int32 block_data = trial->C3D.Parameters.GetInteger("POINT", "DATA_START", header_data_start);

    // must be +ve and after header section
    if (block_data <= 2)
      throw(MotionFileException(MotionFileException::formaterror, "C3D POINT:DATA_START less than 3"));
    Int32 index_data = 512*(block_data-1);


		// get any origin from C3D params if software version is present
		if (trial->C3D.Parameters.GetText(trial->Acq.MeasurementSystem.Name.Value(), "MANUFACTURER", "COMPANY"))
		{
			trial->C3D.Parameters.GetText(trial->Acq.MeasurementSystem.SoftwareName.Value(), "MANUFACTURER", "SOFTWARE");
			trial->C3D.Parameters.GetText(trial->Acq.MeasurementSystem.SoftwareVersion.Value(), "MANUFACTURER", "VERSION");
		}
		else
		{
			// if no software name is specified, set default values for company, software name & version
			// (simply say it's an import)
			trial->Acq.MeasurementSystem.Name = "File Import";
			trial->Acq.MeasurementSystem.Company = "Charnwood Dynamics Ltd.";
			trial->Acq.MeasurementSystem.SoftwareName = context.ProgramName();
			trial->Acq.MeasurementSystem.SoftwareVersion = context.ProgramVersion();
		}

    // numbers of channels, rates and stuff
		Int32 num_frames_marker = trial->C3D.Parameters.GetInteger("POINT", "FRAMES", 1+header_frame_last-header_frame_first);
		Int32 num_markers = trial->C3D.Parameters.GetInteger("POINT", "USED", header_point_used);
		double rateMarker = trial->C3D.Parameters.GetReal("POINT", "RATE", header_point_rate);
		double scale_marker = trial->C3D.Parameters.GetReal("POINT", "SCALE", header_point_scale);
		Int32 num_analog = trial->C3D.Parameters.GetInteger("ANALOG", "USED", header_total_analog_per_frame / header_analog_per_marker);
		double rateAnalog = trial->C3D.Parameters.GetReal("ANALOG", "RATE", header_point_rate * header_analog_per_marker);
		double analog_gen_scale = trial->C3D.Parameters.GetReal("ANALOG", "GEN_SCALE");

		// compute number of analog frames for each marker frame
    Int32 analog_per_marker = (Int32)(0.5 + (rateAnalog / rateMarker));

    // negative scale means no scaling
    if (scale_marker <= 0)
      scale_marker = 1.0;

		// vectors of time sequence iterators (one iterator per channel)
		vector<TSC3DIter> tsmarker;
		vector<TSScalarIter> tsanalog;

    // marker names
    if (num_markers)
    {
			// Time range for marker data
			TimeRange tr;
			tr.Start = 0.0;
			tr.Frames = num_frames_marker;
			tr.Rate = rateMarker;

			// find which set of names it falls into
			// - due to C3D restrictions names must be in multiple groups 
			//   with up to 255 per group
			Int32 setofnames = 0;
			Int32 namesetindex = 0;

      for (Int32 i = 0; i < num_markers; i++)
      {
				// make a new time sequence and let trial object take ownership
				TimeSequence* ts = TSFactoryC3D().New(tr, memfactory);
				trial->Acq.TimeSequences.Add( ts );

				// create iterator
				tsmarker.push_back(TSC3DIter(*ts));

				// set group name
				ts->Group =TrialSectionAcq::TSGroupMarker;

				// get names
				string shortname, longname;

				// lookup within this set and, if not found, try next set
				for (int trylookup = 0; trylookup < 2; trylookup++)
				{
					// create paths in tree
					string recordname_short("LABELS");
					string recordname_long("DESCRIPTIONS");
					if (setofnames > 0)
					{
						std::ostrstream number;
						number << (setofnames+1) << std::ends;
						recordname_short += number.str();
						recordname_long += number.str();
					}

					// try to find it
					if (trial->C3D.Parameters.GetText(shortname, "POINT", recordname_short.c_str(), namesetindex) || 
						  trial->C3D.Parameters.GetText(shortname, "POINT", recordname_long.c_str(), namesetindex) )
					{
						namesetindex++;
						break;
					}
					else
					{
						// not found - try incrementing to next set
						setofnames++;
						namesetindex = 0;
					}
				}

				// use Int16 one if available
        if (shortname.size())
        {
          // use given Int16 name
					ts->Channel = shortname;
        }
        else if (longname.size())
        {
          // use given Int32 name if no Int16 name
					ts->Channel = longname;
        }
        else
        {
          // invent a Int16 name
          ostrstream osTempname;
          osTempname << "M" << (i+1) << '\0';
					ts->Channel = osTempname.str();
        }

        // TODO: make use of Int32 name even when Int16 name present
      }
    }

    // Copy force-plate parameters to tree structure
    // required by our analysis software
		Int32 num_force_plates = trial->C3D.Parameters.GetInteger("FORCE_PLATFORM", "USED");
		vector<bool> isforcechannel(num_analog, false);
    for (Int32 iplate = 0; iplate < num_force_plates; iplate++)
    {
      // get force plate type
      Int32 c3dForceType = trial->C3D.Parameters.GetInteger("FORCE_PLATFORM", "TYPE");
			
			// create structure to populate
			ForcePlate* fpm = new ForcePlate;

			if (c3dForceType == 2 || c3dForceType == 4)
      {
				// AMTI plates
				fpm->Type = ForcePlate::TypeAMTI;
				fpm->Model ="Generic";

				// AMTI type 4 will have calibration matrix whereas type 2 does not
				if (c3dForceType == 4)
				{
					// offset to cal
					const C3DRecordReal* c3dcalmatrix = trial->C3D.Parameters.GetRecordType<C3DRecordReal>("FORCE_PLATFORM", "CAL_MATRIX");
					size_t cal_elements_per_plate = (c3dcalmatrix != NULL) ? (c3dcalmatrix->TotalElements() / num_force_plates) : 0;
					if (cal_elements_per_plate >= 36)
					{
						// read and transpose from C3D
						for (size_t irow = 0; irow < 6; irow++)
						{
							for (size_t icol = 0; icol < 6; icol++)
							{
								fpm->Calibration.Add(c3dcalmatrix->Data()[((size_t)iplate)*cal_elements_per_plate + 6*icol + irow]);
							}
						}
					}
					else
					{
						// default identity
						for (size_t irow = 0; irow < 6; irow++)
						{
							for (size_t icol = 0; icol < 6; icol++)
							{
								fpm->Calibration.Add((irow == icol) ? 1.0 : 0.0);
							}
						}
					}
				}
			}
      else if (c3dForceType == 3)
      {
				// Kistler plates
				fpm->Type = ForcePlate::TypeKistler;
				fpm->Model ="Generic";
			}
      else
      {
        // - unrecognised force plate type
        continue;
      }

			const C3DRecordInteger* c3dchannel = trial->C3D.Parameters.GetRecordType<C3DRecordInteger>("FORCE_PLATFORM", "CHANNEL");
			size_t num_channels = c3dchannel->TotalElements() / num_force_plates;
		
			// copy channels
			for (size_t ichannel = 0; ichannel < num_channels; ichannel++)
      {
				// one-based index (= hardware id)
				Int32 hardwareID = c3dchannel->Data()[ iplate*num_channels + ichannel ];
				fpm->Channels.Add( hardwareID );

				// remember which channels are force because we need to invert them
				isforcechannel[hardwareID-1] = true; 
      }

      // copy corners
			const C3DRecordReal* c3dcorners = trial->C3D.Parameters.GetRecordType<C3DRecordReal>("FORCE_PLATFORM", "CORNERS");
			for (size_t icorner = 0; icorner < 4; icorner++)
			{
				fpm->Outline[icorner].X.Value() = c3dcorners->Data()[12*iplate+3*((icorner+2)%4)+0];
				fpm->Outline[icorner].Y.Value() = c3dcorners->Data()[12*iplate+3*((icorner+2)%4)+1];
				fpm->Outline[icorner].Z.Value() = c3dcorners->Data()[12*iplate+3*((icorner+2)%4)+2];
			}

      // retrieve c3d origin params
			float c3dorigin[3] =
			{
				trial->C3D.Parameters.GetReal("FORCE_PLATFORM", "ORIGIN", 0),
				trial->C3D.Parameters.GetReal("FORCE_PLATFORM", "ORIGIN", 1),
				trial->C3D.Parameters.GetReal("FORCE_PLATFORM", "ORIGIN", 2)
			};

			// copy origin according to fp type
      if (c3dForceType == 3)
      {
        // Kistler

				// set new origin - note axis convention
				fpm->SensorSeparation.X = c3dorigin[1];
				fpm->SensorSeparation.Y = c3dorigin[0];
				fpm->SensorSeparation.Z = -c3dorigin[2];
      }
      else
      {
        // AMTI

        // Set new origin: 
				// codaBiomech expects AMTI calibration values as on printed data sheet
				// whereas C3D uses the negative of all AMTI values
				fpm->CentreOffset.X = -c3dorigin[0];
				fpm->CentreOffset.Y = -c3dorigin[1];
				fpm->CentreOffset.Z = -c3dorigin[2];
      }
			
			// add model data to tree in standard location
			trial->Acq.ForcePlates.Add( fpm );
    }

    // analog channel names, scales and offsets
    if (num_analog)
    {
			// analog time range
			TimeRange tr;
			tr.Start = 0.0;
			tr.Frames = num_frames_marker * analog_per_marker;
			tr.Rate = rateAnalog;

			for (Int32 i = 0; i < num_analog; i++)
      {
				// make a new time sequence and let trial object take ownership
				TimeSequence* ts = TSFactoryValue(1).New(tr, memfactory);
				trial->Acq.TimeSequences.Add( ts );

				// make iterator
				tsanalog.push_back(TSScalarIter(*ts));

				// set group and hardware id
				ts->Group = TrialSectionAcq::TSGroupAnalog;
				ts->HardwareID = i + 1;

				// get available names
				string shortname;
				string longname;
				trial->C3D.Parameters.GetText(shortname, "ANALOG", "LABELS", i);
				trial->C3D.Parameters.GetText(longname, "ANALOG", "DESCRIPTIONS", i);

        if (shortname.size())
        {
          // use given Int16 name
					ts->Channel = shortname;
        }
        else if (longname.size())
        {
          // use given Int32 name if no Int16 name
					ts->Channel = longname;
        }
        else
        {
          // invent a Int16 name
          ostrstream tempname;
          tempname << "CH" << (i+1) << '\0';
					ts->Channel = tempname.str();
        }

				// units (default to V)
				if (!trial->C3D.Parameters.GetText(ts->Units.Value(), "ANALOG", "UNITS", i))
					ts->Units = "V";

				// get scale and observe force sign convention (set positive if was negative)
				float c3dscale = trial->C3D.Parameters.GetReal("ANALOG", "SCALE", 1.0f, i);
				if (isforcechannel[i])
					c3dscale = -c3dscale;
				ts->Scale = analog_gen_scale * (double)c3dscale;
				ts->Offset = (double)trial->C3D.Parameters.GetInteger("ANALOG", "OFFSET", 0, i);
      }
		}

		// all is set up -- read the data
    is.seekg(index_data, ios::beg);
    for (Int32 iframe = 0; iframe < num_frames_marker; iframe++)
    {
      // one set of markers
      for (int imarker = 0; imarker < num_markers; imarker++)
      {
        // get marker info
				TSC3DIter& iter_ts = tsmarker[imarker];
      
        // 3D co-ord
				if (c3doptions.FloatingPoint)
        {
          float f;
          machine->ReadFloat(f,is);
          iter_ts.Value()[0] = f;
          machine->ReadFloat(f,is);
          iter_ts.Value()[1] = f;
          machine->ReadFloat(f,is);
          iter_ts.Value()[2] = f;
        }
        else
        {
          UInt16 w;
          machine->ReadWord(w,is);
          iter_ts.Value()[0] = (float)((Int16)w) * scale_marker;
          machine->ReadWord(w,is);
          iter_ts.Value()[1] = (float)((Int16)w) * scale_marker;
          machine->ReadWord(w,is);
          iter_ts.Value()[2] = (float)((Int16)w) * scale_marker;
        }
    
        // inview status
        UInt16 status;
        if (c3doptions.FloatingPoint)
        {
          float f;
          machine->ReadFloat(f, is);
          status = (UInt16)((Int16)f);
        }
        else
        {
          machine->ReadWord(status,is);
        }
        iter_ts.Occluded() = (status & 0x8080) ? true : false;
        iter_ts.CameraFlag() = (char)(status >> 8);
        if ((status & 0x80) == 0)
        {
          double res = (float)(status & 0x7F) * scale_marker;
					iter_ts.Confidence()[0] = iter_ts.Confidence()[1] = iter_ts.Confidence()[2] = res*res;
        }

				// increment iterator
				iter_ts.Next();
      }
		
      // one or more sets of analog
      for (int analog_subframe = 0; analog_subframe < analog_per_marker; analog_subframe++)
      {
				// read data
				for (int ianalog = 0; ianalog < num_analog; ianalog++)
        {
					// analog channel info
					TSScalarIter& iter_ts = tsanalog[ianalog];

          float a;
          if (c3doptions.FloatingPoint)
          {
            machine->ReadFloat(a, is);
          }
          else
          {
            Int16 a16;
            machine->ReadWord((UInt16&)a16, is);
            a = (float)a16;
          }							

					// set in sequence array
					iter_ts.Value() = a;				

					// increment iterator
					iter_ts.Next();
        }
      }
    }

		// transfer entire trial structure
		return trial->ToTree();
  }

  //----------------------------------------------------------
  // Write C3D

  void FileFormatC3D::Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
  {
		// parse options
		FileFormatOptionsC3D c3doptions;
		c3doptions.FromTree( writeoptions );

		// create machine-specific writer
		auto_ptr<C3DMachine> machine( c3doptions.NewMachine() );

		size_t i;

		// parse to trial structure
		TrialC3D trial;
		trial.FromTree(contents);
		
		// remap calanalog and allfp to convert 12-channel AMTI plate to AMTI 6-channel (C3D type 4)
		// TODO:
		// RemapForceplate rfp;
		// rfp.Remap(trial, L"C3D");

		// get details for all markers
    vector<const TimeSequence*> marker;
		trial.Acq.GetTSGroup(marker,TrialSectionAcq::TSGroupMarker);

    // get details for all analog
		vector<const TimeSequence*> analog;
		trial.Acq.GetTSGroup(analog, TrialSectionAcq::TSGroupAnalog);

		// numbers
    size_t num_markers = marker.size();
		size_t num_analog = analog.size();

		if (num_analog > 255)
			throw MotionFileException(MotionFileException::notsupported, "C3D file save with more than 255 channels of analogue is not supported in this version");

		// for marker rate assume all markers the same
    Int32 num_frames(0);
    double rateCommonMarker(0);
    if (num_markers)
    {
			num_frames = marker[0]->NumFrames();
			rateCommonMarker = marker[0]->Rate;
			for (i = 1; i < num_markers; i++)
        if (fabs(marker[i]->Rate-rateCommonMarker) > 1E-6)
          throw(MotionFileException(MotionFileException::notsupported, "Multiple marker rates not supported"));
    }

    // for analog it will be common for rates to vary
    // - find lowest common multiple over analog rates and marker rates
    double rateCommonAnalog(0);
    Int32 analog_per_marker(1);
    vector<Int32> analog_subsample(num_analog);
    vector<float> rateAnalog(num_analog);
    if (num_analog)
    {
      // rates across all analog channels
      vector<double> allrates(num_analog+1);
      for (i = 0; i < num_analog; i++)
        rateAnalog[i] = (float)(allrates[i] = analog[i]->Rate);

      // include marker rate (or zero if not present)
      allrates[num_analog] = rateCommonMarker;

      // begin with the first one we can find
      rateCommonAnalog = floor(allrates[0] - 1.0);

      // now choose lowest common multiple
      // across all analog rates, and common marker rate
      vector<double>::const_iterator irate;
      bool common_not_found(true);
      while (common_not_found)
      {
        rateCommonAnalog++;
        common_not_found = false;
        for (irate = allrates.begin(); irate != allrates.end(); irate++)
          if (fmod(rateCommonAnalog,*irate) > 1E-6)
          {
            common_not_found = true;
            break;
          }
      }

      // subsamples required to interpolate to new rate
      for (i = 0; i < num_analog; i++)
        analog_subsample[i] = (Int32)(0.5+(rateCommonAnalog/allrates[i]));

      // how does analog compare to marker rate?
      if (num_markers)
      {
        analog_per_marker = (Int32)(0.5+(rateCommonAnalog/rateCommonMarker));
      }
      else
      {
        // if no markers C3D still requires marker rate
        // - so set equal to analog
        rateCommonMarker = rateCommonAnalog;
        analog_per_marker = 1;
        num_frames = (Int32)(0.5+(rateCommonAnalog/allrates[0])) * analog[0]->NumFrames();
      }
    }

    // look for original C3D scale factors, otherwise default scale factors
		// TODO: when using integer scaling, compute appropriate scale factors based on data
		double scale_marker = trial.C3D.Parameters.GetReal("POINT", "SCALE", 0.1f);
		double gen_scale_analog = trial.C3D.Parameters.GetReal("ANALOG", "GEN_SCALE", 1.0f);

    // - may be -ve to indicate floating point format
    if (scale_marker < 0.0)
      scale_marker *= -1.0;

    // marker labels
		// these are done in sets of 127, due to a restriction of C3D string array sizes
		int numlabelsets_marker = (num_markers % 127 == 0) ? (num_markers/127) : (1 + (num_markers/127));
    vector< vector<string> > label_marker(numlabelsets_marker), desc_marker(numlabelsets_marker);
		int maxchars_label_marker = 4;
		int maxchars_pointdesc = 32;
		i = 0;
		for (int setnumber = 0; setnumber < numlabelsets_marker; setnumber++)
		{
			// arrays for this set
			vector<string>& labelset = label_marker[setnumber];
			vector<string>& descset = desc_marker[setnumber];

			// number of markers in this set (max 127 per set)
			int setsize = ((i + 127) < num_markers) ? 127 : (num_markers-i);
			labelset.resize(setsize);
			descset.resize(setsize);

			// tree index for trying to retrieve any existing descriptions
				// TODO: fix this - it won't work if there were actually 255 marker names
			string descrecordname = "DESCRIPTIONS";
			if (setnumber > 0)
			{
				ostrstream number;
				number << (setnumber+1) << ends;
				descrecordname += number.str();
			}
			
			// copy names to arrays
			for (int iset = 0; iset < setsize; iset++, i++)
		  {
				// use marker name
				labelset[iset] = marker[i]->Channel.Value();

				// use old description if available, otherwise blank
				// TODO: fix this - it won't work if there were actually 255 marker names
				trial.C3D.Parameters.GetText(descset[iset], "POINT", descrecordname.c_str(), iset);

				if (labelset[iset].size() > (size_t)maxchars_label_marker)
					maxchars_label_marker = (int)labelset[iset].size();

				if (descset[iset].size() > (size_t)maxchars_pointdesc)
					maxchars_pointdesc = (int)descset[iset].size();
			}
		}

		// analog labels, units & scaling
    vector<string> label_analog(num_analog);
		vector<string> desc_analog(num_analog);
		vector<string> units_analog(num_analog);
		vector<float> scale_analog(num_analog, 1.0f);
		vector<Int16> offset_analog(num_analog, 0);
		size_t maxchars_label_analog = 4;
		size_t maxchars_analogdesc = 32;

    for (i = 0; i < num_analog; i++)
    {
      // use name 
			label_analog[i] = analog[i]->Channel.Value();

			// use units
			units_analog[i] = analog[i]->Units.Value();

      // use C3D description if available, otherwise blank
      // - may override this later for force channel descriptions
			trial.C3D.Parameters.GetText(desc_analog[i], "ANALOG", "DESCRIPTIONS", i);

			// find string lengths
			if (label_analog[i].size() > (size_t)maxchars_label_analog)
				maxchars_label_analog = (int)label_analog[i].size();
			if (desc_analog[i].size() > (size_t)maxchars_analogdesc)
				maxchars_analogdesc = (int)desc_analog[i].size();

      // use scale and offset if available
			if (analog[i]->Scale.IsSet())
      {
				// calibration spec from tree

				// copy to arrays for C3D writing
				// must un-apply gen scale here
				scale_analog[i] = (float)(analog[i]->Scale / gen_scale_analog);
				offset_analog[i] = (Int16)(analog[i]->Offset.Value());
			}
      else
      {
					// autoscale required - find range of data
          double amin(numeric_limits<double>::max());
          double amax(-numeric_limits<double>::max());
					const TimeSequence* ts = analog[i];
					for (TSScalarConstIter iter_ts( *ts ); iter_ts.HasFrame(); iter_ts.Next())
          {
						if (iter_ts.Value() < amin)
              amin = iter_ts.Value();
            if (iter_ts.Value() > amax)
              amax = iter_ts.Value();
          }
          amin = fabs(amin);
          amax = fabs(amax);

					// find unipolar max
          double arange = (amax >= amin) ? amax : amin;

					// Old version - pre 20090305
          // offset_analog[i] = 8191;
          // scale_analog[i] = (float)(arange / 8191.0);

					// Use signed version - 20090305
					offset_analog[i] = 0;
					scale_analog[i] = (float)(arange / 32767);
      }
    }

    // Extra analog stuff not included in data set

    // preserve extra offsets
		{
			const C3DRecordInteger* offsetrec = trial.C3D.Parameters.GetRecordType<C3DRecordInteger>("ANALOG", "OFFSET");
			if (offsetrec)
			{
				for (i = num_analog; i < offsetrec->Data().size(); i++)  
				{
					offset_analog.push_back(offsetrec->Data()[i]);
				}
			}
		}

    // preserve extra scale
		{
			const C3DRecordInteger* scalerec = trial.C3D.Parameters.GetRecordType<C3DRecordInteger>("ANALOG", "SCALE");
			if (scalerec)
			{
				for (i = num_analog; i < scalerec->Data().size(); i++)  
				{
					scale_analog.push_back(scalerec->Data()[i]);
				}
			}
		}

    // preserve extra labels
		{
			const C3DRecordText* labelsrec = trial.C3D.Parameters.GetRecordType<C3DRecordText>("ANALOG", "LABELS");
			if (labelsrec)
			{
				for (i = num_analog; i < labelsrec->Data().size(); i++)  
				{
					label_analog.push_back(labelsrec->Data()[i]);
				}
			}
		}

    // preserve extra desc
		{
			const C3DRecordText* descsrec = trial.C3D.Parameters.GetRecordType<C3DRecordText>("ANALOG", "DESCRIPTIONS");
			if (descsrec)
			{
				for (i = num_analog; i < descsrec->Data().size(); i++)  
				{
					desc_analog.push_back(descsrec->Data()[i]);
				}
			}
		}

    // preserve extra units
		{
			const C3DRecordText* unitsrec = trial.C3D.Parameters.GetRecordType<C3DRecordText>("ANALOG", "UNITS");
			if (unitsrec)
			{
				for (i = num_analog; i < unitsrec->Data().size(); i++)  
				{
					units_analog.push_back(unitsrec->Data()[i]);
				}
			}
		}

		// get the subset of forceplates which can be saved to C3D
		vector<ForcePlate> c3dfp;
		vector<Int16> platetype;
		for (i = 0; i < trial.Acq.ForcePlates.NumElements(); i++)
		{
			// generic plate
			const ForcePlate& fpm = trial.Acq.ForcePlates[i];

			// determinte c3d type and expected number of channels
			size_t channels_to_save = 0;
			Int16 mytype = 0;
			if (fpm.Type.Value().compare(ForcePlate::TypeAMTI) == 0)
			{
				channels_to_save = 6;
				if (fpm.Calibration.NumElements() == 36)
					mytype = 4;
				else
					mytype = 2;
			}
			else if (fpm.Type.Value().compare(ForcePlate::TypeKistler) == 0)
			{
				channels_to_save = 8;
				mytype = 3;
			}
			else
			{
				// unrecognised plate
				continue;
			}

			// check number of channels is as expected
			// NOTE: this will exclude 12-channel AMTI data
			if (fpm.Channels.NumElements() != channels_to_save)
			{
				continue;
			}

			// add plate to list of c3d supported ones, and plate type to list of types
			platetype.push_back(mytype);
			c3dfp.push_back(fpm);
		}

		// count of all supported force plates
		size_t num_force_plates = c3dfp.size();

		// override analog descriptions for force channels and set negative scales
    size_t max_channels_per_plate(0);
		if (num_force_plates)
    {
      const static char strForceLabelA[6][4] = 
      { "FX", "FY", "FZ", "MX", "MY", "MZ" };
      const static char strForceDescA[6][5] = 
      { "Fx", "Fy", "Fz", "Mx", "My", "Mz" };
      const static char strForceLabelK[8][4] = 
      { "X1", "X3", "Y1", "Y2", "Z1", "Z2", "Z3", "Z4" };
      const static char strForceDescK[8][5] = 
      { "X1+2", "X3+4", "Y1+4", "Y2+3", "Z1", "Z2", "Z3", "Z4" };

			for (i = 0; i < num_force_plates; i++)
      {
				// model for this plate
				const ForcePlate& fpm = c3dfp[i];

				// channels on this plate
				size_t plate_channels = fpm.Channels.NumElements();

				// find max channels for any individual plate
				if (plate_channels > max_channels_per_plate)
					max_channels_per_plate = plate_channels;

				// override channel names and negate data
				for (size_t ichannel = 0; ichannel < plate_channels; ichannel++)
        {
					// get analog channel hardware ID specified in tree
					Int32 hardwareID = fpm.Channels[ichannel];
					
					// convert to analog index
					size_t ianalog(0);
					for (ianalog = 0; ianalog < num_analog; ianalog++)
					{
						if (analog[ianalog]->HardwareID.IsSet() && (analog[ianalog]->HardwareID == hardwareID))
							break;
					}

					if (ianalog < num_analog)
					{
            ostrstream sname, sdesc;
            if (platetype[i] == 3)	// Kistler plates
            {
              sname << "F" << (i+1) << strForceLabelK[ichannel] << '\0';
              sdesc << "Force Plate #" << (i+1) << " - " << strForceDescK[ichannel] << '\0';
            }
            else if (platetype[i] == 2 || platetype[i] == 4)	// AMTI plates
            {
              sname << strForceLabelA[ichannel] << (i+1) << '\0';
              sdesc << "Force Plate #" << (i+1) << " - " << strForceDescA[ichannel] << '\0';
            }
						else
						{
							// should not happen - plate types were restricted earlier
							continue;
						}

            // use new strings
            label_analog[ianalog] = sname.str();
            desc_analog[ianalog] = sdesc.str();

            // set scale negative - convention used for force
						scale_analog[ianalog] *= -1.0;
					}
        }
      }
    }

    // tree of records
    MapC3DParameters param;

    // markers
		param.AddRecord(new C3DRecordGroup(1, "POINT", "3-D point parameters"));
    param.AddRecord(new C3DRecordInteger(1, "USED", "Number of markers used", (Int16)num_markers));
    param.AddRecord(new C3DRecordInteger(1, "FRAMES", "Number of frames for marker data", (Int16)num_frames));
    param.AddRecord(new C3DRecordReal(1, "RATE", "Marker rate", (float)rateCommonMarker));
    param.AddRecord(new C3DRecordText(1, "UNITS", "Marker units", "mm"));
    param.AddRecord(new C3DRecordReal(1, "SCALE", "Marker scale factor", 
      c3doptions.FloatingPoint ? -(float)scale_marker : (float)scale_marker));
    if (num_markers)
    {
			for (i = 0; i < label_marker.size(); i++)
			{
				ostrstream strlabels;
				ostrstream strdescs;
				strlabels << "LABELS";
				strdescs << "DESCRIPTIONS";
				if (i > 0)
				{
					strlabels << (i+1);
					strdescs << (i+1);
				}
				strlabels << ends;
				strdescs << ends;
				param.AddRecord(new C3DRecordText(1, strlabels.str(), "Short marker names", label_marker[i], maxchars_label_marker));
				param.AddRecord(new C3DRecordText(1, strdescs.str(),  "Marker names", desc_marker[i], maxchars_pointdesc));
			}
    }

    // analog
    param.AddRecord(new C3DRecordGroup(2, "ANALOG", "Analog data parameters"));
    param.AddRecord(new C3DRecordInteger(2, "USED", "Analog channels", (Int16)num_analog));
    param.AddRecord(new C3DRecordReal(2, "RATE", "Rate for stored analog data - see ACQUISITION_RATE to find true acquisiton rates for each channel", (float)rateCommonAnalog));
    param.AddRecord(new C3DRecordReal(2, "GEN_SCALE", "Scale factor for all analog", (float)gen_scale_analog));
    if (num_analog)
    {
      param.AddRecord(new C3DRecordText(2, "LABELS", "Short analog channel names", label_analog, maxchars_label_analog));
      param.AddRecord(new C3DRecordText(2, "DESCRIPTIONS", "Analog channel names", desc_analog, maxchars_analogdesc));
      param.AddRecord(new C3DRecordText(2, "UNITS", "Analog measurement units", units_analog, 4));
      param.AddRecord(new C3DRecordReal(2, "SCALE", "Scale factor", scale_analog));
      param.AddRecord(new C3DRecordInteger(2, "OFFSET", "Offset", offset_analog));
      param.AddRecord(new C3DRecordReal(2, "ACQUISITION_RATE", "Original data acquisition rate for each analog channel", rateAnalog));

			// 20090305 set binary number format to signed when saving integer
			// this should be the default for all readers but not so for Visual3D
			if (c3doptions.FloatingPoint)
				param.AddRecord(new C3DRecordText(2, "FORMAT", "Binary Format", "SIGNED"));
		}

		// copy native force plate section if no existing c3d one
    param.AddRecord(new C3DRecordGroup(3, "FORCE_PLATFORM", "Force plate parameters"));
    param.AddRecord(new C3DRecordInteger(3, "USED", "Number of force plates", (Int16)num_force_plates));

    if (num_force_plates)
    {
      // dimensions for force channel info
      vector<UInt32> dimchannels(2);
      dimchannels[0] = max_channels_per_plate;
      dimchannels[1] = (Int32)num_force_plates;

      // dimensions for corners
      vector<UInt32> dimcorners(3);
      dimcorners[0] = 3;
      dimcorners[1] = 4;
      dimcorners[2] = (Int32)num_force_plates;

      // dimensions for origin
      vector<UInt32> dimorigin(2);
      dimorigin[0] = 3;
      dimorigin[1] = (Int32)num_force_plates;

			// dimensions for cal
			vector<UInt32> dimcalmatrix(3);
			dimcalmatrix[0] = 6;
			dimcalmatrix[1] = 6;
			dimcalmatrix[2] = (Int32)num_force_plates;

      // arrays
      vector<Int16> channel(num_force_plates*max_channels_per_plate);
      vector<float> corners(num_force_plates*4*3);
      vector<float> origin(num_force_plates*3);
			vector<float> cal_matrix(num_force_plates*36);

			// clear cal matrix
			bool have_cal_matrix(false);
			memset(&cal_matrix[0], 0, num_force_plates*36*sizeof(float));

      // find data for arrays
			for (i = 0; i < num_force_plates; i++)
		  { 
				// force plate model
				const ForcePlate& fpm = c3dfp[i];

				// get channel numbers
        for (size_t ichan = 0; ichan < max_channels_per_plate; ichan++)
        {
					// c3d channel index (0 means don't use)
					Int16 c3dchannel = 0;

					// get channel from model
					if (ichan < fpm.Channels.NumElements())
					{
						// probably safe to use hardware ID here (assuming analog channels start at 1 and are sequential)
						// but do full remapping just in case
						Int32 hardwareID = fpm.Channels[ichan];
						for (size_t analogindex_zerobased = 0; analogindex_zerobased < analog.size(); analogindex_zerobased++)
						{
							if (analog[analogindex_zerobased]->HardwareID.IsSet() && analog[analogindex_zerobased]->HardwareID == hardwareID)
							{
								c3dchannel = (Int16)(analogindex_zerobased + 1);
								break;
							}
						}
					}

					// set in array
          channel[i*max_channels_per_plate+ichan] = c3dchannel;
        }

        // corners
				for (size_t icorner = 0; icorner < 4; icorner++)
				{
					corners[12*i+3*((icorner+2)%4)+0] = (float) fpm.Outline[icorner].X.Value();
					corners[12*i+3*((icorner+2)%4)+1] = (float) fpm.Outline[icorner].Y.Value();
					corners[12*i+3*((icorner+2)%4)+2] = (float) fpm.Outline[icorner].Z.Value();
				}

        // origin sensor params
        if (platetype[i] == 3)
        {
          // Kistler

          // Set value - note change in axis convention
					origin[3*i+0] = (float)fpm.SensorSeparation.Y.Value();
          origin[3*i+1] = (float)fpm.SensorSeparation.X.Value();
          origin[3*i+2] = (float)-fpm.SensorSeparation.Z.Value();
        }
        else if (platetype[i] == 2 || platetype[i] == 4)
        {
          // Set value: 
					// codaBiomech expects AMTI calibration values as on printed data sheet
					// whereas C3D uses the negative of all AMTI values
          origin[3*i+0] = (float)(-fpm.CentreOffset.X.Value());
          origin[3*i+1] = (float)(-fpm.CentreOffset.Y.Value());
          origin[3*i+2] = (float)(-fpm.CentreOffset.Z.Value());

				}

				// get calibration if type 4 plate
				if (platetype[i] == 4 && fpm.Calibration.NumElements() == 36)
				{
					// TODO: verify transposition
					have_cal_matrix = true;
					for (size_t row = 0; row < 6; row++)
					{
						for (size_t col = 0; col < 6; col++)
						{
							cal_matrix[i*36 + 6*row + col] = (float) fpm.Calibration[6*col + row];
						}
					}
				}
      }

			// SXC: Andrew recognised that the default vals for this two should be 0s 
			//      rather than 1 and 20 initially assigned
      vector<Int16> zero(2);
			zero[0] = 0;
      zero[1] = 0;

      // add records
      param.AddRecord(new C3DRecordInteger(3, "TYPE", "Kistler=3;AMTI=2,4", platetype));
      param.AddRecord(new C3DRecordInteger(3, "CHANNEL", "Analog force channels", channel, dimchannels));
      param.AddRecord(new C3DRecordReal(3, "CORNERS", "Force plate outline", corners, dimcorners));
      param.AddRecord(new C3DRecordReal(3, "ORIGIN", "Sensor parameters", origin, dimorigin));
      param.AddRecord(new C3DRecordInteger(3, "ZERO", "Frames used to compute zero-point", zero));

			// cal matrix if present on any plate
			if (have_cal_matrix)
			{
	      param.AddRecord(new C3DRecordReal(3, "CAL_MATRIX", "Sensor Calibration", cal_matrix, dimcalmatrix));
				param.AddRecord(new C3DRecordText(3, "MATRIX_STORE", "Binary Format", "BYCOLUMN"));
			}
    }

		// make manufacturer section
		param.AddRecord(new C3DRecordGroup(4,"MANUFACTURER", ""));
		
		// Add company if exists, otherwise indicate that we did the import
		if (trial.Acq.MeasurementSystem.Company.IsSet())
		{
			param.AddRecord(new C3DRecordText(4, "COMPANY", "", trial.Acq.MeasurementSystem.Company));
		}
		else
		{
			if (trial.Acq.MeasurementSystem.Name.Value().compare("File Import") == 0)
			{
				// If it's an import with company unknown, do put our company name 
				// but indicate that it's an import in case it didn't originally come from us
				param.AddRecord(new C3DRecordText(4, "COMPANY", "", "File Import by Charnwood Dynamics Ltd."));
			}
		}

		if (trial.Acq.MeasurementSystem.SoftwareName.IsSet())
			param.AddRecord(new C3DRecordText(4, "SOFTWARE", "", trial.Acq.MeasurementSystem.SoftwareName));
		if (trial.Acq.MeasurementSystem.SoftwareVersion.IsSet())
			param.AddRecord(new C3DRecordText(4, "VERSION", "", trial.Acq.MeasurementSystem.SoftwareVersion));

		// add export info to C3D (this is non-standard but useful)
		param.AddRecord(new C3DRecordText(4, "FILE_EXPORT_COMPANY", "", "Charnwood Dynamics Ltd."));
		param.AddRecord(new C3DRecordText(4, "FILE_EXPORT_SOFTWARE", "", context.ProgramName().c_str()));
		param.AddRecord(new C3DRecordText(4, "FILE_EXPORT_VERSION", "", context.ProgramVersion().c_str()));

    // merge any existing info which was created when this trial was imported as C3D
		param.MergeFrom(trial.C3D.Parameters);

		// remove DATA_START if it got merged in, as updated file data start is about to be added
		param.RemoveRecord("POINT", "DATA_START");

    // find total size of param section
		size_t total_record_bytes(0);

		// parameter header
		total_record_bytes += 4;  
	
    // dummy data start record 
    // - real one will be added when data start is calculated
    total_record_bytes += C3DRecordInteger(1, "DATA_START", "", 0).FileBytes(); 

		// parameter records
		total_record_bytes += param.FileBytes();
		
    // compute where to put data section, zero-based block
    // plus 1 is for header block
    size_t block_data = 1 + (total_record_bytes / 512);
    if ((total_record_bytes % 512) != 0)
      block_data++;

    // extra record indicating data start
    // - plus 1 is for 1-based fortran convention
    param.AddRecord(new C3DRecordInteger(1, "DATA_START", "", (Int16)(block_data+1)));

    // c3d header
    C3DMachine::WriteByte(0x02,os);
    C3DMachine::WriteByte(0x50,os);     

    // numbers of channels
    machine->WriteWord((UInt16)num_markers, os);  
    machine->WriteWord((UInt16)(num_analog*analog_per_marker), os);

    // 1-based data start and end time
    machine->WriteWord((UInt16)1,os); 
    machine->WriteWord((UInt16)num_frames,os);

    // interpolation gap - not used
		Int32 interp_gap = trial.C3D.InterpolationGap;
    machine->WriteWord((UInt16)interp_gap, os);

    // scale factor, -ve for floating pt
    if (c3doptions.FloatingPoint)
      machine->WriteFloat(-(float)scale_marker, os);
    else
      machine->WriteFloat((float)scale_marker, os);

    // 1-based data block location
    machine->WriteWord((UInt16)(block_data+1), os);

    // analog samples per frame
    machine->WriteWord((UInt16)analog_per_marker, os);

    // rate
    machine->WriteFloat((float)rateCommonMarker, os);

    // cout << "PositionA: " << os.tellp() << endl;

    // not used
    char blank[512];
    memset(blank, 0, 512);
    os.write(blank, 274);

    // support 4-char event labels
    machine->WriteWord(12345, os);

    // write num events
		EventArray events;
		const EventGroup* ef = trial.Acq.GetEventGroup("C3D Events");
		if (ef != NULL)
			ef->GetEvents(events);
		UInt16 num_events = events.NumEvents() >= 18 ? 18 : (UInt16)events.NumEvents();
    machine->WriteWord(num_events,os);

    // not used
    machine->WriteWord(0, os);

    if (num_events)
    {
      // event times
			UInt16 icountevent;
      for (icountevent = 0; icountevent < num_events; icountevent++)
				machine->WriteFloat((float)events.EventTime(icountevent) , os);
      os.write(blank,(18-num_events)*4);

      // event display on/off
      for (icountevent = 0; icountevent < num_events; icountevent++)
      {
				UInt8 off(0x00);
				const string& name = events.EventName(icountevent);
				if (name.size() > 4 && name.substr(4).compare(" (DISPLAY OFF") == 0)
					off = 0x01;

        C3DMachine::WriteByte(off,os);
      }
      os.write(blank,(18-num_events));
   
      // not used
      machine->WriteWord(0,os);

      // event labels
      for (icountevent = 0; icountevent < num_events; icountevent++)
			{
				const string& name = events.EventName(icountevent);
				if (name.size() < 4)
				{
					os.write(name.c_str(),(std::streamsize)name.size());
					for (size_t j = name.size(); j < 4; j++)
						os.put(' ');
				}
				else
				{
					os.write(name.c_str(),4);
				}
			}
      os.write(blank,(18-num_events)*4);
    }
    else
    {
      os.write(blank, 2+18*9);
    }

    // not used
    os.write(blank,44);

    // cout << "PositionB: " << os.tellp() << endl;

    // param header
		UInt16 param_header = (UInt16)trial.C3D.ParameterHeader.Value();
    machine->WriteWord((UInt16)param_header,os);
    C3DMachine::WriteByte((UInt8)param.NumRecords(), os);
    C3DMachine::WriteByte(machine->ProcessorID(), os);

    // write params
		param.ToFile(*machine, os);

    // fill to end of block
    size_t remainder = 512 - (total_record_bytes % 512);
    os.write(blank, (streamsize)remainder);

    // cout << "PositionC: " << os.tellp() << endl;

		// marker iterators
		vector<TSC3DConstIter> marker_iter;
		for (i = 0; i < num_markers; i++)
			marker_iter.push_back(TSC3DConstIter(*marker[i]));

		// analog iterators
		vector<TSScalarConstIter> analog_iter;
		for (i = 0; i < num_analog; i++)
			analog_iter.push_back( TSScalarConstIter(*analog[i]) );

		// write the data
    for (Int32 iframe = 0; iframe < num_frames; iframe++)
    {
			for (i = 0; i < num_markers; i++)
      {
       // marker measurement
        TSC3DConstIter& iter_ts = marker_iter[i];

        // make status word
        UInt16 status(0);

				// use camera data if exists
				if (iter_ts.HasCameraFlag())
					status = (UInt16)iter_ts.CameraFlag() << 8;

        // TODO: use eigenvalues to compute residual
				if (iter_ts.Occluded())
				{
					// sign bit set means data not valid
          status |= 0x8000;
				}
				else
				{
					// compute residual as fraction of marker scale
					const double* conf = iter_ts.Confidence();
					double dResidual = conf ? (sqrt(conf[0]*conf[0] + conf[1]*conf[1] + conf[2]*conf[2]) / scale_marker) : 0.0;

					// clamp to range [ 1 127 ]
					UInt16 wResidual;
					if (dResidual < 1.0)
						wResidual = 1;
					else if (dResidual > 127.0)
						wResidual = 0x7F;
					else
						wResidual = (UInt8)(dResidual + 0.5);

					// put in residual byte of status
					status |= wResidual;
				}

        if (c3doptions.FloatingPoint)
        {
          // write un-scaled floats
          machine->WriteFloat((float)iter_ts.Value()[0], os);
          machine->WriteFloat((float)iter_ts.Value()[1], os);
          machine->WriteFloat((float)iter_ts.Value()[2], os);
          machine->WriteFloat((float)((Int16)status),os);
        }
        else
        {
          // write scaled signed integers (use rounding)
          double m;
          m = iter_ts.Value()[0] / scale_marker;
          machine->WriteWord((UInt16)((m > 0.0) ? (Int16)(m+0.5) : (Int16)(m-0.5)), os);
          m = iter_ts.Value()[1] / scale_marker;
          machine->WriteWord((UInt16)((m > 0.0) ? (Int16)(m+0.5) : (Int16)(m-0.5)), os);
          m = iter_ts.Value()[2] / scale_marker;
          machine->WriteWord((UInt16)((m > 0.0) ? (Int16)(m+0.5) : (Int16)(m-0.5)), os);
          machine->WriteWord(status,os);
        }

				// incrment marker iterators
				iter_ts.Next();
      }

      // one or more sets of analog per marker frame
      for (Int32 analog_subframe = 0; analog_subframe < analog_per_marker; analog_subframe++)
      {
        // do each set
        for (i = 0; i < num_analog; i++)
        {
          // output array index
          Int32 ioutput = iframe*analog_per_marker+analog_subframe;
          Int32 iinput = ioutput / analog_subsample[i];
          Int32 isub = ioutput % analog_subsample[i];

					// get time sequence
					TSScalarConstIter& ts_iter = analog_iter[i];

					// check bounds
					if (iinput >= (Int32)ts_iter.NumFrames())
          {
            // gone over end (happens if marker and analog not quite same size)
            // - stay fixed at last known sample
						iinput =  (Int32)ts_iter.NumFrames() - 1;
          }

					// find value from data (interpolate if necessary)
					double value;
          if (isub == 0 || iinput == ( ts_iter.NumFrames()-1))
          {
            // at end of array - just use input frame
						ts_iter.SeekTo(iinput);
						value = ts_iter.Value();
          }
          else
          {
						ts_iter.SeekTo(iinput);
						double a0 = ts_iter.Value();
						ts_iter.Next();
						double a1 = ts_iter.Value();

						// do interpolation
            double lambda = (double)isub / (double)analog_subsample[i];
            value = (1.0-lambda)*a0 + lambda *a1;
          }

					// write
          if (c3doptions.FloatingPoint)
          {
            machine->WriteFloat((float)value, os);
          }
          else
          {
            Int16 a16 = (value > 0.0) ? (Int16)(value+0.5) : (Int16)(value-0.5);
            machine->WriteWord((UInt16)a16, os);
          }
        }
      }
    }
  }
}
