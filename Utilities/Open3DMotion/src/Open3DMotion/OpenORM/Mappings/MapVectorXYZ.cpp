/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapVectorXYZ.h"


namespace Open3DMotion
{
	MapVectorXYZ::MapVectorXYZ()
	{
		REGISTER_MEMBER(X);
		REGISTER_MEMBER(Y);
		REGISTER_MEMBER(Z);
	}

	void MapVectorXYZ::GetVector(double* xyz) const
	{
		xyz[0] = X;
		xyz[1] = Y;
		xyz[2] = Z;
	}

	void MapVectorXYZ::SetVector(const double* xyz)
	{
		X = xyz[0];
		Y = xyz[1];
		Z = xyz[2];
	}

}
