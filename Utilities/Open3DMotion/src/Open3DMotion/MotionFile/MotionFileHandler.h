/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/Biomechanics/Trial/Trial.h"
#include "Open3DMotion/MotionFile/MotionFileException.h"

namespace Open3DMotion
{
	class MotionFileHandler
	{
	public:
		
		static const char LibraryName[];

		static const char LibraryVersion[];

	public:

		MotionFileHandler(const char* _programname, const char* _programversion);

	public:
		
		const std::string& ProgramName() const
		{ return programname; }

		const std::string& ProgramVersion() const
		{ return programname; }

		TreeValue* Read(const char* filename) throw(MotionFileException);

		void Write(const char* filename, const TreeValue* contents, const TreeValue* writeoptions) throw(MotionFileException);

	private:
		std::string programname;
		std::string programversion;
	};

}
