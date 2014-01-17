/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MA_TEXT_INPUT_STREAM_H_
#define _OPEN3DMOTION_MA_TEXT_INPUT_STREAM_H_

#include "Open3DMotion/OpenORM/Types.h"
#include <istream>
#include <string>
#include <vector>

namespace Open3DMotion
{
	class MATextInputStream
	{
	public:
		MATextInputStream(std::istream& _is);

		void SkipLine();

		void ReadTextRow(std::vector<std::string>& text);

		bool ReadDataRow(std::vector<double>& data, size_t count);

	protected:
		std::istream& is;
	};
}

#endif