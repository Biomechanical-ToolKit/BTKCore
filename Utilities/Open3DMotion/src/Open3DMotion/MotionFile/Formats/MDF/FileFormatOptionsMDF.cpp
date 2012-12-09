/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "FileFormatOptionsMDF.h"

namespace Open3DMotion
{

	FileFormatOptionsMDF::FileFormatOptionsMDF()
	{
		REGISTER_MEMBER(FormatVersion);

		// default value
		FormatID = "MDF";
		FormatVersion = FileFormatOptionsMDF::VERSION3;
	}

#ifdef __GNUC__
  // Need explcit implementation of these in GNU
	// though constant values are given in header
  const Int32 FileFormatOptionsMDF::VERSION2;
  const Int32 FileFormatOptionsMDF::VERSION3;
#endif
}
