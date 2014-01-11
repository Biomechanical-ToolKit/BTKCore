/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BINARY_FIELD_SPEC_H_
#define _ORMPP_BINARY_FIELD_SPEC_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"

namespace Open3DMotion
{
	template<typename DataType> const char* BinaryFieldTypeString()
	{ return NULL; }

	template<> const char* BinaryFieldTypeString<UInt8>();

	template<> const char* BinaryFieldTypeString<Int32>();

	template<> const char* BinaryFieldTypeString<float>();

	template<> const char* BinaryFieldTypeString<double>();

	class BinaryFieldSpec : public MapCompound
	{
	public:
		template<typename DataType> static BinaryFieldSpec FromType(const char* name, Int32 dimension)
		{
			BinaryFieldSpec spec;
			spec.Name = name;
			spec.Dimension = dimension;
			spec.Type = BinaryFieldTypeString<DataType>();
			spec.Bytes = sizeof(DataType) * dimension;
			return spec;
		}

	public:
		BinaryFieldSpec();

		BinaryFieldSpec(const BinaryFieldSpec& src);

		const BinaryFieldSpec& operator=(const BinaryFieldSpec& src);

	protected:
		void RegisterMembers();

	public:
		MapString Name;
		MapString Type;
		MapInt32 Bytes;
		MapInt32 Dimension;
	};

}

#endif