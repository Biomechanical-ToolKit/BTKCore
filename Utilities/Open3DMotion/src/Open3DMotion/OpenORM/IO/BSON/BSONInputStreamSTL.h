/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_INPUT_STREAM_STL_H_
#define _ORMPP_BSON_INPUT_STREAM_STL_H_

#include "BSONInputStream.h"
#include <istream>

namespace Open3DMotion
{
	/** Implementation of BSONReader for reading from STL stream */
	class BSONInputStreamSTL : public BSONInputStream
	{
	public:
		BSONInputStreamSTL(std::istream& _input);

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
