/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _FILE_FORMAT_MDF_H_
#define _FILE_FORMAT_MDF_H_

#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include <string>

namespace Open3DMotion
{
	class MDFSection : public MapCompound
	{
	public:
		MDFSection()
		{
			REGISTER_MEMBER(HipOffsetRatioX);
			REGISTER_MEMBER(HipOffsetRatioY);
			REGISTER_MEMBER(HipOffsetRatioZ);
			REGISTER_MEMBER(COM_LThigh);
			REGISTER_MEMBER(COM_LShank);
			REGISTER_MEMBER(COM_LFoot);
			REGISTER_MEMBER(COM_RThigh);
			REGISTER_MEMBER(COM_RShank);
			REGISTER_MEMBER(COM_RFoot);
		}

	public:
		MapOptionalFloat64 HipOffsetRatioX;
		MapOptionalFloat64 HipOffsetRatioY;
		MapOptionalFloat64 HipOffsetRatioZ;
		MapOptionalFloat64 COM_LThigh;
		MapOptionalFloat64 COM_LShank;
		MapOptionalFloat64 COM_LFoot;
		MapOptionalFloat64 COM_RThigh;
		MapOptionalFloat64 COM_RShank;
		MapOptionalFloat64 COM_RFoot;
	};

	class TrialMDF : public Trial
	{
	public:
		TrialMDF()
		{
			REGISTER_MEMBER(MDF);
		}

	public:
		MapOptional<MDFSection> MDF;
	};

	// MDF read/write
	class FileFormatMDF : public MotionFileFormat
  {
		friend class MotionFileFormatList;

  protected:
    // Protected constructor for derived classes
    // Parameters 
    //  _summary : Int16 description of file format
    //  _extension : comma-separated list of file extensions
    FileFormatMDF() :
			MotionFileFormat("MDF", "MDF", "mdf,mdr")
		{
		}

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
