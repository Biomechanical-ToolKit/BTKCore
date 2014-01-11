/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/C3D/C3DTSFactory.h"

namespace Open3DMotion
{
	const char TSFactoryC3D::fieldname_confidence[] = "confidence";
	const char TSFactoryC3D::fieldname_c3dflag[] = "c3dflag";

	TSFactoryC3D::TSFactoryC3D() :
		TSFactoryOccValue(3)
	{
		AddField( BinaryFieldSpec::FromType<double>( fieldname_confidence, 3 ) );
		AddField( BinaryFieldSpec::FromType<UInt8>( fieldname_c3dflag, 1 ) );
	}
}
