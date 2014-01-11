/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MOTION_FILE_FORMAT_H_
#define _OPEN3DMOTION_MOTION_FILE_FORMAT_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/MotionFile/MotionFileHandler.h"

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

namespace Open3DMotion
{

	// Motion file format
  class MotionFileFormat
  {
  protected:
    // Protected constructor for derived classes
    // Parameters 
    //  _summary : Int16 description of file format
    //  _extension : comma-separated list of file extensions
    MotionFileFormat(
      const char* _formatid,
      const char* _summary, 
      const char* _extensions);

	public:

    // Destructor
    virtual ~MotionFileFormat()
    {
    }

  public:
    // id string
    const char* FormatID() const
    { return strFormatID; }

    // description (for file dialogs)
    const char* Summary() const
    { return strSummary; }

    // number of filename extensions supported
    // by this format
    int NumExtensions() const
    { return (int)extension_array.size(); }

    // retrieve filename extensions
    const char* Extension(int i) const
    { return extension_array[i].c_str(); }

    // Is the given stream one of our files?
    // Returns: true if can load the file, false otherwise
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException) = 0;

		// Read the file into trial object (or derived trial object)
    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, const BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) = 0;

		// Write the file
    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException) = 0;

	protected:

		// Helper to look for searchstring in first searchrange characters
		static bool ProbeTextString(std::istream& is, const char* searchstring, size_t searchrange);

  protected:
    const char* strFormatID;
    const char* strSummary;     // summary string
    const char* strExtensions;
		std::vector<std::string> extension_array;
  };

}
#endif