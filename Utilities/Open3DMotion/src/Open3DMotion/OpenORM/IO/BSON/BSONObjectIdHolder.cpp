/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONObjectIdHolder.h"
#include <sstream>
#include <iomanip>
#include <stdio.h>

namespace Open3DMotion
{
	BSONObjectIdHolder::BSONObjectIdHolder()
	{
		REGISTER_MEMBER(BSONObjectId);
	}

	void BSONObjectIdHolder::FromBinary(const BSONObjectIdBinary& binary)
	{
		std::ostringstream buffer;
		for (size_t i = 0; i < BSONObjectIdBytes; i++)
		{
			buffer << std::setfill('0') << std::setw(2) << std::hex << (int)binary[i];
		}
		buffer << std::ends;
		BSONObjectId.Value() = buffer.str().c_str();
	}

	void BSONObjectIdHolder::ToBinary(BSONObjectIdBinary& binary) const
	{
		for (size_t i = 0; i < BSONObjectIdBytes; i++)
		{
			size_t index = 2*i;
			int x(0);
			if (index < BSONObjectId.Value().size())
			{
				sscanf(&BSONObjectId.Value()[index], "%02x", &x);
			}
			binary[i] = (UInt8)x;
		}
	}

}
