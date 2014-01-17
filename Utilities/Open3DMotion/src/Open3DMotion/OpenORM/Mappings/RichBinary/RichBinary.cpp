/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"

#include <memory>
#include <memory.h>

namespace Open3DMotion
{
	const char RichBinary::BinaryName[] = "Data";

	RichBinary::RichBinary(const char* structurename)
	{
		Register(structurename, &Structure);
		Register(BinaryName, &Binary);
	}

	void RichBinary::Allocate(const std::vector<BinaryFieldSpec>& layout, size_t numframes, const BinMemFactory& memfactory)
	{
		Structure.SetLayout(layout);
		size_t datasize = Structure.TotalBytes() * numframes;
		Binary.SetData( memfactory.Allocate(datasize) );
	}

	void RichBinary::DeepCopyFrom(const RichBinary& src, const BinMemFactory& memfactory)
	{
		// make a shallow copy of binary data and deep copy of all meta-data
		std::auto_ptr<TreeValue> tree_copy( src.ToTree() );
		FromTree(tree_copy.get());

		// replace data with a copy
		if (src.DataSizeBytes())
		{
			Binary.SetData( memfactory.Allocate(src.DataSizeBytes()) );
			memcpy(Data(), src.Data(), src.DataSizeBytes());
		}
	}


}
