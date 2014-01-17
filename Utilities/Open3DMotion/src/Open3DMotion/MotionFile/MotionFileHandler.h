/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MOTION_FILE_HANDLER_H_
#define _OPEN3DMOTION_MOTION_FILE_HANDLER_H_

#include "Open3DMotion/Biomechanics/Trial/Trial.h"
#include "Open3DMotion/MotionFile/MotionFileException.h"
#include "Open3DMotion/MotionFile/MotionFileFormatList.h"
#include "Open3DMotion/MotionFile/MotionFileFormatListAll.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"

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
		{ return programversion; }

		/** Read from file */
		TreeValue* Read(const char* filename, const MotionFileFormatList& formatlist = MotionFileFormatListAll(), const BinMemFactory& memfactory = BinMemFactoryDefault()) throw(MotionFileException);

		/** Read from STL stream */
		TreeValue* Read(std::istream& is, const MotionFileFormatList& formatlist = MotionFileFormatListAll(), const BinMemFactory& memfactory = BinMemFactoryDefault()) throw(MotionFileException);

		/** Write to file */
		void Write(const char* filename, const TreeValue* contents, const TreeValue* writeoptions, const MotionFileFormatList& formatlist = MotionFileFormatListAll()) throw(MotionFileException);

		/** Write to STL stream */
		void Write(std::ostream& os, const char* filename, const TreeValue* contents, const TreeValue* writeoptions, const MotionFileFormatList& formatlist = MotionFileFormatListAll()) throw(MotionFileException);

	private:
		std::string programname;
		std::string programversion;
	};

}
#endif