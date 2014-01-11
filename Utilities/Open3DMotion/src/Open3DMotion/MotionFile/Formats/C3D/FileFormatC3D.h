/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FILE_FORMAT_C3D_H_
#define _OPEN3DMOTION_FILE_FORMAT_C3D_H_

#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include <string>

namespace Open3DMotion
{	
	// base class for C3D reading
	class FileFormatC3D : public MotionFileFormat
  {
	public:
    FileFormatC3D() :
			MotionFileFormat("C3D", "C3D", "c3d")
		{
		}

  public:

		// check id and number type
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException);

    // Read C3D
    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, const BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) ;

    // Write C3D
    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException);
		
	};

}

#endif
