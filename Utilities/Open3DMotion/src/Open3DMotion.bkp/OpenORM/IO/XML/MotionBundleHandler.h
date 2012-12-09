/*--
  Open3DMotion 
  Copyright (c) 2004-2012 Charnwood Dynamics Ltd.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _MOTION_BUNDLE_HANDLER_H_
#define _MOTION_BUNDLE_HANDLER_H_

#include "Open3DMotion/MotionFile/MotionFileException.h"
#include "Open3DMotion/OpenORM/TreeValueh"

namespace Codamotion
{
	class MOBLFormat;

	class MotionBundleHandler
	{
	public:
		MotionBundleHandler();
		virtual ~MotionBundleHandler();

	public:

		void Open(const char* filename) throw(MotionFileException);

		UInt32 TrialCount();

		TreeValue* ReadTrial(UInt32 index)  throw(MotionFileException);

	protected:
		MOBLFormat* mobl;
	};

}

#endif
