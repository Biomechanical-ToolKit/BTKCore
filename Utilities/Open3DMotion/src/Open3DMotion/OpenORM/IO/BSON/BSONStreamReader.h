/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _BSON_STREAM_READER_H_
#define _BSON_STREAM_READER_H_

#include "BSONReader.h"
#include <istream>

namespace Open3DMotion
{
	/** Implementation of BSONReader for reading from STL stream */
	class BSONStreamReader : public BSONReader
	{
	public:
		BSONStreamReader(std::istream& _input);

	public:

		virtual void SkipBytes(UInt32 count)  throw(BSONReadException);

		virtual void ReadBinary(void* binary, UInt32 size)  throw(BSONReadException);

		virtual bool HaveMore()
		{ return input.good(); }

	protected:
		std::istream& input;

	};

}

#endif
