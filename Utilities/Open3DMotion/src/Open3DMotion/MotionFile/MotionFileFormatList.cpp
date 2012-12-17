/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "MotionFileFormatList.h"
#include "MotionFileFormat.h"

namespace Open3DMotion
{
	MotionFileFormatList::MotionFileFormatList() :
		default_format(NULL)
	{
	}

	MotionFileFormatList::~MotionFileFormatList()
	{
		for (std::map<std::string, MotionFileFormat*>::iterator i( formatmap.begin() ); i != formatmap.end(); i++)
		{
			MotionFileFormat* format = i->second;
			delete format;
		}
		formatmap.clear();
	}

	void MotionFileFormatList::Register(MotionFileFormat* format)
	{
		// ensure one default format
		if (default_format == NULL)
		{
			default_format = format;
		}

		// put into map
		formatmap[format->FormatID()] = format;
	}

	MotionFileFormat* MotionFileFormatList::Find(const std::string& formatid) const
	{
		std::map<std::string, MotionFileFormat*>::const_iterator iter = formatmap.find(formatid);
		return (iter != formatmap.end()) ? iter->second : NULL;
	}

}