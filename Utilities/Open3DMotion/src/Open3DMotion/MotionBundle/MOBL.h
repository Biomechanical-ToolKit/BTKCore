/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MOBL_H_
#define _OPEN3DMOTION_MOBL_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/MotionFile/MotionFileException.h"

namespace Open3DMotion
{
	class BSONInputStream;

	/** Used internally by MOBLReader to hold format information during a read */
	class MOBLReadOptions : public MapCompound
	{
	public:
		MOBLReadOptions();

	public:
		MapBool UseGZIP;
		MapInt32 FormatVersion;
	};

	/** The reader class for motion bundle files */
	class MOBLFormatReader
	{
		friend class MOBLFormat;

	protected:
		/** Protected constructor - use MOBLFormat to construct */
		MOBLFormatReader(std::istream* _is, const MOBLReadOptions& _readoptions);

	public:
		virtual ~MOBLFormatReader();

		/** Read the specified trial.
		    @param index Zero-based index of trial to read
			*/
		TreeValue* ReadTrial(UInt32 index) throw(MotionFileException);

		/** Find total trials in the bundle.
				@return The number of trials
		 */
		UInt32 TrialCount()  throw(MotionFileException);

	protected:
		
		void ClearReader();

		bool PositionReader(UInt32 doc_index_needed)  throw(MotionFileException);

	protected:
    std::auto_ptr<std::istream> is;
		MOBLReadOptions readoptions;
		BSONInputStream* stream;
		UInt32 num_docs_read;
	};

	/** Factory for probing and reading from MOBL format Motion Bundle files */
	class MOBLFormat
	{
	public:
    
		/** Establish whether the stream is in MOBL format */
    bool Probe(MOBLReadOptions& readoptions, std::istream& is) throw(MotionFileException);

		/** For valid MOBL streams create a reader object to extract trial information */
		MOBLFormatReader* ReadBegin(std::istream* is, const MOBLReadOptions& readoptions);
	};

}

#endif
