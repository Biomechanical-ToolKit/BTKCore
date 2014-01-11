/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_OBJECT_ID_HOLDER_H_
#define _ORMPP_BSON_OBJECT_ID_HOLDER_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"

namespace Open3DMotion
{
	/** Constant used by BSON's object ID's - always 12 bytes */
	const size_t BSONObjectIdBytes = 12;

	/** Conventient fixed-size array for BSON 12-byte Object Id's */
	typedef UInt8 BSONObjectIdBinary[BSONObjectIdBytes];

	/** A representation of BSON's 12-byte ObjectId values */
	class BSONObjectIdHolder : public MapCompound
	{
	public:
		BSONObjectIdHolder();

		/** Convert from 12-byte binary representation 
		    @param binary representation 
			*/
		void FromBinary(const BSONObjectIdBinary& binary);

		/** Convert to 12-byte binary representation
		    @param the binary representation to be filled  */
		void ToBinary(BSONObjectIdBinary& binary) const;

	public:
		MapString BSONObjectId;
	};
}

#endif
