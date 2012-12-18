/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "FileFormatOptionsXMove.h"

namespace Open3DMotion
{
	FileFormatOptionsXMove::FileFormatOptionsXMove()
	{
		REGISTER_MEMBER(Extended);

		FormatID = "XMove";
		Extended = false;
	}

}
