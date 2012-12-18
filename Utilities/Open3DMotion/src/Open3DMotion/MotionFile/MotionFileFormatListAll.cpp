/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "MotionFileFormatListAll.h"
#include "Formats/MDF/FileFormatMDF.h"
#include "Formats/C3D/FileFormatC3D.h"
#include "Formats/XMove/FileFormatXMove.h"
#include "Formats/CODAText/FileFormatCODAText.h"
#include "Formats/MoXie/FileFormatMoXie.h"

namespace Open3DMotion
{
	MotionFileFormatListAll::MotionFileFormatListAll()
	{
		Register(new FileFormatXMove);
		Register(new FileFormatMDF);
		Register(new FileFormatC3D);
		Register(new FileFormatCODAText);
		Register(new FileFormatMoXie);
	}
}
