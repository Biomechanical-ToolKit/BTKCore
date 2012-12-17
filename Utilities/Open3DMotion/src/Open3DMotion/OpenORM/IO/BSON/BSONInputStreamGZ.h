/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_STREAM_READER_GZ_H_
#define _ORMPP_BSON_STREAM_READER_GZ_H_

#include "BSONInputStream.h"
#include <istream>

struct z_stream_s;

namespace Open3DMotion
{
	/** Implementation of BSONReader for reading from STL stream in which the data was previously compressed using the gzip algorithm */
	class BSONInputStreamGZ: public BSONInputStream
	{
	public:
		BSONInputStreamGZ(std::istream& _input, UInt32 _chunk_size = 4096UL);

		~BSONInputStreamGZ();

	public:

		virtual void SkipBytes(UInt32 count)  throw(BSONReadException);

		virtual void ReadBinary(void* binary, UInt32 size)  throw(BSONReadException);

		virtual bool HaveMore();

	protected:
		std::istream& input;
		z_stream_s* strm;
		UInt32 chunk_size;
		UInt8* input_buffer;
		UInt8* output_buffer;
		UInt32 bytes_in_buffer;
		UInt8* output_pointer;
	};

}

#endif
