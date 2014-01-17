/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_READER_MOBL_H_
#define _ORMPP_BSON_READER_MOBL_H_

#include "Open3DMotion/OpenORM/IO/BSON/BSONReader.h"

namespace Open3DMotion
{
	/** Read from a stream of BSON-encoded data and translate into OpenORM++ objects 
	    with modification to binary reading for MOBL support */
	class BSONReaderMOBL : public BSONReader
	{
	public:
		BSONReaderMOBL(BSONInputStream& _input, const BinMemFactory& _memfactory) :
			BSONReader(_input, _memfactory)
		{
		}

	protected:
		
		/** Read the value part of a BSON element */
		virtual TreeValue* ReadElementValue(UInt8 elementcode) throw (BSONReadException);
	};

}

#endif
