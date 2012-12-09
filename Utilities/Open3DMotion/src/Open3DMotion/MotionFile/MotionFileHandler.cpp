/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/MotionFile/FileFormatOptions.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"

namespace Open3DMotion
{
	using namespace std;

	// library name
	const char MotionFileHandler::LibraryName[] = "Codamotion File Library";

	// library version: update this each time changes are made
	const char MotionFileHandler::LibraryVersion[] =  "2.00 Build 0 Revision 0";

	MotionFileHandler::MotionFileHandler(const char* _programname, const char* _programversion) :
		programname(_programname),
		programversion(_programversion)
	{
	}

	TreeValue* MotionFileHandler::Read(const char* filename) throw(MotionFileException)
	{
		const MotionFileFormatList& formatlist =  *MotionFileFormatList::GetInstance();

		// default for now
		BinMemFactoryDefault memfactory;

#ifdef _MSC_VER
		// creating a C-style file object may work better with UTF-8
		FILE* cfile = fopen(filename, "rb");
        if (cfile == NULL)
            throw MotionFileException(MotionFileException::noaccess, "file does not exist or you have insufficient permissions to open it");
		// assign C++ style object
		ifstream is(cfile);
#else
        ifstream is(filename, std::ios::binary);
#endif

		// establish the format
		TreeValue* readoptions(NULL);
		const MotionFileFormat* format(NULL);
		for (MotionFileFormatList::const_iterator i( formatlist.begin() ); i != formatlist.end(); i++)
		{
      // can i get a rewind?
      is.clear();
      is.seekg(0, ios::beg);

      // probe this format
			const MotionFileFormat* testformat = i->second;
			if (testformat->Probe(*this, readoptions, is))
			{
				format = testformat;
				break;
			}
		}

    // format must exist
    if (format == NULL)
      throw(MotionFileException(MotionFileException::formatnotrecognised, "Unrecognised format"));

    // can i get a rewind?
    is.clear();
    is.seekg(0, ios::beg);

		// format found - read it
		TreeValue* trialobject(NULL);
    try
    {
      // read file
      trialobject = format->Read(*this, is, memfactory, readoptions);
    }
    catch(ios::failure)
    {
      // read past end of file
      // - translate into a motion file exception
      throw(MotionFileException(MotionFileException::formaterror, "Format error - unexpected end of file"));
    }

		// explicitly close file
		is.close();

    // TODO: insert format id in parameters
		// trial->FileFormat.ID = format->FormatID();

		// return new trial object
		return trialobject;
	}

	void MotionFileHandler::Write(const char* filename, const TreeValue* contents, const TreeValue* writeoptions) throw(MotionFileException)
	{
		// ensure write options non-NULL
		TreeCompound emptyoptions;
		if (writeoptions == NULL)
		{
			writeoptions = &emptyoptions;
		}

		// get format id structure
		FileFormatOptions formatoptions;
		formatoptions.FromTree(writeoptions);

		// take specified format if found, otherwise default
		const MotionFileFormatList& formatlist = *MotionFileFormatList::GetInstance();
		map<string, MotionFileFormat*>::const_iterator iformat =  formatlist.find(formatoptions.FormatID);
		MotionFileFormat* chosenformat = (iformat != formatlist.end()) ? iformat->second : formatlist.begin()->second;

#ifdef _MSC_VER
		// creating a C-style file object may work better with UTF-8
		FILE* cfile = fopen(filename, "wb");
        if (cfile == NULL)
            throw MotionFileException(MotionFileException::noaccess, "file does not exist or you have insufficient permissions to open it");
		// assign C++ style object
		ofstream os(cfile);
#else
        ofstream os(filename, std::ios::binary);
#endif

    // write
		chosenformat->Write(*this, contents, os, writeoptions);

		// explicitly close file
		os.close();
	}

}
