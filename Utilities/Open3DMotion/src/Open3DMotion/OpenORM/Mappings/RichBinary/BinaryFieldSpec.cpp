/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinaryFieldSpec.h"

namespace Open3DMotion
{
	const char field_type_name_uint8[] = "byte";
	const char field_type_name_int32[] = "long";
	const char field_type_name_float32[] = "float";
	const char field_type_name_float64[] = "double";

	template<> const char* BinaryFieldTypeString<UInt8>()
	{ return field_type_name_uint8; }

	template<> const char* BinaryFieldTypeString<Int32>()
	{ return field_type_name_int32; }

	template<> const char* BinaryFieldTypeString<float>()
	{ return field_type_name_float32; }

	template<> const char* BinaryFieldTypeString<double>()
	{ return field_type_name_float64; }

	BinaryFieldSpec::BinaryFieldSpec()
	{
		RegisterMembers();
	}

	BinaryFieldSpec::BinaryFieldSpec(const BinaryFieldSpec& src) :
		MapCompound()
	{
		RegisterMembers();
		*this = src;
	}

	void BinaryFieldSpec::RegisterMembers()
	{
		REGISTER_MEMBER(Name);
		REGISTER_MEMBER(Type);
		REGISTER_MEMBER(Bytes);
		REGISTER_MEMBER(Dimension);
	}

	const BinaryFieldSpec& BinaryFieldSpec::operator=(const BinaryFieldSpec& src)
	{
		Name = src.Name;
		Type = src.Type;
		Bytes = src.Bytes;
		Dimension = src.Dimension;
		return *this;
	}
}
