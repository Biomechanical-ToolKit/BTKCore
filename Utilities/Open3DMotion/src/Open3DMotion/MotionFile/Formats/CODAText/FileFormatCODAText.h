/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FILE_FORMAT_CODA_TEXT_H_
#define _OPEN3DMOTION_FILE_FORMAT_CODA_TEXT_H_

#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include <string>

namespace Open3DMotion
{
	// Text read/write
	class FileFormatCODAText : public MotionFileFormat
  {
	public:

		FileFormatCODAText();

		virtual ~FileFormatCODAText();

  public:

		// check for presence of Codamotion Analysis text signature
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException);

    // Read Text
    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) ;

    // Write Text
    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException);
		
	};
}


#endif
