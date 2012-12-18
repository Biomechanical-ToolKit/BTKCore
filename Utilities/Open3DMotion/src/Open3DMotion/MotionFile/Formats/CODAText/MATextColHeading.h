/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MA_TEXT_COL_HEADING_H_
#define _OPEN3DMOTION_MA_TEXT_COL_HEADING_H_

#include <string>

namespace Open3DMotion
{
	class MATextColHeading
	{
	public:
		MATextColHeading()
		{
		}

		MATextColHeading(const std::string& ngroup, const std::string& nchannel, const std::string& nunits) :
			group(ngroup), channel(nchannel), units(nunits)
		{
		}


	public:
		std::string group;
		std::string channel;
		std::string units;
	};

}

#endif
