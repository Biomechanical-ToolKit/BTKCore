/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/MathsException.h"

namespace Open3DMotion
{
	MathsException::MathsException()
	{
	}

	MathsException::MathsException(const char* _condition) :
		condition(_condition)
	{
	}

	MathsException::MathsException(const MathsException& src)
	{
		operator=(src);
	}

	const MathsException& MathsException::operator=(const MathsException& src)
	{
		condition = src.condition;
		return *this;
	}
}
