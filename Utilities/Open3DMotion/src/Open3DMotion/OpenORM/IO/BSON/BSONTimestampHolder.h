/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_TIMESTAMP_HOLDER_H_
#define _ORMPP_BSON_TIMESTAMP_HOLDER_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"

namespace Open3DMotion
{
	/** Used inside  BSONTimestampHolder as representation of the components of BSON's timestamp object */
	class BSONTimestampData : public MapCompound
	{
	public:
		BSONTimestampData();

	public:
		MapInt32 Increment;
		MapInt32 Seconds;
	};

	/** Representation of BSON's 8-byte timestamp object */
	class BSONTimestampHolder : public MapCompound
	{
	public:
		BSONTimestampHolder();

	public:
		BSONTimestampData BSONTimestamp;
	};
}

#endif
