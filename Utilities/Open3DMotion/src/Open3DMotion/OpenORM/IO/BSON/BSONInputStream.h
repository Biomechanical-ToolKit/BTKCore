/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_INPUT_STREAM_H_
#define _ORMPP_BSON_INPUT_STREAM_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReadException.h"

namespace Open3DMotion
{
	/** Abstract base class for (forward-only) reading from binary streams */
	class BSONInputStream
	{
	public:
		virtual ~BSONInputStream();

		/** Skip forwards by a specified number of bytes 
		    @param count number of bytes to skip
		 */
		virtual void SkipBytes(UInt32 count)  throw(BSONReadException) = 0;

		/** Read a specified amount of data 
		    @param binary pointer to buffer of required size to hold data
				@param size the amount of data to read, in bytes
		  */
		virtual void ReadBinary(void* binary, UInt32 size)  throw(BSONReadException) = 0;

		/** Discover whether there are more bytes available for subsequent read or skip operations
		    @return true if more bytes, false otherwise
			*/
		virtual bool HaveMore() = 0;

	};

}

#endif
