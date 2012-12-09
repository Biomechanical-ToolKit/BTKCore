/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "MotionFileFormat.h"
#include "Formats/MDF/FileFormatMDF.h"
#include "Formats/C3D/FileFormatC3D.h"
#include "Formats/XMove/FileFormatXMove.h"

namespace Open3DMotion
{
	void MotionFileFormatList::RegisterAll()
	{
		Register(new FileFormatMDF);
		Register(new FileFormatC3D);
		Register(new FileFormatXMove);
	}
}
