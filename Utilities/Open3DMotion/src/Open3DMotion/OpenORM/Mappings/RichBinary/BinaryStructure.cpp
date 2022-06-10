/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BinaryStructure.h"

namespace Open3DMotion
{
	const char BinaryStructure::LayoutElementName[] = "Field";

	BinaryStructure::BinaryStructure() :
		Layout(LayoutElementName)

	{
		REGISTER_MEMBER(Bytes);
		REGISTER_MEMBER(Layout);
	}

	void BinaryStructure::SetLayout(const std::vector<BinaryFieldSpec>& _layout)
	{
		for (std::vector<BinaryFieldSpec>::const_iterator i (_layout.begin() ); i != _layout.end(); i++)
		{
			Layout.Add(new BinaryFieldSpec(*i));
			Bytes.Value() += i->Bytes.Value();
		}
	}

	void BinaryStructure::GetFieldOffset(BinaryFieldSpec const*& spec, size_t& offset, const char* fieldname) const
	{
		size_t n = Layout.NumElements();
		offset = 0;
		for (size_t i = 0; i < n; i++)
		{
			spec = &Layout[i];
			if (spec->Name.Value().compare(fieldname) == 0)
				return;
			offset += spec->Bytes;
		}
		
		// field not found
		throw NoSuchFieldException(fieldname, NoSuchFieldException::missing);
	}
}
