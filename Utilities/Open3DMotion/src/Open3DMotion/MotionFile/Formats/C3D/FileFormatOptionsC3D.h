/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FILE_FORMAT_OPTIONS_C3D_H_
#define _OPEN3DMOTION_FILE_FORMAT_OPTIONS_C3D_H_

#include "Open3DMotion/MotionFile/FileFormatOptions.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"

namespace Open3DMotion
{
	// forward decl
	class C3DMachine;

	// options class for C3D
	class FileFormatOptionsC3D : public FileFormatOptions
	{
	public:
		static const char ProcessorPC[];
		static const char ProcessorDEC[];
		static const char ProcessorSGI[];

	public:
		FileFormatOptionsC3D();

		C3DMachine* NewMachine() const;

	public:
		MapString Processor;
		MapBool FloatingPoint;
	};
}

#endif
