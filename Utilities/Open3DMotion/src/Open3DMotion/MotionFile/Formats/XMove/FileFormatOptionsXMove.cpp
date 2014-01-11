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
		REGISTER_MEMBER(LegacyCompoundNames);
		REGISTER_MEMBER(ConvertBinaryFloat32);
		REGISTER_MEMBER(ExcludeCalc);

		FormatID = "XMove";
		Extended = false;
		LegacyCompoundNames = false;
		ConvertBinaryFloat32 = true;
		ExcludeCalc = false;
	}

}
