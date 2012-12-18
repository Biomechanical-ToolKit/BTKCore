/*--
Open3DMotion 
Copyright (c) 2004-2012.
All rights reserved.
See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/CODAText/FileFormatCODAText.h"
#include "Open3DMotion/MotionFile/Formats/CODAText/FileFormatOptionsCODAText.h"
#include "Open3DMotion/MotionFile/Formats/CODAText/MATextReader.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include <sstream>
#include <iomanip>

namespace Open3DMotion
{
	FileFormatCODAText::FileFormatCODAText() :
MotionFileFormat("CODAText", "Codamotion Analysis Text", "TXT")
{
}

FileFormatCODAText::~FileFormatCODAText()
{
}

bool FileFormatCODAText::Probe(const MotionFileHandler& /*context*/, TreeValue*& /*readoptions*/, std::istream& is) const throw(MotionFileException)
{
	// string id to find
	static const char* str_id = "CODA Motion Analysis Text";

	// compare string
	char buffID[26];
	is.getline(buffID, 26);
	return (strncmp(buffID, str_id, 26) == 0);
}

TreeValue* FileFormatCODAText::Read(const MotionFileHandler& /*context*/, std::istream& is, BinMemFactory& memfactory, const TreeValue* /*readoptions*/) const throw(MotionFileException)
{
	// read it in
	MATextReader reader;
	if (!reader.Read(is))
	{
		throw MotionFileException(MotionFileException::formaterror, "error reading text data");
	}

	// trial container
	Trial trial;

	// comment
	trial.UserInput.Comment = reader.Comment();

	// get names of all markers in the order they occur
	std::vector<std::string> marker_names;
	reader.GetChannelList3(marker_names, "Markers");

	// transfer markers
	for (std::vector<std::string>::const_iterator iter_marker_name( marker_names.begin() ); iter_marker_name != marker_names.end(); iter_marker_name++)
	{
		TimeSequence* ts = reader.GetTSOccVector3("Markers", iter_marker_name->c_str(), memfactory);
		if (ts != NULL)
		{
			// replace nonstandard group "Markers" with usual one for trial, "Marker"
			ts->Group = TrialSectionAcq::TSGroupMarker;

			// add to trial
			trial.Acq.TimeSequences.Add(ts);
		}
	}

	return trial.ToTree();
}

	void FileFormatCODAText::Write(const MotionFileHandler& /*context*/, const TreeValue* contents, std::ostream& os, const TreeValue* /*writeoptions*/) const throw(MotionFileException)
	{
		Trial trial;
		trial.FromTree(contents);

		// get details for all markers
		std::vector<const Open3DMotion::TimeSequence*> marker;
		trial.Acq.GetTSGroup(marker, Open3DMotion::TrialSectionAcq::TSGroupMarker);

		// must have some markers
		if (marker.empty())
			throw(MotionFileException(MotionFileException::formaterror, "No marker data present - marker data is required for CODAText format"));

		// even though istream is in binary mode, use DOS line endings for compatbility
		static const char* DOS_line_end = "\r\n";

		// format id line
		os << "CODA Motion Analysis Text" << DOS_line_end;

		// comment line
		const std::string& comment = trial.UserInput.Comment;
		if (comment.size())
			os << comment.c_str();
		os << DOS_line_end;

		// Note: always output 4 columns per marker: include Visibility (CodamotionAnalysis can output 3 or 4)

		// data type names
		os << "Time\tMarkers";
		size_t i;
		size_t num_markers = marker.size();
		for (i = 0; i < num_markers-1; i++)
			os << "\t\t\t\t";
		os << "\t\t\t" << DOS_line_end;

		// Channel names
		os << "Time\t";
		for (i = 0; i < num_markers; i++)
		{
			const std::string& name = marker[i]->Channel;
			os << name.c_str() << ".X\t";
			os << name.c_str() << ".Y\t";
			os << name.c_str() << ".Z\t";
			os << marker[i]->HardwareID.Value() << ".V";
			if (i < num_markers-1)
				os << "\t";
		}
		os << DOS_line_end;

		// units
		os << "(s)\t";
		for (i = 0; i < num_markers; i++)
		{
			// units for x, y, z
			const std::string& units = marker[i]->Units;
			for (size_t j = 0; j < 3; j++)
				os << "(" << units << ")\t";

			// blank entry for visiblity units (and newline at end)
			if (i < num_markers-1)
				os << "\t";
		}
		os << DOS_line_end;

		// motion data
		Int32 num_frames = marker[0]->NumFrames();
		float timeoffset = (float)marker[0]->Start;
		float timescale = (float)(1.0 / marker[0]->Rate);

		// iterators for all markers
		std::vector< TSOccVector3ConstIter > marker_iter;
		for (size_t imarker = 0; imarker < num_markers; imarker++)
			marker_iter.push_back(TSOccVector3ConstIter(*marker[imarker]));

		// loop over all frames
		for (Int32 iframe = 0; iframe < num_frames; iframe++)
		{
			// string buffer for this line
			std::ostringstream linestream(std::ios::binary);

			// format and show time
			float linetime = (float)(timeoffset + iframe*timescale);
			linestream << std::setprecision(3) << std::fixed << linetime << "\t";

			// all markers on this frame
			for (size_t imarker = 0; imarker < num_markers; imarker++)
			{
				// get marker info
				TSOccVector3ConstIter& m = marker_iter[imarker];

				// format 3 x floating point values (tab-separated)
				const double* v = m.Value();
				linestream 
					<< std::setprecision(3) << std::fixed << (float)v[0] << "\t"
					<< std::setprecision(3) << std::fixed << (float)v[1] << "\t"
					<< std::setprecision(3) << std::fixed << (float)v[2] << "\t";

				// visibility flag
				linestream << (m.Occluded() ? "0" : "1");

				// tab (unless last marker in sequence)
				if (imarker < num_markers-1)
				{
					linestream << "\t";
				}

				// increment iterator
				m.Next();
			}

			// write end of line
			linestream << DOS_line_end;

			// number of bytes to write
			size_t bytes_to_write = size_t(linestream.tellp());

			// commit line to file
			os.write(linestream.str().c_str(), bytes_to_write);

		}

	}

}
