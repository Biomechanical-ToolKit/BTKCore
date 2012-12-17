/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/CODAText/MATextReader.h"
#include "Open3DMotion/MotionFile/Formats/CODAText/MATextInputStream.h"

namespace Open3DMotion
{
	MATextReader::MATextReader()
	{
	}

	bool MATextReader::Read(std::istream& is)
	{
	  if (is.fail())
      return false;
	  
		MATextInputStream tf(is);

		// file format descriptor line
		tf.SkipLine();	

		// comment/title line
		std::vector<std::string> comment_line;
		tf.ReadTextRow(comment_line);	
		if (comment_line.size())
			comment = comment_line[0];

		// column headings
		std::vector<std::string> group, channel, units;
		tf.ReadTextRow(group);
		tf.ReadTextRow(channel);
		tf.ReadTextRow(units);

		// must have same number of headings for all column-heading rows
		size_t numcolumns = group.size();
		if (numcolumns != channel.size())
			return false;
		if (numcolumns != units.size())
			return false;

		// time headings must be first
		if ((!group.empty()) && (group[0].compare("Time") != 0))
			return false;
		if ((!channel.empty()) && channel[0].compare("Time") != 0)
			return false;

		// copy to array
		std::string groupname;
		for (size_t i = 0; i < numcolumns; i++)
		{
			// use prev group name if none here
			if (group[i].length())
				groupname = group[i];
		
			// remove brackets from units name
			std::string unitsname;
			if (units[i].length() >= 2 && units[i][0] == '(' && units[i][units[i].length()-1] == ')')
				unitsname = units[i].substr(1, units[i].length()-2);
			else
				unitsname = units[i];

			// channel name as-is
			std::string channelname = channel[i];

			// add to array
			heading.push_back(MATextColHeading(groupname, channelname, unitsname));
		}

    if (numcolumns != 0)
    {
    	// read data
    	while (tf.ReadDataRow(data, numcolumns))
    		;

    	// must have complete rows
    	if (data.size() % numcolumns != 0)
    		return false;
    }
    
		return true;
	}

	size_t MATextReader::ColumnIndex(const char* groupname, const char* channelname) const
	{
		std::vector<MATextColHeading>::const_iterator iter_heading;
		size_t index;
		for (iter_heading = heading.begin(), index = 0; iter_heading != heading.end(); index++, iter_heading++)
		{
			if (_stricmp(iter_heading->group.c_str(), groupname) == 0 &&
					_stricmp(iter_heading->channel.c_str(), channelname) == 0)
					return index;
		}

		return invalid_index;
	}

	void MATextReader::GetChannelList3(std::vector<std::string>& channels, const char* groupname) const
	{
		for (std::vector<MATextColHeading>::const_iterator iter_heading = heading.begin(); 
					iter_heading != heading.end(); 
					iter_heading++)
		{
			// Match all columns with this group name
			if (_stricmp(iter_heading->group.c_str(), groupname) == 0)
			{
				// Match only channels with a ".Z" suffix for 3D data
				const std::string& h = iter_heading->channel;
				if (h.substr(h.size()-2) == std::string(".Z"))
				{
					// Remove ".Z" suffix to get name
					std::string c = h.substr(0, h.size()-2);
					channels.push_back(c);
				}
			}
		}
	}

	TimeSequence* MATextReader::GetTSScalar(const char* groupname, const char* channelname, BinMemFactory& memfactory) const
	{
		size_t index = ColumnIndex(groupname, channelname);
		if (index == invalid_index)
		{
			return NULL;
		}
		else
		{
			// make time sequence
			TimeRange tr;
			tr.Rate = RateHz();
			tr.Start = StartTime();
			tr.Frames = NumFrames();		
			TimeSequence* ts = TSFactoryValue(1).New(tr, memfactory);

			// copy channel info and units
			const MATextColHeading& col = heading[index];
			ts->Group = groupname;
			ts->Channel = channelname;
			ts->Units = col.units;

			// copy data
			const double* iter_input = &data[index];
			TSScalarIter iter_output( *ts );
			size_t input_stride = NumColumns();
			for ( ; iter_output.HasFrame(); iter_input+=input_stride, iter_output.Next())
			{
				iter_output.Value() = *iter_input;
			}

			return ts;
		}
	}


	TimeSequence* MATextReader::GetTSOccVector3(const char* groupname, const char* channelname, BinMemFactory& memfactory) const
	{
		// expected channel col names
		std::string channel_x = std::string(channelname) + ".X";
		std::string channel_y = std::string(channelname) + ".Y";
		std::string channel_z = std::string(channelname) + ".Z";

		// get index for x
		size_t index_x = ColumnIndex(groupname, channel_x.c_str());	
		if (index_x == invalid_index)
			return NULL;

		// assumed indices for y, z, v
		size_t index_y = index_x + 1;
		size_t index_z = index_x + 2;

		// subsequent cols must match .Y and .Z endings
		if ((index_z >= heading.size()) || 
				(_stricmp(heading[index_y].channel.c_str(), channel_y.c_str()) != 0) ||
				(_stricmp(heading[index_z].channel.c_str(), channel_z.c_str()) != 0))
				return NULL;

		// visibility is optional depending on whether there is a .V column
		// and may contain a hardware ID number
		bool visibility = false;
		size_t index_v = invalid_index;
		int hardware_id = 0;

		// see if exists before data channels as when graphs are saved
		// - in this case must contain name
		if (index_x > 0)
		{
			const std::string& c = heading[index_x-1].channel;
			std::string channel_v = std::string(channelname) + ".V";
			if (_stricmp(c.c_str(), channel_v.c_str()) == 0)
			{
				visibility = true;
				index_v = index_x - 1;
			}
		}

		// see if exists after data channels as when whole trials are saved
		// - in this case usually contains hardware ID
		if (!visibility && ((index_x+3) < heading.size()))
		{
			const std::string& c = heading[index_x+3].channel;
			if (c.substr(c.size()-2) == std::string(".V"))
			{
				// set visibility on
				visibility = true;
				index_v = index_x + 3;

				// remember hardware id if exists
				sscanf(c.c_str(), " %d.V", &hardware_id);
			}
		}

		// X col heading for units info
		const MATextColHeading& col_x = heading[index_x];

		// make time sequence
		TimeRange tr;
		tr.Rate = RateHz();
		tr.Start = StartTime();
		tr.Frames = NumFrames();		
		TimeSequence* ts = TSFactoryOccValue(3).New(tr, memfactory);

		// copy channel info and units
		ts->Group = groupname;
		ts->Channel = channelname;
		ts->Units = col_x.units;

		// hardware id if available
		if (hardware_id)
			ts->HardwareID = (Int32)hardware_id;

		// copy data
		const double* iter_input = &data[0];
		TSOccVector3Iter iter_output( *ts );
		size_t input_stride = NumColumns();
		for ( ; iter_output.HasFrame(); iter_input+=input_stride, iter_output.Next())
		{
			iter_output.Value()[0] = iter_input[index_x];
			iter_output.Value()[1] = iter_input[index_y];
			iter_output.Value()[2] = iter_input[index_z];
			iter_output.Occluded() = (visibility && iter_input[index_v] < 0.001) ? 1 : 0;
		}

		return ts;
	}

	double MATextReader::RateHz() const
	{
		if (NumColumns() < 1 || NumFrames() < 2)
			return 0.0;
	
		double t0 = data[0];
		double t1 = data[NumColumns()];
		double dt = t1 - t0;
		double rateHz = 1.0 / dt;
		return rateHz;
	}

	double MATextReader::StartTime() const
	{
		if (data.size() < 1)
			return 0.0;

		return data[0];
	}
}