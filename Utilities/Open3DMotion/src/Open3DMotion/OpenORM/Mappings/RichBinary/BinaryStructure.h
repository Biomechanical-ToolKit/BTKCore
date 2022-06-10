/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BINARY_STRUCTURE_H_
#define _ORMPP_BINARY_STRUCTURE_H_

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinaryFieldSpec.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/NoSuchFieldException.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayCompound.h"
#include <stdlib.h>

namespace Open3DMotion
{
	class BinaryStructure : public MapCompound
	{
	public:
		
		BinaryStructure();

		void SetLayout(const std::vector<BinaryFieldSpec>& _layout);

		size_t TotalBytes() const
		{ return Bytes.Value(); }

		const MapArrayCompound<BinaryFieldSpec>& GetLayout() const
		{ return Layout; }

		void GetFieldOffset(BinaryFieldSpec const*& spec, size_t& offset, const char* fieldname) const;

		static const char LayoutElementName[];

	protected:
		void RegisterMembers();

	private:
		MapInt32 Bytes;
		MapArrayCompound<BinaryFieldSpec> Layout;
	};
}

#endif
