/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_INTERPOLATE_H_
#define _OPEN3DMOTION_INTERPOLATE_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"

namespace Open3DMotion
{

	// Perform linear interpolation
  class Interpolate
  {
	public:
		static const size_t cubic_default_maxnumslopesamples = 10UL;

	public:
		
		// Apply to 3D sequence (individual xyz coords treated independently)
		static void Linear(TimeSequence& ts) throw(NoSuchFieldException);

		// Apply to 3D sequence (individual xyz coords treated independently)
		static void Cubic(TimeSequence& ts, size_t maxnumslopesamples = cubic_default_maxnumslopesamples) throw(NoSuchFieldException);

	};
}

#endif
