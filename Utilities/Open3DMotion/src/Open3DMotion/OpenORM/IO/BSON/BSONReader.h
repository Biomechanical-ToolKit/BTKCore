/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_READER_H_
#define _ORMPP_BSON_READER_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReadException.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStream.h"

namespace Open3DMotion
{
	/** Read from a stream of BSON-encoded data and translate into OpenORM++ objects */
	class BSONReader
	{
	public:

		/** Construct for given stream */
		BSONReader(BSONInputStream& _input);

		/** Virtual destructor */
		virtual ~BSONReader();

	public:

		/** Read whole document - any unsupported fields are skipped */
		void ReadDocument(TreeCompound& compound)  throw(BSONReadException);

		/** Read document as a list (require first item to be a string containing element name, returns NULL if not a string there) */
		TreeList* ReadList()  throw(BSONReadException);

		/** Read next BSON element in stream

				@param name filled with name of element
				@param value filled with pointer to new TreeValue, or NULL if class unsupported
				@return true if there was another element in the stream, false otherwise
		*/
		bool ReadElement(std::string& name, TreeValue*& value)  throw(BSONReadException);

		/** Read a C-string (ASCII subset of UTF-8) */
		void ReadCString(std::string& s) throw(BSONReadException);

		/** Read generic UTF-8 string */
		void ReadString(std::string& s) throw(BSONReadException);

	protected:

		/** Read the value part of a BSON element */
		virtual TreeValue* ReadElementValue(UInt8 elementcode) throw (BSONReadException);

	public:

		/** Skip forwards by a specified number of bytes 
		    @param count number of bytes to skip
		 */
		void SkipBytes(UInt32 count)  throw(BSONReadException)
		{ input.SkipBytes(count); }

		/** Read a specified amount of data 
		    @param binary pointer to buffer of required size to hold data
				@param size the amount of data to read, in bytes
		  */
		void ReadBinary(void* binary, UInt32 size)  throw(BSONReadException)
		{ input.ReadBinary(binary, size); }

		/** Discover whether there are more bytes available for subsequent read or skip operations
		    @return true if more bytes, false otherwise
			*/
		bool HaveMore()
		{ return input.HaveMore(); }

	private:
		BSONInputStream& input;
	};

}

#endif
