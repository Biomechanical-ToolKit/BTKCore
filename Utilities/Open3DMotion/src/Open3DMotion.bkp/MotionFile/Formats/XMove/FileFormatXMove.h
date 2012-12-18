/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _FILE_FORMAT_XMOVE_H_
#define _FILE_FORMAT_XMOVE_H_

#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include <string>

namespace Open3DMotion
{
	// MDF read/write
	class FileFormatXMove : public MotionFileFormat
  {
		friend class MotionFileFormatList;

  protected:
    // Protected constructor for derived classes
    // Parameters 
    //  _summary : Int16 description of file format
    //  _extension : comma-separated list of file extensions
    FileFormatXMove();

		virtual ~FileFormatXMove();

  public:

		// check id and number type
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException);

    // Read MDF
    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) ;

    // Write MDF
    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException);
		
	};
}


#endif
