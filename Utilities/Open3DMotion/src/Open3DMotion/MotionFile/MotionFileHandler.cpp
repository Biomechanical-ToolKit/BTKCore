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

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'fopen' instead of 'fopen_s' for portability reasons;
  #pragma warning( disable : 4996 )
#endif

namespace Open3DMotion
{
	using namespace std;

	// library name
	const char MotionFileHandler::LibraryName[] = "Open3DMotion";

	// library version: update this each time changes are made
	const char MotionFileHandler::LibraryVersion[] =  "2.00 Build 2 Revision 0";

	MotionFileHandler::MotionFileHandler(const char* _programname, const char* _programversion) :
		programname(_programname),
		programversion(_programversion)
	{
	}

	TreeValue* MotionFileHandler::Read(
		const char* filename, 
		const MotionFileFormatList& formatlist /*=MotionFileFormatListAll()*/, 
		const BinMemFactory& memfactory /*=BinMemFactoryDefault()*/ )
	{
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

		// read
		TreeValue* result = Read(is, formatlist, memfactory);

		// explicitly close file
		is.close();

		return result;
	}

	TreeValue* MotionFileHandler::Read(
		std::istream& is, 
		const MotionFileFormatList& formatlist /*=MotionFileFormatListAll()*/, 
		const BinMemFactory& memfactory /*=BinMemFactoryDefault()*/ )
	{
		// establish the format
		TreeValue* readoptions(NULL);
		const MotionFileFormat* format(NULL);
		for (MotionFileFormatList::ConstIterator i( formatlist.Begin() ); i != formatlist.End(); i++)
		{
      // can i get a rewind?
      is.clear();
      is.seekg(0, ios::beg);

      // probe this format
			const MotionFileFormat* testformat = i.Format();
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

		// return new trial object
		return trialobject;
	}

	void MotionFileHandler::Write(const char* filename, const TreeValue* contents, const TreeValue* writeoptions, const MotionFileFormatList& formatlist /*=MotionFileFormatListAll()*/)
	{
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
		Write(os, filename, contents, writeoptions, formatlist);

		// explicitly close file
		os.close();
	}

	void MotionFileHandler::Write(std::ostream& os, const char* filename, const TreeValue* contents, const TreeValue* writeoptions, const MotionFileFormatList& formatlist /*=MotionFileFormatListAll()*/)
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
		MotionFileFormat* chosenformat = formatlist.Find(formatoptions.FormatID);
		if (chosenformat == NULL)
		{
			chosenformat = formatlist.DefaultFormat();
			if (chosenformat == NULL)
			{
				throw MotionFileException(MotionFileException::libraryerror, "no formats provided in format list");
			}
		}

		// append filename in case needed
		TreeCompound writeoptions_with_filename;
		writeoptions_with_filename.CopyFrom(writeoptions);
		writeoptions_with_filename.Set(MEMBER_NAME(FileFormatOptions::PathName), new TreeString(filename));

    // write
		chosenformat->Write(*this, contents, os, &writeoptions_with_filename);
	}

}
