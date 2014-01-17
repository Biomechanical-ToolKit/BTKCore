/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MOTION_BUNDLE_HANDLER_H_
#define _OPEN3DMOTION_MOTION_BUNDLE_HANDLER_H_

#include "Open3DMotion/MotionFile/MotionFileException.h"
#include "Open3DMotion/OpenORM/TreeValue.h"
#include <memory>

namespace Open3DMotion
{
	class MOBLFormat;
	class MOBLFormatReader;

	/** Read bundle files which contain more than one trial */
	class MotionBundleHandler
	{
	public:
		
		MotionBundleHandler();

		virtual ~MotionBundleHandler();

	public:

		/** Probe the file format and return a valid reader or throw an exception 
		    @return An valid object to read the file.
				@throw MotionFileException if there was a problem recognising or reading the file format */
		MOBLFormatReader* Open(const char* filename) throw(MotionFileException);

	protected:
		std::auto_ptr<MOBLFormat> mobl;
	};

}

#endif
