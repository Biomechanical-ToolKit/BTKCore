/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/IO/BSON/BSONReaderMOBL.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"

namespace Open3DMotion
{
	TreeValue* BSONReaderMOBL::ReadElementValue(UInt8 elementcode)  throw (BSONReadException)
	{
		if (elementcode == UInt8(0x05))
		{
			// Read beginning of binary
			UInt32 numbytes(0);
			UInt8 subtype(0);
			ReadBinary(&numbytes, 4);
			ReadBinary(&subtype, 1);

			// MOBL files insert an additional 4 bytes containing length of subsequent data.
			// This will always be equal to (numbytes-4) so just skip these for MOBL compatibility.
			if (numbytes > 4)
			{
				SkipBytes(4);
				numbytes -= 4;
				MemoryHandlerBasic memory(numbytes);
				ReadBinary(memory.Data(), numbytes);
				return new TreeBinary(&memory);
			}
			else
			{
				// When <= than 4 bytes available data was not actually MOBL-compatible
				// - don't know how to read this so just skip and return NULL
				SkipBytes(numbytes);
				return NULL;
			}
		}
		else
		{
			return BSONReader::ReadElementValue(elementcode);
		}
	}

}