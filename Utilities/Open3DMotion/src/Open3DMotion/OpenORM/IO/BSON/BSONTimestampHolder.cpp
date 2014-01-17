/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONTimestampHolder.h"

namespace Open3DMotion
{
	BSONTimestampData::BSONTimestampData()
	{
		REGISTER_MEMBER(Increment);
		REGISTER_MEMBER(Seconds);
	}

	BSONTimestampHolder::BSONTimestampHolder()
	{
		REGISTER_MEMBER(BSONTimestamp);
	}
}
