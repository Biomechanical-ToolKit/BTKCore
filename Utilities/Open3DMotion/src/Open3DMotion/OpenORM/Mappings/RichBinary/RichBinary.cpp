/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"

namespace Open3DMotion
{
	const char RichBinary::BinaryName[] = "Data";

	RichBinary::RichBinary(const char* structurename)
	{
		Register(structurename, &Structure);
		Register(BinaryName, &Binary);
	}

	void RichBinary::Allocate(const std::vector<BinaryFieldSpec>& layout, size_t numframes, BinMemFactory& memfactory)
	{
		Structure.SetLayout(layout);
		Binary.SetData( memfactory.Allocate(numframes, Structure) );
	}

}
