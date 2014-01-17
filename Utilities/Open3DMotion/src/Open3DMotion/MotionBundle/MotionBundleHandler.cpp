/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "MotionBundleHandler.h"
#include "MOBL.h"
#include <fstream>

namespace Open3DMotion
{
	MotionBundleHandler::MotionBundleHandler() :
		mobl(new MOBLFormat)
	{
	}

	MotionBundleHandler::~MotionBundleHandler()
	{
	}

	MOBLFormatReader* MotionBundleHandler::Open(const char* filename) throw(MotionFileException)
	{
#ifdef _MSC_VER
		// creating a C-style file object may work better with UTF-8
		FILE* cfile = fopen(filename, "rb");
        if (cfile == NULL)
            throw MotionFileException(MotionFileException::noaccess, "file does not exist or you have insufficient permissions to open it");
		// assign C++ style object
		std::auto_ptr< std::ifstream > is( new std::ifstream(cfile) );
#else
        std::auto_ptr< std::ifstream > is( new std::ifstream(filename, std::ios::binary) );
#endif


		// check for read
		MOBLReadOptions readoptions;
		MOBLFormatReader* reader(NULL);
		if (mobl->Probe(readoptions, *is.get()))
		{
			// can read
			reader = mobl->ReadBegin(is.release(), readoptions);
		}
		else
		{
			// unrecognised
			throw MotionFileException(MotionFileException::formatnotrecognised, "unrecognised file format for motion bundle");
		}

		return reader;
	}

}
