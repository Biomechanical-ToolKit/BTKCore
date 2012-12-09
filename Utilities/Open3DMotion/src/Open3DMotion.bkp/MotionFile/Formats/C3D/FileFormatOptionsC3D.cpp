/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "FileFormatOptionsC3D.h"
#include "C3DMachine.h"

namespace Open3DMotion
{
	const char FileFormatOptionsC3D::ProcessorPC[] = "PC";
	const char FileFormatOptionsC3D::ProcessorDEC[] =  "DEC";
	const char FileFormatOptionsC3D::ProcessorSGI[] = "SGI";

	FileFormatOptionsC3D::FileFormatOptionsC3D()
	{
		REGISTER_MEMBER(Processor);
		REGISTER_MEMBER(FloatingPoint);

		// defaults
		FormatID = "C3D";
		Processor = ProcessorPC;
		FloatingPoint = true;
	}

	C3DMachine* FileFormatOptionsC3D::NewMachine() const
	{
		if (Processor.Value() == ProcessorDEC)
			return new C3DMachine_DEC;
		else if (Processor.Value() == ProcessorSGI)
			return new C3DMachine_SGI;
		else
			return new C3DMachine_PC;
	}
}
