/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONInputStreamGZ.h"
#include <zlib.h>

namespace Open3DMotion
{
	BSONInputStreamGZ::BSONInputStreamGZ(std::istream& _input, UInt32 _chunk_size /*= 4096UL*/) :
		input(_input),
		chunk_size(_chunk_size),
		bytes_in_buffer(0),
		output_pointer(NULL)
	{
		strm = (z_stream*)malloc(sizeof(z_stream));
		input_buffer = (UInt8*)malloc(chunk_size);
		output_buffer = (UInt8*)malloc(chunk_size);
		memset(strm, NULL, sizeof(z_stream));
		inflateInit2(strm, (16UL + 15UL));
	}

	BSONInputStreamGZ::~BSONInputStreamGZ()
	{
		inflateEnd(strm);
		free(strm);
		free(input_buffer);
		free(output_buffer);
	}

	void BSONInputStreamGZ::SkipBytes(UInt32 count)  throw(BSONReadException)
	{
		ReadBinary(NULL, count);
	}

	void BSONInputStreamGZ::ReadBinary(void* binary, UInt32 size)  throw(BSONReadException)
	{
		UInt32 bytes_remaining = size;
		while (true)
		{
			// copy what we can from the buffer
			UInt32 bytes_to_transfer = (bytes_in_buffer > bytes_remaining) ? bytes_remaining : bytes_in_buffer;
			if (bytes_to_transfer)
			{
				// do the transfer (also allow NULL transfers for skipping purposes)
				if (binary != NULL)
				{
					memcpy((UInt8*)binary + (size-bytes_remaining), output_pointer, bytes_to_transfer);
				}

				// decrement number remaining
				bytes_remaining -= bytes_to_transfer;

				// will go to zero if read whole buffer or just decremented if size was less than buffer
				bytes_in_buffer -= bytes_to_transfer;

				// increment ready for next call
				output_pointer += bytes_to_transfer;
			}

			// might be done now
			if (bytes_remaining == 0)
				break;

			// get new input as and when needed
			if (strm->avail_in == 0)
			{
				// we have to fail if nothing more in stream
				if (!input.good())
					throw BSONReadException("error reading from compressed BSON stream");

				// read as much as available up to buffer size
				strm->next_in = input_buffer;
				input.read((char*)input_buffer, chunk_size);
				strm->avail_in = (Int32)input.gcount();
			}

			// read and unzip a new buffer full
			strm->avail_out = chunk_size;
			strm->next_out = output_buffer;
			int result = inflate(strm, Z_NO_FLUSH);
			bytes_in_buffer = chunk_size - strm->avail_out;
			output_pointer = output_buffer;
			if (result == Z_STREAM_ERROR || result == Z_NEED_DICT || result == Z_DATA_ERROR || result == Z_MEM_ERROR)
				throw BSONReadException("error reading from compressed BSON stream");

		}
	}

	bool BSONInputStreamGZ::HaveMore()
	{
		return bytes_in_buffer || input.good();
	}

}
