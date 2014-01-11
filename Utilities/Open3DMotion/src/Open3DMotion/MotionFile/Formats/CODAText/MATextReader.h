/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPTION3DMOTION_MA_TEXT_READER_H_
#define _OPTION3DMOTION_MA_TEXT_READER_H_

#include "Open3DMotion/MotionFile/Formats/CODAText/MATextColHeading.h"
#include "Open3DMotion/MotionFile/Formats/CODAText/MATextInputStream.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

#include <vector>
#include <string>
#include <istream>

namespace Open3DMotion
{
	class MATextReader
	{
	public:
		MATextReader();

		bool Read(std::istream& is);

		TimeSequence* GetTSScalar(const char* groupname, const char* channelname, const BinMemFactory& memfactory) const;

		TimeSequence* GetTSOccVector3(const char* groupname, const char* channelname, const BinMemFactory& memfactory) const;

		void GetChannelList3(std::vector<std::string>& channels, const char* groupname) const;

		const std::string& Comment() const
		{ return comment; }

		size_t NumFrames() const
		{ return (NumColumns() > 0) ? (data.size() / NumColumns()) : 0; }

		double RateHz() const;

		double StartTime() const;

	protected:

		const static size_t invalid_index = 0xFFFFFFFFUL;

		size_t ColumnIndex(const char* groupname, const char* channelname) const;

		size_t NumColumns() const
		{ return heading.size(); }

	protected:
		std::string comment;
		std::vector<double> data;
		std::vector<MATextColHeading> heading;
	};

}

#endif
