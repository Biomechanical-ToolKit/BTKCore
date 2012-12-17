/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "FileFormatMoXie.h"
#include "FileFormatOptionsMoXie.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceMeasurements.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculatorFactory.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculator.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <pugixml.hpp>
#include <float.h>
#include <sstream>
#include <iomanip>

namespace Open3DMotion
{
	const char* moxie_xml_doc = "moxie_viewer_datafile";

	const char* moxie_timecode = "Timecode";

	const char* moxie_force_names[6] =
	{
		"FP - Force X",
		"FP - Force Y",
		"FP - Force Z",
		"FP - Moment X",
		"FP - Moment Y",
		"FP - Moment Z"
	};

	const double moxie_force_to_tree[6] =
	{
		1.0,
		1.0,
	 -1.0,
	-1000.0,
	-1000.0,
	-1000.0
	};

  FileFormatMoXie::FileFormatMoXie() :
		MotionFileFormat("MoXie", "MoXie", "mox")
	{
	}

	FileFormatMoXie::~FileFormatMoXie()
	{
	}
	
			// examine stream for xmove info
  bool FileFormatMoXie::Probe(const MotionFileHandler& /*context*/, TreeValue*& /*readoptions*/, std::istream& is)  const throw(MotionFileException)
  {
    return ProbeTextString(is, moxie_xml_doc, 1024);
  }

  TreeValue* FileFormatMoXie::Read(const MotionFileHandler& /*context*/, std::istream& is, BinMemFactory& memfactory, const TreeValue* /*readoptions*/) const throw(MotionFileException)
  {
		// trial to fill
		std::auto_ptr<Trial> trial(new Trial);

		// load
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(is);
		if (!result)
		{
			throw MotionFileException(MotionFileException::formaterror, result.description());
		}

		// moxie section
		pugi::xml_node moxie = doc.child(moxie_xml_doc);
		if (!moxie)
		{
			throw MotionFileException(MotionFileException::formaterror, "missing moxie_xml_doc");
		}

		// find header
		pugi::xml_node header = moxie.child("viewer_header");

		// must have xmove node
		if (!header)
		{
			throw MotionFileException(MotionFileException::formaterror, "missing viewer_header");
		}

		// find header
		pugi::xml_node data = moxie.child("viewer_data");

		// must have xmove node
		if (!data)
		{
			throw MotionFileException(MotionFileException::formaterror, "missing viewer_data");
		}

		// optional patient id
		const char* patient_id = header.child_value("patient_id");
		if (patient_id && strlen(patient_id))
		{
			trial->UserInput.Subject.ID = patient_id;
		}

		// optional trial date
		const char* trial_date = header.child_value("trial_date");
		if (trial_date)
		{
			int year(0), month(0), day(0);
			if (sscanf(trial_date, " %d-%d-%d ", &year, &month, &day) == 3)
			{
				trial->Acq.Date.Year = year;
				trial->Acq.Date.Month = month;
				trial->Acq.Date.Day = day;
			}
		}

		// optional trial number
		const char* trial_number = header.child_value("trial_number");
		if (trial_number && strlen(trial_number))
		{
			trial->UserInput.RepLabel = trial_number;
		}

		// optional comments
		const char* comments = header.child_value("comments");
		if (comments && strlen(comments))
		{
			trial->UserInput.Comment = comments;
		}

		// optional trial condition
		const char* trial_condition = header.child_value("trial_condition");
		if (trial_condition && strlen(trial_condition))
		{
			trial->UserInput.TrialCondition = trial_condition;
		}

		// optional DLT info
		pugi::xml_node coords = header.child("coordinate_transformations");
		if (coords)
		{
			for (pugi::xml_node dlt = coords.child("dlt_matrix"); dlt; dlt = dlt.next_sibling("dlt_matrix"))
			{
				const char* dlt_string = dlt.child_value();
				if (dlt_string != NULL)
				{
					double p[12];
					int numparsed = sscanf(dlt_string, " %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
							&p[0], &p[1], &p[2], &p[3], &p[4], &p[5], &p[6], &p[7], &p[8], &p[9], &p[10], &p[11] );
					if (numparsed == 12)
					{		
						std::auto_ptr<TrialVideoCamera> camera (new TrialVideoCamera);
						for (int i = 0; i < 12; i++)
							camera->Calibration.ProjectionMatrix.Add(p[i]);
						trial->Acq.Video.Add( camera.release() );
					}
				}
			}
		}

		// add force channels only
		size_t numforcefound(0);
		for (size_t ichannel = 0; ichannel < 6; ichannel++)
		{
			// find a node corresponding to this channel
			pugi::xml_node forcechannel;
			const char* channelname = moxie_force_names[ichannel];
			for (pugi::xml_node channel = data.child("viewer_channel"); channel; channel = channel.next_sibling("viewer_channel"))
			{
				const char* channel_label = channel.child_value("channel_label");
				if (channel_label && strcmp(channelname, channel_label) == 0)
				{
					forcechannel = channel;
					break;
				}
			}

			// process the data
			if (forcechannel)
			{
				pugi::xml_node rawchannel = forcechannel.child("raw_channel_data");
				if (rawchannel)
				{
					const char* freq = rawchannel.child_value("sampling_frequency");
					const char* sequence = rawchannel.child_value("channel_data");
					if (freq && sequence)
					{
						double rateHz(0.0);
						if (sscanf(freq, " %lf ", &rateHz) == 1)
						{
							std::list<double> sequence_list;
							std::istringstream channelstream(sequence);
							while (true)
							{
								double x(0.0);
								channelstream >> x;
								if (channelstream.fail())
									break;
								sequence_list.push_back(x);
							}

							if (sequence_list.size())
							{
								TimeRange tr;
								tr.Rate = rateHz;
								tr.Frames = sequence_list.size();
								tr.Start = 0.0;

								TimeSequence* ts ( TSFactoryValue(1).New(tr, memfactory) );

								ts->Group = TrialSectionAcq::TSGroupAnalog;
								ts->Channel = channelname;
								ts->HardwareID = ichannel + 1;
								ts->Units = (ichannel < 3) ? "N" : "Nmm";

								// this is applied to convert data to units convention used on tree
								// but the actual analog scale factor is just set to one (above)
								double scale = moxie_force_to_tree[ichannel];

								std::list<double>::const_iterator iter_x = sequence_list.begin();
								TSScalarIter iter_ts(*ts);
								for (; iter_x != sequence_list.end(); iter_x++, iter_ts.Next())
								{
									iter_ts.Value() = scale * *iter_x;
								}
								
								trial->Acq.TimeSequences.Add( ts );

								numforcefound++;
							}
						}
					}
				}
			}
		}

		// add canonical strain gauge plate
		if (numforcefound == 6)
		{
			ForcePlate fp;
			fp.Type = ForcePlate::TypeAMTI;
			fp.Model = "MoXie";
			fp.CentreOffset.X = 0;
			fp.CentreOffset.Y = 0;
			fp.CentreOffset.Z = 0;
			fp.Outline[0].X = -250.0; 
			fp.Outline[0].Y = -250.0; 
			fp.Outline[0].Z =    0.0; 
			fp.Outline[1].X =  250.0; 
			fp.Outline[1].Y = -250.0; 
			fp.Outline[1].Z =    0.0; 
			fp.Outline[2].X =  250.0; 
			fp.Outline[2].Y =  250.0; 
			fp.Outline[2].Z =    0.0; 
			fp.Outline[3].X = -250.0; 
			fp.Outline[3].Y =  250.0; 
			fp.Outline[3].Z =    0.0; 
			fp.Channels.Add(1);
			fp.Channels.Add(2);
			fp.Channels.Add(3);
			fp.Channels.Add(4);
			fp.Channels.Add(5);
			fp.Channels.Add(6);
			trial->Acq.ForcePlates.Add(fp);
		}

		return trial->ToTree();
  }

  void FileFormatMoXie::Write(const MotionFileHandler& /*context*/, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
  {
		// parse to trial
		std::auto_ptr<Trial> trial( new Trial );
		trial->FromTree(contents);

		// read options
		FileFormatOptionsMoXie moxie_options;
		moxie_options.FromTree(writeoptions);

		pugi::xml_document doc;
		pugi::xml_node moxie = doc.append_child(moxie_xml_doc);
		moxie.append_attribute("xmlns") = "http://www.smalll.nl/vumc/rev/moxie";
		moxie.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
		moxie.append_attribute("xsi:schemaLocation") = "http://www.smalll.nl/vumc/rev/moxie/moxie_viewer.xsd";

		pugi::xml_node viewer_header = moxie.append_child("viewer_header");
		pugi::xml_node viewer_data = moxie.append_child("viewer_data");

		// filename is compulsory in header according to schema
		std::string outputfilename;
		ExtractFileName(outputfilename, moxie_options.PathName);
		viewer_header.append_child("filename").text().set(outputfilename.c_str());

		// videofile section is compulsory in header according to schema, and should also contain at least one filename
		pugi::xml_node videofile = viewer_header.append_child("videofile");

		// set VITC start time as zero
		pugi::xml_node vitc_start = viewer_header.append_child("vitc_start");
		vitc_start.append_child("vitc").text().set("0");

		// subject id (patient id)
		if ( trial->UserInput.Subject.ID.IsSet() )
		{
			viewer_header.append_child("patient_id").text().set( trial->UserInput.Subject.ID );
		}

		// session date (trial date)
		if (trial->Acq.Date.IsSet())
		{
			// char szDate[256];
			// sprintf(szDate, "%04d-%02d-%02d", trial->Acq.Date.Year.Value(), trial->Acq.Date.Month.Value(), trial->Acq.Date.Day.Value());

			const TrialDate& date = trial->Acq.Date;
			std::ostringstream strdate;
			strdate 
				<< std::setfill('0') << std::setw(4) << date.Year.Value()
				<< '-'
				<< std::setfill('0') << std::setw(2) << date.Month.Value()
				<< '-'
				<< std::setfill('0') << std::setw(2) << date.Day.Value();
			viewer_header.append_child("trial_date").text().set(strdate.str().c_str());
		}

		// repetition label (trial number)
		if (trial->UserInput.RepLabel.IsSet())
		{
			viewer_header.append_child("trial_number").text().set(trial->UserInput.RepLabel);
		}

		// user comments
		if (trial->UserInput.Comment.IsSet())
		{
			viewer_header.append_child("comments").text().set(trial->UserInput.Comment);
		}

		// trial condition
		if (trial->UserInput.TrialCondition.IsSet())
		{
			viewer_header.append_child("trial_condition").text().set(trial->UserInput.TrialCondition);
		}

		// optional coord transforms section
		// created on-demand only, as schema requires it has at least one element if it exists
		std::string videofilenamearray;
		if (trial->Acq.Video.NumElements() >= 1 && trial->Acq.Video[0].Calibration.ProjectionMatrix.NumElements() == 12)
		{
			pugi::xml_node coordinate_transformations = viewer_header.append_child("coordinate_transformations");
			for (size_t icamera = 0; icamera < trial->Acq.Video.NumElements(); icamera++)
			{
				const TrialVideoCamera& camera = trial->Acq.Video[icamera];
				if (camera.Calibration.ProjectionMatrix.NumElements() == 12)
				{
					// make one string with all file names
					if (camera.VideoSpec.VideoFilePattern.IsSet() && camera.VideoSpec.VideoFilePattern.Value().size())
					{
						// asterisk separator between filenames
						if (videofilenamearray.size())
							videofilenamearray += "*";

						// append this one
						videofilenamearray += camera.VideoSpec.VideoFilePattern.Value();
					}


					// make matrix
					std::ostringstream paramstream;
					for (int i = 0; i < 12; i++)
					{
						// spaces in between
						if (i > 0)
							paramstream << " ";

						// ensure normalised
						double normparam = camera.Calibration.ProjectionMatrix[i] / camera.Calibration.ProjectionMatrix[11];
						
						// put into stream
						paramstream << std::setprecision(12) << normparam;
					}
					paramstream << std::ends;

					// append
					coordinate_transformations.append_child("dlt_matrix").text().set(paramstream.str().c_str());
				}
			}
		}

		// default filename, since this is compulsory
		if (videofilenamearray.size() == 0)
			videofilenamearray = "blank.avi";

		// set filename value
		videofile.text().set(videofilenamearray.c_str());

		// Convert plate 1 to canonical strain gauge style
		if (trial->Acq.ForcePlates.NumElements() >= 1)
		{
			// create calculator object for this plate, if supported
			const ForcePlate& fp = trial->Acq.ForcePlates[0];
			std::auto_ptr<ForceCalculator> calculator( ForceCalculatorFactory().CreateCalculator(fp) );

			if (calculator.get() != NULL)
			{
				// retrieve all analog sequences
				std::vector<const TimeSequence*> analog_all;
				trial->Acq.GetTSGroup(analog_all, TrialSectionAcq::TSGroupAnalog);

				// map to the ones applicable to this plate
				ForceMeasurementTimeSequence mts;
				mts.Set(fp, analog_all);

				// compute force
				TimeSequence* force(NULL);
				TimeSequence* point(NULL);
				TimeSequence* freemoment(NULL);
        BinMemFactoryDefault memfactory_default;
				if (calculator->Compute(force, point, freemoment, mts, memfactory_default))
				{
					// reverse compute canonical strain gauge force values 
					
					// time range for computation
					TimeRange tr;
					tr.Start = 0.0;
					tr.Frames = mts.NumFrames();
					tr.Rate = mts.Rate();

					// sequences for results in canonical coords
					std::auto_ptr<TimeSequence> timecode( TSFactoryValue(1).New(tr, memfactory_default) );
					std::auto_ptr<TimeSequence> Fx( TSFactoryValue(1).New(tr, memfactory_default) );
					std::auto_ptr<TimeSequence> Fy( TSFactoryValue(1).New(tr, memfactory_default) );
					std::auto_ptr<TimeSequence> Fz( TSFactoryValue(1).New(tr, memfactory_default) );
					std::auto_ptr<TimeSequence> Mx( TSFactoryValue(1).New(tr, memfactory_default) );
					std::auto_ptr<TimeSequence> My( TSFactoryValue(1).New(tr, memfactory_default) );
					std::auto_ptr<TimeSequence> Mz( TSFactoryValue(1).New(tr, memfactory_default) );

					// iterators
					TSVector3ConstIter iter_force( *force );
					TSVector3ConstIter iter_point( *point );
					TSVector3ConstIter iter_freemoment( *freemoment );
					TSScalarIter iter_timecode( *timecode );
					TSScalarIter iter_Fx( *Fx );
					TSScalarIter iter_Fy( *Fy );
					TSScalarIter iter_Fz( *Fz );
					TSScalarIter iter_Mx( *Mx );
					TSScalarIter iter_My( *My );
					TSScalarIter iter_Mz( *Mz );

					// do computation
					while (iter_timecode.HasFrame())
					{
						// current timecode is latest 25Hz field boundary
						size_t vitc = (size_t)((double)iter_timecode.FrameIndex() * 25.0 / tr.Rate);
						iter_timecode.Value() = (double)vitc;

						// For force, forward formula looks like:
						// force[0] = -Fx
						// force[1] = -Fy
						// force[2] = -Fz
						// point[0] = -(My+Model().CentreOffset.Z*Fx)/Fz+Model().CentreOffset.X
						// point[1] =  (Mx-Model().CentreOffset.Z*Fy)/Fz+Model().CentreOffset.Y
						// point[2] = 0
						// freemoment_z = -(Mz - Fy*point[0] + Fx*point[1]);

						// with zero centre offset the point values are:
						// point[0] = -My/Fz
						// point[1] =  Mx/Fz
						
						// and so inverse would be:
						// Fx = -force[0]
						// Fy = -force[1]
						// Fz = -force[2]
						// Mx =  point[1]*Fz
						// My = -point[0]*Fz
						// Mz = -freemoment_z + Fy*point[0] - Fx*point[1]

						// Assume free moment pure vertical hence unaffected by plate transform
						// Is acceptable as MoXie doesn't really support non-horizontal plates anyway
						double freemoment_z = iter_freemoment.Value()[2];

						// compute inverse transform
						iter_Fx.Value() = -iter_force.Value()[0];
						iter_Fy.Value() = -iter_force.Value()[1];
						iter_Fz.Value() = -iter_force.Value()[2];
						iter_Mx.Value() =  iter_point.Value()[1] * iter_Fz.Value();
						iter_My.Value() = -iter_point.Value()[0] * iter_Fz.Value();
						iter_Mz.Value() = -freemoment_z + iter_Fy.Value()*iter_point.Value()[0] - iter_Fx.Value()*iter_point.Value()[1];

						// scale using MoXie convention
						iter_Fx.Value() /= moxie_force_to_tree[0];
						iter_Fy.Value() /= moxie_force_to_tree[1];
						iter_Fz.Value() /= moxie_force_to_tree[2];
						iter_Mx.Value() /= moxie_force_to_tree[3];
						iter_My.Value() /= moxie_force_to_tree[4];
						iter_Mz.Value() /= moxie_force_to_tree[5];

						// increment iterators
						iter_force.Next();
						iter_point.Next();
						iter_freemoment.Next();
						iter_timecode.Next();
						iter_Fx.Next();
						iter_Fy.Next();
						iter_Fz.Next();
						iter_Mx.Next();
						iter_My.Next();
						iter_Mz.Next();
					}

					// there are 7 channels to add in total (one of timecode, six of force data)
					std::ostringstream str_seven;
					str_seven << 7 << std::ends;
					viewer_header.append_child("nr_of_channels").text().set(str_seven.str().c_str());

					// add the data
					AddMoXieChannel(viewer_data, moxie_timecode, 0, timecode.get());
					AddMoXieChannel(viewer_data, moxie_force_names[0], 2, Fx.get());
					AddMoXieChannel(viewer_data, moxie_force_names[1], 2, Fy.get());
					AddMoXieChannel(viewer_data, moxie_force_names[2], 2, Fz.get());
					AddMoXieChannel(viewer_data, moxie_force_names[3], 2, Mx.get());
					AddMoXieChannel(viewer_data, moxie_force_names[4], 2, My.get());
					AddMoXieChannel(viewer_data, moxie_force_names[5], 2, Mz.get());

					// manually delete these parts
					delete force;
					delete point;
					delete freemoment;
				}
			}
		}
			
		// write to output
		doc.save(os);
	}

	void FileFormatMoXie::AddMoXieChannel(pugi::xml_node& viewer_data, const char* channelname, int decimalplaces, TimeSequence* ts)
	{
		pugi::xml_node viewer_channel = viewer_data.append_child("viewer_channel");
		viewer_channel.append_child("channel_label").text().set(channelname);
		pugi::xml_node raw_channel_data = viewer_channel.append_child("raw_channel_data");
		std::ostringstream freq;
		freq << std::setiosflags(std::ios::fixed) << std::setprecision(2) << ts->Rate << std::ends;
		raw_channel_data.append_child("sampling_frequency").text().set(freq.str().c_str());

		// make string of all channel values
		std::ostringstream channelstream;
		for (TSScalarConstIter iframe( *ts ); iframe.HasFrame(); iframe.Next())
		{
			channelstream << " " << std::setiosflags(std::ios::fixed) << std::setprecision(decimalplaces) << iframe.Value();
		}
		channelstream << std::ends;

		// append
		raw_channel_data.append_child("channel_data").text().set(channelstream.str().c_str());
	}

	void FileFormatMoXie::ExtractFileName(std::string& filename, const std::string& fullpath)
	{
		size_t index_slash = fullpath.find_last_of("/\\");
		filename = (index_slash != std::string::npos) ? fullpath.substr(index_slash+1) : fullpath;
	}
}
