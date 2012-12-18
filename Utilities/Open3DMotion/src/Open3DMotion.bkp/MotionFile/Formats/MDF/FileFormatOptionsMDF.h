/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _FILE_FORMAT_OPTIONS_MDF_H_
#define _FILE_FORMAT_OPTIONS_MDF_H_

#include "Open3DMotion/MotionFile/FileFormatOptions.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"

namespace Open3DMotion
{
	// forward declaration
	class MDFDescriptor;

	// options class for C3D
	class FileFormatOptionsMDF : public FileFormatOptions
	{
	public:
		static const Int32 VERSION2 = 2;
		static const Int32 VERSION3 = 3;

	public:
		FileFormatOptionsMDF();

	public:
		MapInt32 FormatVersion;

	public:
		MDFDescriptor* NewDescriptor() const;
	};

}

#endif
