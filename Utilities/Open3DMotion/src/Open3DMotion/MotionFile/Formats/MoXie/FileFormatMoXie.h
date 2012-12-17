/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FILE_FORMAT_MOXIE_H_
#define _OPEN3DMOTION_FILE_FORMAT_MOXIE_H_

#include "Open3DMotion/MotionFile/MotionFileFormat.h"

// forward declarations
namespace pugi { class xml_node; }
class TestMoXie;

namespace Open3DMotion
{
	// MDF read/write
	class FileFormatMoXie : public MotionFileFormat
  {
		friend class TestMoXie;

  public:

		// Protected constructor used by friend class MotionFileFormatList
		FileFormatMoXie();

		virtual ~FileFormatMoXie();

  public:

		// check id and number type
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException);

    // Read MDF
    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) ;

    // Write MDF
    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException);
		
	protected:

		static void AddMoXieChannel(pugi::xml_node& viewer_data, const char* channelname, int decimalplaces, TimeSequence* ts);

		static void ExtractFileName(std::string& filename, const std::string& fullpath);

	};
}


#endif
