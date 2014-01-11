/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "LinearCalibration.h"
#include <vector>

namespace Open3DMotion
{
	// This is to ensure a vector compiles ok
	void __LinearCalibration__Compilation__();
	void __LinearCalibration__Compilation__()
	{
		std::vector<LinearCalibration> x;
	}
}
