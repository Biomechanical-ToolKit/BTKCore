/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatXMove.h"
#include <set>
#include "Open3DMotion/OpenORM/IO/XML/XMLReadingMachine.h"
#include "Open3DMotion/OpenORM/IO/XML/XMLWritingMachine.h"
#include <pugixml.hpp>

namespace Open3DMotion
{	
	FileFormatXMove::FileFormatXMove() :
			MotionFileFormat("XMove", "XMove", "xmove,xml")
	{
	}

	FileFormatXMove::~FileFormatXMove()
	{
	}

	bool FileFormatXMove::Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException)
	{
		return ProbeTextString(is, "<xmove", 1024);
	}

  TreeValue* FileFormatXMove::Read(const MotionFileHandler& context, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) 
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(is);
		if (!result)
		{
			throw MotionFileException(MotionFileException::formaterror, result.description());
		}

		// find xmove node
		pugi::xml_node xmove_node = doc.child("xmove");

		// must have xmove node
		if (!xmove_node)
		{
			throw MotionFileException(MotionFileException::formaterror, "XML missing xmove section");
		}

		// read info
		XMLReadingMachine reader;
		TreeValue* trial_object = reader.ReadValue(xmove_node);

		return trial_object;

	}

  void FileFormatXMove::Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
	{
		os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";
		XMLWritingMachine writer(os);
		writer.WriteValue("xmove", contents);
	}
	
}
