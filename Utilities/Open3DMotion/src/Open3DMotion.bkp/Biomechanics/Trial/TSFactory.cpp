/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

namespace Open3DMotion
{
	using namespace std;

	const char TSFactoryValue::fieldname_value[] = "value";
	const char TSFactoryOccValue::fieldname_occluded[] = "occluded";

	TimeSequence* TSFactory::New(const TimeRange& tr, BinMemFactory& memfactory) const
	{
		TimeSequence* ts = new TimeSequence;
		ts->Allocate(layout, tr, memfactory);
		return ts;
	}

	void TSFactory::AddField(const BinaryFieldSpec& field)
	{
		layout.push_back(field);
	}

	TSFactoryValue::TSFactoryValue(Int32 dimension)
	{
		AddField( BinaryFieldSpec::FromType<double>( fieldname_value, dimension ) );
	}

	TSFactoryOccValue::TSFactoryOccValue(Int32 dimension) :
		TSFactoryValue(dimension)
	{
		AddField( BinaryFieldSpec::FromType<UInt8>( fieldname_occluded, 1 ) );
	}
}
