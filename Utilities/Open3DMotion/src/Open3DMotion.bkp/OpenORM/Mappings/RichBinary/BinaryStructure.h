/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinaryFieldSpec.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/NoSuchFieldException.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayCompound.h"
#include <stdlib.h>

#ifdef MSVC_VER
  // Disables MS warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  #pragma warning( disable : 4290 )
#endif

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

		void GetFieldOffset(BinaryFieldSpec const*& spec, size_t& offset, const char* fieldname) const throw(NoSuchFieldException);

		static const char LayoutElementName[];

	protected:
		void RegisterMembers();

	private:
		MapInt32 Bytes;
		MapArrayCompound<BinaryFieldSpec> Layout;
	};
}
